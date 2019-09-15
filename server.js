const express = require('express');
const {PythonShell} = require('python-shell');
const app = express();
const PORT = process.env.PORT || 8080;

var fans = require('./fans.json');

app.get('/', (req, res) => {
    res.send('Harbor Breeze Hub');
});

app.get('/api/fans', (req, res) => {
    // Return list of all fans added to hub.
    res.send({success:true,result:JSON.stringify(fans)});
});

app.get('/api/fans/:remote_id', (req, res) => {
    var fan = getFan(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan with this remote id does not exist.'});
    } else {
        res.send(fan);
    }
});

app.get('/api/fans/:remote_id/:option', (req, res) => {
    var fan = getFan(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan with this remote id does not exist.'});
    } else {
        if (req.params.option in fan) {
            var result_json = {};
            result_json[req.params.option] = fan[req.params.option];
            res.json({success:true,result:JSON.stringify(result_json)});
        } else {
            res.status(404).json({success:false,msg:'Option does not exist for fan.'});
        }
    }
});

app.put('/api/fans/:remote_id/:option', (req, res) => {
    // Check if correct query data passed.
    if (!('access_code' in req.query)) {
        res.status(400).json({success:false,msg:'Missing access code.'});
    }
    if (!('value' in req.query)) {
        res.status(400).json({success:false,msg:'Missing new value.'});
        return;
    }
    // Get fan from remote id.
    var fan = getFan(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan with this remote id does not exist.'});
        return;
    }
    // Check if fan access code is correct.
    if (fan.access_code != req.query.access_code) {
        res.status(403).json({success:false,msg:'Invalid fan access code provided.'});
        return;
    }
    // Check if option is invalid.
    if (!(req.params.option in fan)) {
        res.status(404).json({success:false,msg:'Option does not exist for fan.'});
        return;
    }
    // Check if new value provided for option.
    if (req.params.option == 'light_brightness' || fan[req.params.option].toString() != req.query.value) {
        // Contains option and arguments to run python fan remote script.
        var python_options = {
            mode: 'text',
            pythonPath: 'python3',
            args: [fan.remote_id]
        };
        // Run FanRemote python script with python_options
        function run_fan_remote(callback) {
            PythonShell.run('FanRemote.py', python_options, function(err, results) {
                if (typeof callback === 'function') {
                    if (err) {
                        callback(false);
                    } else {
                        callback(true);
                    }
                }
            });
        }

        // Process put request received.
        switch(req.params.option) {
            case 'light_on':
                // Toggle fan light.
                python_options.args.push('light');
                run_fan_remote((success) => {
                    if (success) {
                        // Update fan option.
                        var is_light_on = (req.query.value.toLowerCase() == 'true');
                        fan['light_on'] = is_light_on;
                        res.json({success:true});
                    } else {
                        res.status(500).json({success:false,msg:'Error with fan communication python script.'});
                    }
                });
                break;
            case 'light_brightness':
                const MAX_DIM_CNT = 220.0;
                var brightness = parseInt(req.query.value);
                if (isNaN(brightness) || brightness <= 0) {
                    res.status(400).json({success:false,msg:'Invalid fan light brightness provided.'});
                    return;
                }

                // convert input from 1-100 to 1-MAX_DIM_CNT
                brightness = (brightness*MAX_DIM_CNT)/100.0
                if (brightness > MAX_DIM_CNT) {
                    brightness = MAX_DIM_CNT;
                } else if (brightness <= 0) {
                    brightness = 1;
                }
                var brightness_0to1 = brightness/MAX_DIM_CNT;
                
                if (fan['light_on']) {
                    if (fan['light_brightness'] <= brightness_0to1) {
                        // user is trying to increase light brightness
                        var dim_cmd_cnt = brightness - (fan['light_brightness']*MAX_DIM_CNT);
                        if (dim_cmd_cnt == 0) {
                            res.json({success:true});
                            return;
                        } else if (dim_cmd_cnt < 12) {
                            res.json({success:false,msg:'Requested brightness is too similar to current light brightness.'});
                            return;
                        }
                        python_options.args.push('lightd');
                        python_options.args.push(dim_cmd_cnt);
                        run_fan_remote((success) => {
                            if (success) {
                                fan['light_brightness'] = brightness_0to1;
                                res.json({success:true});
                            } else {
                                res.status(500).json({success:false,msg:'Error with fan communication python script.'});
                            }
                        });
                    } else {
                        // user is trying to decrease light brightness
                        // turn off light first
                        python_options.args.push('light');
                        run_fan_remote((success) => {
                            if (success) {
                                fan['light_on'] = false;
                                python_options.args.pop();
                                // Send brighten/dim command.
                                python_options.args.push('lightd');
                                python_options.args.push(brightness);
                                run_fan_remote((success2) => {
                                    if (success2) {
                                        fan['light_on'] = true;
                                        fan['light_brightness'] = brightness_0to1;
                                        res.json({success:true});
                                    } else {
                                        res.status(500).json({success:false,msg:'Error with fan communication python script [2].'});
                                    }
                                });
                            } else {
                                res.status(500).json({success:false,msg:'Error with fan communication python script [1].'});
                            }
                        });
                    }
                } else {
                    if (fan['light_brightness'] > brightness_0to1) {
                        // user is trying to turn light to lower brightness than prior saved brightness.
                        python_options.args.push('lightd');
                        python_options.args.push(brightness);
                        run_fan_remote((success) => {
                            if (success) {
                                fan['light_on'] = true;
                                fan['light_brightness'] = brightness_0to1;
                                res.json({success:true});
                            } else {
                                res.status(500).json({success:false,msg:'Error with fan communication python script.'});
                            }
                        });
                    } else {
                        // user is trying to turn light on to prior saved brightness, then increase further
                        var dim_cmd_cnt = brightness - (fan['light_brightness']*MAX_DIM_CNT);
                        // First turn on light.
                        python_options.args.push('light');
                        run_fan_remote((success) => {
                            if (success) {
                                fan['light_on'] = true;
                                if (dim_cmd_cnt > 0) {
                                    python_options.args.pop();
                                    // increase brightness
                                    python_options.args.push('lightd');
                                    python_options.args.push(dim_cmd_cnt);
                                    run_fan_remote((success2) => {
                                        if (success2) {
                                            fan['light_brightness'] = brightness_0to1;
                                            res.json({success:true});
                                        } else {
                                            res.status(500).json({success:false,msg:'Error with fan communication python script [2].'});
                                        }
                                    });
                                }
                            } else {
                                res.status(500).json({success:false,msg:'Error with fan communication python script [1].'});
                            }
                        });
                    }
                }

                break;
            case 'fan_speed':
                if (req.query.value == 'wind') {
                    python_options.args.push('speed_w');
                } else {
                    var speed = parseInt(req.query.value);
                    if (isNaN(speed) || speed < 0 || speed > 6) {
                        res.status(400).json({success:false,msg:'Invalid fan speed provided.'});
                        return;
                    }
                    python_options.args.push('speed_'+speed);
                }
                run_fan_remote((success) => {
                    if (success) {
                        // Update fan status.
                        fan['fan_speed'] = speed;
                        res.json({success:true});
                    } else {
                        res.status(500).json({success:false,msg:'Error with fan communication python script.'});
                    }
                });
                break;
            default:
                fan[req.params.option] = req.query.value;
                res.json({success:true});
        }

    } else {
        res.status(400).json({success:false,msg:'Option not changed.'});
    }
});

function getFan(remoteId) {
    return fans.find(o => o.remote_id == remoteId);
}

function startServer() {
    app.listen(PORT, () => console.log(`Hub listening on port ${PORT}...`));
}

console.log('Harbor Breeze Ceiling Fan Hub');
console.log('Make sure all fans are at default state (as listed in fans.json), then press any key to start hub.');
process.stdin.setRawMode(true);
process.stdin.resume();
process.stdin.once('data', () => {
    process.stdin.setRawMode(false);
    startServer();
});

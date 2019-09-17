const express = require('express');
const {PythonShell} = require('python-shell');
const app = express();
const PORT = process.env.PORT || 8080;

const config = require('./config.json');
var fans = require('./fans.json');

app.get('/', (req, res) => {
    res.send('Harbor Breeze Hub');
});

function isValidRequest(request) {
    // Check if access code sent.
    if (!('access_code' in request.query)) {
        return false;
    }
    // Check if valid access code.
    if (request.query.access_code == config.access_code) {
        return true;
    } else {
        return false;
    }
}

// Filters out private data from object in fans.json
function getFanPublicObject(fan) {
    return {
        "remote_id": fan.remote_id,
        "displayname": fan.name,
        "light": (fan.light_on == true) ? 'on' : 'off',
        "light_brightness": fan.light_brightness,
        "fan_speed": fan.fan_speed,
        "fan_mode": fan.fan_mode
    };
}

app.get('/api/fans', (req, res) => {
    if (!isValidRequest(req)) {
        res.status(403).json({success:false,msg:'Invalid hub access code.'});
        return;
    }
    // Return list of all fans added to hub.
    var fanslist = [];
    for (var i = 0; i < fans.length; i++) {
        fanslist.push(getFanPublicObject(fans[i]));
    }
    res.json({success:true,result:fanslist});
});

app.get('/api/fans/:remote_id', (req, res) => {
    if (!isValidRequest(req)) {
        res.status(403).json({success:false,msg:'Invalid hub access code.'});
        return;
    }
    var fan = getFan(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan with this id does not exist.'});
    } else {
        res.json({success:true,result:getFanPublicObject(fan)});
    }
});

app.get('/api/fans/:remote_id/:option', (req, res) => {
    if (!isValidRequest(req)) {
        res.status(403).json({success:false,msg:'Invalid hub access code.'});
        return;
    }
    var fan = getFan(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan with this remote id does not exist.'});
    } else {
        fan = getFanPublicObject(fan);
        if (req.params.option in fan) {
            var result_json = {};
            result_json[req.params.option] = fan[req.params.option];
            res.json({success:true,result:result_json});
        } else {
            res.status(404).json({success:false,msg:'Option does not exist for fan.'});
        }
    }
});

app.put('/api/fans/:remote_id/:option', (req, res) => {
    if (!isValidRequest(req)) {
        res.status(403).json({success:false,msg:'Invalid hub access code.'});
        return;
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
    var public_fan = getFanPublicObject(fan);
    // Check if option is invalid.
    if (!(req.params.option in public_fan)) {
        res.status(404).json({success:false,msg:'Option does not exist for fan.'});
        return;
    }

    // Convert to lower case
    req.query.value = req.query.value.toLowerCase();

    // Check if new value provided for option.
    if (req.params.option == 'light_brightness' || public_fan[req.params.option].toString() != req.query.value) {
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
            case 'light':
                console.log('Received request to turn light ' + req.query.value + ', (current: ' + fan['light_on'] + ')');
                if (!(req.query.value == 'on' || req.query.value == 'off')) {
                    res.status(400).json({success:false,msg:'Invalid fan light state provided.'});
                    return;
                }
                // Toggle fan light.
                python_options.args.push('light');
                console.log('updating light: ' + req.query.value + ', current: ' + fan['light_on']);
                run_fan_remote((success) => {
                    if (success) {
                        // Update fan option.
                        fan['light_on'] = (fan['light_on']) ? false : true;
                        res.json({success:true});
                    } else {
                        res.status(500).json({success:false,msg:'Error with fan communication python script.'});
                    }
                });
                break;
            case 'light_brightness':
                const MAX_DIM_CNT = 220.0;
                var brightness = parseInt(req.query.value);
                console.log('Received request to set brightness to ' + brightness + ', (current: ' + fan['light_brightness'] + ')');
                fan['light_brightness'] = brightness/100.0;
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
                console.log('setting light to: ' + brightness_0to1);
                
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
                        console.log('increasing light by: ' + dim_cmd_cnt);
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
                                console.log('turning fan on first');
                                fan['light_on'] = false;
                                python_options.args.pop();
                                // Send brighten/dim command.
                                console.log('increastinb brightness by ' + brightness);
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

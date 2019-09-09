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
    res.send(fans);
});

app.get('/api/fans/:remote_id', (req, res) => {
    var fan = getFan(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:0,msg:'Fan with this remote id does not exist.'});
    } else {
        res.send(fan);
    }
});

app.get('/api/fans/:remote_id/:option', (req, res) => {
    var fan = getFan(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:0,msg:'Fan with this remote id does not exist.'});
    } else {
        if (req.params.option in fan) {
            res.send(fan[req.params.option]);
        } else {
            res.status(404).json({success:0,msg:'Option does not exist for fan.'});
        }
    }
});

app.put('/api/fans/:remote_id/:option', (req, res) => {
    // Check if correct query data passed.
    if (!('value' in req.query)) {
        res.status(400).json({success:0,msg:'Missing new value.'});
        return;
    }
    // Get fan from remote id.
    var fan = getFan(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:0,msg:'Fan with this remote id does not exist.'});
        return;
    }
    // Check if option is invalid.
    if (!(req.params.option in fan)) {
        res.status(404).json({success:0,msg:'Option does not exist for fan.'});
        return;
    }
    // Check if new value provided for option.
    if (fan[req.params.option].toString() != req.query.value) {
        var python_options = {
            mode: 'text',
            pythonPath: 'python3',
            args: [fan.remote_id]
        };
        function run_fan_remote(successCallback) {
            PythonShell.run('FanRemote.py', python_options, function(err, results) {
                if (err) {
                    res.status(500).json({success:0,msg:'Error with fan communication module.'});
                }
                res.json({success:1});
                if (typeof successCallback === 'function') {
                    successCallback();
                }
            });
        }
        switch(req.params.option) {
            case 'light_on':
                // Toggle fan light.
                python_options.args.push('light');
                run_fan_remote(() => {
                    // Update fan option.
                    var setLightOn = (req.query.value.toLowerCase() == 'true');
                    fan[req.params.option] = setLightOn;
                });
                break;
            default:
                fan[req.params.option] = req.query.value;
        }
    } else {
        res.status(400).json({success:0,msg:'Option not changed.'});
    }
});

function getFan(remoteId) {
    return fans.find(o => o.remote_id == remoteId);
}

function startServer() {
    app.listen(PORT, () => console.log(`Hub listening on port ${PORT}...`));
}

console.log('Harbor Breeze Ceiling Fan Hub');
console.log('Turn off all fans, then press any key to start hub.');
process.stdin.setRawMode(true);
process.stdin.resume();
process.stdin.once('data', () => {
    process.stdin.setRawMode(false);
    startServer();
});

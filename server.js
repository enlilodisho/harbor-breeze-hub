const express = require('express');
const app = express();
const RFTransmitter = require('./RFTransmitter.js');
const HarborBreezeHub = require('./HarborBreezeHub.js');
const config = require('./config.json');

console.log("Harbor Breeze Hub - v" + process.env.npm_package_version);
var transmitter = new RFTransmitter(config.transmitter_pin);
transmitter.start();
var hbhub = new HarborBreezeHub(transmitter, config.my_fans);
const hbhub_api = require('./routes/hbhub_api1.js')(hbhub);

// Check access code for /api access.
app.use('/api', function(req, res, next) {
    var authenticated = false;
    // Authentication check.
    if ('access_code' in req.query) {
        if (req.query.access_code == config.access_code) {
            authenticated = true;
        }
    }
        
    if (authenticated) {
        next();
    } else {
        res.status(403).json({success:false,msg:"Invalid access code."});
    }
});
// Assign api routes.
app.use('/api', hbhub_api);

// Start listening for requests.
app.listen(config.network_port);

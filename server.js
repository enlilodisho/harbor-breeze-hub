const express = require('express');
const hbhub_api = require('./routes/hbhub_api1.js');
const RFTransmitter = require('./RFTransmitter.js');
const HarborBreezeHub = require('./HarborBreezeHub.js');
const config = require('./config.json');

const app = express();

console.log("Harbor Breeze Hub - v" + process.env.npm_package_version);
// Start rf transmitter.
var transmitter = new RFTransmitter(config.transmitter_pin);
transmitter.start();
// Load all fans.
// TODO: Load HarborBreezeHub and pass to hbhub_api
//new HarborBreezeHub(transmitter, config.my_fans);

// Assign all routes.
app.use('/api', hbhub_api);

// Start listening for requests.
app.listen(config.network_port);

var RFTransmitter = require('./RFTransmitter.js');

var transmitter = new RFTransmitter(21);

var light = [400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000];

//var threesecon = [3000000, 3000000];
var threesecon = [30000, 3000];

//var light12 = light.concat(light).concat(light).concat(light).concat(light).concat(light).concat(light).concat(light).concat(light).concat(light).concat(light).concat(light).concat(light);

//transmitter.transmit(threesecon);
//transmitter.transmit(light12);
transmitter.transmit(light);

//transmitter.destroy();

const RFTransmitter = require('./RFTransmitter.js');
const remoteconfig = require('./fanremote_config.json');

// Build timing array from array of timing ids (SS, SL, etc.)
var buildTimingsArray = function(timingIds, timings) {
    var cmdTimings = [];
    for (var i = 0; i < timingIds.length; i++) {
        switch (timingIds[i]) {
            case 'SS':
                cmdTimings.push(timings.SHORT_ON);
                cmdTimings.push(timings.SHORT_OFF);
                break;
            case 'SL':
                cmdTimings.push(timings.SHORT_ON);
                cmdTimings.push(timings.LONG_OFF);
                break;
            case 'LL':
                cmdTimings.push(timings.LONG_ON);
                cmdTimings.push(timings.LONG_OFF);
                break;
            case 'LS':
                cmdTimings.push(timings.LONG_ON);
                cmdTimings.push(timings.SHORT_OFF);
                break;
            case 'SR':
                cmdTimings.push(timings.SHORT_ON);
                cmdTimings.push(timings.REST);
                break;
        }
    }
    return cmdTimings;
};

function FanRemote() {
    this.timings = remoteconfig.timings;
    this.remoteIds = remoteconfig.remote_ids;  // All available remote ids.
    this.commands = remoteconfig.commands; // All available commands.
    this.transmitter = new RFTransmitter(remoteconfig.transmitter_pin);
}

module.exports = FanRemote;

FanRemote.prototype.sendCommand = function(cmdName, remoteId) {
    // Validate parameters.
    if (!(remoteId in this.remoteIds)) {
        throw new Error("Invalid remote id.");
    }
    if (!(cmdName in this.commands)) {
        throw new Error("Invalid fan command.");
    }
    // Build transmission.
    var command = this.remoteIds[remoteId].concat(this.commands[cmdName]);
    var cmdTimings = buildTimingsArray(command, this.timings);
    // Send command.
    this.transmitter.transmit(cmdTimings);
};

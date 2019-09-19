const remoteconfig = require('./fanremote_config.json');

// Build timing array from array of timing ids (SS, SL, etc.)
var buildTimingsArray = function(timingIds) {
    var cmdTimings = [];
    for (var i = 0; i < timingIds.length; i++) {
        switch (timingIds[i]) {
            case 'SS':
                cmdTimings.push(FanRemote.timings.SHORT_ON);
                cmdTimings.push(FanRemote.timings.SHORT_OFF);
                break;
            case 'SL':
                cmdTimings.push(FanRemote.timings.SHORT_ON);
                cmdTimings.push(FanRemote.timings.LONG_OFF);
                break;
            case 'LL':
                cmdTimings.push(FanRemote.timings.LONG_ON);
                cmdTimings.push(FanRemote.timings.LONG_OFF);
                break;
            case 'LS':
                cmdTimings.push(FanRemote.timings.LONG_ON);
                cmdTimings.push(FanRemote.timings.SHORT_OFF);
                break;
            case 'SR':
                cmdTimings.push(FanRemote.timings.SHORT_ON);
                cmdTimings.push(FanRemote.timings.REST);
                break;
        }
    }
    return cmdTimings;
};

function FanRemote(transmitter, remoteId) {
    this.transmitter = transmitter;
    if (!(remoteId in FanRemote.remoteIds)) {
        throw new Error("Invalid remote id.");
    }
    this.remoteTimings = FanRemote.remoteIds[remoteId];
}


/* Init static variables for FanRemote. */

FanRemote.timings = remoteconfig.timings;
// Init all available remote ids.
FanRemote.remoteIds = {};
for (var remote_id in remoteconfig.remote_ids) {
    FanRemote.remoteIds[remote_id] = buildTimingsArray(remoteconfig.remote_ids[remote_id]);
}
// Init all available commands.
FanRemote.commands = {};
for (var command in remoteconfig.commands) {
    FanRemote.commands[command] = buildTimingsArray(remoteconfig.commands[command]);
}

module.exports = FanRemote;

// Send command to fan.
FanRemote.prototype.sendCommand = function(cmdName) {
    // Check if valid command.
    if (!(cmdName in FanRemote.commands)) {
        throw new Error("Invalid fan command.");
    }
    // Build transmission.
    var cmdTimings = this.remoteTimings.concat(FanRemote.commands[cmdName]);
    // Send command.
    this.transmitter.transmit(cmdTimings);
};

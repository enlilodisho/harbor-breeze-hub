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
FanRemote.commands = remoteconfig.commands;
function init_commands_recursive(commands) {
    for (var item in commands) {
        if (Array.isArray(commands[item])) {
            commands[item] = buildTimingsArray(commands[item]);
        } else {
            init_commands_recursive(commands[item]);
        }
    }
}
init_commands_recursive(FanRemote.commands);

module.exports = FanRemote;

// Send command to fan.
FanRemote.prototype.sendCommand = function(cmdTimings, repeat=null) {
    // Build transmission.
    cmdTimings = this.remoteTimings.concat(cmdTimings);
    // Send command.
    if (repeat != null) {
        this.transmitter.transmit(cmdTimings, repeat);
    } else {
        this.transmitter.transmit(cmdTimings);
    }
};

// Turn on/off light.
FanRemote.prototype.toggleLight = function() {
    this.sendCommand(FanRemote.commands.light.power);
};

// Adjust light brightness.
FanRemote.prototype.adjustLight = function(value=null) {
    this.sendCommand(FanRemote.commands.light.dim, value);
};

// Turn on/off fan.
FanRemote.prototype.toggleFan = function() {
    this.sendCommand(FanRemote.commands.fan.power);
};

// Turn on/off chime.
FanRemote.prototype.toggleChime = function() {
    this.sendCommand(FanRemote.commands.chime);
};

// Turn on fan to speed.
FanRemote.prototype.setFanSpeed = function(speed) {
    if (!(speed in FanRemote.commands.fan.speed)) {
        throw new Error("Invalid fan speed.");
    }
    this.sendCommand(FanRemote.commands.fan.speed[speed]);
};

// Change fan rotation.
FanRemote.prototype.setFanRotation = function(direction) {
    var rotation_key = "rotate_" + direction;
    if (!(rotation_key in FanRemote.commands.fan)) {
        throw new Error("Invalid fan rotation");
    }
    this.sendCommand(FanRemote.commands.fan[rotation_key]);
};

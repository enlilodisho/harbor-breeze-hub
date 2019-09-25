const readline = require('readline');
const RFTransmitter = require('./RFTransmitter.js');
const FanRemote = require('./FanRemote.js');
const remoteconfig = require('./fanremote_config.json');
const config = require('./config.json');

// Create instance of readline
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

console.log("Fan Remote Pairing Utility");
console.log("");
console.log(">> Before you start, please completely shut off the electricity to your fan.");

rl.question("Press the ENTER key when the fan has no power.", function(_) {
    console.log("Great, now let's get started!");
    setTimeout(() => {
        console.log("");
        pairUtility();
    }, 1000);
});

function pairUtility() {
    // print list of all valid remote ids.
    console.log("Available remote ids:");
    for (var rid in remoteconfig.remote_ids) {
        console.log("--> " + rid);
    }
    rl.question("Which remote id do you want to use for the fan? ", function(remote_id) {
        if (!(remote_id in remoteconfig.remote_ids)) {
            console.log("ERROR. This remote id is invalid.");
            rl.close();
            return;
        }
        console.log("Got it! We are now ready to pair this remote id to the fan.");
        console.log();

        var transmitter = new RFTransmitter(config.transmitter_pin);
        transmitter.start();
        var remote = new FanRemote(transmitter, remote_id);
        rl.question("Turn on the power to the fan then press the ENTER key within 30 seconds.", function(_) {
            rl.close();
            remote.pairFanRemote(); // pair fan remote
            console.log("Pair command sent to fan. The fan should now be paired to this remote id.");
            setTimeout(function() {
                transmitter.stop();
            }, 3000);
        });
    });
}

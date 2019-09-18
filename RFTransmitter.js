const GPIO = require('onoff').Gpio;

//var queueDelayUs = 10000; // Microseconds to wait before dequeing next transmission.

// Processes first item in queue.
var processQueue = function(transmitter) {
    if (transmitter.transmitting) {
        return; // Already transmitting something else.
    }
    transmitter.transmitting = true;

    // Dequeue
    var dataToSend = transmitter.queue.shift();
    if (dataToSend == null) {
        transmitter.transmitting = false;
        return; // Nothing in queue.
    }
    // Transmit
    transmitDataAsync(transmitter, dataToSend);
};

var transmitDataSync = function(transmitter, timings) { 
    var nextValue = 1;
    var nextDelay = timings.shift();
    while (nextDelay != null) {
        transmitter.rf.writeSync(nextValue);
        nextValue = (nextValue == 0) ? 1 : 0;
        delayus(nextDelay);
        nextDelay = timings.shift();
    }
    transmitter.transmitting = false;
};

var transmitDataAsync = function(transmitter, timings, value=1) {
    // Get next time.
    var nextDelay = timings.shift();
    if (nextDelay == null) {
        transmitter.transmitting = false;
        return true; // Done transmitting.
    }
    transmitter.rf.write(value, (err) => {
        if (err) {
            transmitter.transmitting = false;
            return;
        }
        delayus(nextDelay);
        transmitDataAsync(transmitter, timings, (value == 0) ? 1 : 0);
    });
};

// Suspends thread execution for x microseconds.
var delayus = function(us) {
    var wait = true;
    const ns = us * 1000; // convert microseconds to nanoseconds.
    const start = process.hrtime.bigint();
    while (wait) {
        var ns_passed = process.hrtime.bigint() - start;
        if (ns_passed >= ns) {
            wait = false;
        }
    }
}

function RFTransmitter(rfpin) {
    this.rfpin = rfpin;
    // Init gpio pin as output
    this.rf = new GPIO(rfpin, 'out');
    this.queue = []; // Queue with data to transmit.

    this.transmitting = false;
    this.queueProcessor = setInterval(processQueue.bind(null, this), 1);
}

module.exports = RFTransmitter;

// Stop queueProcessor and de-initialize rf gpio pin.
RFTransmitter.prototype.destroy = function() {
    clearInterval(this.queueProcessor);
    this.rf.unexport();
}

// Adds array of high/low timings to transmit in queue.
// Must be an even number of timings, as RF transmitter must always end in LOW state.
RFTransmitter.prototype.transmit = function(timings, count=12) {
    // Parameter validation
    if (timings.length % 2 != 0) {
        throw new Error("RF Transmitter must end in LOW state after transmission.");
    } else if (timings.length == 0) {
        throw new Error("Missing data to transmit.");
    }
    // Add repeated timings.
    var timings_repeated = [];
    for (var n = 0; n < count; n++) {
        timings_repeated = timings_repeated.concat(timings);
    }
    // Add to queue.
    this.queue.push(timings_repeated);
};

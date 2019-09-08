const express = require('express');
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
    var fan = fans.find(o => o.remote_id == req.params.remote_id);
    if (fan == null) {
        res.status(404).send('Fan with this remote id does not exist.');
    } else {
        res.send(fan);
    }
});

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

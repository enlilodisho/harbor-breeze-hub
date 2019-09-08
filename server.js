const express = require('express');
var app = express();

app.get('/', function(req, res) {
    res.send('Hello World');
});

var server = app.listen(8080, function() {
    var host = server.address().address;
    var port = server.address().port;
    console.log("Hub listening for requests at http://%s:%s", host, port);
});

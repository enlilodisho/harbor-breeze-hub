const config = require('../config.json');
const express = require('express');
const router = express.Router();

router.get('/', function(req, res) {
    var output = "<h1>Harbor Breeze Fan Control API v1</h1><hr>";
    if (module.hbhub == null) {
        output += "<span style='color:red;'>Hub error.</span>";
        res.status(500);
    } else {
        output += "Ready.";
        res.status(200);
    }
    res.send(output);
});

module.exports = router;

const config = require('../config.json');
const express = require('express');
const router = express.Router();

router.get('/', function(req, res) {
    var output = "<h1>Harbor Breeze Fan Control API v1</h1><hr>";
    if (router.hbhub == null) {
        output += "<span style='color:red;'>Hub error.</span>";
        res.status(500);
    } else {
        output += "Ready.";
        res.status(200);
    }
    res.send(output);
});

router.get('/fans', function(req, res) {
    var fansJson = [];
    for (var fanRid in router.hbhub.fans) {
        fansJson.push(router.hbhub.fans[fanRid].getJsonFan());
    }
    res.json({success:true,data:fansJson});
});

router.get('/fans/:remote_id', function(req, res) {
    if (!(req.params.remote_id in router.hbhub.fans)) {
        res.status(404).json({success:false,msg:'Fan remote id does not exist.'});
        return;
    }
    var fan = router.hbhub.fans[req.params.remote_id].getJsonFan();
    res.json({success:true,data:fan});
});

module.exports = function(hbhub) {
    router.hbhub = hbhub;
    return router;
};

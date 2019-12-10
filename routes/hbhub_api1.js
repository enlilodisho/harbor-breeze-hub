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


/* GETTERS */ 

// Returns all fans along with their capabilities.
router.get('/fans', function(req, res) {
    var fansJson = [];
    for (var fanRid in router.hbhub.fans) {
        fansJson.push(router.hbhub.fans[fanRid].getJsonFan());
    }
    res.json({success:true,data:fansJson});
});

// Returns fan capabilities.
router.get('/fans/:remote_id', function(req, res) {
    var fan = getFanByRemoteId(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan remote id does not exist.'});
        return;
    }
    res.json({success:true,data:fan.getJsonFan()});
});

// Returns fan current light state.
router.get('/fans/:remote_id/light', function(req, res) {
    var fan = getFanByRemoteId(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan remote id does not exist.'});
        return;
    }
    res.json({success:true,data:{
        light: fan.light,
        light_brightness: fan.lightBrightness
    }});
});

// Returns fan current fan state.
router.get('/fans/:remote_id/fan', function(req, res) {
    var fan = getFanByRemoteId(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan remote id does not exist.'});
        return;
    }
    res.json({success:true,data:{
        fan: fan.fan,
        fan_speed: fan.fanSpeed,
        fan_direction: fan.fan_direction
    }});
});


/* SETTERS */

// Control fan light.
router.put('/fans/:remote_id/light', function(req, res) {
    var fan = getFanByRemoteId(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan remote id does not exist.'});
        return;
    }
    if ('power' in req.query) {
        var lightPower = req.query.power.toLowerCase();
        if (lightPower == 'on') {
            fan.turnOnLight();
        } else if (lightPower == 'off') {
            fan.turnOffLight();
        } else {
            res.status(400).json({success:false,msg:'Invalid fan light power provided.'});
            return;
        }
    }
    // Light brightness.
    if ('brightness' in req.query) {
        var lightBrightness = parseFloat(req.query.brightness);
        if (!isNaN(lightBrightness)) {
            if (lightBrightness < 0.01) lightBrightness = 0.01;
            else if (lightBrightness > 1) lightBrightness = 1.0;
            fan.adjustLightBrightness(lightBrightness);
        } else {
            res.status(400).json({success:false,msg:'Invalid fan light brightness provided.'});
            return;
        }
    }
    res.json({success:true});
});

// Control fan.
router.put('/fans/:remote_id/fan', function(req, res) {
    var fan = getFanByRemoteId(req.params.remote_id);
    if (fan == null) {
        res.status(404).json({success:false,msg:'Fan remote id does not exist.'});
        return;
    }
    if ('direction' in req.query) {
        var fanDir = req.query.direction.toLowerCase();
        if (fanDir == 'ccw' || fanDir == 'summer') {
            fan.setFanRotation('ccw');
        } else if (fanDir == 'cw' || fanDir == 'winter') {
            fan.setFanRotation('cw');
        } else {
            res.status(400).json({success:false,msg:'Invalid fan direction provided.'});
            return;
        }       
    }
    if ('power' in req.query) {
        var fanPower = req.query.power.toLowerCase();
        if (fanPower == 'off') {
            fan.turnOffFan();
        } else if (fanPower == 'on') {
            if ('speed' in req.query) {
                fan.turnOnFan(req.query.speed);
            } else {
                fan.turnOnFan();
            }
        } else {
            res.status(400).json({success:false,msg:'Invalid fan power provided.'});
            return;
        }
    }
    res.json({success:true});
});

module.exports = function(hbhub) {
    router.hbhub = hbhub;
    return router;
};

// Get fan in Harbor Breeze Hub from remote id.
function getFanByRemoteId(remoteId) {
    if (!(remoteId in router.hbhub.fans)) {
        return null;
    }
    return router.hbhub.fans[remoteId];
}

# Harbor Breeze Hub

### DISCLAIMER: ALPHA SOFTWARE!!

_This software may not work as intended, or may not work at all. It it is currently under heavy development._

Use at your own risk.

-----------------

Do you have a Harbor Breeze ceiling fan that is controlled by a remote like the one below?
![Harbor Breeze Remote](https://i.imgur.com/wlD3SRFl.jpg =201x268)
Then with this project -- along with a Raspberry Pi and 315MHz RF Transmitter -- you will be able to make your dumb ceiling fan smart!

#### Before you get started:

Please note that this is a **smart hub** (or sometimes called a 'bridge') for Harbor Breeze ceiling fans controlled with a very specific remote. Using only this project, you will be able to control the fan by sending HTTP requests to the hub, however, this is not ideal to use everyday. Most likely you will want to control your fans using an app on your phone. This is not possible using only this hub. Currently, I am working on a [homebridge](https://github.com/nfarina/homebridge) plugin that will allow you to communicate with this hub and control your ceiling fan using Apple HomeKit. To learn more about this, visit the [homebridge-harbor-breeze-hub repository](https://github.com/enlilodisho/homebridge-harbor-breeze-hub/).

Furthermore, keep in mind that I have only reverse engineered the RF signals for **my** remotes (with help from this [post](https://www.instructables.com/id/Reverse-Engineer-RF-Remote-Controller-for-IoT/)).

The signals sent to the fan are composed of 3 parts:
1. Prefix: This is unique to each remote, thus I call this the "remote id" (although it is not an id, rather a sequence of on/off signals). I have access to 3 Harbor Breeze remotes, and I noticed that the remote id is the same for all of them when the dip-switch on the back of the remote is set to 0. However, when it is set to to 1, the remote id differs across the remotes. I have provided all the remote ids for my remotes, although none of these remote ids may work for your fan. If this is the case, you may need to reverse engineer the remote id yourself using a [RTL-SDR USB Tuner](https://www.amazon.com/RTL-SDR-Blog-RTL2832U-Software-Defined/dp/B011HVUEME/ref=sr_1_3). For help with this, please see the link above for the post that helped me find my remote ids.
2. Message: This tells the fan what to do. If you have the same type of remote as me, these should be the same for us. Although, some messages may not work for your fan if it is missing the capability. For example, my fan has the ability to change directions using a slider switch on the remote. If your remote does not have this switch, then chances are that this feature won't work if you send the fan_direction message thru the hub.
3. Suffix: All signals end with this sequence.

## Creating the hub.

To get started you will need:

- Raspberry Pi (any **should** work, but hub was only tested on a Pi 3 Model B+)
- 315MHz Transmitter (I used [this one](https://www.amazon.com/HiLetgo-Transmitter-Receiver-Arduino-Raspberry/dp/B00LNADJS6/).)
- A way to connect the transmitter to the Pi (such as a breadboard and Male <--> Female wires)
- A wire to act as the transmitter antenna (you will need to solder this wire  to the transmitter)

Instructions:

![315MHz RF Transmitter](https://i.imgur.com/Lyn5IaL.png =300x283)
1. Connect the power pin on the transmitter to any 5v pin on the Pi.
2. Connect the ground pin on the transmitter to any ground pin on the Pi.
3. Connect the signal pin on the transmitter to any GPIO pin on the Pi.
4. Solder a wire to the antenna hole in the transmitter.


## Installing & setting up the hub.

I am assuming that you have set up your Raspberry Pi properly and it is connected to the internet.
I am also assuming that:

- You are using Raspbian Buster Lite as the operating system, however other ones should also work.
- Your Raspberry Pi is up-to-date (run `sudo apt-get update`).
- You have Node.js and npm installed (Hub tested on node version 12.10.0)
- You have decent computer skills.

1. First clone the repository to your raspberry pi and `cd` into the repository.
```
git clone https://github.com/enlilodisho/harbor-breeze-hub.git
cd harbor-breeze-hub-master
```
2. Run the following to install the hub.
```
npm install
```
4. Take a look at `fanremote_config.json`, but don't change anything. Take a look at all the "remote_ids" available. At the time of writing this, I have provided ids: 0, 1_1, 1_2, and 1_3.
5. Now lets set up the hub for your fans. Open the `config.json` file in any text editor. Adjust all the settings as needed. Explanation for each setting is below.

| Setting         | Description
|-----------------|-------------------
| network_port    | The port the hub runs on. You can change this if there is something else running on this port that conflicts with the hub.
| access_code     | The hub only responds to requests sent with this code. Think of it as a pasword to control your fans.
| transmitter_pin | The GPIO pin that the RF transmitter's signal pin is connected to. Take a look at [pinout.xyz](https://pinout.xyz) to find the pin number. Use the BCM (Broadcom) pin number, NOT the board pin number.
| my_fans         | Contains all the fans you want the hub to control.

To add a fan to the hub, change the template below so that it corresponds to your fan, then insert it in the `my_fans` object in `config.json`.
#### Fan JSON Template
```json
"fan_remote_id": {
	"name": "Bedroom 1 fan",
	"has_light": true,
	"dimmable": false,
	"max_dim_steps": 500,
	"max_speed": 6,
	"has_breeze": true,
	"has_rotation": true
}
```

##### Explanation of the template:

| Option         | Description
|----------------|-------------------
| name           | The fan name. Currently isn't very important.
| has_light      | 'true' if fan has a light, 'false' if fan does not have a light
| dimmable       | 'true' if you can change fan light's brightness, 'false' if you cannot (currently this feature is not implemented)
| max_dim_steps  | leave this set to 500 unless you know what you're doing. This is the number of times we have to send the brighten/dim light command to get the light from 1% to 100% and from 100% to 1% brightness.
| max_speed      | The maximum speed the fan can be set to. If you are using the correct remote this should be 6. Please note if setting a number greater than 6 then you will have to add the proper remote message in `fanremote_config.json` as well as modify some source code.
| has_breeze     | 'true' if your fan has the breeze functionality where it randomly goes fast and then slows down, 'false' otherwise. This button is between the 1 and 6 speed buttons.
| has_rotation   | 'true' if you can change fan rotation from the fan remote, 'false' otherwise.

##### Fan Remote Id

Make sure you change the 'fan_remote_id' to your fan's remote id. You may have to keep trying different ids until you find the correct one.
Remove the battery cover from the back of your fan's remote. If the dip-switch is set to 0, first try the remote id '0'. If the dip-switch is set to 1, first try the remote ids that begin with '1'. If none of the provided remote ids work, your dip-switch is set to 1, and you have no other Harbor Breeze ceiling fan with their remote's dip-switch set to 0, then you may be able to switch the dip-switch on the remote to 0, re-pair your remote with the fan, and then try remote id '0'.
If you are not able to get any of the remote ids I provided to work, then as mentioned at the beginning, you will have to reverse engineer the signal and find your remote id yourself. After finding it, you will have to add your remote id in the `fanremote_config.json` file.


## Running the hub.

To run the hub run:
```
npm start
```
To stop the hub press the CTRL+C key.

Instructions to start the hub automatically when the Raspberry Pi boot and connects to the internet are coming soon. However if you are tech-savvy you should be able to figure this out with help from DuckDuckGo (or Google).

## Controlling your fans (Harbor Breeze Hub API)

Communication with the hub is done through HTTP GET/PUT requests. Feel free to create apps/plugins/etc. that use this hub to control your fans!

### Hub Responses
All responses are in JSON and contain the 'success' key.
Sample success responses:
```json
{success:true}
```
```json
{success:true,data:/*expected data*/}
```
Sample error responses:
```json
{success:false}
```
```json
{success:false,msg:'Invalid fan remote id.'}
```

### All available requests:

#### Get Available Fans

[GET] /api/fans
This is used to get a list of all the fans available and their capabilities.

| Parameters    | Type     | Description
|---------------|----------|-----------------
| access_code   | string   | Hub access code (in `config.json`)

Sample output:
```json
{
	"success": true,
	"data": [
		{
			"remote_id": "0",
			"has_light": true,
			"dimmable": false,
			"max_dim_steps": 500,
			"has_breeze": true,
			"has_rotation": true
		},
		{
			"remote_id": "1_2",
			"has_light": true,
			"dimmable": false,
			"max_dim_steps": 500,
			"has_breeze": true,
			"has_rotation": false
		}
	]
}
```

#### Get Fan Capabilities

[GET] /api/fans/:remote_id
This is used to get a fans capabilities.

| Parameters    | Type     | Description
|---------------|----------|-----------------
| access_code   | string   | Hub access code (in `config.json`)

Sample output for `/api/fans/0`:
```json
{
	"success": true,
	"data": {
		"remote_id": "0",
		"has_light": true,
		"dimmable": false,
		"max_dim_steps": 500,
		"has_breeze": true,
		"has_rotation": true
	}
}
```

#### Get Fan Current State

[GET] /api/fans/:remote_id/fan
This is used to get a fans current state.

| Parameters    | Type     | Description
|---------------|----------|-----------------
| access_code   | string   | Hub access code (in `config.json`)

Sample output for `/api/fans/0/fan`:
```json
{
	"success": true,
	"data": {
		"fan": "off",
		"fan_speed": 3,
		"fan_direction": "ccw"
	}
}
```

#### Get Fan Current Light State

[GET] /api/fans/:remote_id/light
This is used to get a fans current light state.

| Parameters    | Type     | Description
|---------------|----------|-----------------
| access_code   | string   | Hub access code (in `config.json`)
| direction     | string   | 'ccw'/'summer' to spin fan counter-clockwise; 'cw'/'winter' to spin fan clockwise
| power         | string   | 'on' to turn fan on, 'off' to turn fan off
| speed         | string   | speed to set turn fan on to. Ignored when power parameter is not 'on'.

Sample output for `/api/fans/0/light`:
```json
{
	"success": true,
	"data": {
		"light": "on",
		"light_brightness": 0.5
	}
}
```

#### Update Fan

[PUT] /api/fans/:remote_id/fan
This is used to turn a fan on/off, change it's speed and/or direction.

| Parameters    | Type     | Description
|---------------|----------|-----------------
| access_code   | string   | Hub access code (in `config.json`)
| power         | string   | 'on' to turn light on, 'off' to turn light off

Sample output for `/api/fans/0/light`:
```json
{
	"success": true
}
```

#### Update Fan Light

[PUT] /api/fans/:remote_id/light
This is used to turn a fan's light on/off and set it's brightness.

| Parameters    | Type     | Description
|---------------|----------|-----------------
| access_code   | string   | Hub access code (in `config.json`)
| power         | string   | 'on' to turn light on, 'off' to turn light off

Sample output for `/api/fans/0/light`:
```json
{
	"success": true
}
```

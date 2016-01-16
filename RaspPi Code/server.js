var app = require('express').createServer();
var io = require('socket.io').listen(app,{log:false});

io.set('browser client minification', true);  // send minified client
io.set('browser client etag', true);          // apply etag caching logic based on version number

app.listen(8080);		// bind to port 8080, listen for both http and socket requests here

// this handles the sending of files
app.get('/', function (req, res) { res.sendfile(__dirname + '/index.html');});
app.get('/icon/', function (req, res) { res.sendfile(__dirname + '/icon.png');});
app.get('/startTheScion/', function () {console.log('got request for start the car');});

// create objects to hold the data we get from UDP
var current_Heat_and_AC_data = '';
var current_OBD_Blinker_Door_TrunkLid_Temp_Data = '';

// Setup socket.io, listening for socket.io connections for each current Data buffer. Each data buffer gets its own namespace.
var OBD = io.of('/DUEdata'); // 
/*OBD.on('connection', function () {
    OBD.emit('message',current_OBD_Blinker_Door_TrunkLid_Temp_Data);
});*/

var DoorStuff = io.of('/HEATACdata');
/*DoorStuff.on('connection', function () {
    DoorStuff.emit('message',current_Heat_and_AC_data);
});*/

//  setup UDP 
var dgram = require("dgram");
var OBD_Blinker_Door_TrunkLid_Temp_Server = dgram.createSocket("udp4");
var Heat_and_AC_server = dgram.createSocket("udp4");

// When the server recieves a UDP packet, one of these gets called
// depending on which port recieved the packet
OBD_Blinker_Door_TrunkLid_Temp_Server.on("message", function (msg1, rinfo) {
	current_OBD_Blinker_Door_TrunkLid_Temp_Data = msg1;
	OBD.emit('message',current_OBD_Blinker_Door_TrunkLid_Temp_Data);
});

Heat_and_AC_server.on("message", function(msg1, rinfo) {
	current_Heat_and_AC_data = msg1;
	DoorStuff.emit('message',current_Heat_and_AC_data);
});

// Bind the UDP server to the ports we use
OBD_Blinker_Door_TrunkLid_Temp_Server.bind(41234);
Heat_and_AC_server.bind(41235);


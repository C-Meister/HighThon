var net = require('net');
var iconv = require('iconv-lite');

var server = net.createServer(function (client){
    client.on('connect', function (){
        console.log('Client connected');
        
    });
});

server.listen(1331);

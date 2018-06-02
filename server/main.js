var net = require('net');
var connection_id = 0;
var room_queue = require('./handler/room_queue');
global.iconv = require('iconv-lite');
var handler = require('./handler/handler');
/**
 * 
 * @param {String} str utf-8로 인코딩 된 문자열
 * @returns {String} euckr로 인코딩 된 문자열
 */
global.toHangul = function(str){
    return iconv.encode(str, 'euckr');
};
handler.addListener(room_queue.push);
handler.addListener(room_queue.pop);
var server = net.createServer(function (client){
    client.id = connection_id++;
    console.log('client connected');
    client.write(toHangul('한글'));
    handler.handler(client);  

    client.on('close', function(err){
        room_queue.pop.callback('', client);
    });
    client.on('error', function(err){
   //     room_queue.pop.callback('', client);

    });
});

server.on('error', function(err){
    console.log(err);
});

server.listen(1331, function () {
    console.log('TCP 서버 시작');    
});

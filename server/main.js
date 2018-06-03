var net = require('net');
var connection_id = 0;
var room_queue = require('./handler/room_queue');
var game = require('./handler/game');

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
handler.addListener(game.join);
handler.addListener(game.Move);

var server = net.createServer(function (client){
    
    client.id = connection_id++;
    console.log('client connected');
    client.write(toHangul('한글'));
    client.status = 0;
    handler.handler(client);  
    client.on('close', function(err){
        switch(client.status){
            case 0:
            case 1:
            room_queue.pop.callback('', client);
            break;
            default:
            game.onExit(client);
            break;
        }
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

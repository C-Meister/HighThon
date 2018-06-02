var net = require('net');
var iconv = require('iconv-lite');
var handler = require('./handler/handler');
/**
 * 
 * @param {String} str utf-8로 인코딩 된 문자열
 * @returns {String} euckr로 인코딩 된 문자열
 */
function toHangul(str){
    return iconv.encode(str, 'euckr');
}
handler.addListener(require('./handler/room_queue'));
var server = net.createServer(function (client){
    console.log('client connected');
    client.write(toHangul('한글'));
    handler.handler(client);  
});

server.listen(1331, function () {
    console.log('TCP 서버 시작');    
});

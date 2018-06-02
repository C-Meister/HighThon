var net = require('net');
var iconv = require('iconv-lite');

/**
 * 
 * @param {String} str utf-8로 인코딩 된 문자열
 * @returns {String} euckr로 인코딩 된 문자열
 */
function toHangul(str){
    return iconv.encode(str, 'euckr');
}

var server = net.createServer(function (client){
    console.log('client generated');
    
    client.write(toHangul('한글'));
    
});

server.listen(1331, function () {
    console.log('TCP 서버 시작');    
});

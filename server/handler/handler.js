var handler = {};

var listners = [];

/**
 * @typedef Listener
 * @type {Object}
 * @property {String} event 이벤트 이름
 * @property {(data:String)=>void} 콜백 함수
 */

/**
 * 
 * @param {Listener} object 
 */
handler.addListener = function (object){
    listners.push({event : object.event, callback : object.callback});
};

/**
 * @param {net.Socket} client 
 */
handler.handler = function(client){
    client.on('data', function(data){
        data = iconv.decode(data, 'euc-kr');
        data = data.toString();
        
        for(var list of listners){
            if (data.indexOf(list.event) === 0){
                list.callback(data.split(list.event + ' ')[1], client);
                break;
            }
        }
    });
};

module.exports = handler;
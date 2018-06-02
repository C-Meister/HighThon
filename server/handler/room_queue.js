
var queuecnt = 0;

module.exports = {
    event : 'queue',
    callback : function(data){
        console.log('queue ' + data);
        queuecnt++;
    }
};
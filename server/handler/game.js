
var rooms=[];

module.exports = {
    Join : {
        event : 'join',
        callback : function(data){
            rooms.push({});
        }
    },

    Move : {
        event : 'move',
        callback : function(data){

        }
    }
}
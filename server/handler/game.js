var rooms = {};


module.exports = {
    join : {
        event : 'join',
        callback : function(data, client){
            console.log(client.status);
            if (client.status != 2)
            {
                client.write('access denied');
                return;
            }
            console.log(rooms[data]);
            if (rooms[data] == undefined){
                rooms[data] = [client];
                console.log('create room' + data);
                client.write('join_success');
            }
            else{
                rooms[data].push(client);
                console.log('join room' + data);
                client.write('join_success');
            }
        }
    },
    Move : {
        event : 'move',
        callback : function(data, client){
                  
        }
    },
    onExit : function(client){
        console.log(`close room${client.inroom}`);
        rooms[client.inroom][0].write('exitroom');
        rooms[client.inroom][1].write('exitroom');

        delete rooms[client.inroom];
    }
}
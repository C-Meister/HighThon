var rooms = {};


module.exports = {
    join: {
        event: 'join',
        callback: function (data, client) {
            console.log(client.status);
            if (client.status != 2 || client.inroom != data) {
                client.write('access denied');
                return;
            }
            if (rooms[data] == undefined) {
                rooms[data] = [client];
                console.log('create room' + data);
                client.write('join_success');
            } else {
                rooms[data].push(client);
                console.log('join room' + data);
                client.write('join_success');
            }
            client.status = 4;
        }
    },
    Move: {
        event: 'move',
        callback: function (data, client) {
            if (client.status != 4) {
                client.write('access denied');
                return;
            }
            rooms[client.inroom][0].write(data);
            rooms[client.inroom][1].write(data);
        }
    },
    onExit: function (client) {
        console.log(`close room${client.inroom}`);

            if (rooms[client.inroom][0])
                rooms[client.inroom][0].write('exitroom');
            if (rooms[client.inroom[1]])
                rooms[client.inroom][1].write('exitroom');

            delete rooms[client.inroom];
        
    }
}
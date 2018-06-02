var queuecnt = 0;
var queuematch = [];
var roomcnt = 0;

module.exports = {
    push: {
        event: 'queue',
        callback: function (data, client) {
            client.status = 1;
            console.log('queue ' + data);
            queuematch.push({
                client: client,
                name: data
            });
            queuecnt++;
            if (queuecnt % 2 === 0) {
                queuecnt -= 2;
                
                roomcnt++;
                queuematch[0].client.status = 2;
                queuematch[1].client.status = 2;
                queuematch[0].client.inroom = roomcnt;
                queuematch[1].client.inroom = roomcnt;
                /*
                client_1.status = 2;
                client_2.status = 2;
                client_1.inroom = roomcnt;
                client_2.inroom = roomcnt; */
                var client_1 = queuematch.shift();
                var client_2 = queuematch.shift();
                client_1.client.write(toHangul(`match ${client_2.name}`));
                client_2.client.write(toHangul(`match ${client_1.name}`));
                client_1.client.write(`room ${roomcnt}`);
                client_2.client.write(`room ${roomcnt}`);
                console.log(`match ${client_1.name} and ${client_2.name}`);
            }
        }
    },
    pop : {
        event : 'dequeue',
        callback:function(data, client){
            client.status = 0;
            console.log('dequeue ' + data);
            // queuematch.splice(queuematch.filter(function (el){
            //     return el.client.id !== client.id;
            // }), 1);
            const idx = queuematch.map((data)=>{return data.client.id}).indexOf(client.id);
            if (idx !== -1){
            queuematch.splice(idx, 1);
            queuecnt--;
            }
            console.log('delete ' + client.id);
        }

    }
};
var queuecnt = 0;
var queuematch = [];

module.exports = {
    push: {
        event: 'queue',
        callback: function (data, client) {
            console.log('queue ' + data);
            queuematch.push({
                client: client,
                name: data
            });
            queuecnt++;
            if (queuecnt % 2 === 0) {
                queuecnt -= 2;
                var client_1 = queuematch.shift();
                var client_2 = queuematch.shift();
                client_1.client.write(toHangul(`match ${client_2.name}`));
                client_2.client.write(toHangul(`match ${client_1.name}`));
                
                console.log(`match ${client_1.name} and ${client_2.name}`);
            }
        }
    },
    pop : {
        event : 'dequeue',
        callback:function(data, client){
            console.log('dequeue ' + data);
            // queuematch.splice(queuematch.filter(function (el){
            //     return el.client.id !== client.id;
            // }), 1);
            queuematch.splice(queuematch.map((data)=>{return data.client.id}).indexOf(client.id), 1);
            queuecnt--;
            console.log('delete ' + client.id);
        }

    }
};
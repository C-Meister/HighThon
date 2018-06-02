#include "socket.h"

#pragma comment (lib, "ws2_32.lib")

int main(void) {
	connectServer();

	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ReceiveHandler, NULL, 0, NULL);
	char buff[255] = "";
	while (1) {
		fgets(buff, sizeof(buff), stdin);
		buff[strlen(buff)-1] = 0;
		send(server, buff, strlen(buff), 0);
		memset(buff, 0, sizeof(buff));
	}
	
	return 0;
}
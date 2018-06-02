#pragma once
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>


#define ErrorHandling(X) printf(X)

SOCKET server;

int status = 0;
char lastrecv[128] = "";

void ReceiveHandler(void) {
	char msg[255] = "";
	while (recv(server, msg, sizeof(msg), 0) > 0) {
		printf("%s\n", msg);
		strcpy(lastrecv, msg);
	}
}
int connectServer(void) {

	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	char message[30] = "";
	int strLen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		printf("WSAStartup() errer!");

	server = socket(PF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET)
		printf("hSocketet() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("192.168.100.25");
	servAddr.sin_port = htons(1331);
	printf("connect\n");

	if (connect(server, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		printf("connect() error!");

	printf("recv\n");
	strLen = recv(server, message, sizeof(message) - 1, 0);
	if (strLen == -1)
		printf("read() error!");

	printf("Message from server: %s \n", message);

	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ReceiveHandler, NULL, 0, NULL);
	return 0;
}

void matching_start(const char * str) {
	char string[128] = "";
	sprintf(string, "queue %s", str);
	send(server, string, strlen(string), 0);
	status = 1;
}

void matching_end() {
	send(server, "dequeue", 7, 0);
	status = 0;
}

void join_room(int roomcnt) {
	char string[64] = "";
	sprintf(string, "join %d", roomcnt);
	send(server, string, strlen(string), 0);
	status = 2;
}
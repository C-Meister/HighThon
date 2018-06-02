#pragma once
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>


#define ErrorHandling(X) printf(X); return 0;

SOCKET server;


void ReceiveHandler(void) {
	char msg[255] = "";
	while (recv(server, msg, sizeof(msg), 0) > 0) {
		printf("%s", msg);
	}
}
int connectServer(void) {

	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	char message[30] = "";
	int strLen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() errer!");

	server = socket(PF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET)
		ErrorHandling("hSocketet() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("192.168.2.130");
	servAddr.sin_port = htons(1331);

	if (connect(server, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	strLen = recv(server, message, sizeof(message) - 1, 0);
	if (strLen == -1)
		ErrorHandling("read() error!");
	printf("Message from server: %s \n", message);

	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ReceiveHandler, NULL, 0, NULL);
	return 0;
}
#pragma once
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>


#define ErrorHandling(X) printf(X); return 0;

SOCKET server;


void ReceiveHandler(void) {
	char msg[255] = "";
	while (recv(server, msg, sizeof(msg), 0) > 0) {
		printf("%s\n", msg);
		memset(msg, 0, sizeof(msg));
	}
}
int connectServer(void) {

	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	char message[30] = "";
	int strLen;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		printf("WSAStartup() errer!\n");

	server = socket(PF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET)
		printf("hSocketet() error!\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("192.168.2.130");
	servAddr.sin_port = htons(1331);

	if (connect(server, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		printf("connect() error!\n");

	strLen = recv(server, message, sizeof(message) - 1, 0);
	if (strLen == -1)
		printf("read() error!\n");
	printf("Message from server: %s \n", message);

	return 0;
}
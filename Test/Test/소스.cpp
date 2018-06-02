#include <stdio.h>
#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib")
#define ErrorHandling(X) printf(X)

int main(void) {

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30] = "";
	int strLen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() errer!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("hSocketet() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(1331);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (strLen == -1)
		ErrorHandling("read() error!");
	printf("Message from server: %s \n", message);

	closesocket(hSocket);
	WSACleanup();
	return 0;
	
	return 0;
}
﻿// UDPClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <Winsock2.h>
#include <iostream>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return EXIT_SUCCESS;
	}

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return EXIT_SUCCESS;
	}

	//第一步：创建套接字
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//第二步：向服务器发送数据(sendto)
	sendto(sockClient, "Hell0", strlen("Hello") + 1, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//第三步：关闭套接字
	closesocket(sockClient);

	WSACleanup();
	getchar();
}
// CaptureData.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include<stdlib.h>
#include<WinSock2.h>
#include<mstcpip.h>
#pragma comment (lib,"WS2_32.lib");
//加载动态库

#define OK 0
#define ERR -1
#define HOST_NAME_LEN 256
#define BUFF_LEN 65535

int MY_PORT = 20481;  //使用的端口

typedef struct IP_HEADER
{

	char cHeadLen : 4;	//IP包头长度（Header Length）：长度4比特。这个字段的作用是为了描述IP包头的长度，因为在IP包头中有变长的可选部分。该部分占4个bit位，单位为32bit（4个字节），即本区域值 = IP头部长度（单位为bit） / (8 * 4)，因此，一个IP包头的长度最长为“1111”，即15 * 4＝60个字节。IP包头最小长度为20字节。
	char cVer : 4; //版本号（Version）：长度4比特。标识目前采用的IP协议的版本号。一般的值为0100（IPv4），0110（IPv6）

	char cTOS;
	unsigned short usLen;  //IP包总长（Total Length）：长度16比特。 以字节为单位计算的IP包的长度 (包括头部和数据)，所以IP包最大长度65535字节。
	unsigned short usIdent;//标识符（Identifier）:长度16比特。该字段和Flags和Fragment Offest字段联合使用，对较大的上层数据包进行分段（fragment）操作。路由器将一个包拆分后，所有拆分开的小包被标记相同的值，以便目的端设备能够区分哪个包属于被拆分开的包的一部分。
	unsigned short usFlagOffset; //片偏移（Fragment Offset）：长度13比特。表示该IP包在该组分片包中位置，接收端靠此来组装还原IP包。

	char cTTL;//生存时间（TTL）：长度8比特。当IP包进行传送时，先会对该字段赋予某个特定的值。当IP包经过每一个沿途的路由器的时候，每个沿途的路由器会将IP包的TTL值减少1。如果TTL减少为0，则该IP包会被丢弃。这个字段可以防止由于路由环路而导致IP包在网络中不停被转发。

	char cProtocol; //协议（Protocol）：长度8比特。标识了上层所使用的协议。
 //以下是比较常用的协议号：
  //  1    ICMP   2    IGMP   6    TCP  17    UDP    88    IGRP    89    OSPF

	unsigned short usChkSum; //头部校验（Header Checksum）：长度16位。用来做IP头部的正确性检测，但不包含数据部分。 因为每个路由器要改变TTL的值,所以路由器会为每个通过的数据包重新计算这个值。
	unsigned int   uiSrcIp;
	unsigned int   uiDstIp;
	//起源和目标地址（Source and Destination Addresses）：这两个地段都是32比特。标识了这个IP包的起源和目标地址。要注意除非使用NAT，否则整个传输的过程中，这两个地址不会改变。
}IP_HEADER_S;
int CaptureData();

void PrintData(char szBuff[],int len);
void PrintDataInfo(char szBuff[], int len);


void PrintData1(char szBuff[], int len)
{
	int i;
	IP_HEADER_S *pHeader;
	SOCKADDR_IN stAddr = { 0 };

	pHeader = (IP_HEADER_S*)szBuff;
	if (pHeader->cProtocol != 6)
	{
		//  1    ICMP ，  2    IGMP ，  6    TCP ， 17    UDP  ，  88    IGRP    89    OSPF

		//return;
	}

	printf("\nRCV:\n");
	printf("Version:%d\n", pHeader->cVer);
	printf("IP len:%d\n", pHeader->cHeadLen);
	printf("TOS:%d\n", pHeader->cTOS);
	printf("Data Len:%d\n", ntohs(pHeader->usLen)); //注意字节序转化

	printf("TTL:%d\n", pHeader->cTTL);
	printf("Protocol:%d\n", pHeader->cProtocol);

	stAddr.sin_addr.s_addr = pHeader->uiSrcIp;  //stAddr.sin_addr.s_addr 存的是网络字节序的int
												//inet_ntoa 把 in_addr 转成字符串的ip地址
	printf("Source IP:%s\n", inet_ntoa(stAddr.sin_addr));

	//目标地址
	stAddr.sin_addr.s_addr = pHeader->uiDstIp;
	printf("Dest IP:%s\n", inet_ntoa(stAddr.sin_addr));

	printf("\n");
	return;
	//printf(" RCV: %s  \n",szBuff);
	
	/*
	//打印16进制数据
	printf("\n RCV:\n");
	for (int i = 0; i < len; i++)
	{
		printf("%0x ", szBuff[i]&0xff);
	}

	return;
	*/
}


void PrintDataInfo(char szBuff[], int len)
{
	int i;
 
	//printf(" RCV: %s  \n",szBuff);

	
	/*printf("\n RCV:\n");
	for (int i = 0; i < len; i++)
	{
		printf("%0x ", szBuff[i] & 0xff);
	}
	*/
	

	 
	IP_HEADER_S *pHeader;
	SOCKADDR_IN stAddr = { 0 };

	pHeader = (IP_HEADER_S*)szBuff;
	if (pHeader->cProtocol != 6)
	{
		//  1    ICMP ，  2    IGMP ，  6    TCP ， 17    UDP  ，  88    IGRP    89    OSPF

		//return;
	}

	printf("\nRCV:\n");
	printf("Version:%d\n", pHeader->cVer);
	printf("IP len:%d\n", pHeader->cHeadLen);
	printf("TOS:%d\n", pHeader->cTOS);
	printf("Data Len:%d\n", ntohs(pHeader->usLen)); //注意字节序转化

	printf("TTL:%d\n", pHeader->cTTL);
	printf("Protocol:%d\n", pHeader->cProtocol);

	stAddr.sin_addr.s_addr = pHeader->uiSrcIp;  //stAddr.sin_addr.s_addr 存的是网络字节序的int
												//inet_ntoa 把 in_addr 转成字符串的ip地址
	printf("Source IP:%s\n", inet_ntoa(stAddr.sin_addr));

	//目标地址
	stAddr.sin_addr.s_addr = pHeader->uiDstIp;
	printf("Dest IP:%s\n", inet_ntoa(stAddr.sin_addr));

//	printf("\n");


	//打印16进制数据
	printf("\n RCV Data:\n");
	for (int i = pHeader->cHeadLen*4; i < ntohs(pHeader->usLen); i++)
	{
		printf("%0x ", szBuff[i] & 0xff);
	}
	return;
 
}

int CaptureData( )
{
	SOCKET iFd;
	char szHostName[HOST_NAME_LEN]; //主机名
	HOSTENT *pHost;

	DWORD dwInBuff = 1;
	DWORD dwOutBuff;
	DWORD dwBytesRet;
	int iRet=0;
	char szBuff[BUFF_LEN];
	SOCKADDR_IN  stAddr = { 0 };
	WSADATA stWsa;//用于打开动态链接库，是网络编程接口可用

	//0x0202, 版本号 
	if (WSAStartup(0x0202, &stWsa) != OK)
	{
		printf(" WSA Start up failed \n");
		return ERR;

	}
	 
	//获得系统 计算机名
	if (gethostname(szHostName, HOST_NAME_LEN) != OK)
	{
		WSACleanup();
		return ERR;
	}

	printf("gethostname :%s \n", szHostName);

	pHost = gethostbyname(szHostName);

	printf("my ip :%d. %d. %d. %d\n", pHost->h_addr_list[0][0] & 0xff, pHost->h_addr_list[0][1]&0xff, pHost->h_addr_list[0][2] & 0xff, pHost->h_addr_list[0][3] & 0xff );

	//整数表示的ip
	stAddr.sin_addr.s_addr = *(unsigned long *)pHost->h_addr;
	stAddr.sin_family = AF_INET;
	//大头 小头 转化
	stAddr.sin_port = htons(MY_PORT);
	//接受原始数据流，接受所有端口的 不仅仅MY_PORT
	SOCKET hTcpSocket;

	iFd = socket(AF_INET, SOCK_RAW, IPPROTO_IP);

	if (iFd < 0)
	{
		printf("socket  failed \n"); 
		WSACleanup();
		return ERR;
	}
	hTcpSocket = iFd;
	if(INVALID_SOCKET == hTcpSocket)
	{
		printf("socket failed with error: \n");
		printf("  failed %d \n", WSAGetLastError());
		WSACleanup();
		return ERR;
	}//end

	if (bind(iFd, (SOCKADDR*)&stAddr, sizeof(SOCKADDR)) != OK)
	{
		

		printf("bind  failed %d \n", WSAGetLastError());
	closesocket(iFd);

		WSACleanup();
		return ERR;
	}
	//设置ifd收包规则    SIO_RCVALL接受所有的
	iRet = WSAIoctl(iFd, SIO_RCVALL, &dwInBuff, sizeof(dwInBuff),
		&dwOutBuff, sizeof(dwOutBuff), &dwBytesRet, NULL, NULL);

	if (iRet != OK)
	{
		printf("Ioctl failed \n");

		closesocket(iFd);

		WSACleanup();
		return ERR;
	}
	printf("aceept \n");
	  int t = 0;
	while (true)
	{
		memset(szBuff, 0, BUFF_LEN);

		iRet = recv(iFd,szBuff,BUFF_LEN,0);
		if (iRet < 0)
		{
			break;

		}
		else
		{
			PrintDataInfo(szBuff, iRet);

		}


		t++;
		printf("\n	%d.\n", t);
		if (t>10)
		{
			break;

		}
	}


	printf("over \n");
	WSACleanup();
	return OK;
}

int main()
{

	CaptureData();

	system("pause");
    return 0;
}


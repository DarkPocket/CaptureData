// CaptureData.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include<stdlib.h>
#include<WinSock2.h>
#include<mstcpip.h>
#pragma comment (lib,"WS2_32.lib");
//���ض�̬��

#define OK 0
#define ERR -1
#define HOST_NAME_LEN 256
#define BUFF_LEN 65535

int MY_PORT = 20481;  //ʹ�õĶ˿�

typedef struct IP_HEADER
{

	char cHeadLen : 4;	//IP��ͷ���ȣ�Header Length��������4���ء�����ֶε�������Ϊ������IP��ͷ�ĳ��ȣ���Ϊ��IP��ͷ���б䳤�Ŀ�ѡ���֡��ò���ռ4��bitλ����λΪ32bit��4���ֽڣ�����������ֵ = IPͷ�����ȣ���λΪbit�� / (8 * 4)����ˣ�һ��IP��ͷ�ĳ����Ϊ��1111������15 * 4��60���ֽڡ�IP��ͷ��С����Ϊ20�ֽڡ�
	char cVer : 4; //�汾�ţ�Version��������4���ء���ʶĿǰ���õ�IPЭ��İ汾�š�һ���ֵΪ0100��IPv4����0110��IPv6��

	char cTOS;
	unsigned short usLen;  //IP���ܳ���Total Length��������16���ء� ���ֽ�Ϊ��λ�����IP���ĳ��� (����ͷ��������)������IP����󳤶�65535�ֽڡ�
	unsigned short usIdent;//��ʶ����Identifier��:����16���ء����ֶκ�Flags��Fragment Offest�ֶ�����ʹ�ã��Խϴ���ϲ����ݰ����зֶΣ�fragment��������·������һ������ֺ����в�ֿ���С���������ͬ��ֵ���Ա�Ŀ�Ķ��豸�ܹ������ĸ������ڱ���ֿ��İ���һ���֡�
	unsigned short usFlagOffset; //Ƭƫ�ƣ�Fragment Offset��������13���ء���ʾ��IP���ڸ����Ƭ����λ�ã����ն˿�������װ��ԭIP����

	char cTTL;//����ʱ�䣨TTL��������8���ء���IP�����д���ʱ���Ȼ�Ը��ֶθ���ĳ���ض���ֵ����IP������ÿһ����;��·������ʱ��ÿ����;��·�����ὫIP����TTLֵ����1�����TTL����Ϊ0�����IP���ᱻ����������ֶο��Է�ֹ����·�ɻ�·������IP���������в�ͣ��ת����

	char cProtocol; //Э�飨Protocol��������8���ء���ʶ���ϲ���ʹ�õ�Э�顣
 //�����ǱȽϳ��õ�Э��ţ�
  //  1    ICMP   2    IGMP   6    TCP  17    UDP    88    IGRP    89    OSPF

	unsigned short usChkSum; //ͷ��У�飨Header Checksum��������16λ��������IPͷ������ȷ�Լ�⣬�����������ݲ��֡� ��Ϊÿ��·����Ҫ�ı�TTL��ֵ,����·������Ϊÿ��ͨ�������ݰ����¼������ֵ��
	unsigned int   uiSrcIp;
	unsigned int   uiDstIp;
	//��Դ��Ŀ���ַ��Source and Destination Addresses�����������ضζ���32���ء���ʶ�����IP������Դ��Ŀ���ַ��Ҫע�����ʹ��NAT��������������Ĺ����У���������ַ����ı䡣
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
		//  1    ICMP ��  2    IGMP ��  6    TCP �� 17    UDP  ��  88    IGRP    89    OSPF

		//return;
	}

	printf("\nRCV:\n");
	printf("Version:%d\n", pHeader->cVer);
	printf("IP len:%d\n", pHeader->cHeadLen);
	printf("TOS:%d\n", pHeader->cTOS);
	printf("Data Len:%d\n", ntohs(pHeader->usLen)); //ע���ֽ���ת��

	printf("TTL:%d\n", pHeader->cTTL);
	printf("Protocol:%d\n", pHeader->cProtocol);

	stAddr.sin_addr.s_addr = pHeader->uiSrcIp;  //stAddr.sin_addr.s_addr ����������ֽ����int
												//inet_ntoa �� in_addr ת���ַ�����ip��ַ
	printf("Source IP:%s\n", inet_ntoa(stAddr.sin_addr));

	//Ŀ���ַ
	stAddr.sin_addr.s_addr = pHeader->uiDstIp;
	printf("Dest IP:%s\n", inet_ntoa(stAddr.sin_addr));

	printf("\n");
	return;
	//printf(" RCV: %s  \n",szBuff);
	
	/*
	//��ӡ16��������
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
		//  1    ICMP ��  2    IGMP ��  6    TCP �� 17    UDP  ��  88    IGRP    89    OSPF

		//return;
	}

	printf("\nRCV:\n");
	printf("Version:%d\n", pHeader->cVer);
	printf("IP len:%d\n", pHeader->cHeadLen);
	printf("TOS:%d\n", pHeader->cTOS);
	printf("Data Len:%d\n", ntohs(pHeader->usLen)); //ע���ֽ���ת��

	printf("TTL:%d\n", pHeader->cTTL);
	printf("Protocol:%d\n", pHeader->cProtocol);

	stAddr.sin_addr.s_addr = pHeader->uiSrcIp;  //stAddr.sin_addr.s_addr ����������ֽ����int
												//inet_ntoa �� in_addr ת���ַ�����ip��ַ
	printf("Source IP:%s\n", inet_ntoa(stAddr.sin_addr));

	//Ŀ���ַ
	stAddr.sin_addr.s_addr = pHeader->uiDstIp;
	printf("Dest IP:%s\n", inet_ntoa(stAddr.sin_addr));

//	printf("\n");


	//��ӡ16��������
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
	char szHostName[HOST_NAME_LEN]; //������
	HOSTENT *pHost;

	DWORD dwInBuff = 1;
	DWORD dwOutBuff;
	DWORD dwBytesRet;
	int iRet=0;
	char szBuff[BUFF_LEN];
	SOCKADDR_IN  stAddr = { 0 };
	WSADATA stWsa;//���ڴ򿪶�̬���ӿ⣬�������̽ӿڿ���

	//0x0202, �汾�� 
	if (WSAStartup(0x0202, &stWsa) != OK)
	{
		printf(" WSA Start up failed \n");
		return ERR;

	}
	 
	//���ϵͳ �������
	if (gethostname(szHostName, HOST_NAME_LEN) != OK)
	{
		WSACleanup();
		return ERR;
	}

	printf("gethostname :%s \n", szHostName);

	pHost = gethostbyname(szHostName);

	printf("my ip :%d. %d. %d. %d\n", pHost->h_addr_list[0][0] & 0xff, pHost->h_addr_list[0][1]&0xff, pHost->h_addr_list[0][2] & 0xff, pHost->h_addr_list[0][3] & 0xff );

	//������ʾ��ip
	stAddr.sin_addr.s_addr = *(unsigned long *)pHost->h_addr;
	stAddr.sin_family = AF_INET;
	//��ͷ Сͷ ת��
	stAddr.sin_port = htons(MY_PORT);
	//����ԭʼ���������������ж˿ڵ� ������MY_PORT
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
	//����ifd�հ�����    SIO_RCVALL�������е�
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


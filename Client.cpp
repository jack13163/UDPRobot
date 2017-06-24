#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
    WORD socketVersion = MAKEWORD(2,2);
    WSADATA wsaData; 
    if(WSAStartup(socketVersion, &wsaData) != 0)
    {
        return 0;
    }
    SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    int len = sizeof(sin);
    
	//初始化
    char sendData[255]={'\0'};
	
	while(true)
	{
		//客户端输入
		printf("本机：");
		scanf("%s", &sendData);

		//发送信息
		sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);

		char recvData[255]={'\0'};  
		
		//接收数据
		int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
		if(ret > 0)
		{
			recvData[ret] = 0x00;
			printf("服务器：%s", recvData);
		}
	}

    closesocket(sclient);
    WSACleanup();
    return 0;
}
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

const char tmp[255] = "这个问题好难，你可以提示下吗？（Y/N）";

int main(int argc, char* argv[])
{
    WSADATA wsaData;
	//声明winsocket版本
    WORD sockVersion = MAKEWORD(2,2);
	//声明函数
	char* Search(char* path, char* tosearch);
	void appendtofile(char* path, char* towrite);
		
	//智能语料库
	char * filepath = "C:\\data.txt";
	
	//激活wsa
    if(WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }
	
	//建立socket
    SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if(serSocket == INVALID_SOCKET)
    {
        printf("创建套接字失败 !");
        return 0;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(8888);
    serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//绑定socket
    if(bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("绑定失败 !");
        closesocket(serSocket);
        return 0;
    }
    
	//remote_addr代表客户端的IP，但它的值不是由客户端提供的，而是服务端根据客户端的ip指定的
    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr); 

	printf("服务端已启动，等待客户端请求...\n");

	//记录控制开关
	bool flag = false;

    while (true)
    {
		//接收到的请求数据
        char recvData[255]={'\0'};
		//应答数据
		char sendData[255]={'\0'};
		
		//接收数据
        int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);

		//接收成功
        if (ret > 0)
        {
            recvData[ret] = 0x00;
			
			//将一个十进制网络字节序转换为点分十进制IP格式的字符串
            printf("%s: %s\n", inet_ntoa(remoteAddr.sin_addr), recvData);          
        }

		//查找答复
		strcpy(sendData,Search(filepath, recvData));

		//答复长度
		int len = strlen(sendData);
			
		//发送消息
		sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);
		
		//打印服务端提示
		printf("服务端：%s\n", sendData);

		//判断是否查找到回复
		if(strcmp(sendData, tmp)==0)
		{
			flag = true;
		}

		//记录
		if(flag)
		{
			//记下问题
			char record[255] = {'\0'};
			strcpy(record, recvData);
			strcat(record, "\t");

			while(true)
			{
				//初始化
				for(int i=0;i<255;i++)
				{
					recvData[i] = '\0';
				}

				//接收是否记录确认
				int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
				//接收到一个消息
				if (ret > 0)
				{
					recvData[ret] = 0x00;
					
					//将一个十进制网络字节序转换为点分十进制IP格式的字符串
					printf("%s: %s\n", inet_ntoa(remoteAddr.sin_addr), recvData);       
				}
				
				//确认记录
				if(strcmp(recvData,"Y")==0 || strcmp(recvData,"y")==0)
				{
					//给出提示
					strcpy(sendData, "请输入对应的回答奥");
					
					//答复长度
					int len = strlen(sendData);
					
					//发送消息
					sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);
					
					//打印服务端提示
					printf("服务端：%s\n", sendData);

					//接收答案
					int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
					//接收到答案
					if (ret > 0)
					{
						recvData[ret] = 0x00;
						
						//将一个十进制网络字节序转换为点分十进制IP格式的字符串
						printf("%s: %s\n", inet_ntoa(remoteAddr.sin_addr), recvData);       
					}

					//整理答案
					strcat(record, recvData);
					strcat(record, "\n");
					
					//确定后记录到文件
					appendtofile(filepath, record);
					
					//查找答复
					strcpy(sendData, "记录成功！");

					//答复长度
					len = strlen(sendData);
				
					//发送消息
					sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);

					//关闭开关
					flag = false;

					break;
				}
				else if(strcmp(recvData,"N")==0 || strcmp(recvData,"n")==0)
				{
					//查找答复
					strcpy(sendData, "好吧");
					printf("服务端：好吧\n");

					//答复长度
					len = strlen(sendData);
				
					//发送消息
					sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);

					flag = false;

					break;
				}
				else
				{
					//查找答复
					strcpy(sendData, "请重新输入！");

					//答复长度
					len = strlen(sendData);
				
					//发送消息
					sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);
				}
			}
		}
    }

	//关闭socket和wsa
    closesocket(serSocket);
    WSACleanup();

    return 0;
}

//将记录追加到文件
void appendtofile(char* path, char* towrite)
{
	FILE *pFile;
	
	//打开文件
	if((pFile=fopen(path, "a"))==NULL) {
		printf("文件不能打开！\n"); 
		return;
	}

	int len = strlen(towrite);
	//向文件写数据
	fwrite (towrite , 1, len, pFile);

	fflush(pFile);

	fclose(pFile);
}

//查找答复
char* Search(char* path, char* tosearch)
{
	char* File2String(const char* path);
	int StringFind(const char *pSrc, const char *pDst);
	
	//字符串（源）,定义一个字符指针
	const char* src;
	//回答限定在255个字符以内
	char answer[256]={'\0'};

	//从文件中获取字符串
	src = File2String(path);

	if(src!=NULL)
	{
		//查找字符串
		int i=StringFind(src, tosearch);//本行匹配开始
		int j=i;//本行匹配结束

		//查找成功
		if(j!=-1)
		{
			//遇到本行结束
			while(src[j]!='\n')
			{
				j++;
			}
		}

		//答复是否开始
		bool flag = false;
		int p = 0;
		
		//组织答复信息
		for(int k=i;k<j;k++)
		{
			if(src[k]=='\t')
			{
				k++;
				//设置标志位
				flag = true;
			}

			//打印答复
			if(flag)
			{
				answer[p] = src[k];
				p++;
			}
			i++;
		}

		//默认答复
		if(strlen(answer)<=0)
		{
			strcat(answer, tmp);
		}
	}
	else
	{
		printf("读取的文件为空！");
	}

	return answer;
}

//查找字符串中指定子串
int StringFind(const char *pSrc, const char *pDst)  
{  
    int i, j; 

	//循环遍历字符串
    for (i=0; pSrc[i]!='\0'; i++) 
    {
		//字符串和子串开头不匹配，则继续进行下一步
        if(pSrc[i]!=pDst[0])  
            continue; 
		if(i > 0 && pSrc[i-1]!='\n')
			continue;

		//若字符串和子串开头匹配
        j = 0;

		//从当前位置循环遍历两个字符串
        while(pDst[j]!='\0' && pSrc[i+j]!='\0') 
        {  
            j++;

			//期间有不匹配的字符串
            if(pDst[j]!=pSrc[i+j])
				break;  
        }

		//如果遍历到子串结尾，代表全部匹配成功，返回字符串中匹配的开始位置
        if(pDst[j]=='\0' && pSrc[i+j]=='\t')
            return i;  
    }  

	//匹配失败
    return -1;  
} 

//读取文件
char* File2String(const char* path)
{
	FILE *fp;
	
	//打开文件
	if((fp=fopen(path, "r"))==NULL) {
		printf("文件不能打开！/n"); 
		exit(1); 
	}

	//移动文件流的读写位置，
	//欲将读写位置移动到文件开头时:fseek(FILE *stream, 0, SEEK_SET);
	//欲将读写位置移动到文件尾时:fseek(FILE *stream, 0, SEEK_END);
	fseek(fp, 0, SEEK_END);

	//ftell当前的读写位置
	//通过先将文件指针指向文件末尾，再计算文件的读写位置的方式，计算出文件的长度
	int fileLen = ftell(fp);

	//分配内存
	char* src = (char *)malloc(sizeof(char)*(fileLen+1));
	
	//初始化
	for(int i=0;i<=fileLen;i++)
	{
		src[i]='\0';
	}

	//文件指针移动到开始位置
	fseek(fp, 0, SEEK_SET);
	
	//读取文件
	fread(src, fileLen, sizeof(char), fp);

	//关闭文件
	fclose(fp); 

	return src;
}

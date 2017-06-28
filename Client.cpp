#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	//函数声明
	bool validity(char* str);

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
	
	//判断第二个参数是否为空
	if(argv[1]!=NULL)
	{
		//设置本地ip地址
		sin.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	}
	else
	{
		char ipaddr[20];

		printf("请输入服务端IP地址：");
		scanf("%s", &ipaddr);
		
		//ip地址正确性检测
		int ipa = inet_addr(ipaddr);

		while(ipa == -1 || !validity(ipaddr))
		{
			printf("请输入服务端IP地址：");
			scanf("%s", &ipaddr);
		}

		//设置ip地址
		sin.sin_addr.S_un.S_addr = ipa;
	}

	//求ip地址长度
    int len = sizeof(sin);
    
	//初始化
    char sendData[255]={'\0'};
	
	while(true)
	{
		fflush(stdin);
		//客户端输入
		printf("本机：");
		gets(sendData);

		//发送信息
		sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);

		char recvData[255]={'\0'}; 
		
		//接收数据
		int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);

		//接收成功，打印消息
		if(ret > 0)
		{
			recvData[ret] = 0x00;
			printf("服务器：%s\n", recvData);
		}
	}

    closesocket(sclient);
    WSACleanup();
    return 0;
}

//检查ip地址是否合法
bool validity(char* str)
{
	int indexof(char* str, char ch);
	char* substring(char* str, int start);
	int charcount(char * str, char ch);
	char* substring(char* str, int start, int end);
	int str2num(char * str);

	int len = strlen(str);

	//ip地址第一个和最后一个字符不能为.
	if(indexof(str, '.')==0 || indexof(str, '.')==len-1 || charcount(str, '.') != 3)
	{
		return false;
	}

	//将ip地址切分为四块
	int tmp[4] = {0};
	char cut[3];

	for(int i=0;i<4;i++)
	{
		//确定结束位置
		int index = indexof(str, '.');

		if(index != -1)
		{
			//截取需要转换的字符串
			strcpy(cut, substring(str, 0, index-1));
		}
		else
		{
			strcpy(cut, str);
		}

		//将字符串转换为数字
		tmp[i] = str2num(cut);

		//判断是否合法在0~255之间
		if(tmp[i] < 0 || tmp[i] > 255)
		{
			return false;
		}

		//将截取后剩余的字符串赋值给str
		strcpy(str, substring(str, index+1));
	}

	return true;
}

//字符串转换为数字
int str2num(char * str)
{
	int char2num(char ch);
	int len = strlen(str);
	
	//当前字符
	int loc = 0;

	int result = 0;

	while(loc<len)
	{
		result = result * 10 + char2num(str[loc]);
		loc++;
	}

	return result;
}

//将数字字符转换为数字
int char2num(char ch)
{
	return ch - 48;
}

//查找第一个匹配字符所在的位置
int indexof(char* str, char ch)
{
	int i = 0;
	int len = strlen(str);
	while(i < len)
	{
		if(str[i]==ch)
		{
			return i;
		}
		i++;
	}
	return -1;
}

//截取子串
char* substring(char* str, int start)
{
	if(start < 0)
	{
		return NULL;
	}

	int i = 0;
	while(i<start)
	{
		i++;
		continue;
	}

	int len = strlen(str)-start;
	char * tmp = (char*)malloc(sizeof(char)*len);

	//初始化
	int j = 0;
	while(j<len)
	{
		tmp[j]='\0';
		j++;
	}

	//复制
	int k = 0;
	while(k < len)
	{
		tmp[k]=str[i+k];
		k++;
	}

	//设置结束标志
	tmp[len]='\0';

	return tmp;
}

char* substring(char* str, int start, int end)
{
	//合法性判断
	if(start < 0 || end <0 || start > end)
	{
		return NULL;
	}

	//定位到开始索引处
	int i = 0;
	while(i<start)
	{
		i++;
		continue;
	}

	//分配内存
	int len = end-start+1;
	char * tmp = (char*)malloc(sizeof(char)*len + 1);

	//初始化
	int j = 0;
	while(j<len)
	{
		tmp[j]='\0';
		j++;
	}

	//复制
	int k = 0;
	while(k < len)
	{
		tmp[k]=str[i+k];
		k++;
	}

	//设置结束标志
	tmp[len]='\0';

	return tmp;
}

//字符计数
int charcount(char * str, char ch)
{
	char* substring(char* str, int start);

	int i= 0;
	int len = strlen(str);
	int count = 0;
	while(i<len)
	{
		int index = indexof(str, ch);
		//计数器加一
		if(index!=-1)
		{
			count++;
		}
		//截取子串
		strcpy(str,substring(str, index + 1));
		i++;
	}
	return count;
}
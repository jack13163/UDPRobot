#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	//��������
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
	
	//�жϵڶ��������Ƿ�Ϊ��
	if(argv[1]!=NULL)
	{
		//���ñ���ip��ַ
		sin.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	}
	else
	{
		char ipaddr[20];

		printf("����������IP��ַ��");
		scanf("%s", &ipaddr);
		
		//ip��ַ��ȷ�Լ��
		int ipa = inet_addr(ipaddr);

		while(ipa == -1 || !validity(ipaddr))
		{
			printf("����������IP��ַ��");
			scanf("%s", &ipaddr);
		}

		//����ip��ַ
		sin.sin_addr.S_un.S_addr = ipa;
	}

	//��ip��ַ����
    int len = sizeof(sin);
    
	//��ʼ��
    char sendData[255]={'\0'};
	
	while(true)
	{
		fflush(stdin);
		//�ͻ�������
		printf("������");
		gets(sendData);

		//������Ϣ
		sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);

		char recvData[255]={'\0'}; 
		
		//��������
		int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);

		//���ճɹ�����ӡ��Ϣ
		if(ret > 0)
		{
			recvData[ret] = 0x00;
			printf("��������%s\n", recvData);
		}
	}

    closesocket(sclient);
    WSACleanup();
    return 0;
}

//���ip��ַ�Ƿ�Ϸ�
bool validity(char* str)
{
	int indexof(char* str, char ch);
	char* substring(char* str, int start);
	int charcount(char * str, char ch);
	char* substring(char* str, int start, int end);
	int str2num(char * str);

	int len = strlen(str);

	//ip��ַ��һ�������һ���ַ�����Ϊ.
	if(indexof(str, '.')==0 || indexof(str, '.')==len-1 || charcount(str, '.') != 3)
	{
		return false;
	}

	//��ip��ַ�з�Ϊ�Ŀ�
	int tmp[4] = {0};
	char cut[3];

	for(int i=0;i<4;i++)
	{
		//ȷ������λ��
		int index = indexof(str, '.');

		if(index != -1)
		{
			//��ȡ��Ҫת�����ַ���
			strcpy(cut, substring(str, 0, index-1));
		}
		else
		{
			strcpy(cut, str);
		}

		//���ַ���ת��Ϊ����
		tmp[i] = str2num(cut);

		//�ж��Ƿ�Ϸ���0~255֮��
		if(tmp[i] < 0 || tmp[i] > 255)
		{
			return false;
		}

		//����ȡ��ʣ����ַ�����ֵ��str
		strcpy(str, substring(str, index+1));
	}

	return true;
}

//�ַ���ת��Ϊ����
int str2num(char * str)
{
	int char2num(char ch);
	int len = strlen(str);
	
	//��ǰ�ַ�
	int loc = 0;

	int result = 0;

	while(loc<len)
	{
		result = result * 10 + char2num(str[loc]);
		loc++;
	}

	return result;
}

//�������ַ�ת��Ϊ����
int char2num(char ch)
{
	return ch - 48;
}

//���ҵ�һ��ƥ���ַ����ڵ�λ��
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

//��ȡ�Ӵ�
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

	//��ʼ��
	int j = 0;
	while(j<len)
	{
		tmp[j]='\0';
		j++;
	}

	//����
	int k = 0;
	while(k < len)
	{
		tmp[k]=str[i+k];
		k++;
	}

	//���ý�����־
	tmp[len]='\0';

	return tmp;
}

char* substring(char* str, int start, int end)
{
	//�Ϸ����ж�
	if(start < 0 || end <0 || start > end)
	{
		return NULL;
	}

	//��λ����ʼ������
	int i = 0;
	while(i<start)
	{
		i++;
		continue;
	}

	//�����ڴ�
	int len = end-start+1;
	char * tmp = (char*)malloc(sizeof(char)*len + 1);

	//��ʼ��
	int j = 0;
	while(j<len)
	{
		tmp[j]='\0';
		j++;
	}

	//����
	int k = 0;
	while(k < len)
	{
		tmp[k]=str[i+k];
		k++;
	}

	//���ý�����־
	tmp[len]='\0';

	return tmp;
}

//�ַ�����
int charcount(char * str, char ch)
{
	char* substring(char* str, int start);

	int i= 0;
	int len = strlen(str);
	int count = 0;
	while(i<len)
	{
		int index = indexof(str, ch);
		//��������һ
		if(index!=-1)
		{
			count++;
		}
		//��ȡ�Ӵ�
		strcpy(str,substring(str, index + 1));
		i++;
	}
	return count;
}
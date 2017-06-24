#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
    WSADATA wsaData;
	//����winsocket�汾
    WORD sockVersion = MAKEWORD(2,2);
	//��������
	char* Search(char* path, char* tosearch);
		
	//�������Ͽ�
	char * filepath = "C:\\data.txt";
	
	//����wsa
    if(WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }
	
	//����socket
    SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if(serSocket == INVALID_SOCKET)
    {
        printf("�����׽���ʧ�� !");
        return 0;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(8888);
    serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//��socket
    if(bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("��ʧ�� !");
        closesocket(serSocket);
        return 0;
    }
    
	//remote_addr����ͻ��˵�IP��������ֵ�����ɿͻ����ṩ�ģ����Ƿ���˸��ݿͻ��˵�ipָ����
    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr); 

	printf("��������������ȴ��ͻ�������...\n");

    while (true)
    {
		//���յ�����������
        char recvData[255]={'\0'};
		//Ӧ������
		char sendData[255]={'\0'};
		
		//��������
        int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);

		//���ճɹ�
        if (ret > 0)
        {
            recvData[ret] = 0x00;
			
			//��һ��ʮ���������ֽ���ת��Ϊ���ʮ����IP��ʽ���ַ���
            printf("���ܵ�һ������%s������...\n", inet_ntoa(remoteAddr.sin_addr));
            printf("�ͻ��ˣ�%s\n", recvData);          
        }

		//���Ҵ�
		strcat(sendData,Search(filepath, recvData));

		//�𸴳���
		int len = strlen(sendData);

		//����(Ӧ��)����
        sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);    

		//��ӡ�������ʾ
		printf("����ˣ�%s", sendData);
    }

	//�ر�socket��wsa
    closesocket(serSocket); 
    WSACleanup();

    return 0;
}

//���Ҵ�
char* Search(char* path, char* tosearch)
{
	char* File2String(const char* path);
	int StringFind(const char *pSrc, const char *pDst);
	
	//�ַ�����Դ��,����һ���ַ�ָ��
	const char* src;
	//�ش��޶���255���ַ�����
	char answer[256]={'\0'};

	//���ļ��л�ȡ�ַ���
	src = File2String(path);

	if(src!=NULL)
	{
		//�����ַ���
		int i=StringFind(src, tosearch);//����ƥ�俪ʼ
		int j=i;//����ƥ�����

		//���ҳɹ�
		if(j!=-1)
		{
			//�������н���
			while(src[j]!='\n')
			{
				j++;
			}
		}

		//���Ƿ�ʼ
		bool flag = false;
		int p = 0;
		
		//��֯����Ϣ
		for(int k=i;k<j+1;k++)
		{
			if(src[k]=='\t')
			{
				k++;
				//���ñ�־λ
				flag = true;
			}

			//��ӡ��
			if(flag)
			{
				answer[p] = src[k];
				p++;
			}
			i++;
		}

		//Ĭ�ϴ�
		if(strlen(answer)<=0)
		{
			strcat(answer, "��˵ʲô����û��̫��\n");
		}
	}
	else
	{
		printf("��ȡ���ļ�Ϊ�գ�");
	}

	return answer;
}

//�����ַ�����ָ���Ӵ�
int StringFind(const char *pSrc, const char *pDst)  
{  
    int i, j;  

	//ѭ�������ַ���
    for (i=0; pSrc[i]!='\0'; i++) 
    {
		//�ַ������Ӵ���ͷ��ƥ�䣬�����������һ��
        if(pSrc[i]!=pDst[0])  
            continue; 
		
		//���ַ������Ӵ���ͷƥ��
        j = 0;

		//�ӵ�ǰλ��ѭ�����������ַ���
        while(pDst[j]!='\0' && pSrc[i+j]!='\0') 
        {  
            j++;

			//�ڼ��в�ƥ����ַ���
            if(pDst[j]!=pSrc[i+j])
				break;  
        }

		//����������Ӵ���β������ȫ��ƥ��ɹ��������ַ�����ƥ��Ŀ�ʼλ��
        if(pDst[j]=='\0')  
            return i;  
    }  

	//ƥ��ʧ��
    return -1;  
} 

//��ȡ�ļ�
char* File2String(const char* path)
{
	FILE *fp;
	
	//���ļ�
	if((fp=fopen(path, "r"))==NULL) {
		printf("�ļ����ܴ򿪣�/n"); 
		exit(1); 
	}

	//�ƶ��ļ����Ķ�дλ�ã�
	//������дλ���ƶ����ļ���ͷʱ:fseek(FILE *stream, 0, SEEK_SET);
	//������дλ���ƶ����ļ�βʱ:fseek(FILE *stream, 0, SEEK_END);
	fseek(fp, 0, SEEK_END);

	//ftell��ǰ�Ķ�дλ��
	//ͨ���Ƚ��ļ�ָ��ָ���ļ�ĩβ���ټ����ļ��Ķ�дλ�õķ�ʽ��������ļ��ĳ���
	int fileLen = ftell(fp);

	//�����ڴ�
	char* src = (char *)malloc(sizeof(char)*(fileLen+1));
	
	//��ʼ��
	for(int i=0;i<=fileLen;i++)
	{
		src[i]='\0';
	}

	//�ļ�ָ���ƶ�����ʼλ��
	fseek(fp, 0, SEEK_SET);
	
	//��ȡ�ļ�
	fread(src, fileLen, sizeof(char), fp);

	//�ر��ļ�
	fclose(fp); 

	return src;
}

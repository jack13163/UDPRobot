#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

const char tmp[255] = "���������ѣ��������ʾ���𣿣�Y/N��";

int main(int argc, char* argv[])
{
    WSADATA wsaData;
	//����winsocket�汾
    WORD sockVersion = MAKEWORD(2,2);
	//��������
	char* Search(char* path, char* tosearch);
	void appendtofile(char* path, char* towrite);
		
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

	//��¼���ƿ���
	bool flag = false;

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
            printf("%s: %s\n", inet_ntoa(remoteAddr.sin_addr), recvData);          
        }

		//���Ҵ�
		strcpy(sendData,Search(filepath, recvData));

		//�𸴳���
		int len = strlen(sendData);
			
		//������Ϣ
		sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);
		
		//��ӡ�������ʾ
		printf("����ˣ�%s\n", sendData);

		//�ж��Ƿ���ҵ��ظ�
		if(strcmp(sendData, tmp)==0)
		{
			flag = true;
		}

		//��¼
		if(flag)
		{
			//��������
			char record[255] = {'\0'};
			strcpy(record, recvData);
			strcat(record, "\t");

			while(true)
			{
				//��ʼ��
				for(int i=0;i<255;i++)
				{
					recvData[i] = '\0';
				}

				//�����Ƿ��¼ȷ��
				int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
				//���յ�һ����Ϣ
				if (ret > 0)
				{
					recvData[ret] = 0x00;
					
					//��һ��ʮ���������ֽ���ת��Ϊ���ʮ����IP��ʽ���ַ���
					printf("%s: %s\n", inet_ntoa(remoteAddr.sin_addr), recvData);       
				}
				
				//ȷ�ϼ�¼
				if(strcmp(recvData,"Y")==0 || strcmp(recvData,"y")==0)
				{
					//������ʾ
					strcpy(sendData, "�������Ӧ�Ļش��");
					
					//�𸴳���
					int len = strlen(sendData);
					
					//������Ϣ
					sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);
					
					//��ӡ�������ʾ
					printf("����ˣ�%s\n", sendData);

					//���մ�
					int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
					//���յ���
					if (ret > 0)
					{
						recvData[ret] = 0x00;
						
						//��һ��ʮ���������ֽ���ת��Ϊ���ʮ����IP��ʽ���ַ���
						printf("%s: %s\n", inet_ntoa(remoteAddr.sin_addr), recvData);       
					}

					//�����
					strcat(record, recvData);
					strcat(record, "\n");
					
					//ȷ�����¼���ļ�
					appendtofile(filepath, record);
					
					//���Ҵ�
					strcpy(sendData, "��¼�ɹ���");

					//�𸴳���
					len = strlen(sendData);
				
					//������Ϣ
					sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);

					//�رտ���
					flag = false;

					break;
				}
				else if(strcmp(recvData,"N")==0 || strcmp(recvData,"n")==0)
				{
					//���Ҵ�
					strcpy(sendData, "�ð�");
					printf("����ˣ��ð�\n");

					//�𸴳���
					len = strlen(sendData);
				
					//������Ϣ
					sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);

					flag = false;

					break;
				}
				else
				{
					//���Ҵ�
					strcpy(sendData, "���������룡");

					//�𸴳���
					len = strlen(sendData);
				
					//������Ϣ
					sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);
				}
			}
		}
    }

	//�ر�socket��wsa
    closesocket(serSocket);
    WSACleanup();

    return 0;
}

//����¼׷�ӵ��ļ�
void appendtofile(char* path, char* towrite)
{
	FILE *pFile;
	
	//���ļ�
	if((pFile=fopen(path, "a"))==NULL) {
		printf("�ļ����ܴ򿪣�\n"); 
		return;
	}

	int len = strlen(towrite);
	//���ļ�д����
	fwrite (towrite , 1, len, pFile);

	fflush(pFile);

	fclose(pFile);
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
		for(int k=i;k<j;k++)
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
			strcat(answer, tmp);
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
		if(i > 0 && pSrc[i-1]!='\n')
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
        if(pDst[j]=='\0' && pSrc[i+j]=='\t')
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

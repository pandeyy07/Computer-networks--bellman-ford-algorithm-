#include <string>
#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <sys/timeb.h>
using namespace std;
struct jj
{
	int from,to;
	int port;
	int cost;
	char F[200],T[200];
}Map[20000],selfMap[20000];
char ha[200][200];
double last[200];
int sumhash = 0;
int sumedge,selfsumedg;
int self;
int selfport;
void init()
{
	if(freopen("input.txt","r",stdin)==NULL){
		cout<<"file error"<<endl;}
	char s[200];
	int now = 1;
	
	while(scanf("%s",s) != EOF)
	{	
		int i = 0;
		int pos = 0;
		while(s[i] != ',')
		{
			Map[now].F[pos] = s[i];
			i ++;
			pos ++;
		}
		i ++;
		pos = 0;
		while(s[i] != ',')
		{
			Map[now].T[pos] = s[i];
			i ++;
			pos ++;
		}
		i ++;
		while(s[i] != ',')
		{
			Map[now].port *= 10;
			Map[now].port += s[i] - '0';
			i ++;
		}
		i ++;
		while(s[i]>= '0' && s[i] <= '9')
		{
			Map[now].cost *= 10;
			Map[now].cost += s[i] - '0';
			i ++;
		}		
		memset(s,0,sizeof(s));
		int j;
		for(j = 0;j < sumhash;j ++)
			if(strcmp(Map[now].F,ha[j]) == 0)break;
		if(j != sumhash)Map[now].from = j;
		else 
		{
			Map[now].from = sumhash;
			for(unsigned int kk = 0;kk < strlen(Map[now].F);kk ++)
				ha[sumhash][kk] = Map[now].F[kk];		
			sumhash ++;
			
		}
		for(j = 0;j < sumhash;j ++)
			if(strcmp(Map[now].T,ha[j]) == 0)break;
		if(j != sumhash)Map[now].to = j;
		else 
		{
			Map[now].to = sumhash;
			for(unsigned int kk = 0;kk < strlen(Map[now].T);kk ++)
				ha[sumhash][kk] = Map[now].T[kk];		
			sumhash ++;
			
		}
		now ++;
	}sumedge = now-1;
}




int main()
{
	int port =0;
	char buf[2048];
	char to[2048];
	memset(buf,0,sizeof(buf));
	struct sockaddr_in ServerAddr;
  	bzero(&ServerAddr,sizeof(ServerAddr));//initialize the server address
  	ServerAddr.sin_family = AF_INET;
  	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(buf,0,sizeof(buf));
	cin>>to>>buf;
	int T0= 0; 
	int T1= 0;
	init();
	for(int i = 0;i < sumhash;i ++){if(ha[i][0] == to[0])T0 = i;if(ha[i][0] == buf[1])T1 = i;}
	for(int i = 1;i <= sumedge;i ++)if(Map[i].to == T0)port = Map[i].port;
	cout<<port<<endl;
	buf[1] = T1 + '0';
	cout<<buf<<endl;



  	ServerAddr.sin_port = htons(port);
  	int ClientSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if(sendto(ClientSocket,buf,strlen(buf),0,(struct sockaddr *)&ServerAddr,sizeof(struct sockaddr)) < 0)
		{
			printf("receive error!\n");
			close(ClientSocket);
		}
	close(ClientSocket);
	
	return 0;
}

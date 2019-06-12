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
#include <fstream>  
#include <stdlib.h>
#include<iomanip>

using namespace std;
#define INT_MAX 2147483647
int port = 4000;
#define FileNameSize 4000
#define BufferSize 1024
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




class BellmanFordNode{
    private:
        int value; 
        char name; 
        BellmanFordNode* nextP; 
    public: 
        BellmanFordNode(); 
        BellmanFordNode(int testValue, char partnerName); 
        int calculator(int testValue);
        int getValue(); 
        char getName(); 
        void setName(char name);
        void setValue(int value);
        BellmanFordNode* getNext(); 
        void setNext(BellmanFordNode* next);
        
};
BellmanFordNode::BellmanFordNode(){
            nextP = NULL; 
        } 
        BellmanFordNode::BellmanFordNode(int testValue, char partnerName){
                value = testValue; 
                name = partnerName; 
                nextP = NULL; 
        } 
        int BellmanFordNode::calculator(int testValue){
            if(testValue != value){
                value = testValue;
                return 1;
            }
            return 0; 
        }
        int BellmanFordNode::getValue(){
            return value; 
        } 
        char BellmanFordNode::getName(){
            return name; 
        }
        void BellmanFordNode::setName(char testName){
            name = testName; 
        }
        void BellmanFordNode::setValue(int testValue){
            value = testValue; 
        }
        BellmanFordNode* BellmanFordNode::getNext(){
                return nextP;
        } 
        void BellmanFordNode::setNext(BellmanFordNode* next){
                nextP = next;
        }

int hashFunc(char node);
int checkTable(char name, BellmanFordNode* table[]);
int addNode(char name, int value,BellmanFordNode* table[]);
int updateTable(char partnerName, char destName, int value,BellmanFordNode* table[]);
void printTable(BellmanFordNode* table[]); 
BellmanFordNode* search(char destination, BellmanFordNode* table[]);
int hashFunc(char node){
    int num = node -'A';  
    return num; 
}

BellmanFordNode* search(char destination, BellmanFordNode* table[]){
    int hash = hashFunc(destination);
    BellmanFordNode *quickest, *next; 
    next = table[hash];
    quickest = table[hash];
    while(next!=NULL){
        if(quickest->getValue() > next->getValue() ){
            quickest = next;
        }
        next = next->getNext();
    }
    return quickest;
}

int checkTable(char name, BellmanFordNode* table[]){
    BellmanFordNode* next = table[0]; 
    while(next!= NULL){
        if(next->getName() == name){
            return 1;
        }
        next = next->getNext();
    }
    return 0; 
}

int addNode(char name, int value,BellmanFordNode* table[]){
    BellmanFordNode *next, *prev;
    BellmanFordNode* newNode; 
    int hash = hashFunc(name);
    for(int i = 0; i< 26; i++){
        if(i == hash){
        newNode = new BellmanFordNode(value, name);
        }
        else{
         newNode = new BellmanFordNode(10000, name);
        }
        next = table[i];
        prev = table[i];
        while(next!=NULL){
            prev = next;
            next = next->getNext();
        }
        if(table[i]==NULL){
            table[i] = newNode; 
        }
        else{
            prev->setNext(newNode);}
    }

    return 1;
}

int updateTable(char partnerName, char destName, int value,BellmanFordNode* table[]){
    int hashValue = hashFunc(destName);
    BellmanFordNode* next; 
    next = table[hashValue];
    while(next!=NULL){
        if(next->getName() == partnerName){
            return next->calculator(value);
            
        }
        next = next->getNext();
    }
    return -1;
}

void printTable(BellmanFordNode* table[], ofstream* outfile){
    BellmanFordNode* next; 
    next = table[0];
	*outfile << "  | " ;
    char name; 
    while(next!=NULL){
        *outfile<<setw(12)<<next->getName();
        next = next->getNext();
    }
    *outfile<<"\n___________________________________________________________________________________\n";
    for(int i =0; i<26; i++){
        name = i+97;
        *outfile << name << " | ";
        next = table[i];
        while(next!= NULL){
            *outfile<<setw(12)<< next->getValue();
            next = next->getNext();
        }
        *outfile << endl;
    }
}

BellmanFordNode* mapTable[26];




void init()
{
	if(freopen("input.txt","r",stdin)==NULL){
	cout<<"file error"<<endl;
	}
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
int plate = 0;
int pack = -1;
char P[2048];
int LastDV[2048];
void encoder(int type,char *buff)
{
	if(type == 1)
	{
		buff[0] = 'D';
		buff[1] = self + '0';
		int pos = 2;
		for(int j = 0;j < sumhash;j ++)
		{
			char input1 = ha[j][0]; 
			BellmanFordNode* shortest = search(input1, mapTable); 
			int mincost = shortest->getValue();
			if(mincost == 0)
			{	
				buff[pos] = '0';
				pos ++;
				buff[pos] = ',';
				pos ++;
			}
			else
			{
				while(mincost > 0)
				{
					buff[pos] = (mincost%10)+'0';
					mincost/=10;
					pos ++;
				}
				 buff[pos] = ',';
				 pos ++;
			}
		}
		
	}
	else if(type == 2)
	{
		for(unsigned int j = 0;j < strlen(P);j ++)buff[j] = P[j];
		pack = -1;
	}
}
bool sleeping = 0;
void decoder(int from,char *buff, ofstream* outfile)
{
	if(buff[0] == 'D')
	{
		bool modify = false;
		int value0=0;
		for(int i = 0;i < selfsumedg; i ++)
		{
			if(selfMap[i].to == buff[1]-'0')value0 = selfMap[i].cost;
			
		}int pos = 2;
		modify |= updateTable(ha[self][0],ha[buff[1]-'0'][0],value0,mapTable);	
		for(int j = 0;j < sumhash;j ++)
		{
			
			int value1 = 0;		
			int gg = 1;
			while(buff[pos] != ',')
			{
				
				value1+=(buff[pos] - '0')*gg;
				gg*=10;
				pos ++;
			}pos ++;
			modify |= updateTable(ha[buff[1]-'0'][0], ha[j][0], min(value1+value0,100000), mapTable);
			
			if(value1 > 90000)
			{
				for(int k = 0;k < selfsumedg;k ++)
			 modify |= updateTable(ha[selfMap[k].to][0],ha[j][0],100000,mapTable);
				if(j != buff[1] - '0')
			 modify |= updateTable(ha[self][0],ha[j][0],100000,mapTable);
			}
		}

	time_t T1;
	struct timeb t1;
	ftime(&t1);
	time(&T1);
	int TIMER = T1*1000 + t1.millitm;
	LastDV[buff[1]-'0'] = TIMER;
	for(int i = 0;i < selfsumedg;i ++)
	{

		if(TIMER - LastDV[selfMap[i].to] > 3000)
		{
			for(int j = 0;j < sumhash;j ++)
			 modify |= updateTable(ha[selfMap[i].to][0],ha[j][0],100000,mapTable);
			for(int j = 0;j < selfsumedg;j ++)
			 modify |= updateTable(ha[selfMap[j].to][0],ha[selfMap[i].to][0],100000,mapTable);
			 modify |= updateTable(ha[self][0],ha[selfMap[i].to][0],100000,mapTable);

		}
	}
		
	if(modify)
	{
		*outfile<<"time stamp: "<<TIMER<<endl;
		printTable(mapTable, outfile);
	}

	}
	if(buff[0] == 'P')
	{
		int dest = buff[1] - '0';
		if(dest == self)
		{
			cout<<"Arriving at "<<ha[self]<<endl;
			*outfile<<"packet here now"<<ha[self]<<endl;
			cout<<buff<<endl;
			return;
		}
		else{ 
			cout<<"Packet now at "<<ha[self]<<endl;
			*outfile<<endl<<"packet now at " <<ha[self]<<endl;}
		BellmanFordNode* shortest = search(ha[dest][0], mapTable); 
		int to =0;
		cout<<shortest->getValue()<<endl;
		if(shortest->getValue() < 9000)
		{
		for(int j = 0;j < sumhash;j ++)
			if(ha[j][0] == shortest->getName())to = j;
		if(to == self)to = buff[1]-'0';
		pack = to;
		memset(P,0,sizeof(P));
		for(unsigned int j = 0;j < strlen(buff);j ++)P[j] = buff[j];
		cout<<"send to "<<ha[to]<<endl;
		}
		else cout<<"No way"<<endl;
	}
	if(buff[0] == 'S')
	{
		cout<<"Point "<<ha[self]<<" go to sleep"<<endl;
		sleeping = 1;
	}
}
void* client(void* arg)
{
	int thr = *(int*)arg;
	plate = 1;
	struct timeb t1,t2;
	time_t T1,T2;
	ftime(&t1);
	time(&T1);
while(1)
{	
	ftime(&t2);
	time(&T2);
	if(((T2*1000 + t2.millitm - T1*1000 - t1.millitm > 1000)||(pack == selfMap[thr].to)) && (!sleeping))
	{
	if(!(pack == selfMap[thr].to))
	{
		ftime(&t1);
		time(&T1);
	}
	char buf[BufferSize];
	memset(buf,0,sizeof(buf));
	struct sockaddr_in ServerAddr;
  	bzero(&ServerAddr,sizeof(ServerAddr));//initialize the server address
  	ServerAddr.sin_family = AF_INET;
  	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  	ServerAddr.sin_port = htons(selfMap[thr].port);

  	int ClientSocket = socket(PF_INET, SOCK_DGRAM, 0);

  	memset(buf,0,sizeof(buf));

	//check_the_type
	int type = 1;
	if(pack == selfMap[thr].to)type = 2;
	encoder(type,buf);

	if(sendto(ClientSocket,buf,strlen(buf),0,(struct sockaddr *)&ServerAddr,sizeof(struct sockaddr)) < 0)
		{
			printf("receive error!\n");
			close(ClientSocket);
		}
	close(ClientSocket);
	}
	else{}
	//return;
}
	
}
void * server(void* arg)
{	
	printf("I am: %d %s server\n",self,ha[self]);
	memset(last,0,sizeof(last));
	//UDP setup
	char buf[BufferSize];
	memset(buf,0,sizeof(buf));
	struct sockaddr_in ServerAddr;
	struct sockaddr_in ClientAddr;
  	bzero(&ServerAddr,sizeof(ServerAddr));//initialize the server address
  	ServerAddr.sin_family = AF_INET;
  	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  	ServerAddr.sin_port = htons(selfport);
  	int ServerSocket = socket(PF_INET, SOCK_DGRAM, 0);
  	if (bind(ServerSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) == -1)    
    	{    
        	printf("Server Bind Port: %d Error!\n", port);    

    	} 
	socklen_t sin_size = sizeof(struct sockaddr_in);
	printf("server %d ready!\n",self);

	char ans = ha[self][0];
	string ss11 = string(1,ans);
    string id = "file";
	id.append(ss11);
	string ext = ".txt";
	string name1 = id + ext;
	std::ofstream outfile(name1);
	printTable(mapTable, &outfile);

	while(1)
	{
		if(!sleeping)
		{
		memset(buf,0,sizeof(buf));
		if(recvfrom(ServerSocket,buf,BufferSize,0,(struct sockaddr *)&ClientAddr,&sin_size) < 0)
		{
			printf("receive error!");
		}
	
		decoder(self,buf, &outfile);	
		}
	}
	return NULL;	
}

void son_main()
{
	printf("%d %d\n",self,selfport);
	memset(Map,0,sizeof(Map));//clear the map read from txt
	printf("Son NO.%d\n%d\n",self,selfsumedg);
	addNode(ha[self][0],0,mapTable);
	for(int i = 0;i < selfsumedg; i ++)
	{
		addNode(ha[selfMap[i].to][0], selfMap[i].cost, mapTable);
		updateTable(ha[self][0], ha[selfMap[i].to][0], selfMap[i].cost, mapTable);
	}
	pthread_t ThreadID;
	int ret = pthread_create(&ThreadID,NULL,server,NULL);
	if(ret != 0)printf("error\n");
	
	printf("join!\n");
	sleep(2);
	for(int i = 0;i < selfsumedg;i ++)
	{
		pthread_t ThreadID;
		plate = 0;
		int ret = pthread_create(&ThreadID,NULL,client,(void*)&i);
		while(plate == 0){usleep(3);}
		
		if(ret != 0)printf("error\n");
	
	}
	int lasttime = INT_MAX;
	while(1)
	{
		time_t T1;
		struct timeb t1;
		ftime(&t1);
		time(&T1);
		int TIMER = T1*1000 + t1.millitm;
		if(sleeping == 1)
		{
			lasttime = min(lasttime,TIMER);
			if(TIMER - lasttime > 30000)
			{
				sleeping = 0;
				lasttime = INT_MAX;
				cout<<"Wake UP "<<ha[self]<<endl;
			}
		}

	}
}
int main(int argc,char *argv[])
{
	
 	init();
	for(int i = 0;i < sumhash;i ++)
	{
		memset(selfMap,0,sizeof(selfMap));
		int sumedg = 0;
		for(int j = 1;j <= sumedge;j ++)
		{
			if(Map[j].from == i)
			{
				selfMap[sumedg].from = i;
				selfMap[sumedg].to = Map[j].to;
				selfMap[sumedg].cost = Map[j].cost;
				selfMap[sumedg].port = Map[j].port;
				sumedg ++;
			}
			if(Map[j].to == i)selfport = Map[j].port;
			
		}selfsumedg = sumedg;
		self = i;
		if(fork()>0){}
		else
		{
			son_main();
			return 0;
		}
	}
	
	return 0;
}

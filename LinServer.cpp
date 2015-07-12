#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <winsock2.h>

void* SocketHandler(void*);
typedef struct datos{
	char *ip;
	int pto;
	int *arr;
}cliente;



void *f(void *);
void bubble(int [],int ); 



int main(int argv, char** argc){

	cliente c[4];
	int n=1101,i;

	for(i=0; i<4; i++){
		c[i].ip="127.0.0.1";
		c[i].pto=n++;
	}

	
	pthread_t id[4];
	for(i=0; i<4; i++){
		pthread_create(&id[i],NULL,f,(void *) &c[i]);
	}


	for(i=0; i<4; i++){
		pthread_join(id[i],NULL);
	}
	
	

	

}


void *f(void *p){
	cliente *cl;
	cl=(cliente *)p;
	int host_port= cl->pto;

	struct sockaddr_in my_addr;

	int hsock;
	int * p_int ;
	int err;

	socklen_t addr_size = 0;
	int* csock;
	sockaddr_in sadr;
	pthread_t thread_id=0;


	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n", errno);
		exit(0);
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
		
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", errno);
		free(p_int);
		exit(0);
	}
	free(p_int);

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;
	
	if( bind( hsock, (sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		fprintf(stderr,"Error binding to socket, make sure nothing else is listening on this port %d\n",errno);
		exit(0);
	}
	if(listen( hsock, 10) == -1 ){
		fprintf(stderr, "Error listening %d\n",errno);
		exit(0);
	}

	//Now lets do the server stuff

	addr_size = sizeof(sockaddr_in);
	
	
		printf("waiting for a connection\n");
		csock = (int*)malloc(sizeof(int));
		if((*csock = accept( hsock, (sockaddr*)&sadr, &addr_size))!= -1){
			printf("---------------------\nReceived connection from %s, and pto:%d\n",inet_ntoa(sadr.sin_addr),cl->pto);
			
			
	int *buffer;
	int buffer_len = 4;
	int bytecount;
	buffer=(int *)malloc(sizeof(int));
	

	if((bytecount = recv(*csock, buffer, buffer_len, 0))== -1){
		fprintf(stderr, "Error receiving data %d\n", errno);
		exit(0);
	}
	int len=buffer[0];

	printf("LONGITUD RECIBIDA=%d\n",len);
	buffer_len = 4*len;
	free(buffer);
	buffer=(int *)malloc(len*sizeof(int));
		
		if((bytecount = recv(*csock, buffer, buffer_len, 0))== -1){
		fprintf(stderr, "Error receiving data %d\n", errno);
		exit(0);
	}
	int i;
		

           
           
        bubble(buffer,len);  



	if((bytecount = send(*csock, buffer, buffer_len, 0))== -1){
		fprintf(stderr, "Error sending data %d\n", errno);
		exit(0);
	}
	
	printf("Sent bytes %d\n", bytecount);

		}
		else{
			fprintf(stderr, "Error accepting %d\n", errno);
		}
		
	close(hsock);
}


void bubble(int a[],int n)  
  {  
        int i,j,t;  
         for(i=n-2;i>=0;i--)  
         {  
            for(j=0;j<=i;j++)  
  
                  {  
                    if(a[j]>a[j+1])  
                                    {  
                                      t=a[j];  
                                     a[j]=a[j+1];  
                                     a[j+1]=t;  
                                    }  
                   }  
         
  
           }
  }

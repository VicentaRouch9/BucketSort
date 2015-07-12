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
void *__gxx_personality_v0;
#define TAM 1000

void *f(void *);
typedef struct datos{
	char *ip;
	int pto;
	int len;
	int *arr;
}cliente;


int main(int argv, char** argc){
	cliente cl[4];
	int n=1101,i,aleatorio[TAM],longitud[4]={0,0,0,0};

   srand(time(NULL));
   
    for(i=0; i<TAM; i++){
	 aleatorio[i]=rand()%101;
	 if(aleatorio[i]<25)
		longitud[0]++;
	 else if(aleatorio[i]>24 && aleatorio[i]<50)
		longitud[1]++;	
	 else if(aleatorio[i]>49 && aleatorio[i]<75)
		longitud[2]++;
	 else if(aleatorio[i]>74)
		longitud[3]++;

}
	int *a,*b,*c,*d;
	a=(int *)malloc(longitud[0]*sizeof(int));
	b=(int *)malloc(longitud[1]*sizeof(int));
	c=(int *)malloc(longitud[2]*sizeof(int));
	d=(int *)malloc(longitud[3]*sizeof(int));
	int ai=0, bi=0, ci=0, di=0;
     for(i=0; i<TAM; i++){
	 if(aleatorio[i]<25)
		a[ai++]=aleatorio[i];
	 else if(aleatorio[i]>=25 && aleatorio[i]<50)
		b[bi++]=aleatorio[i];	
	 else if(aleatorio[i]>=50 && aleatorio[i]<75)
		c[ci++]=aleatorio[i];
	 else if(aleatorio[i]>=75)
		d[di++]=aleatorio[i];

}
    
    printf("l1=%d\n",longitud[0]);
    printf("l2=%d\n",longitud[1]);
    printf("l3=%d\n",longitud[2]);
    printf("l4=%d\n",longitud[3]);
	for(i=0; i<4; i++){
		cl[i].ip="127.0.0.1";
		cl[i].pto=n++;
		cl[i].len=longitud[i];
		cl[i].arr=(int *)malloc(longitud[i]*sizeof(int));
		switch(i){
		case 0:
		cl[i].arr=a;
		break;
		
		case 1:
		cl[i].arr=b;
		break;
		
		case 2:
		cl[i].arr=c;
		break;
		
		case 3:
		cl[i].arr=d;
		break;
	}
	}

	
	pthread_t id[4];
	for(i=0; i<4; i++){
		pthread_create(&id[i],NULL,f,(void *) &cl[i]);
	}


	for(i=0; i<4; i++){
	  switch(i){
		case 0:
		pthread_join(id[i],(void **)&a);
		break;
		
		case 1:
		pthread_join(id[i],(void **)&b);
		break;
		
		case 2:
		pthread_join(id[i],(void **)&c);
		break;
		
		case 3:
		pthread_join(id[i],(void **)&d);
		break;
	}
	}

int x=0,j;
for(i=0; i<4; i++){
	for(j=0; j<longitud[i]; j++){
		
		switch(i){
		case 0:
		aleatorio[x++]=a[j];
		break;
		
		case 1:
		aleatorio[x++]=b[j];
		break;
		
		case 2:
		aleatorio[x++]=c[j];
		break;
		
		case 3:
		aleatorio[x++]=d[j];
		break;
	}
	
}
}
for(i=0; i<TAM; i++)
	printf("%d-%d\n",i,aleatorio[i]);

}

void *f(void *p){
	
	cliente *cl;
	cl=(cliente *)p;	
	int host_port= cl->pto;
	char* host_name=cl->ip;
	
	printf("pto:%d, b=%d\n",host_port,cl->arr[2]);
	
	struct sockaddr_in my_addr;

	int *buffer;
	int bytecount;
	int buffer_len=cl->len*4;

	int hsock;
	int * p_int;
	int err;

	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n",errno);
		exit(0);
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
		
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n",errno);
		free(p_int);
		exit(0);
	}
	free(p_int);

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = inet_addr(host_name);

	if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		if((err = errno) != EINPROGRESS){
			fprintf(stderr, "Error connecting socket %d\n", errno);
			exit(0);
		}
	}

	//Now lets do the client related stuff
	buffer_len=4;
	buffer=(int *)malloc(sizeof(int));
	buffer[0]=cl->len;
	
	printf("LONGITUD enviada=%d\n",cl->len);
	if( (bytecount=send(hsock, buffer, buffer_len,0))== -1){
		fprintf(stderr, "Error sending data %d\n", errno);
		exit(0);
	}
	buffer_len = (cl->len)*4;
	free(buffer);
	buffer=(int *)malloc(cl->len*sizeof(int));
	
	buffer=cl->arr;
	printf("pto:%d \n",cl->pto);

	
	if( (bytecount=send(hsock, buffer, buffer_len,0))== -1){
		fprintf(stderr, "Error sending data %d\n", errno);
		exit(0);
	}
	printf("Sent bytes %d\n", bytecount);
free(buffer);
	buffer=(int *)malloc(cl->len*sizeof(int));
	if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
		fprintf(stderr, "Error receiving data %d\n", errno);
		exit(0);
	}

	return buffer;
	close(hsock);
	



}

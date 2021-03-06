#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

 
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>//close()
#include<netinet/in.h>//struct sockaddr_in
#include<arpa/inet.h>//inet_ntoa
#define  QUEUE_LINE  12
#define  SOURCE_PORT 25302

#define BUF_SIZE 50
#define addressA "127.0.0.1"
#define portA 21302
#define addressB "127.0.0.1"
#define portB 22302
#define addressC "127.0.0.1"
#define portC 23302

 
#define  SOURCE_IP_ADDRESS "127.0.0.1"
float function;
float value;
float recv2[10];
float result;

void process_info(int s)
{
  int recv_num;
  int send_num;
  float recv_buf[2];
  float send_buf[2];
  recv_num = recv(s,recv_buf,sizeof(recv_buf),0);
  if(recv_num <0){
    perror("recv");
    exit(1);
  }
  else {
    recv_buf[recv_num] = '\0';			 
    function = recv_buf[0];
    value = recv_buf[1];
    if (value == 1){
    printf("The AWS received input <%f> and function = DIV from the client using TCP over port 25302.\n", value);
    }
    else if(value == 2){
    printf("The AWS received input <%f> and function = LOG from the client using TCP over port 25302.\n", value);
    }
  }
 }

void sendback(int s)
{
  int recv_num;
  int send_num;
  float send_buf[1];
  send_buf[0] = result;
  //printf("begin send\n");
  send_num = send(s,send_buf,sizeof(send_buf),0);
  if (send_num < 0){
    perror("send");
    exit(1);
  }
  else {
    printf("The AWS sent <%f> to client.\n", result);
  }
}

void serverA24()
{
  int recv_num;
  int send_num;
  
  //Create serv_sock 
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  //Ready to connect serverA using UDP
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));  
  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_addr.s_addr = inet_addr(addressA);  
  serv_addr.sin_port = htons(portA);  
    
  sockaddr fromAddr;
  int addrLen = sizeof(fromAddr);
  socklen_t serv_addr_size = sizeof(serv_addr);
  float send_buffer[] = {value};
  float recv_buffer[50];

  //Send valuse to serverA
  sendto(sock, send_buffer,sizeof(send_buffer), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
  printf("The AWS sent <%f> to Backend-Server A.\n", send_buffer[0]);
  //Receive square of the value from serverA
  int strLen = recvfrom(sock, recv_buffer, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_size); 
  printf("The AWS received <%f> Backend-Server A using UDP over port 24302.\n",recv_buffer[0]);
      
  recv2[0] = recv_buffer[0];
  send_buffer[0] = recv_buffer[0];
  
  //Send square of the value to serverA
  sendto(sock, send_buffer,sizeof(send_buffer), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
  printf("The AWS sent <%f> to Backend-Server A.\n", send_buffer[0]);
  //Receive square of the square from serverA
  strLen = recvfrom(sock, recv_buffer, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_size);
  printf("The AWS received <%f> Backend-Server A using UDP over port 24302.\n",recv_buffer[0]);
  
  recv2[2] = recv_buffer[0];

  //printf("[0]: %f\n",recv2[0]);
  //printf("[2]: %f\n",recv2[2]);
  //Close the socket
  close(sock);
}

void serverB3()
{
  int recv_num;
  int send_num;
  
  //Create serv_sock
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  if(sock < 0){
    perror("socket");
    exit(1);
  }
  else{
    //printf("sockB3 sucessful\n");
  }
 
  //Ready to connect serverB using UDP
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));  
  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_addr.s_addr = inet_addr(addressB);
  serv_addr.sin_port = htons(portB);  
    
  sockaddr fromAddr;
  int addrLen = sizeof(fromAddr);
  socklen_t serv_addr_size = sizeof(serv_addr);
  float send_buffer[] = {value};
  float recv_buffer[50];
  //Send value to serverB   
  send_num = sendto(sock, send_buffer,sizeof(send_buffer), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
  if (send_num < 0){
    perror("sendto");
    exit(1);
  }
  else{
    printf("The AWS sent <%f> to Backend-Server B.\n", send_buffer[0]);
  }
  //Receive square of the value from serverB
  int strLen = recvfrom(sock, recv_buffer, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_size);
  if (send_num < 0){
    perror("recvfrom");
    exit(1);
  }
  else{
    printf("The AWS received <%f> Backend-Server B using UDP over port 24302.\n",recv_buffer[0]);
  }
  recv2[1] = recv_buffer[0];

  //printf("[0]: %f\n",recv2[0]);
  //printf("[1]: %f\n",recv2[1]);
  //printf("[2]: %f\n",recv2[2]);
  
  //Close the socket
  close(sock);
}

void serverC5()
{
  int recv_num;
  int send_num;
  
  //Create serv_sock 
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  //Ready to connect serverC using UDP
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr)); 
  serv_addr.sin_family = AF_INET;  
  serv_addr.sin_addr.s_addr = inet_addr(addressC);  
  serv_addr.sin_port = htons(portC); 
    
  sockaddr fromAddr;
  int addrLen = sizeof(fromAddr);
  socklen_t serv_addr_size = sizeof(serv_addr);
  float send_buffer[] = {value};
  float recv_buffer[50];
  //Send value to serverC   
  sendto(sock, send_buffer,sizeof(send_buffer), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
  printf("The AWS sent <%f> to Backend-Server C.\n", send_buffer[0]);
  //Receive square of the value from serverC
  int strLen = recvfrom(sock, recv_buffer, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_size);
  printf("The AWS received <%f> Backend-Server C using UDP over port 24302.\n",recv_buffer[0]);
  
  recv2[3] = recv_buffer[0];
  
  //printf("[0]: %f\n",recv2[0]);
  //printf("[1]: %f\n",recv2[1]);
  //printf("[2]: %f\n",recv2[2]);
  //printf("[3]: %f\n",recv2[3]);
  
  //Close the socket
  close(sock);
}

void serverB6()
{
  int recv_num;
  int send_num;
  
  //Create serv_sock 
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  //Ready to connect serverB using UDP
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_addr.s_addr = inet_addr(addressB);
  serv_addr.sin_port = htons(portB);
    
  sockaddr fromAddr;
  int addrLen = sizeof(fromAddr);
  socklen_t serv_addr_size = sizeof(serv_addr);
  float send_buffer[] = {recv2[0]};
  float recv_buffer[50];

  //Send value to serverB
  sendto(sock, send_buffer,sizeof(send_buffer), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
  printf("The AWS sent <%f> to Backend-Server B.\n", send_buffer[0]);
  //Receive square of the value from serverB
  int strLen = recvfrom(sock, recv_buffer, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_size);
  printf("The AWS received <%f> Backend-Server B using UDP over port 24302.\n",recv_buffer[0]);
  
  recv2[4] = recv_buffer[0];

  //printf("[0]: %f\n",recv2[0]);
  //printf("[1]: %f\n",recv2[1]);
  //printf("[2]: %f\n",recv2[2]);
  //printf("[3]: %f\n",recv2[3]);
  //printf("[4]: %f\n",recv2[4]);
  
  //Close the socket
  close(sock);
}

 int main()
 {
   printf("The AWS is up and running.\n");
   int sock_fd,conn_fd;
   socklen_t client_len;
   pid_t pid;
   int process;
   int i;
   //--reused from this link: http://blog.csdn.net/tigerjibo/article/details/6775534
   struct sockaddr_in addr_serv,addr_client;
   sock_fd = socket(AF_INET,SOCK_STREAM,0);
   if(sock_fd < 0){
     perror("socket");
     exit(1);
   }
   else{
     // printf("sock sucessful\n");
   }
   //Bind sock with IP address and port number
   memset(&addr_serv,0,sizeof(addr_serv));
   addr_serv.sin_family = AF_INET;
   addr_serv.sin_port = htons(SOURCE_PORT);
   addr_serv.sin_addr.s_addr =inet_addr(SOURCE_IP_ADDRESS);
   client_len = sizeof(struct sockaddr_in);
   if(bind(sock_fd,(struct sockaddr *)&addr_serv,sizeof(struct sockaddr_in))<0){
     perror("bind");
     exit(1);
   }
   else{
     //printf("bind sucess\n");
   }
   if (listen(sock_fd,QUEUE_LINE) < 0){
     perror("listen");
     exit(1);
   }
   else {
     //printf("listen sucessful\n");
   }
   while(1){
     // printf("begin accept:\n");
     conn_fd = accept(sock_fd,(struct sockaddr *)&addr_client,&client_len);
     if(conn_fd < 0){
       //perror("accept");
       exit(1);
     }
     //--
     //printf("accept a new client,ip:%s\n",inet_ntoa(addr_client.sin_addr));
     pid = fork();
     if(0 == pid){         
       close(sock_fd);
       process_info(conn_fd);
       //printf("function : %f\n", function);
       //printf("value : %f\n", value);
       serverA24();			 
       serverB3();
       serverC5();
       serverB6();
       printf("Values of powers received by AWS: <%f, %f, %f, %f, %f, %f>\n", value, recv2[0],recv2[1],recv2[2],recv2[3],recv2[4]);
       if (function == 1){
         result = 1 + value;
         for(i=0;i<5;i++){
           result = result + recv2[i];    
         }
         printf("AWS calculated DIV on <%f> : <%f>.\n", value, result);
       }
       else if(function == 2){
         result = -1 * value;
         for(i=0;i<5;i++){
           result = result - (recv2[i])/(i+2);
         }
         printf("AWS calculated LOG on <%f> : <%f>.\n", value, result);
       }
       sendback(conn_fd);		  
      }		  		  
     else{
       close(conn_fd);
     }     
   }
 }


#include "control.h"

#include <stdio.h>		/* Basic I/O routines          */
#include <sys/types.h>		/* standard system types       */
#include <netinet/in.h>		/* Internet address structures */
#include <sys/socket.h>		/* socket interface functions  */
#include <netdb.h>		/* host to IP resolution       */
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fstream>

#define SA struct sockaddr
#define	BUFLEN		1024	/* maximum response size    */
#define	PORT		21	/* port of daytime server   */

void err_quit(const char* pszMsg)
{
  puts(pszMsg);
  exit(0);
}
int pasv_connect(struct sockaddr_in	sa, int control_sock)
{
		char		buf[BUFLEN+1]; /* buffer server answer */
		memset(buf,0,sizeof(buf));
		/* 命令 ”PASV\r\n” */
		sprintf(buf,"PASV\r\n");
		/* 客?端告?服?器用被?模式 */
		write(control_sock, buf, strlen(buf));
		/*客?端接收服?器的???和新?的端口?，
		* 正常? ”227 Entering passive mode (<h1,h2,h3,h4,p1,p2>)” */
		recv(control_sock,buf,BUFLEN,0);
		cout<<buf<<endl;
		//cout<<"!!!!!!!!!!!!!"<<endl;

		//Calculate the listening port of PASSIVE mode in FTP server.
		char* tmp = strtok(buf, ",");
		tmp = strtok(NULL, ",");
		tmp = strtok(NULL, ",");
		tmp = strtok(NULL, ",");
		tmp = strtok(NULL, ",");
		int p1 = atoi(tmp);
		tmp = strtok(NULL, ",");
		int p2 = atoi(tmp);
		int server_pasv_port = (p1)*256+(p2);
		//cout<<p1<<"---"<<p2<<endl;
		//cout<<server_pasv_port<<endl;
		
		int data_sock = socket(AF_INET, SOCK_STREAM, 0);
	    if (data_sock < 0) {
	    perror("socket: allocation failed");
        }
		
		/*//Get the local port(N) of the connection to FTP server(port 21).
		struct sockaddr_in Addr;
		socklen_t leng = sizeof(Addr);
	    if(-1==getsockname(control_sock, (SA*)&Addr, &leng))
	    err_quit("getsockname error");
		int to_server_port = ntohs(Addr.sin_port);

	    memset(&Addr, 0, sizeof(Addr));
		Addr.sin_family = AF_INET;
        Addr.sin_addr.s_addr = htonl(INADDR_ANY);	
		Addr.sin_port = htons(to_server_port+1);//(N+1), But in reality, it's no need. Just another available port.
	    if(-1==bind(data_sock,(SA*)&Addr,sizeof(Addr)))
        err_quit("bind error");*/
		
    /*struct sockaddr_in	sa2;
	memset(&sa2, 0, sizeof(sa2));
    sa2.sin_family = AF_INET;
    sa2.sin_port = htons(server_pasv_port);
    memcpy(&sa2.sin_addr.s_addr, hen->h_addr_list[0], hen->h_length);*/
	
	sa.sin_port = htons(server_pasv_port);
    int rc = connect(data_sock, (struct sockaddr *)&sa, sizeof(sa));
    /* check there was no error */
    if (rc) {
	perror("connect failed");
	//cout<<"connect!!"<<endl;
    }
	return data_sock;
}
void ftp()
{
    int			rc;            /* system calls return value storage */
    int         control_sock, data_sock;             /* socket descriptor */
    char		buf[BUFLEN+1]; /* buffer server answer */
    char*		pc;            /* pointer into the buffer */
    struct sockaddr_in	sa;            /* Internet address struct */
    struct hostent*     hen; 	       /* host-to-IP translation */
	size_t result;
    /* check there are enough parameters */
    /*if (argc < 2) {
	fprintf(stderr, "Missing host name\n");
	exit (1);
    }*/

    hen = gethostbyname("211.76.174.219");
    if (!hen) {
  	perror("couldn't resolve host name");
    }

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    memcpy(&sa.sin_addr.s_addr, hen->h_addr_list[0], hen->h_length);

    control_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (control_sock < 0) {
	perror("socket: allocation failed");
    }
    rc = connect(control_sock, (struct sockaddr *)&sa, sizeof(sa));
	//If the connection or binding succeeds, zero is returned. On error, -1 is returned, and errno is set appropriately.

    /* check there was no error */
    if (rc) {
	perror("connect failed");
	//cout<<"connect!!"<<endl;
    }
    pc = buf;
//===============================================
	memset(buf,0,sizeof(buf));
	recv(control_sock,buf,BUFLEN,0);//welcome message
	cout<<buf<<endl;
	//cout<<"!!!!!!!!!!!!!"<<endl;
	
	memset(buf,0,sizeof(buf));
	sprintf(buf,"USER %s\r\n","anonymous");// is stored as a C string
	write(control_sock,buf,strlen(buf));//strlen (without including the terminating null character itself)
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;
	//cout<<"!!!!!!!!!!!!!"<<endl;
	
	memset(buf,0,sizeof(buf));
	sprintf(buf,"PASS %s\r\n","anonymous");
	write(control_sock,buf,strlen(buf));
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;
	//cout<<"!!!!!!!!!!!!!"<<endl;

	/*memset(buf,0,sizeof(buf));
	sprintf(buf,"OPTS UTF8 ON\r\n");
	write(control_sock,buf,strlen(buf));
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;*/
	
	/*memset(buf,0,sizeof(buf));
	sprintf(buf,"CWD /\r\n");
	write(control_sock,buf,strlen(buf));
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;*/
	
	/*memset(buf,0,sizeof(buf));
	sprintf(buf,"TYPE I\r\n");
	write(control_sock,buf,strlen(buf));
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;*/
	
	//======================================================
	data_sock = pasv_connect(sa,control_sock);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"LIST\r\n");
	write(control_sock,buf,strlen(buf));
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;//150 Here comes the directory listing.
	//memset(buf,'\0',sizeof(buf));
	while((result = recv(data_sock,buf,BUFLEN,0)) > 0)
	{
		buf[result] = '\0';
		cout<<buf;
		//memset(buf,0,sizeof(buf));
	}
	//cout<<endl;
	memset(buf,0,sizeof(buf));
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;//226 Directory send OK./**/
	//======================================================
	cout<<"please enter the file name you want to download: ";
	char filename[100];
	fgets(filename, 50, stdin);//max = 50-1
	
	data_sock = pasv_connect(sa,control_sock);
	memset(buf,0,sizeof(buf));
	/* 命令 ”RETR filename\r\n” */
	sprintf(buf,"RETR %s\r\n",filename);
	/* 客?端?送命令?服?器端下?文件 */
	write(control_sock, buf, strlen(buf));
	/* 客?端接收服?器的???和信息，正常? ”150 Opening data connection.” */
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;
	//["RETR", [110, 125, 150, 226, 250, 421, 425, 426, 451, 450, 500, 501, 530, 550]],
	if(atoi(strtok(buf, " 	")) == 150)
	{
		//cout<<"it's OK"<<endl;
		FILE* pFile;
		pFile = fopen (filename,"w");//"genefile18.final"
		if (pFile==NULL)
		{
			cerr << "Data cannot be opened correctly.";
			return ;
		}
		//memset(buf,0,sizeof(buf));
		while((result = recv(data_sock,buf,BUFLEN,0)) > 0)
		{
			fwrite (buf,1,result,pFile);
			//memset(buf,0,sizeof(buf));
		}
		fclose(pFile);
		
		close(data_sock);
		memset(buf,0,sizeof(buf));
		recv(control_sock,buf,BUFLEN,0);
		cout<<buf<<endl;//226 Transfer complete.
	}
	else 
	{
		cout<<"Some errors occur. You may enter the wrong file name."<<endl;
		close(data_sock);
	}
	//======================================================
	//close connection
	memset(buf,0,sizeof(buf));
	/* 命令 ”QUIT\r\n” */
	sprintf(buf,"QUIT\r\n");
	/* 客?端???与服?器端的?接 */
	write(control_sock, buf, strlen(buf));
	/* 客?端接收服?器的???，正常? ”200 Closes connection.” */
	recv(control_sock,buf,BUFLEN,0);
	cout<<buf<<endl;//221 Goodbye.
	/* 客?端??控制?接 */
	close(control_sock);
	//======================================================
	cout<<"======<successfully end!>====="<<endl;
	//getchar();
    *pc = '\0';
}

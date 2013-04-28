/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月27日 03時16分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  grapefruit623 (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define	BUFSIZE 4096			/*  */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  str_cli
 *  Description:  
 * =====================================================================================
 */
		void
str_cli ( int out, int in, int sockfd )
{
		int n;
		char sendLine[BUFSIZE], recvLine[BUFSIZE];

		bzero(recvLine, BUFSIZE);
		bzero(sendLine, BUFSIZE);

//		n = read(sockfd, recvLine, BUFSIZE );
//		recvLine[n] = '\0';
//		write(out, recvLine, strlen(recvLine) );
		while ( 0 < ( n = read(sockfd, recvLine, BUFSIZE ))  ) {

				write(out, recvLine, BUFSIZE );
				read(in, sendLine, BUFSIZE);
				if ( !strcmp(sendLine, "\n") ) {
						printf ( "n = %d\n", strlen(sendLine) );
						sendLine[1] = '\0';
				}
				else {
						sendLine[strlen(sendLine)-1] = '\0';           /* to elimate '\n' */
				}
				write(sockfd, sendLine, BUFSIZE );
				bzero(recvLine, BUFSIZE);
				bzero(sendLine, BUFSIZE);

//				if ( 0 > ( n = read( sockfd, recvLine, BUFSIZE) ) )
//						fprintf(stderr, "server is terminated");
//				recvLine[strlen(recvLine)] = '\0';
//				write(out, recvLine, strlen(recvLine));
		}
		return ;
}		/* -----  end of function str_cli  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
		int
main ( int argc, char *argv[] )
{
		struct sockaddr_in myAddr;
		struct sockaddr_in clientInfo;
		socklen_t len = sizeof(clientInfo);
		int socketId = socket(AF_INET, SOCK_STREAM, 0);
		int port = 8051;
		char mes[BUFSIZE] = "hello world";
		char recvBuf[BUFSIZE];


		printf ( "The client is running\n" );


		if ( !socketId ) {
				fprintf(stderr, "socket failed");
		}
		
		bzero(&myAddr, sizeof(myAddr));
		myAddr.sin_family = AF_INET;
		myAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any ip for the host */
		myAddr.sin_port = htons(port);
		if ( connect( socketId,(struct sockaddr*)&myAddr, sizeof(myAddr)  ) )
				fprintf(stderr, "connect fail\n");

		str_cli(fileno(stdout), fileno(stdin), socketId);
/* 		send( socketId, mes, sizeof(mes), 0 );
 * 		recv( socketId, recvBuf, 4096, 0 );
 * 
 * 		printf ( "%s\n", recvBuf );
 */


		return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

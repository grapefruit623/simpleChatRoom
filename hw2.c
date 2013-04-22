/*
 * =====================================================================================
 *
 *       Filename:  hw2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月21日 17時12分13秒
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
#include <signal.h>
#include <fcntl.h>
#include <time.h>

#define	BUFFSIZE 4096			/*  */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sigFork
 *  Description:  to avoid zombile 
 * =====================================================================================
 */
		void
sigFork ( int sig )
{
		pid_t pid;
		int stat;
		pid = waitpid(-1, &stat, 0);
		return ;
}		/* -----  end of function sigFork  ----- */
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
		int bindId;
		int port = 8051;
		int acceptId;
		int forkId;
		char endBuf[] = "<html><body> <h1> hello world </h1> </body></html>";
		char ip[64];


		printf ( "The server is running\n" );

		signal(SIGCHLD, sigFork);               /* to avoid zombile */

		if ( !socketId ) {
				fprintf(stderr, "socket failed");
		}
		
		bzero(&myAddr, sizeof(myAddr));
		myAddr.sin_family = AF_INET;
		myAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any ip for the host */
		myAddr.sin_port = htons(port);

		bindId = bind( socketId, (struct sockaddr *)&myAddr, sizeof(myAddr));
		
		if ( 0 > bindId  ) {
				fprintf(stderr, "bind failed\n");
				return 0;
		}

		if ( 0 > listen( socketId, 10) ) {
				fprintf(stderr, "listening failed\n");
				return 0;
		}
		while ( 1 ) {
				acceptId = accept(socketId, (struct sockaddr *)&clientInfo, &len);
				strcpy(ip, inet_ntoa(clientInfo.sin_addr )); /* to get ipv4 */
				forkId = fork();
				if ( 0 > forkId ) {
						fprintf(stderr, "fork failed\n");
				}
				else {
						
						if ( 0 == forkId ) {    /* son */
								if ( -1 == acceptId ) {
										fprintf(stderr, "accept fail: %s\n", strerror(errno));
								}
								else {

										printf ( "a request from: %s\n", ip );
										write(acceptId, endBuf, sizeof(endBuf));
										exit(1);
								}
						}
						else {                  /* father */
//								fprintf(stdout, "I am your father\n");
						}
				}

				close(acceptId);
		}
		close(socketId);	
		close(acceptId);
		return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

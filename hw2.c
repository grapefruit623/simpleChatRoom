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

#include "func.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  echo
 *  Description:  
 * =====================================================================================
 */
		void
echo ( int acceptId )
{
		char recvBuf[BUFFSIZE];
		char sendBuf[BUFFSIZE];
		const char welcome[] = "welcome!!!\n";
		int len = 0;

		if ( login( acceptId, recvBuf ) ) {
				bzero(recvBuf, BUFFSIZE);
				write(acceptId, welcome, strlen(welcome));
				while ( 0 < ( len = read(acceptId, recvBuf, BUFFSIZE) ) ) {
						recvBuf[len] = '\0';
						printf ( ">>%d %s\n", len, recvBuf );
						write(acceptId, recvBuf, strlen(recvBuf));
						bzero(recvBuf, BUFFSIZE);
				}
		}
		else {
				printf ( "login failed\n" );
		}
		return ;
}		/* -----  end of function echo  ----- */
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
		printf ( "pid: %d\n", pid );
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
										echo(acceptId);
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

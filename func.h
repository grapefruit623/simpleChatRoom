/*
 * =====================================================================================
 *
 *       Filename:  func.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月27日 12時41分23秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  grapefruit623 (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef FUNC_H

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
#define	userCanHandle 13			/* total users 0, 1, 2, is stdIO */

#define	inputAccount 0			/* this user should send account */
#define	inputPasswd 1			/* this user should send passwd */
#define	cmdMode 2			/* this user in commend mode */
#define	chatMode 3			/* this user in chat mode */
#define	offLine -1			/*  this user is off-line */


struct message {
		char mes[BUFFSIZE];
		char sender[BUFFSIZE];
};				/* ----------  end of struct message  ---------- */

typedef struct message Message;

struct user {
		char name[BUFFSIZE];
		char passwd[BUFFSIZE];
		int socket;
		int toSomeone;
		int stage;
		int serverCache;
};				/* ----------  end of struct user  ---------- */

typedef struct user User;

struct account {
		char name[BUFFSIZE];
		char passwd[BUFFSIZE];
		int howManyOffLine;
		Message offLineMesg[10];	
};				/* ----------  end of struct account  ---------- */

typedef struct account Account;

int login( int acceptId, const char *name );
int	userIsExist( const char *name, const char *passwd );
int requestHandler( int sockfd, char *incomingMes );
void getAccount();
#endif 

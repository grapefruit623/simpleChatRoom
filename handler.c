/*
 * =====================================================================================
 *
 *       Filename:  handler.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月28日 21時02分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  grapefruit623 (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "func.h" 

User allUsers[userCanHandle];
Account matching[userCanHandle];
int howManyUsers = 0;

const char yourName[BUFFSIZE] = ">>Login:\0";
const char yourPasswd[BUFFSIZE] = ">>Password:\0";
const char youAreNotUser[BUFFSIZE] = ">>There is no this account\n\0";
const char yourAreLogout[BUFFSIZE] = "Your Are Logout\n\0"; 
const char youAreOnline[BUFFSIZE] = "Your Are on-line\n\0";
const char youHaveBeenOnline[BUFFSIZE] = "You have been on-line\n\0";
const char cmdPrompt[3] = ">>\0";
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getAccount
 *  Description:  
 * =====================================================================================
 */
		void
getAccount ( )
{
		int fp, i;
		char account[BUFFSIZE];
		char *userList[BUFFSIZE], *userName, *userPasswd;

		fp = open("account/users.txt", O_RDWR);	
		read(fp, account, BUFFSIZE);
		userList[howManyUsers] = strtok(account, "\n"); 
		while ( NULL != userList[howManyUsers] ) {
				howManyUsers++;
				userList[howManyUsers] = strtok(NULL, "\n");
		}

		for ( i=0 ; i < howManyUsers ; i++ ) {
				userName = strtok( userList[i], ":"  );
				userPasswd = strtok( NULL, ":" );
				strcpy(matching[i].name, userName);
				strcpy(matching[i].passwd, userPasswd);
		}
		close(fp);
		return ;
}		/* -----  end of function getAccount  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initial
 *  Description:  
 * =====================================================================================
 */
		void
initial ( )
{
		int i = 0;
		
		getAccount();
		for ( i = 0; i<userCanHandle ; i++ ) {
				allUsers[i].stage = offLine;
		}
		return ;
}		/* -----  end of function initial  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  requestHandler
 *  Description:  
 * =====================================================================================
 */
		int
requestHandler( int sockfd, char *incomingMes)
{
		char name[BUFFSIZE], passwd[BUFFSIZE];
		int i = 0;

		if ( offLine == allUsers[sockfd].stage ) { /* request client to input account */
				printf ( "offline\n" );
				write(sockfd, yourName, strlen(yourName));
				allUsers[sockfd].stage = inputAccount;
				return 1;
		}
		if ( inputAccount == allUsers[sockfd].stage ) { /* it get account, now client should send passwd */
				printf ( "Now the incomingMes is account\n" );
				strcpy(name, incomingMes);
				write(sockfd, yourPasswd, strlen(yourPasswd));
				allUsers[sockfd].stage = inputPasswd; 
				return 1;
		}

		if ( inputPasswd == allUsers[sockfd].stage ) { /* to match account data */
				printf ( "Now your should input passwd\n" );
				strcpy(passwd, incomingMes);

				for ( i=0 ; i<howManyUsers ; i++ ) {

						if ( !strcmp(matching[i].name, name) && !strcmp(matching[i].passwd, passwd) ) { /* the user is exist on prebuilt file */
								printf ( "hello %s, %s\n", name, passwd );

								for ( i=0; i<userCanHandle ; i++ ) { /* check repeat log-in */

										if ( !strcmp( allUsers[i].name, name) &&
														( cmdMode == allUsers[i].stage || chatMode == allUsers[i].stage )) {
												write(sockfd, youHaveBeenOnline, strlen(youHaveBeenOnline));
												return 1;
										}
								}
								write(sockfd, youAreOnline, strlen(youAreOnline));
								allUsers[sockfd].stage = cmdMode;
								strcpy(allUsers[sockfd].name, name);
								strcpy(allUsers[sockfd].passwd, passwd);
								allUsers[sockfd].socket = sockfd;
								return 1;
						}
				}
				allUsers[sockfd].stage = offLine;
				write(sockfd, youAreNotUser, strlen(youAreNotUser));
		}
		if ( cmdMode == allUsers[sockfd].stage ) { /* the cmd mode */
				printf ( "<<%s\n", incomingMes );

				if ( !strcmp("logout", incomingMes) ) { /* logout */
						write( sockfd, yourAreLogout, strlen(yourAreLogout));
						allUsers[sockfd].stage = offLine;
						allUsers[sockfd].socket = -1;
						return 1;
				}
				if ( !strcmp("list", incomingMes)  ) { /* list online user */

						for ( i = 0; i < userCanHandle ; i++ ) {

								if ( ( sockfd != allUsers[i].socket ) && 
											( cmdMode == allUsers[i].stage || chatMode == allUsers[i].stage ) ) {
										write(sockfd, allUsers[i].name, strlen(allUsers[i].name));
										write(sockfd, " \0", 2); 
								}
						}
						return 1;
				}                               

				if ( !strcmp("broadcast", incomingMes) ) {
						for ( i = 0; i < userCanHandle ; i++ ) {
								if ( ( sockfd != allUsers[i].socket ) && 
											( cmdMode == allUsers[i].stage || chatMode == allUsers[i].stage ) ) {
										write(allUsers[i].socket, "broadcast", strlen("broadcast"));
										write(allUsers[i].socket, "\n\0", 2); 
								}
						}
						return 1;
				}
                                                /* echo */
				write(sockfd, cmdPrompt, strlen(cmdPrompt));
				write(sockfd, incomingMes, strlen(incomingMes));
				write(sockfd, "\n\0", 2);

				return 1;
		}
		return 0;
}


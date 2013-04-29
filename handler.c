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
const char youAreOnline[BUFFSIZE] = "\n>>Your Are on-line, in cmd mode\n\0";
const char youHaveBeenOnline[BUFFSIZE] = "You have been on-line\n\0";
const char theOneIsTalking[BUFFSIZE] = "he/she is talking with someone";
const char leaveChat[BUFFSIZE] = "leaving the chat mode";
const char youInChatMode[BUFFSIZE] = "Now you locate at the chat mode";
const char cmdPrompt[BUFFSIZE] = ">>\0";
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
				matching[i].howManyOffLine = -1;
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
				allUsers[i].socket = -1;
				allUsers[i].toSomeone = -1;
				allUsers[i].serverCache = -1;
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
		char name[BUFFSIZE], passwd[BUFFSIZE], cmdBuf[BUFFSIZE];
		char cmd[BUFFSIZE], userSay[BUFFSIZE];
		int i = 0, j = 0;

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

								for ( j=0; j<userCanHandle ; j++ ) { /* check repeat log-in */

										if ( !strcmp( allUsers[j].name, name) &&
														( cmdMode == allUsers[j].stage || chatMode == allUsers[j].stage )) {
												write(sockfd, youHaveBeenOnline, strlen(youHaveBeenOnline));
												return 1;
										}
								}
								write(sockfd, youAreOnline, strlen(youAreOnline));
								allUsers[sockfd].stage = cmdMode;
								strcpy(allUsers[sockfd].name, name);
								strcpy(allUsers[sockfd].passwd, passwd);
								allUsers[sockfd].socket = sockfd;
								allUsers[sockfd].serverCache = i; /* the index of user on the account data  */

								for ( j=0; j<= matching[i].howManyOffLine ; j++ ) { /* show off-line mesg */
										bzero(cmdBuf, strlen(cmdBuf));
										strcpy(cmdBuf, matching[i].offLineMesg[j].mes);
										strcat(cmdBuf, " from ");
										strcat(cmdBuf, matching[i].offLineMesg[j].sender);
										write(sockfd, cmdBuf, strlen(cmdBuf));
										write(sockfd, "\n", strlen("\n")); 
								}
								matching[i].howManyOffLine = -1; 
/* 								if ( NULL != matching[i].offLineMesg ) {
 *
 * 									There are bugs, even if printf
 * 								}
 */

								printf ( "%s %d\n", allUsers[sockfd].name, sockfd );
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
						strcpy(allUsers[sockfd].name, "offLine");
						strcpy(allUsers[sockfd].passwd, "offLine");
						return 1;
				}
				if ( !strcmp("list", incomingMes)  ) { /* list online user */

						for ( i = 0; i < userCanHandle ; i++ ) {

								if ( ( sockfd != allUsers[i].socket ) && 
											( cmdMode == allUsers[i].stage || chatMode == allUsers[i].stage ) ) {
										write(sockfd, cmdPrompt, strlen(cmdPrompt));
										write(sockfd, allUsers[i].name, strlen(allUsers[i].name));
										write(sockfd, "\n", strlen("\n")); 
								}
						}
						write(sockfd, cmdPrompt, strlen(cmdPrompt));
						return 1;
				}                               


				bzero(cmdBuf, strlen(cmdBuf));
				bzero(cmd, strlen(cmd));
				bzero(userSay, strlen(userSay));
				strcpy(cmdBuf, incomingMes);
				for ( i=0; i<strlen(cmdBuf) ;i++ ) {

						if ( ' ' == cmdBuf[i] ) {

								for ( j=0; j<i ; j++ ) {
										cmd[j] = cmdBuf[j];
								}
								cmd[j] = '\0';
								for ( j=i+1; j<strlen(cmdBuf) ; j++ ) {
										userSay[j-i-1] = cmdBuf[j];
								}
								userSay[j+1] = '\0';
								break;
						}
				}
				printf ( "user's cmd is %s%s\n", cmd, userSay );


				if ( !strcmp("broadcast", cmd) ) {
						for ( i = 0; i < userCanHandle ; i++ ) {
								if ( ( sockfd != allUsers[i].socket ) && 
											( cmdMode == allUsers[i].stage || chatMode == allUsers[i].stage ) ) {
										write(sockfd, cmdPrompt, strlen(cmdPrompt));
										write(allUsers[i].socket, userSay, strlen(userSay));
//										write(sockfd, "\n", strlen("\n")); 
								}
						}
						return 1;
				}
				if ( !strcmp("send", cmd) ) {   						
						strcpy(cmdBuf, userSay);
						bzero(userSay, strlen(userSay));
						bzero(cmd, strlen(cmd));
						for ( i=0; i<strlen(cmdBuf) ;i++ ) { /* to cut send name message */
								if ( ' ' == cmdBuf[i] ) {

										for ( j=0; j<i ; j++ ) {
												cmd[j] = cmdBuf[j];
										}
										cmd[j+1] = '\0';
										for ( j=i+1; j<strlen(cmdBuf) ; j++ ) {
												userSay[j-i-1] = cmdBuf[j];
										}
										userSay[j+1] = '\0';
										break;
								}
						}
						printf ( "user want to talk to %s, to say %s\n", cmd, userSay );

						for ( i = 0; i < userCanHandle ; i++ ) {
								if  ( !strcmp(allUsers[i].name, cmd) ) { /* search on-line user */
										if ( cmdMode == allUsers[i].stage || chatMode == allUsers[i].stage )  {
												bzero(cmdBuf, strlen(cmdBuf));
												strcpy(cmdBuf, userSay);
												strcat(cmdBuf, " from ");
												strcat(cmdBuf, allUsers[sockfd].name);
												write(allUsers[i].socket, cmdPrompt, strlen(cmdPrompt));
												write(allUsers[i].socket, cmdBuf, strlen(cmdBuf));
//												write(allUsers[i].socket, "\n", strlen("\n")); 
												return 1;
										}
								}
						}

						for ( j=0; j < userCanHandle ; j++ ) {
								if ( !strcmp(matching[j].name, cmd) ) {  /* he/she have benn on-line, but now is off-line */
										printf ( "not on-line\n" );
										printf ( "%d %s \n", j, matching[j].name );

										matching[j].howManyOffLine = ( ++matching[j].howManyOffLine ) % 10;
										strcpy(matching[j].offLineMesg[ matching[j].howManyOffLine ].sender, allUsers[sockfd].name);
										strcpy(matching[j].offLineMesg[ matching[j].howManyOffLine ].mes, userSay);


										write(sockfd, "he/she not on-line", strlen("he/she not on-line"));
										write(sockfd, "\n", strlen("\n")); 
										write(sockfd, cmdPrompt, strlen(cmdPrompt));
										return 1;
								}
						}

						write(sockfd, "he/she not on-line", strlen("he/she not on-line"));
						write(sockfd, "\n", strlen("\n")); 
						return 1;

				}

				if ( !strcmp("talk", cmd) && strcmp(allUsers[sockfd].name, userSay) ) { /* talk and not talk himself */
						printf ( "%s\n", userSay );
						for ( i = 0; i < userCanHandle ; i++ ) {
								if  ( !strcmp(allUsers[i].name, userSay) ) { /* search on-line user */
										if ( cmdMode == allUsers[i].stage  )  { /* he/she is not in talk */
												allUsers[i].stage = chatMode;
												allUsers[sockfd].stage = chatMode;
												allUsers[i].toSomeone = sockfd;
												allUsers[sockfd].toSomeone = i;
												write(sockfd, youInChatMode, strlen(youInChatMode)); /* to tell user, locate at chat mode */
												write(sockfd, "\n", strlen("\n")); 
												write(i, youInChatMode, strlen(youInChatMode)); /* to tell user, locate at chat mode */
												write(i, "\n", strlen("\n")); 
												return 1;
										}
										else {
												write(sockfd, theOneIsTalking, strlen(theOneIsTalking));
												return 1;
										}
								}
						}
						write(sockfd, "he/she not on-line", strlen("he/she not on-line"));
						write(sockfd, "\n", strlen("\n")); 
						return 1;
				}
				write(sockfd, cmdPrompt, strlen(cmdPrompt));

				return 1;
		}

		if ( chatMode == allUsers[sockfd].stage ) { /* to send mes to another on-line user */
				write(sockfd, cmdPrompt, strlen(cmdPrompt));
				if ( !strcmp("#exit", incomingMes) ) {
						allUsers[sockfd].stage = cmdMode;
						allUsers[ allUsers[sockfd].toSomeone ].stage = cmdMode;
						write(allUsers[sockfd].toSomeone, leaveChat, strlen(leaveChat)); /* tell user has left the chat mode */
						write(allUsers[sockfd].toSomeone, "\n", strlen("\n")); 
						write(sockfd, leaveChat, strlen(leaveChat)); /* tell user has left the chat mode */
						write(sockfd, "\n", strlen("\n")); 
						return 1;
				}
				write(allUsers[sockfd].toSomeone, "\n", strlen("\n")); 
				write(allUsers[sockfd].toSomeone, allUsers[sockfd].name, strlen(allUsers[sockfd].name) );
				write(allUsers[sockfd].toSomeone, ": ", strlen(": "));
				write(allUsers[sockfd].toSomeone, incomingMes, strlen(incomingMes));
				write(allUsers[sockfd].toSomeone, "\n", strlen("\n")); 
				write(allUsers[sockfd].toSomeone, cmdPrompt, strlen(cmdPrompt));
		}
		return 0;
}


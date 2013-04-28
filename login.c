/*
 * =====================================================================================
 *
 *       Filename:  login.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月27日 12時33分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  grapefruit623 (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "func.h"


char loginName[BUFFSIZE];
char loginPasswd[BUFFSIZE];


char *userList[BUFFSIZE];

int userFlag ;                               /* check is user exist? */
int fp ;

int login( int acceptId, const char *name ) {
		int len = 0;
		printf ( "wating for login\n" );

//		userFlag = 0;
//		while ( 1 ) {
//				bzero(loginName, BUFFSIZE);
//				bzero(loginPasswd, BUFFSIZE);
//				if ( !userFlag)                 /* the connect have been wrong account? */
//						write(acceptId, yourName, strlen(yourName));
//				else {
//						write(acceptId, youAreNotUser, strlen(youAreNotUser));
//						userFlag = 0;
//				}
//				len = read(acceptId, loginName, BUFFSIZE );
//				loginName[strlen(loginName)] = '\0';
//
//				write(acceptId, yourPasswd, strlen(yourPasswd));
//				len = read(acceptId, loginPasswd, BUFFSIZE );
//				loginPasswd[strlen(loginPasswd)] = '\0';
//
//				if ( !userIsExist( loginName, loginPasswd )  ) {
//						printf ( "not exist %s %s\n", loginName, loginPasswd );
//						userFlag = 1;
////						write(acceptId, youAreNotUser, strlen(youAreNotUser));
//						continue;
//				}
//				else {
//						printf ( "%s is on-line\n", loginName ); 
//						return 1;
//				}
//		}

		return 0;
}

int userIsExist( const char *name, const char *passwd ) {
//		char account[BUFFSIZE];
//		char *user, *userName, *userPasswd;
//		int howManyUsers = 0, i = 0;
//		bzero(account, BUFFSIZE);
//
//                                                /* read account file and retrive accounts */
//		fp = open("account/users.txt", O_RDWR);	
//		read(fp, account, BUFFSIZE);
//		userList[howManyUsers] = strtok(account, "\n"); 
//		while ( NULL != userList[howManyUsers] ) {
//				howManyUsers++;
//				userList[howManyUsers] = strtok(NULL, "\n");
//		}
//
//		for ( i=0 ; i < howManyUsers ; i++ ) {
//				printf ( "%s\n", userList[i] );
//				userName = strtok( userList[i], ":"  );
//				userPasswd = strtok( NULL, ":" );
//
//				if ( !strcmp( userName, name ) ) { /* the user is exist */
//
//						if ( !strcmp( userPasswd, passwd ) ) {
//								return 1;
//						}
//				}
//				
//		}
//		close(fp);
		return 0;
}

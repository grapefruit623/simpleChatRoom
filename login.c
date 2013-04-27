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

const char yourName[BUFFSIZE] = "Login: ";
const char yourPasswd[BUFFSIZE] = "Password: ";

char loginName[BUFFSIZE];
char loginPasswd[BUFFSIZE];

int login( int acceptId, const char *name ) {
		printf ( "wating for login\n" );
		bzero(loginName, BUFFSIZE);
		bzero(loginPasswd, BUFFSIZE);

/* 		write(acceptId, yourName, BUFFSIZE);
 * 		if ( 0 != read(acceptId, loginName, BUFFSIZE ) )
 * 				fprintf(stderr, "login name recv fail\n");
 * 		write(acceptId, yourPasswd, BUFFSIZE);
 * 		if ( 0 != read(acceptId, loginPasswd, BUFFSIZE ) )
 * 				fprintf(stderr, "login passwd recv fail\n");
 */
		write(acceptId, yourName, BUFFSIZE);
		while (  0 < read(acceptId, loginName, BUFFSIZE ) )
				write(acceptId, yourName, BUFFSIZE);

		write(acceptId, yourPasswd, BUFFSIZE);
		while (  0 < read(acceptId, loginPasswd, BUFFSIZE ))
				write(acceptId, yourPasswd, BUFFSIZE); 

		printf ( "%s %s\n", loginName, loginPasswd ); 
		return 1;
}

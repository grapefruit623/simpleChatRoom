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

int login( int acceptId, const char *name );

#endif 

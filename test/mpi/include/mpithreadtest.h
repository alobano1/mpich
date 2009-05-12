/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */
#ifndef MPITHREADTEST_H_INCLUDED
#define MPITHREADTEST_H_INCLUDED

#include "mpitestconf.h"

/* 
   Define routines to start a thread for different thread packages.  
   The routine that is started is expected to return data when it
   exits; the type of this data is 

   MTEST_THREAD_RETURN_TYPE
*/

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#define MTEST_THREAD_RETURN_TYPE DWORD
#define MTEST_THREAD_HANDLE HANDLE
#define MTEST_THREAD_LOCK_TYPE HANDLE
#else
#include <pthread.h>
#define MTEST_THREAD_RETURN_TYPE void *
#define MTEST_THREAD_HANDLE pthread_t
#define MTEST_THREAD_LOCK_TYPE pthread_mutex_t
#endif
int MTest_Start_thread(MTEST_THREAD_RETURN_TYPE (*fn)(void *p),void *arg);
int MTest_Join_threads( void );
int MTest_thread_lock_create( MTEST_THREAD_LOCK_TYPE *);
int MTest_thread_lock( MTEST_THREAD_LOCK_TYPE *);
int MTest_thread_unlock( MTEST_THREAD_LOCK_TYPE *);
int MTest_thread_lock_free( MTEST_THREAD_LOCK_TYPE *);
int MTest_thread_barrier_init(void);
int MTest_thread_barrier(int);
int MTest_thread_barrier_free(void);
#endif

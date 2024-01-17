#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <boxer/boxer.h>
#include <debugbreak.h>

#include <base.h>

#undef near //fuck deez macros lol
#undef far
#undef min
#undef max
#define COUNT(arr) (sizeof(arr)/sizeof(*arr))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAMP(a,min,max) ((a) < (min) ? (min) : (a) > (max) ? (max) : (a))
#define SWAP(temp,a,b) (temp)=(a); (a)=(b); (b)=(temp)
#define COMPARE(a,b) (((a) > (b)) - ((a) < (b)))
#define RGBA(r,g,b,a) ((r) | ((g)<<8) | ((b)<<16) | ((a)<<24))
#define TSTRUCT(name)\
typedef struct name name;\
struct name

#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if _DEBUG
#define ASSERT(cnd)\
	do {\
		if (!(cnd)){\
			fprintf(stderr,"Assert failed:\nFile: %s\nLine: %d\nCondition: %s\n",FILENAME,__LINE__,#cnd);\
			debug_break();\
			exit(1);\
		}\
	} while (0)
#else
#define ASSERT(cnd)\
	do {\
		if (!(cnd)){\
			fatal_error("Assert failed:\nFile: %s\nLine: %d\nCondition: %s",FILENAME,__LINE__,#cnd);\
			exit(1);\
		}\
	} while (0)
#endif

void fatal_error(char *format, ...);

void *malloc_or_die(size_t size);

void *zalloc_or_die(size_t size);

void *realloc_or_die(void *ptr, size_t size);

char *local_path_to_absolute(char *localPath);

char *load_file_as_cstring(char *localPath);

/*
rand_int
From: https://stackoverflow.com/a/822361
Generates uniform random integers in range [0,n).
*/
int rand_int(int n);

/*
rand_int_range
Generates uniform random integers in range [min,max],
where min >= 0 and max >= min.
*/
int rand_int_range(int min, int max);

/*
fnv_1a
Fowler-Noll-Vo hash function. https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
*/
uint32_t fnv_1a(int keylen, char *key);

//a proper modulo, handles negative numbers
int modulo(int i, int m);
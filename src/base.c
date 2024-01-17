#include <base.h>
#include <whereami.h>

void fatal_error(char *format, ...){
	va_list args;
	va_start(args,format);

	static char msg[4096];
	vsnprintf(msg,COUNT(msg),format,args);
	boxerShow(msg,"Error",BoxerStyleError,BoxerButtonsQuit);

	va_end(args);
	
	exit(1);
}

void *malloc_or_die(size_t size){
	void *p = malloc(size);
	ASSERT(p);
	return p;
}

void *zalloc_or_die(size_t size){
	void *p = calloc(1,size);
	ASSERT(p);
	return p;
}

void *realloc_or_die(void *ptr, size_t size){
	void *p = realloc(ptr,size);
	ASSERT(p);
	return p;
}

static char fmtStr[4096];
char *format_string(char *format, ...){
	va_list args;
	va_start(args,format);

	int len = vsnprintf(fmtStr,COUNT(fmtStr),format,args);
	ASSERT(len > 0 && len < COUNT(fmtStr));

	va_end(args);

	return fmtStr;
}

static char root[4096];
static char *rootEnd = 0;
static char *local_path_to_absolute_internal(char *format, va_list args){
	int len;
	if (!rootEnd){
		len = wai_getExecutablePath(0,0,0);
		ASSERT(len < COUNT(root));
		ASSERT(0 < wai_getExecutablePath(root,len,0));
		root[len] = 0;

		rootEnd = strrchr(root,'/');
		if (!rootEnd){
			rootEnd = strrchr(root,'\\');
			ASSERT(rootEnd);
		}
		rootEnd++;
		rootEnd[0] = 0;
	}

	size_t remaining = root+COUNT(root)-rootEnd;
	len = vsnprintf(rootEnd,remaining,format,args);
	ASSERT(len > 0 && len < remaining);

	return root;
}

char *local_path_to_absolute(char *format, ...){
	va_list args;
	va_start(args,format);

	char *path = local_path_to_absolute_internal(format,args);

	va_end(args);

	return path;
}

char *load_file_as_cstring(char *format, ...){
	va_list args;
	va_start(args,format);

	char *path = local_path_to_absolute_internal(format,args);
	FILE *f = fopen(path,"rb");
	if (!f){
		fatal_error("Could not open file: %s",path);
	}
	fseek(f,0,SEEK_END);
	long len = ftell(f);
	ASSERT(len > 0);
	fseek(f,0,SEEK_SET);
	char *str = malloc_or_die(len+1);
	fread(str,1,len,f);
	str[len] = 0;
	fclose(f);

	va_end(args);

	return str;
}

int rand_int(int n){
	if ((n - 1) == RAND_MAX){
		return rand();
	} else {
		// Chop off all of the values that would cause skew...
		int end = RAND_MAX / n; // truncate skew
		end *= n;
		// ... and ignore results from rand() that fall above that limit.
		// (Worst case the loop condition should succeed 50% of the time,
		// so we can expect to bail out of this loop pretty quickly.)
		int r;
		while ((r = rand()) >= end);
		return r % n;
	}
}

int rand_int_range(int min, int max){
	return rand_int(max-min+1) + min;
}

uint32_t fnv_1a(int keylen, char *key){
	uint32_t index = 2166136261u;
	for (int i = 0; i < keylen; i++){
		index ^= key[i];
		index *= 16777619;
	}
	return index;
}

int modulo(int i, int m){
	return (i % m + m) % m;
}
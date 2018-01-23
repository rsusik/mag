#ifndef COMMON_H
#define COMMON_H

/* Compile mode detection */
#if defined(_WIN32) || defined(_WIN64)
	#define WINDOWS
	#if defined(_WIN64)
		#define ENV64BIT
	#else
		#define ENV32BIT
	#endif
#endif

#if defined(__GNUC__)
	#define UNIX
	#if defined(__x86_64__) || defined(__ppc64__)
		#define ENV64BIT
	#else
		#define ENV32BIT
	#endif
#endif

#include <xmmintrin.h>
//#include <emmintrin.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <math.h>
#include <limits>
#include <iostream>
#include <algorithm> /* std:sort */

#ifdef WINDOWS
#ifndef NOMINMAX
#define NOMINMAX /* to disable windows.h min,max */
#endif
#include <windows.h>
#include <stdint.h> /* int64_t, etc. for Visual Studio */
#include <cstdint>
#endif

/* Print results flag */
#define PRINT_RESULTSS

/* Typedefs */
#ifdef ENV64BIT
	typedef		uint64_t word;
	typedef		long double ldouble;
#endif
#ifdef ENV32BIT
	typedef		uint32_t word;
	typedef		double ldouble;
#endif

typedef unsigned char uchar;
typedef unsigned char byte;

typedef unsigned int encoded_text_type;

/* Sizes of types */
const size_t CH_SIZE = sizeof(char);
const size_t PCH_SIZE = sizeof(char*);
const size_t PINT_SIZE = sizeof(unsigned int*);
const size_t BOOL_SIZE = sizeof(bool);
const size_t UCH_SIZE = sizeof(uchar);
const size_t BYTE_SIZE = sizeof(byte);

/* Other variables */
long counter = 0;
bool *indexes = 0;
long long v_count = 0;
long long verif = 0;
long pattern_text_size =0;
int pattern_size = 0;
unsigned int patt_size = 0;
unsigned long text_size = 0;
int map [256];	// if overoaded then probably std::map is conflicted

/* Time measurement stuff */
#ifdef UNIX

typedef struct {
	timespec start;
	timespec stop;
	timespec sum;
} t_timemes;
#define t_init(t) t.sum.tv_nsec = 0; t.sum.tv_sec = 0
#define t_start(t) clock_gettime(CLOCK_MONOTONIC, &(t.start))
#define t_stop(t) clock_gettime(CLOCK_MONOTONIC, &(t.stop)); \
t.sum.tv_sec += t.stop.tv_sec - t.start.tv_sec; \
t.sum.tv_nsec += t.stop.tv_nsec - t.start.tv_nsec
#define t_get_seconds(t) (double)t.sum.tv_sec + (double)t.sum.tv_nsec / (double)1000000000

#else

typedef struct {
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER tick;
	double start;   // A point in time
	double stop;
	double sum;
} t_timemes;
#define t_init(t) t.sum = 0; QueryPerformanceFrequency(&(t.ticksPerSecond))
#define t_start(t) QueryPerformanceCounter(&(t.tick)); t.start = double(t.tick.QuadPart)/t.ticksPerSecond.QuadPart
#define t_stop(t) QueryPerformanceCounter(&(t.tick)); \
t.stop = double(t.tick.QuadPart)/t.ticksPerSecond.QuadPart; \
t.sum = t.stop - t.start
#define t_get_seconds(t) t.sum

#endif


/*const ldouble log2 = log( (ldouble)2 );
static inline word ilog2( ldouble n ) {
	return (word) ( log( n ) / log2 );  
}*/
#define log2 0.69314718055994529
#define ilog2(n) (word) ( log( n ) / log2 )
// log cache (filled in main() function)
#define log2cache 70000
word log2v[log2cache];

const unsigned int MAX_WORD_SIZE = sizeof(word)*8;//, U = 16;

//void print_result(double data_acc_time, double pre_time, double search_time, long ver_count, long while_loops, long matches, long sigma, long super_sigma) {
//	printf("%ld\t%ld\t%f\t%f\t%f\t%ld\t%ld\t%ld", sigma, super_sigma, data_acc_time, pre_time, search_time, ver_count, while_loops, matches);
//}
//
//void print_resultn(double data_acc_time, double pre_time, double search_time, long ver_count, long while_loops, long matches, long sigma, long super_sigma) {
//	print_result(data_acc_time, pre_time, search_time, ver_count, while_loops, matches, sigma, super_sigma);
//	printf("\n");
//}

template <class T> char * deb_printf_binnary(T var) {
	unsigned int size = sizeof(T)*8;
	char *str = new char[size];
	char *tmp = str;
	for ( int i = size - 1; i >= 0; i--) {
		sprintf(tmp++, "%d", var >> i & 1);
	}
	return str;
}

template <class T> void printf_binnary(T var) {
	unsigned int size = sizeof(T)*8;
	
	for ( int i = size - 1; i >= 0; i--) {
		printf("%d", var >> i & 1);
	}
}

template <class T> long read_file_content( T**buffer, const char* filename ) {
	long length = -1;
	FILE * file = fopen (filename, "rb");
	if (file) {
		fseek (file, 0, SEEK_END);
		length = ftell (file);
		fseek (file, 0, SEEK_SET);
		*buffer = (T*)malloc (length+1);
		if (*buffer==NULL) return 0;
		if(fread (*buffer, 1, length, file)!=length) { printf("Error (read_file_content): Something went wrong"); return 0; }
		*(*buffer+length) = '\0';
		fclose (file);
	}
	return length;
}

static int	* build_verify_table( int m, int k, int r, int l ) {
	int	mk;
	int	h;
	int	mpk = m / k;
	int	c, f, * t;

	t = (int*)malloc( MAX_WORD_SIZE * sizeof( int ));

	for( f = 0; f < r; f++ ) {
		c = ( 1 - f - mpk ) * k;
		mk = ( mpk * l - 1 + f * l );

		for( h = 0; h < k; h++, c-- ) {
			t[ mk ] = c; 
			mk += mpk * l + r - 1;
		}
	}

	return t;
}

template<class T> int CreateMap(T* t, int n) {
	// Map array creation
	int sigma = 0;
	bool symbols [256];
	for (int i = 0; i < 256; ++i) symbols[i] = 0;
	for (long i = 0; i < n; ++i) 
		symbols[ t[i] ] = true;

	for (int i = 0; i < 256; ++i) map[i] = 0;
	for (int i = 0; i < 256; ++i) {
		if ( symbols[i] ) {
			map[i] = sigma++;
		}
	}
	return sigma;
}


bool CreateMapByHistogram_compar (unsigned long* i, unsigned long* j) { return (*i > *j); }

template<class T> int CreateMapByHistogram(T* t, int n) {
	// Map array creation
	int sigma = 0;
	int threshold = 22;
	unsigned long symbols [256];
	unsigned long * symbolsp[256];
	for (int i = 0; i < 256; ++i) {
		symbols[i] = 0;
		symbolsp[i] = &(symbols[i]);
	}
	for (long i = 0; i < n; ++i) 
		symbols[ t[i] ] ++;
	std::sort(symbolsp, &(symbolsp[255]), CreateMapByHistogram_compar);
	for (int i = 0; i < 256; ++i) map[i] = 0;
	for (int i = 0; i < threshold; ++i) {
		if(*symbolsp[i]) {
			*symbolsp[i] = sigma++;
		} else {
			*symbolsp[i] = sigma-1;
		}
	}
	for (int i = threshold; i < 256; ++i) {
		*symbolsp[i] = sigma<threshold?sigma-1:threshold-1;
	}
	for (int i = 0; i < 256; ++i) {
		if ( symbols[i] ) {
			map[i] = symbols[i];
		}
	}
	return sigma;
}

template<class T, unsigned int NEW_SIGMA> int CreateMapByHistogramBinPack(T* t, int n) {
	// Map array creation
	int sigma = 0;
	unsigned long symbols [256];
	unsigned long * symbolsp[256];
	long sum = 0;
	int binSize = n / (NEW_SIGMA-1);
	for (int i = 0; i < 256; ++i) {
		symbols[i] = 0;
		symbolsp[i] = &(symbols[i]);
	}
	for (long i = 0; i < n; ++i) 
		symbols[ t[i] ] ++;

	std::sort(symbolsp, &(symbolsp[255]), CreateMapByHistogram_compar);
	// symbolsp is now most common symbols (the most at first position)

	for (int i = 0; i < 256; ++i) map[i] = 0;
	for (int i = 0; i < 256; ++i) {
		sum += *symbolsp[i];
		if(sum >= binSize) {
			sum = sum - binSize;
			*symbolsp[i] = sigma++;
		} else {
			*symbolsp[i] = sigma;
		}
	}
	
	for (int i = 0; i < 256; ++i) {
		map[i] = symbols[i];
	}

	//for(unsigned int i = 0; i < 256; i++) { printf("map[%c (%d) ] = %d\n", (char)i, i, map[i]); }

	return ++sigma;
}

template<class T> int CreateMapByHistogramBinPackNextElem(T* t, int n, int new_sigma) {
	// Map array creation
	int sigma = 0;
	new_sigma--;
	unsigned long symbols [256];
	unsigned long * symbolsp[256];
	long sum = 0;
	int binSize = n / new_sigma;
	for (int i = 0; i < 256; ++i) {
		symbols[i] = 0;
		symbolsp[i] = &(symbols[i]);
	}
	for (long i = 0; i < n; ++i) 
		symbols[ t[i] ] ++;

	std::sort(symbolsp, &(symbolsp[255]), CreateMapByHistogram_compar);
	// symbolsp is now most common symbols (the most at first position)

	for (int i = 0; i < 256; ++i) map[i] = 0;
	for (int i = 0; i < 256; ++i) {
		sum += *symbolsp[i];
		if(sum >= binSize) {
			sum = sum - binSize;
			*symbolsp[i] = sigma++;
		} else if( i < 255 && sum + *symbolsp[i+1] > binSize && abs( (long)((sum + *symbolsp[i+1]) - binSize) ) > abs((long)(sum - binSize)) ) {
			sum = sum + *symbolsp[i+1] - binSize;
			*symbolsp[i] = sigma;
			*symbolsp[i+1] = sigma;
			sigma++;
			i++;
			printf("I'm here\n");
		} else {
			*symbolsp[i] = sigma;
		}
	}
	
	for (int i = 0; i < 256; ++i) {
		map[i] = symbols[i];
	}

	for(unsigned int i = 0; i < 256; i++) { printf("map[%c (%d) ] = %d\n", (char)i, i, map[i]); }

	return ++sigma;
}


template <unsigned int Q, unsigned int SIGMA> struct get_ { 
    static unsigned long long int gram(const uchar * s, const long start /* at start set to 1 */) {
        return map[s[Q - 1]] * start + get_<Q - 1, SIGMA>::gram(s, start*SIGMA);
    }
};

template <unsigned int SIGMA> struct get_<0, SIGMA> { 
    static unsigned int gram(const uchar * s, const long start /* at start set to 1 */) { return 0; }
};

template <class T, unsigned int Q, unsigned int SIGMA> encoded_text_type * encode(T * t) {
	encoded_text_type * p = (encoded_text_type *)malloc(sizeof( encoded_text_type ) * patt_size);
	for (long int i = 0, it = 0; it < patt_size; i+=Q, ++it) {
		p [it] = get_<Q, SIGMA>::gram(t + i, 1);
	}
	return p;
}

template<unsigned int Q, unsigned int SIGMA> 
struct get_super_ {
	static unsigned long sigma() {
		return get_super_<Q-1, SIGMA>::sigma() * SIGMA;
	}
};

template<unsigned int SIGMA> 
struct get_super_<0, SIGMA> {
	static unsigned long sigma() {
		return 1;
	}
};

#endif
#ifndef MAG_DYN_H
#define MAG_DYN_H

#include "common.h"

// plain text
uchar *text = NULL;
// patterns in text form
uchar *patterns_text = NULL;
// how many bytes can be compared (pattern_size - q)
int pelem_compar_bytes;

// patterns in text form
uchar patterns2[40000][500];

// data structures
typedef struct pelem {
	uchar * pattern; // shifed pattern
	uchar * op;      // pattern
	int offset;      // offset of shifted pattern
} pelem_;

typedef struct {
	pelem * list;    // list of patterns that have the same index (first q-gram)
	int count;       // number of patterns
} plist;

// patterns indexed by first qgram
plist *indexed_patterns = NULL;

// number of patterns
long patterns_number = 0;

// pelem comparator
int pelem_compar (const pelem* a, const pelem* b) {
	return memcmp(a->pattern, b->pattern, pelem_compar_bytes);
}

/* Definitions */
void print_result(double data_acc_time, double pre_time, double search_time, 
	long ver_count, long while_loops, long matches, long sigma, long super_sigma,
	long text_file_size, long pattern_size
) {
	double inmb = text_file_size / 1000.0 / 1000.0;
	double pre_search_time = pre_time + search_time;										// pre+search time
	printf("%ld\t%ld\t", sigma, super_sigma);												// sigmas
	printf("%ld\t", pattern_size);															// pattern size
	printf("%ld\t", text_file_size);														// data information
	printf("%ld\t%ld\t%ld\t", ver_count, while_loops, matches);								// search information
	printf("%f\t%f\t%f\t%f\t", data_acc_time, pre_time, search_time, pre_search_time);		// times
	printf("%f\t%f", inmb/search_time, inmb/pre_search_time);								// speed
	printf("\n");
}

namespace q_gram_utilities_dna_opt_l2 {

	struct get_1_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ( ( ( (*(uint64_t*)x) & 0xFF) >> 1 ) & 0x03 );
		}
		static int const q_size = 1;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_2_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ( ( ( ( ( (*(uint64_t*)x) & 0xFFFF) >> 1 ) & 0x0303 ) * 0x401 ) >> 8 ) & 0xF;
		}
		static int const q_size = 2;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_3_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ( ( ( ( ( (*(uint64_t*)x) & 0xFFFFFF ) >> 1 ) & 0x030303) * 0x100401) >> 16 ) & 0x3F;
		}
		static int const q_size = 3;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_4_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ( ( ( ( ( (*(uint64_t*)x) & 0xFFFFFFFF ) >> 1 ) & 0x03030303) * 0x40100401) >> 24 ) & 0xFF;
		}
		static int const q_size = 4;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_5_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ( ( ( ( ( (*(uint64_t*)x) & 0xFFFFFFFFFF ) >> 1 ) & 0x0303030303) * 0x4010040100401) >> 32 ) & 0x3FF;
		}
		static int const q_size = 5;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_6_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_3_gram::get_q_gram(x+3) | get_3_gram::get_q_gram(x)<<6;
		}
		static int const q_size = 6;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_7_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_4_gram::get_q_gram(x+3) | get_3_gram::get_q_gram(x)<<8;
		}
		static int const q_size = 7;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_8_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_4_gram::get_q_gram(x+4) | get_4_gram::get_q_gram(x)<<8;
		}
		static int const q_size = 8;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_9_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_5_gram::get_q_gram(x+4) | get_4_gram::get_q_gram(x)<<10;
		}
		static int const q_size = 9;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

	struct get_10_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_5_gram::get_q_gram(x+5) | get_5_gram::get_q_gram(x)<<10;
		}
		static int const q_size = 10;
		static size_t const super_sigma = (uint64_t)1<<(2*q_size);
	};

};

namespace q_gram_utilities_dna_opt_l3 {

	struct get_1_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ( ( ( (*(uint64_t*)x) & 0xFF) >> 1 ) & 0x07 );
		}
		static int const q_size = 1;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_2_gram {
		static uint64_t get_q_gram(uchar *x) {
			//return ( ( ( ( ( (*(uint64_t*)x) & 0xFFFF) >> 1 ) & 0x0707 ) * 0x421 ) >> 10 ) & 0x3F;
			return ( ( ( ( ( (*(uint64_t*)x) & 0xFFFF) >> 1 ) & 0x0707 ) * 0x200400801 ) >> 8 ) & 0x3F;
		}
		static int const q_size = 2;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_3_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ( ( ( ( ( (*(uint64_t*)x) & 0xFFFFFF ) >> 1 ) & 0x070707) * 0x200400801) >> 16 ) & 0x1FF;
		}
		static int const q_size = 3;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_4_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_2_gram::get_q_gram(x+2) | get_2_gram::get_q_gram(x)<<6;
			//uint64_t s1 = ( ( ( ( ( (*(uint64_t*)x) ) >> 1 ) & 0x0707 ) * 0x400801 ) >> 8 ) & 0x3F;
			//uint64_t s2 = ( ( ( ( ( (*(uint64_t*)(x+2))) >> 1 ) & 0x0707 ) * 0x400801 ) >> (8-6) ) & 0xFC0;
			//return s1 | s2;
		}
		static int const q_size = 4;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_5_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_3_gram::get_q_gram(x+2) | get_2_gram::get_q_gram(x)<<9;
		}
		static int const q_size = 5;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_6_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_3_gram::get_q_gram(x+3) | get_3_gram::get_q_gram(x)<<9;
		}
		static int const q_size = 6;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_7_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_4_gram::get_q_gram(x+3) | get_3_gram::get_q_gram(x)<<12;
		}
		static int const q_size = 7;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_8_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_4_gram::get_q_gram(x+4) | get_4_gram::get_q_gram(x)<<12;
		}
		static int const q_size = 8;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_9_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_5_gram::get_q_gram(x+4) | get_4_gram::get_q_gram(x)<<15;
		}
		static int const q_size = 9;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

	struct get_10_gram {
		static uint64_t get_q_gram(uchar *x) {
			return get_5_gram::get_q_gram(x+5) | get_5_gram::get_q_gram(x)<<15;
		}
		static int const q_size = 10;
		static size_t const super_sigma = (uint64_t)1<<(3*q_size);
	};

};

namespace q_gram_utilities_dna_l_bits {

	static uint64_t const l = Q_GRAM_UTIL_NEW_L;
	static uint64_t const b = ((uint64_t)1<<l)-(uint64_t)1;

	struct get_1_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[0] >> 1) & b);
		}
		static int const q_size = 1;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_2_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[1] >> 1) & b) + (((x[0] >> 1) & b) << l);
		}
		static int const q_size = 2;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_3_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[2] >> 1) & b) + (((x[1] >> 1) & b) << l) + (((x[0] >> 1) & b) << 2*l);
		}
		static int const q_size = 3;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_4_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[3] >> 1) & b) + (((x[2] >> 1) & b) << l) + (((x[1] >> 1) & b) << 2*l) + (((x[0] >> 1) & b) << 3*l);
		}
		static int const q_size = 4;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_5_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[4] >> 1) & b) + (((x[3] >> 1) & b) << l) + (((x[2] >> 1) & b) << 2*l) + (((x[1] >> 1) & b) << 3*l) + (((x[0] >> 1) & b) << 4*l);
		}
		static int const q_size = 5;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_6_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[5] >> 1) & b) + (((x[4] >> 1) & b) << l) + (((x[3] >> 1) & b) << 2*l) + (((x[2] >> 1) & b) << 3*l) + (((x[1] >> 1) & b) << 4*l) + (((x[0] >> 1) & b) << 5*l);
		}
		static int const q_size = 6;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_7_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[6] >> 1) & b) + (((x[5] >> 1) & b) << l) + (((x[4] >> 1) & b) << 2*l) + (((x[3] >> 1) & b) << 3*l) + (((x[2] >> 1) & b) << 4*l) + (((x[1] >> 1) & b) << 5*l) + (((x[0] >> 1) & b) << 6*l);
		}
		static int const q_size = 7;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_8_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[7] >> 1) & b) + (((x[6] >> 1) & b) << l) + (((x[5] >> 1) & b) << 2*l) + (((x[4] >> 1) & b) << 3*l) + (((x[3] >> 1) & b) << 4*l) + (((x[2] >> 1) & b) << 5*l) + (((x[1] >> 1) & b) << 6*l) + (((x[0] >> 1) & b) << 7*l);
		}
		static int const q_size = 8;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_9_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[8] >> 1) & b) + (((x[7] >> 1) & b) << l) + (((x[6] >> 1) & b) << 2*l) + (((x[5] >> 1) & b) << 3*l) + (((x[4] >> 1) & b) << 4*l) + (((x[3] >> 1) & b) << 5*l) + (((x[2] >> 1) & b) << 6*l) + (((x[1] >> 1) & b) << 7*l) + (((x[0] >> 1) & b) << 8*l);
		}
		static int const q_size = 9;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_10_gram {
		static uint64_t get_q_gram(uchar *x) {
			return ((x[9] >> 1) & b) + (((x[8] >> 1) & b) << l) + (((x[7] >> 1) & b) << 2*l) + (((x[6] >> 1) & b) << 3*l) + (((x[5] >> 1) & b) << 4*l) + (((x[4] >> 1) & b) << 5*l) + (((x[3] >> 1) & b) << 6*l) + (((x[2] >> 1) & b) << 7*l) + (((x[1] >> 1) & b) << 8*l) + (((x[0] >> 1) & b) << 9*l);
		}
		static int const q_size = 10;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

};

namespace q_gram_utilities_l_bits {

	static uint64_t const l = Q_GRAM_UTIL_NEW_L;
	static uint64_t const b = ((uint64_t)1<<l)-(uint64_t)1;

	struct get_1_gram {
		static uint64_t get_q_gram(uchar *x) {
			return x[0] & b;
		}
		static int const q_size = 1;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_2_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[1] & b) + ((x[0] & b) << l);
		}
		static int const q_size = 2;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_3_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[2] & b) + ((x[1] & b) << l) + ((x[0] & b) << 2*l);
		}
		static int const q_size = 3;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_4_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[3] & b) + ((x[2] & b) << l) + ((x[1] & b) << 2*l) + ((x[0] & b) << 3*l);
		}
		static int const q_size = 4;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_5_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[4] & b) + ((x[3] & b) << l) + ((x[2] & b) << 2*l) + ((x[1] & b) << 3*l) + ((x[0] & b) << 4*l);
		}
		static int const q_size = 5;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_6_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[5] & b) + ((x[4] & b) << l) + ((x[3] & b) << 2*l) + ((x[2] & b) << 3*l) + ((x[1] & b) << 4*l) + ((x[0] & b) << 5*l);
		}
		static int const q_size = 6;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_7_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[6] & b) + ((x[5] & b) << l) + ((x[4] & b) << 2*l) + ((x[3] & b) << 3*l) + ((x[2] & b) << 4*l) + ((x[1] & b) << 5*l) + ((x[0] & b) << 6*l);
		}
		static int const q_size = 7;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_8_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[7] & b) + ((x[6] & b) << l) + ((x[5] & b) << 2*l) + ((x[4] & b) << 3*l) + ((x[3] & b) << 4*l) + ((x[2] & b) << 5*l) + ((x[1] & b) << 6*l) + ((x[0] & b) << 7*l);
		}
		static int const q_size = 8;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_9_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[8] & b) + ((x[7] & b) << l) + ((x[6] & b) << 2*l) + ((x[5] & b) << 3*l) + ((x[4] & b) << 4*l) + ((x[3] & b) << 5*l) + ((x[2] & b) << 6*l) + ((x[1] & b) << 7*l) + ((x[0] & b) << 8*l);
		}
		static int const q_size = 9;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

	struct get_10_gram {
		static uint64_t get_q_gram(uchar *x) {
			return (x[9] & b) + ((x[8] & b) << l) + ((x[7] & b) << 2*l) + ((x[6] & b) << 3*l) + ((x[5] & b) << 4*l) + ((x[4] & b) << 5*l) + ((x[3] & b) << 6*l) + ((x[2] & b) << 7*l) + ((x[1] & b) << 8*l) + ((x[0] & b) << 9*l);
		}
		static int const q_size = 10;
		static size_t const super_sigma = (uint64_t)1<<(l*q_size);
	};

};

/* Declarations */
template<class T, int U, int K> int mag_dyn();
template<class T> void mag_dyn_build_indexed_patterns();

/* Definitions */

// fill indexed_patterns structure (this is needed for verification)
template<class T> void mag_dyn_build_indexed_patterns() {
	uchar *pattern = 0;
	for ( long l = 0; l < pattern_text_size; l += pattern_size) {
		pattern = patterns_text + l;
		memcpy(patterns2[patterns_number], pattern, pattern_size);
		for(int i =0; i < T::q_size; i++) {
			unsigned long long int index = T::get_q_gram(patterns2[patterns_number] + i);
			indexed_patterns[index].list = (pelem*)realloc(indexed_patterns[index].list, sizeof(pelem) * (indexed_patterns[index].count+1));
			indexed_patterns[index].list[indexed_patterns[index].count].pattern = patterns2[patterns_number] + i ;
			indexed_patterns[index].list[indexed_patterns[index].count].op = patterns2[patterns_number];
			indexed_patterns[index].list[indexed_patterns[index].count].offset = i ;
			indexed_patterns[index].count++;
		}
		++patterns_number;
	}

	for(unsigned int i = 0; i < T::super_sigma; ++i) {
		if(indexed_patterns[i].count > 0) {
			qsort(indexed_patterns[i].list, indexed_patterns[i].count, sizeof(pelem), (int(*)(const void*,const void*)) pelem_compar);
		}
	}
}


// verification
template <class T> static void mag_dyn_verification( word d, word mm, int pos, int * vt ) {
	int s, c;
	d = ( d & mm ) ^ mm;
	++v_count;
	int index1;
	pelem el;
	el.offset = 0;
	pelem *el2;
	pelem* ref = NULL;
	while( d ) {
		s = d>=log2cache?ilog2(d):log2v[d];
		c = vt[ s ] * T::q_size + pos;
		verif++;
		el.pattern = text + c;
		index1 = T::get_q_gram(el.pattern);

		el2 = (pelem*)bsearch(&el, indexed_patterns[index1].list, indexed_patterns[index1].count, sizeof(pelem), (int (*)(const void *, const void *))pelem_compar);
		if(el2) {
			ref = el2;
			while( el2 - indexed_patterns[index1].list >= 0 && memcmp(el2->pattern, text + c, pelem_compar_bytes)==0) {
				if(memcmp(el2->op, text + c - el2->offset, pattern_size)==0) {
					indexes[c - el2->offset] = true;
				}
				el2--;
			}
			el2 = ++ref;
			while( el2 - indexed_patterns[index1].list < indexed_patterns[index1].count && memcmp(el2->pattern, text + c, pelem_compar_bytes)==0) {
				if(memcmp(el2->op, text + c - el2->offset, pattern_size)==0) {
					indexes[c - el2->offset] = true;
				}
				el2++;
			}
		}
		d &= ~(( word )1 << s );
	}
}

/*
Expands to:
	d = ( d << 1 ) | b[ get_Q_gram( (tt + 0  * K * Q), sigma)];
	d = ( d << 1 ) | b[ get_Q_gram( (tt + 1  * K * Q), sigma)];
	d = ( d << 1 ) | b[ get_Q_gram( (tt + 2  * K * Q), sigma)];
	d = ( d << 1 ) | b[ get_Q_gram( (tt + 3  * K * Q), sigma)];
	... U times
*/
template <class T, unsigned int U_i, unsigned int U, unsigned int K>
struct mag_ { 
    static void get_d(word &d, uchar *tt, word *b) {
		d = ( d << 1 ) | b[ T::get_q_gram(tt + (U-U_i)  * K * T::q_size) ];
		mag_<T, U_i-1, U, K>::get_d(d, tt, b);
    }
};

template <class T, unsigned int U, unsigned int K>
struct mag_<T, 0, U, K> {
	static void get_d(word &d, uchar *tt, word *b) {
		return;
	}
};

// main searching function based on faoso
template<class T, int U, int K> int mag_dyn() {
	word  *b = (word*)calloc(T::super_sigma, sizeof(word));
	word  d;
	word  mm;
	int  i, j, h, r, foo = U * K * T::q_size, *vt;
	uchar  * tt;

	if( patt_size > (int)MAX_WORD_SIZE ) { printf("Error: patt_size > MAX_WORD_SIZE (%d > %d)", patt_size, MAX_WORD_SIZE); exit(EXIT_FAILURE); }

	for( i = 0; i < T::super_sigma; i++ ) b[ i ] = ~( word )0; 

	for( j = h = 0, mm = ( word )0; j < K; j++ ) {
		for( i = 0; i < patt_size/K; i++ ) {
			for( int z = 0; z < patterns_number; ++z) {
				for (int offset = 0; offset < T::q_size; offset++) { // Can be replaced by switch
					b[ T::get_q_gram(&patterns2[z][ i * K * T::q_size + j * T::q_size + offset]) ] &= ~(( word )1 << ( h ));
				}
			}
			++h;
		}
		for( r = 0; r < U; r++, h++ )
			mm = mm | (( word )1 << ( h - 1 )); 
		--h;
	}

	if( h > (int)MAX_WORD_SIZE ) { printf("Error: h=%d > %d\n", h, ( int )MAX_WORD_SIZE ); exit(1); }
	for( i = 0; i < T::super_sigma; i++ ) b[ i ] &= ~( mm & ( mm << 1 ));

	vt = build_verify_table( patt_size, K, U, 1 );
	d = ~mm; i = text_size; tt = text;
	do {
		mag_<T, U, U, K>::get_d(d, tt, b);

		if(( d & mm ) != mm ) mag_dyn_verification<T>( d, mm, tt - text - K * T::q_size + foo, vt );
		d &= ~mm;
		tt += foo;
	} while ( ( i -= foo ) > 0 );

	free(b);
	free(vt);

	return EXIT_SUCCESS;
}

#endif
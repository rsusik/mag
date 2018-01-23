#ifndef MAG_H
#define MAG_H

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

/* Declarations */
template<int U, int K, int Q, int SIGMA> int mag();
template<unsigned int Q, unsigned int SIGMA> void mag_build_indexed_patterns();

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

// fill indexed_patterns structure (this is needed for verification)
template<unsigned int Q, unsigned int SIGMA> void mag_build_indexed_patterns() {
	uchar *pattern = 0;
	for ( long l = 0; l < pattern_text_size; l += pattern_size) {
		pattern = patterns_text + l;
		memcpy(patterns2[patterns_number], pattern, pattern_size);
		for(int i =0; i < Q; i++) {
			unsigned long long int index = get_<Q, SIGMA>::gram((patterns2[patterns_number] + i), 1);
			indexed_patterns[index].list = (pelem*)realloc(indexed_patterns[index].list, sizeof(pelem) * (indexed_patterns[index].count+1));
			indexed_patterns[index].list[indexed_patterns[index].count].pattern = patterns2[patterns_number] + i ;
			indexed_patterns[index].list[indexed_patterns[index].count].op = patterns2[patterns_number];
			indexed_patterns[index].list[indexed_patterns[index].count].offset = i ;
			indexed_patterns[index].count++;
		}
		++patterns_number;
	}

	for(int i = 0; i < get_super_<Q, SIGMA>::sigma(); ++i) {
		if(indexed_patterns[i].count > 0) {
			qsort(indexed_patterns[i].list, indexed_patterns[i].count, sizeof(pelem), (int(*)(const void*,const void*)) pelem_compar);
		}
	}
}


// verification
template <unsigned int Q, unsigned int SIGMA> static void mag_verification( word d, word mm, int pos, int * vt ) {
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
		c = vt[ s ] * Q + pos;
		verif++;
		el.pattern = text + c;
		index1 = get_<Q, SIGMA>::gram(el.pattern, 1);

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
template <unsigned int U_i, unsigned int U, unsigned int Q, unsigned int K, unsigned int SIGMA>
struct mag_ { 
    static void get_d(word &d, uchar *tt, word *b) {
		d = ( d << 1 ) | b[ get_<Q, SIGMA>::gram( (tt + (U-U_i)  * K * Q), 1)];
		mag_<U_i-1, U, Q, K, SIGMA>::get_d(d, tt, b);
    }
};

template <unsigned int U, unsigned int Q, unsigned int K, unsigned int SIGMA>
struct mag_<0, U, Q, K, SIGMA> {
	static void get_d(word &d, uchar *tt, word *b) {
		return;
	}
};

// main searching function based on faoso
template<int U, int K, int Q, int SIGMA> int mag() {
	word  *b = (word*)calloc(get_super_<Q, SIGMA>::sigma(), sizeof(word));
	word  d;
	word  mm;
	int  i, j, h, r, foo = U * K * Q, *vt;
	uchar  * tt;

	if( patt_size > (int)MAX_WORD_SIZE ) { printf("Error: patt_size > MAX_WORD_SIZE"); exit(EXIT_FAILURE); }

	for( i = 0; i < get_super_<Q, SIGMA>::sigma(); i++ ) b[ i ] = ~( word )0; 

	for( j = h = 0, mm = ( word )0; j < K; j++ ) {
		for( i = 0; i < patt_size/K; i++ ) {
			for( int z = 0; z < patterns_number; ++z) {
				for (int offset = 0; offset < Q; offset++) { // Can be replaced by switch
					b[ get_<Q, SIGMA>::gram((&patterns2[z][ i * K * Q + j * Q + offset]), 1) ] &= ~(( word )1 << ( h ));
				}
			}
			++h;
		}
		for( r = 0; r < U; r++, h++ )
			mm = mm | (( word )1 << ( h - 1 )); 
		--h;
	}

	if( h > (int)MAX_WORD_SIZE ) { printf("Error: h=%d > %d\n", h, ( int )MAX_WORD_SIZE ); exit(1); }
	for( i = 0; i < get_super_<Q, SIGMA>::sigma(); i++ ) b[ i ] &= ~( mm & ( mm << 1 ));

	vt = build_verify_table( patt_size, K, U, 1 );
	d = ~mm; i = text_size; tt = text;
	do {
		mag_<U, U, Q, K, SIGMA>::get_d(d, tt, b);

		if(( d & mm ) != mm ) mag_verification<Q, SIGMA>( d, mm, tt - text - K * Q + foo, vt );
		d &= ~mm;
		tt += foo;
	} while ( ( i -= foo ) > 0 );

	free(b);
	free(vt);

	return EXIT_SUCCESS;
}

#endif
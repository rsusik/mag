#ifndef SMAG_H
#define SMAG_H

#include "common.h"

typedef struct pelem {
	uchar * pattern; // can be removed
	uchar * op;
	//pelem * same;
	int offset;
} pelem_;

typedef struct {
	pelem * list;
	int count;
} plist;

plist *indexed_patterns = NULL;

encoded_text_type *encoded_text;
encoded_text_type *encoded_pattern_text;
unsigned long long encoded_size = 0;

uchar patterns2[40000][100];
long patterns_number = 0;

// Other variables
int pelem_compar_bytes;
encoded_text_type **encoded_patterns = new encoded_text_type*[400000];
int enp_count = 0, p_count = 0;
uchar *text;
uchar *patterns_text = NULL;
int patt_size_bytes;

// old pelem_compar_2g
int pelem_compar (const pelem* a, const pelem* b) {
	return memcmp(a->pattern, b->pattern, pelem_compar_bytes);
}

/* Declarations */
template <unsigned int U, unsigned int K, unsigned int Q, unsigned int SIGMA> int smag();
template<unsigned int Q, unsigned int SIGMA> void smag_build_indexed_patterns();

/* Definitions */
template<unsigned int Q, unsigned int SIGMA> static void smag_verification( encoded_text_type * t, int m, word d, word mm, int pos, int * vt ) {
	int	s, c_t, c_x;
	encoded_text_type index1;
	d = ( d & mm ) ^ mm;
	++v_count;
	pelem el;
	el.offset = 0;
	pelem *el2;
	pelem* ref = NULL;
	while( d ) {
		s = ilog2(d);
		c_t = vt[ s ] + pos;	// position in encoded text
		c_x = c_t * Q;			// position in plain text
		verif++;
		

		for( int j = 0; j <= Q; j++) {
			int posss = c_x + j;
			index1 = get_<Q, SIGMA>::gram(text + posss, 1);
		
			for( int i = 0; i < indexed_patterns[index1].count; i++ ) {
				if( memcmp(indexed_patterns[index1].list[i].op, text + posss - indexed_patterns[index1].list[i].offset, pattern_size) == 0 ) {
					indexes[posss - indexed_patterns[index1].list[i].offset] = true;
				}
			}
		}
		d &= ~(( word )1 << s );
	}
}

template <class T, unsigned int U_i, unsigned int U, unsigned int Q, unsigned int K>
struct smag_ { 
    static void get_d(word &d, T *tt, word *b) {
		d = ( d << 1 ) | b[ tt [(U-U_i) * K] ];
		smag_<T, U_i-1, U, Q, K>::get_d(d, tt, b);
    }
};

template <class T, unsigned int U, unsigned int Q, unsigned int K>
struct smag_<T, 0, U, Q, K> {
	static void get_d(word &d, T *tt, word *b) {
		return;
	}
};

template <unsigned int U, unsigned int K, unsigned int Q, unsigned int SIGMA> int smag() {
	word  *b = (word*)calloc(get_super_<Q, SIGMA>::sigma(), sizeof(word));
	word  d;
	word  mm;
	int  i, j, h, r, matches = 0, foo = U * K, *vt;
	encoded_text_type  * tt;

	if( patt_size > MAX_WORD_SIZE ) { printf("Error: patt_size > MAX_WORD_SIZE"); exit(EXIT_FAILURE); }

	for( i = 0; i < get_super_<Q, SIGMA>::sigma(); i++ ) b[ i ] = ~( word )0; 

	for( j = h = 0, mm = ( word )0; j < K; j++ ) {
		for( i = 0; i < patt_size/K; i++ ) {
			for( int z = 0; z < enp_count; ++z) {
				b[ encoded_patterns[z][ i * K + j ]] &= ~(( word )1 << ( h ));
			}
			++h;
		}
		for( r = 0; r < 4; r++, h++ )
			mm = mm | (( word )1 << ( h - 1 )); 
		--h;
	}

	if( h > MAX_WORD_SIZE ) { printf("h=%d > %d\n", h, ( int )MAX_WORD_SIZE ); exit(1); }
	for( i = 0; i < get_super_<Q, SIGMA>::sigma(); i++ ) b[ i ] &= ~( mm & ( mm << 1 ));

	vt = build_verify_table( patt_size, K, U, 1 );
	d = ~mm; i = encoded_size-U; tt = encoded_text; 
	
	do {
		smag_<encoded_text_type, U, U, Q, K>::get_d(d, tt, b);
		if(( d & mm ) != mm ) smag_verification<Q, SIGMA>( encoded_text, patt_size, d, mm, tt - encoded_text - K + foo, vt);
		d &= ~mm;
		tt += foo;
	} while ( ( i -= U * K ) > 0 );

	free(b);
	free(vt);

	return EXIT_SUCCESS;
}

template<unsigned int Q, unsigned int SIGMA> void smag_build_indexed_patterns() {
	uchar *pattern = 0;
	encoded_text_type * enp;
	for ( long l = 0; l < pattern_text_size; l += pattern_size) {
		pattern = patterns_text + l;
		//printf("%.*s\n", pattern_size, patterns_text + l);
		memcpy(patterns2[patterns_number], pattern, pattern_size);
		for(int i =0; i < pattern_size-Q+1; i++) {
			long index = get_<Q, SIGMA>::gram((patterns2[patterns_number] + i), 1);
			indexed_patterns[index].list = (pelem*)realloc(indexed_patterns[index].list, sizeof(pelem) * (indexed_patterns[index].count+1));
			indexed_patterns[index].list[indexed_patterns[index].count].pattern = patterns2[patterns_number] + i ;
			indexed_patterns[index].list[indexed_patterns[index].count].op = patterns2[patterns_number];
			indexed_patterns[index].list[indexed_patterns[index].count].offset = i ;
			indexed_patterns[index].count++;

			enp = encode<const uchar, Q, SIGMA>(pattern + i);
			encoded_patterns[enp_count++] = enp;

		}
		++patterns_number;
	}
	for(int i = 0; i < get_super_<Q, SIGMA>::sigma(); ++i) {
		if(indexed_patterns[i].count > 0) {
			qsort(indexed_patterns[i].list, indexed_patterns[i].count, sizeof(pelem), (int(*)(const void*,const void*)) pelem_compar);
		}
	}
}


template<class T, unsigned int Q, unsigned int U, unsigned int K, unsigned int SIGMA> void EncodeText(T * text) {
	encoded_size = text_size / Q + (text_size % Q > 0 ? 1 : 0);
	//encoded_size = text_size * Q;
	encoded_text = (encoded_text_type *)malloc(sizeof( encoded_text_type ) * (encoded_size + U * K));
	unsigned long int temp = 0;
	for (unsigned long int i = 0; i < text_size; i+=Q) {
	//for (unsigned long int i = 0; i < text_size; i++) {
	encoded_text [temp++] = get_<Q, SIGMA>::gram( (text + i), 1);
	}
	while(temp < encoded_size + U * K) {
		encoded_text [temp++] = 0;
	}
}

#endif
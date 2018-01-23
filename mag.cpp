#include "mag.h"
#define SEARCH_FUNCTION_NAME mag
#define BUILD_INDEXED_PATTERNS_FUNCTION mag_build_indexed_patterns
#define MAP_CREATEION_FUNCTION CreateMapByHistogramBinPack
#include "wrappers.h"

/* MAIN */
int main(int argc, char *argv[]) {
	if ( argc < 4 ) return EXIT_FAILURE;
	t_timemes t_data_acc = {0}, t_pre = {0}, t_run = {0};
	t_init(t_data_acc); t_init(t_pre); t_init(t_run);
	t_start(t_pre);

	const char *patterns_filename = argv[1], *pattern_size_pch = argv[2], *text_filename = argv[3];
	const unsigned int U = argc>4 ? atoi(argv[4]) : 4;
	const int k = argc>5 ? atoi(argv[5]) : 2;	/* q */
	const int q_size = argc>6 ? atoi(argv[6]) : 2;
	long sigma = argc>7 ? atoi(argv[7]) : 5;

	pattern_size = atoi(pattern_size_pch); text = NULL; FILE * file = NULL;
	pelem_compar_bytes = pattern_size - q_size;

	int max_m = ( MAX_WORD_SIZE / k + 1 - U ) * k - 2;
	
	int mod = 0, div = 0;
	mod = pattern_size % q_size;
	div = pattern_size / q_size;
	patt_size = div - ( mod < (q_size - 1) ? 1 : 0 );
	if(patt_size <= k) { printf("Error: (m/2 and max_m)>=k, m=%d, max_m=%d, k=%d\n", patt_size, max_m, k); return EXIT_FAILURE; }
	if(max_m < (int)patt_size) { 
		patt_size = max_m;
	}
	t_stop(t_pre);

	/* read text from disk */
	t_start(t_data_acc);
	text_size = read_file_content<uchar>(&text, text_filename);
	if (text == NULL) { printf("Error: text==NULL\n"); return EXIT_FAILURE;}
	t_stop(t_data_acc);
	/* read patterns file */
	t_start(t_data_acc);
	pattern_text_size = read_file_content<uchar>(&patterns_text, patterns_filename);
	t_stop(t_data_acc);

	t_start(t_pre);

	sigma = CreateMap_wrapper<uchar>(patterns_text, pattern_text_size, sigma);
	const int super_sigma = 
		q_size==1 ? sigma : 
		q_size==2 ? sigma * sigma : 
		q_size==3 ? sigma * sigma * sigma : 
		q_size==4 ? sigma * sigma * sigma * sigma : 
		q_size==5 ? sigma * sigma * sigma * sigma * sigma : 
		q_size==6 ? sigma * sigma * sigma * sigma * sigma * sigma : 
		q_size==7 ? sigma * sigma * sigma * sigma * sigma * sigma * sigma : 
		q_size==8 ? sigma * sigma * sigma * sigma * sigma * sigma * sigma * sigma :
		q_size==9 ? sigma * sigma * sigma * sigma * sigma * sigma * sigma * sigma * sigma : 
		q_size==10 ? sigma * sigma * sigma * sigma * sigma * sigma * sigma * sigma * sigma * sigma : 0;

	size_t RAM = super_sigma * sizeof(plist) + 10000*sizeof(pelem) + 10000*pattern_size + text_size;
	if(RAM > 2000000000) { printf("Error: There is not enough RAM memory available ( %lld )\n", RAM); exit(EXIT_FAILURE); }
	if( super_sigma > 100000000 || super_sigma == 0) { printf("Error: Q is too big for choosen sigma size { q=%d, super_sigma=%d }\n", q_size, super_sigma); exit(EXIT_FAILURE); }

	indexed_patterns = (plist*)calloc(super_sigma, sizeof(plist));

	for(int i = 0; i < super_sigma; ++i) { indexed_patterns[i].list = (pelem*)calloc(1, sizeof(pelem)); }

	build_indexed_patterns_wrapper(sigma, q_size);

	for(unsigned int l = 0; l < log2cache; ++l) log2v[l] = ilog2((ldouble)l);
	indexes = (bool*)calloc(text_size, BOOL_SIZE);

	t_stop(t_pre);

	t_start(t_run);
	search_wrapper(sigma, U, k, q_size);
	t_stop(t_run);
	long int matches = 0;
	for(unsigned long l = 0; l < text_size; ++l) if( indexes[l] ) ++matches;
#ifdef PRINT_RESULTS
	for(unsigned long l = 0; l < text_size; ++l) if( indexes[l] ) printf("%lu\t%.*s\n", l, pattern_size, text+l);
	print_result(t_get_seconds(t_data_acc), t_get_seconds(t_pre), t_get_seconds(t_run), v_count, verif, matches, sigma, super_sigma, text_size, patt_size);
#else
	print_result(t_get_seconds(t_data_acc), t_get_seconds(t_pre), t_get_seconds(t_run), v_count, verif, matches, sigma, super_sigma, text_size, patt_size);
#endif
	for(int i = 0; i < super_sigma; ++i) { free(indexed_patterns[i].list); }
	free(indexed_patterns);
	free(patterns_text);
	free(text);
	free(indexes);
	return EXIT_SUCCESS;
}

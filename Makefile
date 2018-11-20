mag_dna_opt_l2: mag_dyn.cpp  mag_dyn.h common.h
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_opt_l2 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o mag_dna_opt_l2 mag_dyn.cpp 

mag_dna_opt_l3: mag_dyn.cpp  mag_dyn.h common.h
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_opt_l3 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o mag_dna_opt_l3 mag_dyn.cpp 

mag_dna_l2: mag_dyn.cpp  mag_dyn.h common.h
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_l_bits -D Q_GRAM_UTIL_NEW_L=2 -O3 -std=c++11 -Wno-aggressive-loop-optimizations -o mag_dna_l2 mag_dyn.cpp 
	
mag_dna_l3: mag_dyn.cpp  mag_dyn.h common.h
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_l_bits -D Q_GRAM_UTIL_NEW_L=3 -O3 -std=c++11 -Wno-aggressive-loop-optimizations -o mag_dna_l3 mag_dyn.cpp 
	
mag_dna_l4: mag_dyn.cpp  mag_dyn.h common.h
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_l_bits -D Q_GRAM_UTIL_NEW_L=4 -O3 -std=c++11 -Wno-aggressive-loop-optimizations -o mag_dna_l4 mag_dyn.cpp 
	
mag_l2: mag_dyn.cpp  mag_dyn.h common.h
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_l_bits -D Q_GRAM_UTIL_NEW_L=2 -O3 -std=c++11 -Wno-aggressive-loop-optimizations -o mag_l2 mag_dyn.cpp 
	
mag_l3: mag_dyn.cpp  mag_dyn.h common.h
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_l_bits -D Q_GRAM_UTIL_NEW_L=3 -O3 -std=c++11 -Wno-aggressive-loop-optimizations -o mag_l3 mag_dyn.cpp 
	
mag_l4: mag_dyn.cpp  mag_dyn.h common.h
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_l_bits -D Q_GRAM_UTIL_NEW_L=4 -O3 -std=c++11 -Wno-aggressive-loop-optimizations -o mag_l4 mag_dyn.cpp
	
mag: mag.cpp mag.h common.h
	g++ -O3 -std=c++11 -Wno-aggressive-loop-optimizations -o mag mag.cpp 
	
all: mag_dna_opt_l2 mag_dna_opt_l3 mag_dna_l2 mag_dna_l3 mag_dna_l4 mag_l2 mag_l3 mag_l4 mag

clean:
	rm -f mag_dna_opt_l2 mag_dna_opt_l3 mag_dna_l2 mag_dna_l3 mag_dna_l4 mag_l2 mag_l3 mag_l4 mag *.exe *.pdb
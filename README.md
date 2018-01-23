# Multi AOSO on *q*-Grams (MAG)

This source code was written for research purpose (http://www.stringology.org/event/2014/p07.html) and has a minimal error checking. The code may be not very readable and comments may not be adequate. There is no warranty, your use of this code is at your own risk.

## Requirements
* C++ compiler compatible 
* Unix based 64-bit OS (compilation works also in Cygwin)

## Compilation
To compile the code run below line commands:
```shell
git clone https://github.com/rsusik/mag mag
cd mag
make all
```

## Running
To execute searching following command needs to be executed:

```shell 
./[alg] [patterns] [m] [set] [u] [k] [q] [sig]
```

where:
* alg      - one of compiled executable (e.g. *mag*)
* patterns - file with patterns (each *m* bytes - no separator)
* set      - text file (e.g. *english.200MB*)
* u        - FAOSO parameter [FG09] (e.g. 4)
* k        - FAOSO parameter [FG09] (e.g. 2)
* q        - *q*-gram size (e.g. 3)
* sig      - alph size (used for mapping, e.g. 5)

**Note:** For more details about the parameters please refer to [SGF2014] (http://www.stringology.org/event/2014/p07.html)

Example:

```shell
./mag ./patterns.r100/patterns.english.10MB.8.bin 8 ./sets/english.10MB 8 1 2 5
```

## Testing

### Scripts
* `generatePatterns.py` - generate patterns (see Testing section)
* `mag_test.py` - tests the algorithms (see Testing section)

To make the testing with multiple parameters easier the `mag_test.py` script can be used. 

**Note:** Please replace *pattern_loc*, *set_loc* and *alg_loc* variables in the file.

To execute test for:
- english.10MB dataset,
- 100 patterns,
- of length 8, 16, 32, 64,
- with u = 8,
- k = [1, 2],
- q = [2, 3, 4, 5, 6, 7, 8]
- and sigma = 5

the following command needs to be run:

```shell
python mag_test.py mag english.10MB 100 8,16,32,64 8 1,2 2,3,4,6,8 5
```

**Note:** If the set of parameters is incorrect (e.g. *q > m*) the errors will produced in output (which can be ignored).

**Generate patterns**

To generate patterns the script `generatePatterns.py` may be used. 

Example:
```shell
$ python generatePatterns.py english.10MB 100 8,16,32,64
Patterns generated to file ./patterns.r100/patterns.english.10MB.8.bin
Patterns generated to file ./patterns.r100/patterns.english.10MB.16.bin
Patterns generated to file ./patterns.r100/patterns.english.10MB.32.bin
Patterns generated to file ./patterns.r100/patterns.english.10MB.64.bin
```

The above command generates 100 patterns of length *m*={8, 16, 32, 64}.

**Note:** Please make sure the folders *patterns.r[r]* (where [r] is the number of patterns) exists (e.g. ./patterns.r100).

## Algorithms based on MAG
* MAGA         - MAG for Approximate pattern matching [Sus2017]
* MAGC         - MAG for Circullar pattern matching
* MAG for ETDC - the same algorithm but runs on ETDC

## References
[FG09] K. Fredriksson, S. Grabowski.
*Average-optimal string matching. Journal of Discrete*,
pages 579–594. Algorithms 7(4), 2009.

[SGF2014] R. Susik, Sz. Grabowski, and K. Fredriksson.
*Multiple pattern matching revisited.*
In Proceedings of the Prague Stringology Conference, pages 59–70, Czech Technical University in Prague, Czech Republic, 2014

[Sus2017] R. Susik. 
*Applying a q-gram based multiple string matching algorithm for approximate matching.*
Informatyka Automatyka Pomiary w Gospodarce i Ochronie Środowiska (IAPGOŚ), 7(3):47–50, 2017

## Authors
* Robert Susik
* Szymon Grabowski
* Kimmo Fredriksson
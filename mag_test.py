import subprocess, sys, os, platform

pattern_loc = './patterns' # each folder should have name with suffix .r1000 where 1000 is number of patterns
set_loc = './sets'
alg_loc = './'

args_num = len(sys.argv) - 1
curr_path = os.getcwd()

arg_offset = 0

# Error
if args_num < 1:
    print >> sys.stderr, "Error not enought parameters. See --help."
    exit()

if sys.argv[1]=='--help':
	print 'Pattern matching tester help.'
	print ''
	print 'Syntax:'
	print 'test [--help] algorithm[.exe] test_set r m u k q-size sigma'
	print ''
	print '--help      Shows the help'
	print 'algorithm   Algorithm(eg. cf,fso)'
	print 'test_set    Test set(eg. english.100MB)'
	print 'r           Pattern number(eg. 1000)'
	print 'm           Pattern length(eg. 8)'
	print 'u           U parameter(eg. 8)'
	print 'k           K parameter(eg. 2)'
	print 'q           q-gram size'
	print 'sig         sigma'
	print ''
	print 'Example:'
	print 'mag_test.py mag english.200MB 100,1000 8,16,32,64 8 1,2 2,3,4,6,8 5'
	print ''
	exit()

# Error
if args_num < 7:
    print >> sys.stderr, "Error not enought parameters. See --help."
    exit()

# algorithm
algs      = sys.argv[1] # algorithms
# arguments:
set_args  = sys.argv[2] # sets
r_args    = sys.argv[3] # number of patterns
m_args    = sys.argv[4] # pattern length
u_args    = sys.argv[5] # U
k_args    = sys.argv[6] # K
q_args    = sys.argv[7] # Q
sig_args  = sys.argv[8] # Sigma

set_args_list  = set_args.split(',')
a_args_list    = algs.split(',')
r_args_list    = r_args.split(',')
m_args_list    = m_args.split(',')
u_args_list    = u_args.split(',')
k_args_list    = k_args.split(',')
q_list         = q_args.split(',')
sig_list       = sig_args.split(',')

print 'Starting calculation for following parameters'
print 'Sets      : ' + set_args
print 'M         : ' + m_args
print 'U         : ' + u_args
print 'K         : ' + k_args
print 'Q-Size    : ' + q_args
print 'Sigma     : ' + sig_args


print 'A\t' + 'S\t' + 'R\t' + 'Q\t' + 'M\t' + 'U\t' + 'K\t' + 'Sig\t' + 'Sig^Q\t' + 'PattSize\t' + 'n\t' + 'ver\t' + 'whloops\t' + 'matches\t' + 'acc\t' + 'pre_t\t' + 'search_t\t' + 'full_t\t' + 'search[MB/s]\t' + 'full[MB/s]'
for a in a_args_list:
	for sig in sig_list:
		for q in q_list:
			for s in set_args_list:
				for r in r_args_list:
					for m in m_args_list:
						for u in u_args_list:
							for k in k_args_list:
								proc_filename    = alg_loc + '/' + a 
								pattern_filename = pattern_loc + '.r' + r + '/patterns.' + s + '.' + m + '.bin'
								set_filename     = set_loc + '/' + s
								if not os.path.isfile(proc_filename):
									print "Error: The prog file not found ( "+proc_filename+")"
									exit()
								if not os.path.isfile(pattern_filename):
									print "Error: The pattern file not found ( "+pattern_filename+")"
									exit()
								if not os.path.isfile(set_filename):
									print "Error: The set file not found ( "+set_filename+")"
									exit()
								#print proc_filename, pattern_filename, m, set_filename, u, k, q, sig
								proc = subprocess.Popen([proc_filename, pattern_filename, m, set_filename, u, k, q, sig], stdout=subprocess.PIPE)
								output = proc.stdout.read()
								output = a + '\t' + s + '\t' + r + '\t' + q + '\t' + m + '\t' + u + '\t' + k + '\t' + output;
								sys.stdout.write(output)
								sys.stdout.flush()

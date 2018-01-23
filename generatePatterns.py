import sys, random

if sys.argv[1]=='--help':
	print 'Generates r patterns of length m for the pattern matching problem'
	print ''
	print 'Syntax:'
	print 'generatePatterns [--help] test_set r m'
	print ''
	print '--help        Shows the help'
	print 'test_set arr  Test set(eg. english.100MB)'
	print 'r arr         Numer of generated patterns'
	print 'm arr         Pattern length(eg. 8)'
	print ''
	print 'Example:'
	print 'generatePatterns english.100MB,dna.200MB 10,100 8,16,32,64,128'
	print ''
	print ''
	exit()

t_args = sys.argv[1]
r_args = sys.argv[2]
m_args = sys.argv[3]		 # pattern length

# int(sys.argv[2])	# no of patterns, must be an even number!
t_args_list	= t_args.split(',')
r_args_list	= r_args.split(',')
m_args_list	= m_args.split(',')

for t_el in t_args_list:
	for r_el in r_args_list:
		for m_el in m_args_list:
			m = int(m_el)
			t = open("./sets/" + t_el, "rb").read()
			filename = "./patterns.r" + str(r_el) + "/patterns." + t_el + "." + str(m) + ".bin"
			outFile = open(filename, "wb")
			for i in xrange(int(r_el)):
				startPos = random.randint(0, len(t) - m)
				patt = t[startPos:startPos + m]
				outFile.write(patt)
			print "Patterns generated to file " + filename
			outFile.close()

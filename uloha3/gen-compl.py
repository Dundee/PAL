#!/usr/bin/env python
#

import os
import sys
import random

def main():
	if len(sys.argv) < 2:
		print 'Usage: %s #node' % sys.argv[0]
		sys.exit(1)

	nodes = int(sys.argv[1])
	print nodes
	for i in xrange(nodes):
		for j in xrange(nodes):
			if i == j: continue
			price = random.randint(1,100)
			print '%d %d %d' % (i, j, price)
	print '0 0 0'
	
	

if __name__ == '__main__': main()

#!/usr/bin/env python
#

import os
import sys
import random

def printMatrix(matrix):
	lines = []
	lines.append(str(len(matrix)))
	i = 0
	for row in matrix:
		for j in xrange(len(row)):
			if row[j] == 0: 
				continue
			lines.append(str(i) + ' ' + str(j))
		i += 1
	buff = "\n".join(lines)
	return buff

def genMatrix(nodes, etch):
	matrix = [[0 for j in xrange(nodes)] for i in xrange(nodes)]
	
	while etch > 0:
		for i in xrange(nodes):
			for j in xrange(nodes):
				if j == i: continue
				if matrix[i][j] == 0 and random.randint(0,1) and etch > 0:
					matrix[i][j] = 1
					#matrix[j][i] = 1
					etch -= 1
	return matrix

def writeMatrix(matrix):
	buff = printMatrix(matrix)
	fp = open('./output', 'w')
	fp.write(buff)
	fp.close()
	
def main():
	if len(sys.argv) < 3:
		print 'Usage: %s #node #etch' % sys.argv[0]
		sys.exit(1)

	nodes = int(sys.argv[1])
	etch = int(sys.argv[2])
	m = genMatrix(nodes, etch)
	writeMatrix(m)

if __name__ == '__main__': main()

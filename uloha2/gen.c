

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char** argv)
{
	int n;
	int e;
	int i;
	int src, dst;
	int ** matrix;
	
	if (argc < 3) {
		printf("Usage: %s #nodes #etches\n", argv[0]);
		/*return 1;*/
		n = 200000;
		e = 260000;
		
	} else {
		sscanf(argv[1], "%d", &n);
		sscanf(argv[2], "%d", &e);
	}
	
	if (e < n -1) {
		printf("#etches must be at least #nodes-1\n");
		return 1;
	}
	
	printf("%d\n", n);
	srand(time(NULL));
	matrix = (int**) malloc(n * sizeof(int *));
	memset(matrix, 0, n);
	for (i = 0; i < n; ++i) {
		matrix[i] = (int*) malloc(n * sizeof(int));
		memset(matrix[i], 0, n);
	}
	
	for (i = 0; i < n; ++i) {
		dst = random() % n;
		printf("%d %d\n", i, dst);
		matrix[i][dst] = 1;
		matrix[dst][i] = 1;
	}
	
	for (i = 0; i < e - n; ++i) {
		src = random() % n;
		while (1) {
			dst = random() % n;
			if (dst == src) continue;
			if (matrix[src][dst]) continue;
			matrix[src][dst] = 1;
			matrix[dst][src] = 1;
			printf("%d %d\n", src, dst);
			break;
		}
	}
	printf("0 0\n");
	
	return 0;
}

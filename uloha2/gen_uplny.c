

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	int n;
	int i, j;

	if (argc < 2) {
		printf("Usage: %s #nodes \n", argv[0]);
		/*return 1;*/
		n = 100;

	} else {
		sscanf(argv[1], "%d", &n);
	}

	printf("%d\n", n);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (i != j) printf("%d %d\n", i, j);
		}
	}

	printf("0 0\n");

	return 0;
}

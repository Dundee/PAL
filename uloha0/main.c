#include <stdio.h>
#include <stdlib.h>

int max(int *list, int size)
{
	int max = 0;
	int i = 0;

	for (i = 0; i < size; i++) {
		if (max < list[i]) max = list[i];
	}
	return max;
}

int bigger(int *list, int size, int border)
{
	int count = 0;
	int i;

	for (i = 0; i < size; i++) if (list[i] > border) count++;
	return count;
}

int main(int argc, char * argv[])
{
	int s1_len[1], s2_len[1], s3_len;
	int *s1, *s2, *cur;
	int i, border;

	scanf("%d ", s1_len);

	if (*s1_len == 0) {
		border = -111111111;
	} else {
		s1 = (int *) calloc(*s1_len, sizeof(int));

		cur = s1;
		for (i=0; i < *s1_len; i++) {
			scanf("%d", cur++);
		}

		border = max(s1, *s1_len);
	}

	scanf("\n");

	scanf("%d ", s2_len);
	s2 = (int *) calloc(*s2_len, sizeof(int));
	cur = s2;
	for (i=0; i < *s2_len; i++) {
		scanf("%d", cur++);
	}

	s3_len = bigger(s2, *s2_len, border);

	printf("%d\n", s3_len);
	for (i = 0; i < *s2_len; i++) {
		if (s2[i] > border) printf("%d\n", s2[i]);
	}

	/*printf("input 1: ");
	for (i=0; i < *s1_len; i++) printf("%d ", s1[i]);
	printf("\ninput 2: ");
	for (i=0; i < *s2_len; i++) printf("%d ", s2[i]);
	printf("\n");*/

	return 0;
}

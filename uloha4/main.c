#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readString(char ** string)
{
	int buff_size = 255;
	int size = buff_size;
	char c;
	int i = 0;
	char * s;
	
	s = *string = (char *) calloc(buff_size, sizeof(char));
	
	while (1) {
		c = getchar();
		while( (c != '\n' && c != '\r')
			   && i < size-1 ) {
			s[i] = c;
			++i;
			c = getchar();
		}
		if (c == '\r') c = getchar();
		if (c == '\n') break;
		size += buff_size;
		s = *string = (char *) realloc(s, size);
		memset(s+i, 0, buff_size);
		s[i] = c;
		++i;
	}
	s[i] = '\0';
}

int main(int argc, char** argv)
{
	char *alphabet, *gen, *dna;
	unsigned int i, j, k, min, gen_size, dna_size;
	unsigned int *prev_row, *row;
	
	readString(&alphabet);
	readString(&gen);
	scanf("%u\n", &k);
	readString(&dna);
	
	gen_size = strlen(gen);
	dna_size = strlen(dna);
	
	prev_row = (unsigned int *) calloc(dna_size+1, sizeof(unsigned int));
	row = (unsigned int *) calloc(dna_size+1, sizeof(unsigned int));
	row[0] = 1;
	
	i = 0;
	while (i < gen_size) {
		for (j=0; j < dna_size; ++j) {
			if (gen[i] == dna[j]) row[j+1] = prev_row[j];
			else { 
				min = row[j] + 1;
				if (prev_row[j+1] + 1 < min) min = prev_row[j+1] + 1;
				
				row[j+1] = min;
			}
		}
		free(prev_row);
		prev_row = row;
		row = (unsigned int *) calloc(dna_size+1, sizeof(unsigned int));
		row[0] = i+2;
		
		++i;
	}
	
	for (i=1; i <= dna_size; ++i) {
		if (prev_row[i] <= k) printf("%d\r\n", i-1);
	}
	
	free(alphabet);
	free(gen);
	free(dna);
	free(row);
	free(prev_row);
	return 0;
}

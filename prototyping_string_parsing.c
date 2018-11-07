#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ARG_IN_BIDS 4
#define SIZE_OF_ARG 4

/* Prototyping how to send bids through to server, 
Haven't considered \0 and \n */

int main()
{
	char bid[4][4];
	const char *my_str_literal = "1,B,10,99";
	char *token, *str, *tofree;

	tofree = str = strdup(my_str_literal);  // We own str's memory now.
	int count = 0;
	while ((token = strsep(&str, ",")))
	{
		//printf("%s count is %d\n", token, count);
		strcpy(bid[count], token);
		count++;
	}
	printf("%s", str);
	printf("%s\n", tofree);
	free(tofree);	

	int i;
	for(i = 0; i < SIZE_OF_ARG; i++)
	{
		printf("%s ", bid[i]);
	}

	return 0;
}
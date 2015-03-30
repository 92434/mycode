/* Compile thuswise:    
 *   gcc -Wall pcre1.c -I/usr/local/include -L/usr/local/lib -R/usr/local/lib -lpcre
 *      
 */     

#include <stdio.h>
#include <string.h>
#include <pcre.h> 

#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EBUFLEN 128            
#define BUFLEN 1024           

int main() 
{               
	pcre            *re; 
	const char      *error;
	int             erroffset;
	int             ovector[OVECCOUNT];
	int             rc, i;

	char            src    [] = "111 <title>Hello World</title> 222";
	char            pattern   [] = "<title>(.*)</title>";

	printf("String : %s\n", src);
	printf("Pattern: \"%s\"\n", pattern);


	re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
	if (re == NULL) {
		printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return 1;
	}

	rc = pcre_exec(re, NULL, src, strlen(src), 0, 0, ovector, OVECCOUNT);
	if (rc < 0) {
		if (rc == PCRE_ERROR_NOMATCH) printf("Sorry, no match ...\n");
		else    printf("Matching error %d\n", rc);
		free(re);
		return 1;
	}

	printf("\nOK, has matched ...\n\n");

	for (i = 0; i < rc; i++) {
		char *substring_start = src + ovector[2*i];
		int substring_length = ovector[2*i+1] - ovector[2*i];
		printf("%2d: %.*s\n", i, substring_length, substring_start);
	}

	free(re);
	return 0;
}

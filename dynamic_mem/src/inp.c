#include<stdio.h>

#include"terminal_interface.h"
#include"controller.h"

void pretty_fgets(char *restrict buf, size_t n, FILE *restrict stream)
{
		fgets(buf, n, stream);

		if(buf[strlen(buf) - 1] != '\n')
			while(getchar() != '\n') {}

		buf[strcspn(buf, "\n")] = '\0';
}



void get_correct_val(char *restrict buf, size_t n, FLIE *restrict stream, buf_validator *is_valid, const char *prompt, const char *failure)
{
	do {
		pretty_fgets(buf, n, stream);

		if(!(*is_valid(buf)))
			printf("%s\n", prompt_failure);
	} while(!(*is_valid)(buf)) 
}

#ifndef _INCLUDE_TUI_H_
#define _INCLUDE_TUI_H_

#include<stdio.h>
#include<stddef.h>
#include<string.h>

typedef int(buf_validator)(char*);

// set of prompt strings used for getting user input
extern const char menu_prompt[];
extern const char menu_failure[];

extern const char name_prompt[];
extern const char name_failure[];

extern const char second_name_prompt[];
extern const char second_name_failure[];

extern const char tel_prompt[];
extern const char tel_failure[];

extern const char confirm_prompt[];
extern const char confirm_failure[];

void pretty_fgets(char *restrict buf, size_t n, FILE *restrict stream);
void get_correct_val(char *restrict buf, size_t n, FILE *restrict stream, buf_validator *is_valid, const char *prompt, const char *failure);


#endif /* ifndef SYMBOL */

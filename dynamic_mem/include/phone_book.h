#ifndef INCLUDE_PHONE_BOOK_H
#define INCLUDE_PHONE_BOOK_H

#include<string.h>
#include<stdio.h>

#include"../include/dl_list.h"
#include"../include/contact.h"
#include"../include/tui.h"

#define MAX_MATCHES 100
#define BUF_SIZE 20
#define LONG_BUF 1024

typedef dl_list phone_book;

void new_book(phone_book *b);
void add_abonent(phone_book *b);
void delete_abonent(phone_book *b);
void search_abonent(phone_book *b);
void print_all_abonents(phone_book *b);
void free_book(phone_book *b);

int is_confirm(const char *buf);
int is_command(const char *buf);

#endif

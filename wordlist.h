#ifndef WORDLIST_H
#define WORDLIST_H

#include "structs.h"

struct word_node *load_wordlist(const char *filename);
void free_wordlist(struct word_node *head);
int count_words(struct word_node *head);

#endif

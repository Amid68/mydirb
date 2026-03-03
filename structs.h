#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define STRING_SIZE     256
#define MAX_FAILS       3
#define TIMEOUT         50
#define AGENT_STRING    "Mozilla/4.0 (compatible: MSIE 6.0: Windows NT 5.1)"

/* A node in our wordlist linked list */
struct word_node {
    char word[STRING_SIZE];
    struct word_node *next;
};

/* Result of one HTTP request */
struct http_result {
    char    url[STRING_SIZE];
    int     http_code;
    int     body_size;
    int     body_words;
    char    location[STRING_SIZE];
};

#endif

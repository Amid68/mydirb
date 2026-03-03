#include "wordlist.h"

struct word_node *load_wordlist(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: cannot open wordlist '%s'\n", filename);
        return NULL;
    }

    struct word_node *head = NULL;
    struct word_node *tail = NULL;
    char line[STRING_SIZE];

    while (fgets(line, sizeof(line), f)) {
        /* strip trailing newline */
        char *end = line + strlen(line) - 1;
        while (end >= line && (*end == '\n' || *end == '\r'))
            *end-- = '\0';

        /* skip empty lines and comments */
        if (strlen(line) == 0 || line[0] == '#')
            continue;

        struct word_node *node = malloc(sizeof(struct word_node));
        memset(node, 0, sizeof(struct word_node));
        strncpy(node->word, line, STRING_SIZE - 1);
        node->next = NULL;

        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    fclose(f);
    return head;
}

void free_wordlist(struct word_node *head) {
    struct word_node *cur = head;
    while (cur) {
        struct word_node *next = cur->next;
        free(cur);
        cur = next;
    }
}

int count_words(struct word_node *head) {
    int count = 0;
    struct word_node *cur = head;
    while (cur) {
        count++;
        cur = cur->next;
    }
    return count;
}


#include "get_url.h"
#include "wordlist.h"

void banner(void) {
    printf("\n");
    printf("-----------------\n");
    printf("MyDIRB v0.1\n");
    printf("-----------------\n");
    printf("\n");
}

void usage(void) {
    printf("Usage: mydirb <url> [wordlist]\n");
}

int main(int argc, char **argv) {
    banner();

    if (argc < 3) {
        usage();
        return -1;
    }

    struct word_node *words = load_wordlist(argv[2]);
    if (!words) return -1;

    printf("Loaded %d words\n", count_words(words));

    /* print first 5 to verify */
    struct word_node *cur = words;
    int i = 0;
    while (cur && i < 5) {
        printf("    [%d]    '%s'\n", i, cur->word);
        cur = cur->next;
        i++;
    }

    free_wordlist(words);
    return 0;
}


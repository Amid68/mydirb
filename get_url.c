#include "get_url.h"

/* libcurl write callbacks store data here between calls */
static struct http_result g_result;
static int g_in_word;

/* Called by libcurl for each chunk of response headers */
static size_t header_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t len = size * nmemb;
    char *data = (char *)ptr;

    /* Capture Location header for redirects */
    if (strncasecmp(data, "Location: ", 10) == 0) {
        strncpy(g_result.location, data + 10, STRING_SIZE - 1);
        /* strip trailing \r\n */
        char *end = g_result.location + strlen(g_result.location) - 1;
        while (end >= g_result.location && (*end == '\r' || *end == '\n'))
            *end-- = '\0';
    }

    return len;
}

/* Called by libcurl for each chunk of response body */
static size_t body_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t len = size * nmemb;
    char *data = (char *)ptr;

    for (size_t i = 0; i < len; i++) {
        g_result.body_size++;
        char c = data[i];

        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (g_in_word) {
                g_in_word = 0;
                g_result.body_words++;
            }
        } else {
            g_in_word = 1;
        }
    }

    return len;
}

struct http_result get_url(const char *url) {
    CURL *curl;
    memset(&g_result, 0, sizeof(g_result));
    strncpy(g_result.url, url, STRING_SIZE - 1);
    g_in_word = 1;

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return g_result;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, AGENT_STRING);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, body_callback);
    /* Don't verify SSL certs by default */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    curl_easy_perform(curl);

    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    g_result.http_code = (int)code;

    curl_easy_cleanup(curl);
    return g_result;
}

/*
 * Ver - Universal Version Control Interface
 * String utility functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "string.h"

char *string_trim(const char *str) {
    if (!str) return NULL;
    
    const char *start = str;
    const char *end = str + strlen(str) - 1;
    
    /* Trim leading whitespace */
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    /* Trim trailing whitespace */
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    
    /* Copy trimmed string */
    size_t len = end - start + 1;
    char *result = malloc(len + 1);
    if (result) {
        memcpy(result, start, len);
        result[len] = '\0';
    }
    
    return result;
}

char *string_join(const char *separator, char **strings, int count) {
    if (!strings || count <= 0) return strdup("");
    
    /* Calculate total length */
    size_t total_len = 0;
    size_t sep_len = separator ? strlen(separator) : 0;
    
    for (int i = 0; i < count; i++) {
        if (strings[i]) {
            total_len += strlen(strings[i]);
            if (i < count - 1) {
                total_len += sep_len;
            }
        }
    }
    
    char *result = malloc(total_len + 1);
    if (!result) return NULL;
    
    result[0] = '\0';
    
    for (int i = 0; i < count; i++) {
        if (strings[i]) {
            strcat(result, strings[i]);
            if (i < count - 1 && separator) {
                strcat(result, separator);
            }
        }
    }
    
    return result;
}

char **string_split(const char *str, const char *delimiter, int *count) {
    if (!str || !delimiter) return NULL;
    
    /* Count delimiters to determine array size */
    int delim_count = 0;
    const char *p = str;
    size_t delim_len = strlen(delimiter);
    
    while ((p = strstr(p, delimiter)) != NULL) {
        delim_count++;
        p += delim_len;
    }
    
    *count = delim_count + 1;
    char **result = calloc(*count + 1, sizeof(char *));
    if (!result) return NULL;
    
    /* Split the string */
    char *str_copy = strdup(str);
    char *token = strtok(str_copy, delimiter);
    
    for (int i = 0; i < *count && token; i++) {
        result[i] = strdup(token);
        token = strtok(NULL, delimiter);
    }
    
    free(str_copy);
    return result;
}

int string_starts_with(const char *str, const char *prefix) {
    if (!str || !prefix) return 0;
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

int string_ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) return 0;
    
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    
    if (suffix_len > str_len) return 0;
    
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

char *string_replace(const char *str, const char *old_sub, const char *new_sub) {
    if (!str || !old_sub) return strdup(str);
    
    /* If new_sub is NULL, treat as empty string */
    if (!new_sub) new_sub = "";
    
    /* Count occurrences of old_sub */
    int count = 0;
    const char *p = str;
    size_t old_len = strlen(old_sub);
    
    while ((p = strstr(p, old_sub)) != NULL) {
        count++;
        p += old_len;
    }
    
    if (count == 0) return strdup(str);
    
    /* Calculate new string length */
    size_t str_len = strlen(str);
    size_t new_len = strlen(new_sub);
    size_t result_len = str_len + (new_len - old_len) * count;
    
    char *result = malloc(result_len + 1);
    if (!result) return NULL;
    
    /* Build result string */
    const char *src = str;
    char *dst = result;
    
    while (*src) {
        if (strncmp(src, old_sub, old_len) == 0) {
            strcpy(dst, new_sub);
            dst += new_len;
            src += old_len;
        } else {
            *dst++ = *src++;
        }
    }
    
    *dst = '\0';
    return result;
}

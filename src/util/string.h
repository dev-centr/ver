#ifndef STRING_H
#define STRING_H

/* String utility functions */

char *string_trim(const char *str);
char *string_join(const char *separator, char **strings, int count);
char **string_split(const char *str, const char *delimiter, int *count);
int string_starts_with(const char *str, const char *prefix);
int string_ends_with(const char *str, const char *suffix);
char *string_replace(const char *str, const char *old_sub, const char *new_sub);

#endif /* STRING_H */

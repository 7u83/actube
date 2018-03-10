#include <string.h>
#include <stdlib.h>

/**
 * @brief Duplicate a string
 * @param s string to duplicate
 * @return duplicated string, the memory acllocated has to be freed by #free.
 */
char *cw_strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p) 
        memcpy(p, s, size);
    return p;
}

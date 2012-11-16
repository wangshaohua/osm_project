/*
 *  a simple implementation of circular buffer
 *
 *  author: Yitao Li 
 *
 */

#ifndef CIRC_BUFF_H
#define CIRC_BUFF_H

#include <string.h>
#include <fstream>

char *circ_str_chr(char *, const size_t, const size_t, const int);
char *circ_strncpy(char *, char *, const size_t, const size_t, const size_t, const size_t);
char *circ_substr(char *, const char *, const size_t, const size_t, const size_t, const size_t);
size_t circ_len(const size_t, const size_t, const size_t, const size_t);
size_t update_buffer(char *, const size_t, const size_t, size_t &, std::ifstream &);
size_t update_buffer(char *, const size_t, const size_t, size_t &, const char *, const size_t, size_t &);

#endif

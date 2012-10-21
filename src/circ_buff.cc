/*
 *  a simple implementation of circular buffer
 *
 *  author: Yitao Li 
 *
 */

#include "circ_buff.h"

//note: for debugging purpose only

#include <stdlib.h>

char *circ_str_chr(char *buffer, const size_t buffer_size, const size_t offset, const int c){   //locate the first occurance of a character in a circular buffer
	char *p, *s, *e = buffer + buffer_size;
	p = s = buffer + offset;
	while (p < e){
		if (*p == -1){
			return NULL;
		}
		if (*p == c){
			return p;
		}
		++p;
	}
	p = buffer;
	while (p < s){
		if (*p == -1){
			return NULL;
		}
		if (*p == c){
			return p;
		}
		++p;
	}
	return NULL; 
}

char *circ_strncpy(char *dest, char *buffer, const size_t buffer_size, const size_t offset, const size_t pos, const size_t n){  //return a substring from circular buffer (assuming begin + n <= buffer_size)
	size_t p = (offset + pos) % buffer_size, r;
	char *s = buffer + p;
	if (p > offset){
		if (p + n < buffer_size){
			strncpy(dest, s, n);
		}else{
			strncpy(dest, s, r = buffer_size - 1 - p);
			strncpy(dest + r, buffer, n - r);
		}
	}else{
		strncpy(dest, s, n);
	}
	dest[n] = '\0';
	return dest;
}

char *circ_substr(char *dest, const char *buffer, const size_t buffer_size, const size_t offset, const size_t firstIndex, const size_t lastIndex){
	size_t n = lastIndex - firstIndex + 1, r;
	const char *s = buffer + firstIndex;
	if (firstIndex >= offset){
		if (lastIndex >= firstIndex){
			strncpy(dest, s, n);
		}else{
			strncpy(dest, s, r = buffer_size - firstIndex);
			strncpy(dest + r, buffer, lastIndex + 1);
			n += buffer_size;    //note: otherwise n would be negative
		}
	}else{
		strncpy(dest, s, n);
	}
	dest[n] = '\0';
	return dest;
}

size_t circ_len(const size_t buffer_size, const size_t offset, const size_t firstIndex, const size_t lastIndex){
	return firstIndex >= offset ? lastIndex >= firstIndex ? lastIndex - firstIndex + 1 : lastIndex - firstIndex + 1 + buffer_size : lastIndex - firstIndex + 1;
}

size_t update_buffer(char *buffer, const size_t buffer_size, const size_t current_pos, size_t & offset, std::ifstream & f){
	if (!f.eof()){
		if (current_pos < offset){
			f.read(buffer + offset, buffer_size - offset);
			f.read(buffer, current_pos + 1);
		}else{
			f.read(buffer + offset, current_pos - offset + 1);
		}
	}else{		
		buffer[offset] = -1;   //EOF
	}
	return offset = (current_pos + 1) % buffer_size;
}

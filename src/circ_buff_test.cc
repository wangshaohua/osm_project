/* 
 * some tests for the circular buffer code
 *
 * author: Yitao Li
 *
 */

#include <stdio.h>
#include "circ_buff.h"

int main(int argc, char *argv[]){
	char buffer[10] = "abcdefghi";
	char result[10];
	circ_strncpy(result, buffer, 10, 0, 1, 3);   //result should be "bcd"
	printf("%s\n", result);
	circ_strncpy(result, buffer, 10, 0, 1, 9);   //result should be "bcdedfghia"
	printf("%s\n", result);
	circ_strncpy(result, buffer, 10, 2, 1, 3);   //result should be "def"
	printf("%s\n", result);
	circ_strncpy(result, buffer, 10, 2, 1, 6);   //result should be "defghi"
	printf("%s\n", result);
	circ_strncpy(result, buffer, 10, 2, 1, 8);   //result should be "defghiab"
	printf("%s\n", result);
	circ_strncpy(result, buffer, 10, 2, 1, 9);   //result should be "defghiabc"
	printf("%s\n", result);
	circ_strncpy(result, buffer, 10, 4, 5, 3);   //result should be "abc"
	printf("%s\n", result);
	printf("%ld\n", circ_str_chr(buffer, 10, 2, 'a') - buffer);  //result should be 0 
	printf("%ld\n", circ_str_chr(buffer, 10, 2, 'c') - buffer);  //result should be 2
	printf("%ld\n", circ_str_chr(buffer, 10, 2, 'f') - buffer);  //result should be 5
	return 0;
}

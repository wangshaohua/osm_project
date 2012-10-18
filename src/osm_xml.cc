/*
 *  code for parsing simple OSM XML file
 *
 *  author: Yitao Li
 *
 */

#include "osm_xml.h"

void read_xml_elem(char *buffer, const size_t buffer_size, char *tag_name, size_t & offset, std::ifstream & f){ 
	char c[2], *s, *e;
	while (!f.eof()){
		while (!(s = circ_str_chr(buffer, buffer_size, offset, '<'))){
			offset = 0;
			f.read(buffer, buffer_size);
		}
		while (s){
			update_buffer(buffer, buffer_size, s - buffer, offset, f);
			//circ_substr(c, buffer, buffer_size, offset, offset, (offset + 1) % buffer_size);	
			circ_strncpy(c, buffer, buffer_size, offset, 0, 1);	
			if (c[0] != '/'){
				e = circ_str_chr(buffer, buffer_size, offset, ' ');
				if (!e || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
					perror("Error: XML tag name contains too many characters\n");
					exit(-1);
				}
				circ_substr(tag_name, buffer, buffer_size, offset, offset, e - buffer);	
				printf("tag_name == %s\n", tag_name);
				update_buffer(buffer, buffer_size, e - buffer, offset, f);
			}
			while (!(s = circ_str_chr(buffer, buffer_size, offset, '>'))){
				offset = 0;
				f.read(buffer, buffer_size);
			}
			update_buffer(buffer, buffer_size, s - buffer, offset, f);
			s = circ_str_chr(buffer, buffer_size, offset, '<');
		}
	}
}

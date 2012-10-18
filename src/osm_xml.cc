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

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Usage: %s <OSM ML data>\n", argv[0]);
		exit(0);
	}
	char buffer[DEFAULT_BUFFER_SIZE + 1];
	char tag_name[MAX_TL + 1];
	size_t offset = 0;
	buffer[DEFAULT_BUFFER_SIZE] = '\0';
	tag_name[MAX_TL] = '\0';
	std::ifstream f(argv[1]);
	if (!f.is_open()){
		perror("Error opening input file\n");
		exit(1);
	}
	f.read(buffer, DEFAULT_BUFFER_SIZE);
	read_xml_elem(buffer, DEFAULT_BUFFER_SIZE, tag_name, offset, f);
	//while (read_xml_elem(buffer, DEFAULT_BUFFER_SIZE, tag_name, offset, f));
	return 0;
}

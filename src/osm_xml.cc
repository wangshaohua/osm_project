/*
 *  code for parsing simple OSM XML file
 *
 *  author: Yitao Li
 *
 */

#include "osm_xml.h"

void read_osm_xml_elem(char *buffer, const size_t buffer_size, char *tag_name, size_t & offset, std::ifstream & f){ 
	char c[2], attr[MAX_TL], *s, *e;
	while (!f.eof()){
		while (!(s = circ_str_chr(buffer, buffer_size, offset, '<'))){
			offset = 0;
			f.read(buffer, buffer_size);
		}
		while (s){
			update_buffer(buffer, buffer_size, s - buffer, offset, f);
			circ_strncpy(c, buffer, buffer_size, offset, 0, 1);	
			if (c[0] != '/'){
				e = circ_str_chr(buffer, buffer_size, offset, ' ');
				if (!e || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
					perror("Error: XML tag name contains too many characters\n");
					exit(-1);
				}

				circ_substr(tag_name, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
				//if (VERBOSE){ printf("reading XML element <%s>\n", tag_name); }
				if (!strcmp(tag_name, "node")){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node id
					printf("ID: %ld\n\n", get_attr_val<long>(attr));
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lat
					printf("LAT: %f\n\n", get_attr_val<double>(attr));
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lon
					printf("LON: %f\n\n", get_attr_val<double>(attr));
				}else if (!strcmp(tag_name, "way")){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //way id
				}
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

void read_osm_xml_attr(char *buffer, char *attr, const size_t buffer_size, char *&s, char *&e, size_t & offset, std::ifstream & f){ 
	update_buffer(buffer, buffer_size, e - buffer, offset, f);
	s = e; 
	e = circ_str_chr(buffer, buffer_size, offset, ' ');
	if (!e || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
		perror("Error: XML tag name contains too many characters\n");
		exit(-1);
	}
	circ_substr(attr, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
	//if (VERBOSE){ printf("reading XML attribute: %s\n", attr); }
}

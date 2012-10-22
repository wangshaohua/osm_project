/*
 *  code for parsing simple OSM XML file
 *
 *  author: Yitao Li
 *
 */

#include "osm_xml.h"

void read_osm_xml_elem(char *buffer, const size_t buffer_size, char *tag_name, size_t &offset, std::ifstream &f, osm_parse_result &res){ 
	char c[2], attr[MAX_TL], *s, *e;
	long n_id, w_id = 0;
	double lon, lat; 
	do{
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
					std::cerr<<"Error: XML tag name contains too many characters\n";
					exit(-1);
				}

				circ_substr(tag_name, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
				if (!strcmp(tag_name, "node")){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node id
					n_id = get_attr_val<long>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lat
					lat = get_attr_val<double>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lon
					lon = get_attr_val<double>(attr);
					res.insert_node_ref(n_id, lat, lon);
				}else if (!strcmp(tag_name, "way")){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //way id
					w_id = get_attr_val<long>(attr);
				}else if (!strcmp(tag_name, "nd")){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //nd ref id
					n_id = get_attr_val<long>(attr);
					res.insert_way_ref(n_id, w_id);
				}else if(w_id && !strcmp(tag_name, "tag")){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
					if (!strncmp(get_attr_substr(attr), W_TYPE, W_TYPE_LEN)){  
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						res.insert_way_type(w_id, get_attr_substr(attr));
					}
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
	}while (!f.eof());
	res.insert_end_pts();
}

void read_osm_xml_attr(char *buffer, char *attr, const size_t buffer_size, char *&s, char *&e, size_t & offset, std::ifstream & f){ 
	char *es = NULL; 
	update_buffer(buffer, buffer_size, e - buffer, offset, f);
	s = e; 
	e = circ_str_chr(buffer, buffer_size, offset, ' ');
	es = circ_str_chr(buffer, buffer_size, offset, '/');
	if (es){
		if (!e || (es - buffer - offset + buffer_size) % buffer_size < (e - buffer - offset + buffer_size) % buffer_size){
			e = es;
		}
	}
	es = circ_str_chr(buffer, buffer_size, offset, '>');
	if (es){
		if (!e || (es - buffer - offset + buffer_size) % buffer_size < (e - buffer - offset + buffer_size) % buffer_size){
			e = es;
		}
	}
	if (!e || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
		perror("Error: XML tag name contains too many characters\n");
		exit(-1);
	}
	circ_substr(attr, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
}

char *get_attr_substr(char *attr){   //note: the source attribute may contain space in it
	char *s = strchr(attr, '"'), *e = strchr(++s, '"');
	if (e){
		*e = '\0';
	}else{
		*s = '\0';
	}
	return s;	
}

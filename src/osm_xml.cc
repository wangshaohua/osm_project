/*
 *  code for parsing simple OSM XML file
 *
 *  author: Yitao Li
 *
 */

#include "osm_xml.h"

void read_osm_xml_elem(char *buffer, const size_t buffer_size, char *tag_name, size_t &offset, std::ifstream &f, osm_parse_result &res){ 
	char c[2], attr[MAX_TL], *s, *e, *attr_n;
	size_t n_id, w_id = 0;
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
				if (!strcmp(tag_name, T_NODE)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node id
					n_id = get_attr_val<size_t>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lat
					lat = get_attr_val<double>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lon
					lon = get_attr_val<double>(attr);
					res.insert_node_ref(n_id, lat, lon);
				}else if (!strcmp(tag_name, T_WAY)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //way id
					w_id = get_attr_val<size_t>(attr);
				}else if (!strcmp(tag_name, T_ND)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //nd ref id
					n_id = get_attr_val<size_t>(attr);
					res.insert_way_ref(n_id, w_id);
				}else if(w_id && !strcmp(tag_name, T_ATTR)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
					attr_n = get_attr_str(attr);
					if (!strcmp(attr_n, T_NAME)){  
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						res.insert_way_name(w_id, get_attr_str(attr));
					}else if (!strcmp(attr_n, T_TYPE)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						res.insert_way_type(w_id, get_attr_str(attr));
					}else if (!strcmp(attr_n, T_ONEWAY)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						if (!strcmp(get_attr_str(attr), T_YES)){
							res.insert_oneway(w_id);
						}
					}else if (!strcmp(attr_n, T_MAXSPEED)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						res.insert_maxspeed(w_id, get_attr_num<double>(attr));
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

char *get_attr_str(char *attr){   //note: the source attribute may contain space in it
	char *s = strchr(attr, '"'), *e = strchr(++s, '"');
	if (e){
		*e = '\0';
	}else{
		*s = '\0';
	}
	return s;	
}

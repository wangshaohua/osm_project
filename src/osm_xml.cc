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
//char *dbg0 = new char[buffer_size];
//char *dbg1 = new char[buffer_size];
	do{
		while (!(s = circ_str_chr(buffer, buffer_size, offset, '<'))){
			offset = 0;
			f.read(buffer, buffer_size);
		}
//printf("line 19: buffer == \n%s%s\noffset == %ld\n\n\n", strncpy(dbg0, buffer + offset, buffer_size - offset), strncpy(dbg1, buffer, offset), offset);
		while (s){
			update_buffer(buffer, buffer_size, s - buffer, offset, f);
			circ_strncpy(c, buffer, buffer_size, offset, 0, 1);	
			if (c[0] != '/'){
//printf("line 23: buffer == \n%s%s\noffset == %ld\n\n\n", strncpy(dbg0, buffer + offset, buffer_size - offset), strncpy(dbg1, buffer, offset), offset);
				e = circ_str_chr(buffer, buffer_size, offset, ' ');
				if (!e || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
					perror("Error: XML tag name contains too many characters\n");
					exit(-1);
				}

				circ_substr(tag_name, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
//printf("tag_name == %s\n", tag_name);
				if (!strcmp(tag_name, "node")){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node id
					n_id = get_attr_val<long>(attr);
//printf("n_id == %ld\n", n_id);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lat
					lat = get_attr_val<double>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lon
					lon = get_attr_val<double>(attr);
					res.insert_node_ref(n_id, lat, lon);
				}else if (!strcmp(tag_name, "way")){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //way id
					w_id = get_attr_val<long>(attr);
//printf("w_id == %ld\n", w_id);
				}else if (!strcmp(tag_name, "nd")){
//printf("line 47: buffer == \n%s%s\noffset == %ld\n\n\n", strncpy(dbg0, buffer + offset, buffer_size - offset), strncpy(dbg1, buffer, offset), offset);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //nd ref id
					n_id = get_attr_val<long>(attr);
printf("nd_ref == %ld\n", n_id);
					res.insert_way_ref(n_id, w_id);
				}
//else{
//printf("unknown\n");
//}
				update_buffer(buffer, buffer_size, e - buffer, offset, f);
			}
//else{
//printf("untag\n");
//}

//printf("line 59: buffer == \n%s%s\noffset == %ld\n\n\n", strncpy(dbg0, buffer + offset, buffer_size - offset), strncpy(dbg1, buffer, offset), offset);
			while (!(s = circ_str_chr(buffer, buffer_size, offset, '>'))){
				offset = 0;
				f.read(buffer, buffer_size);
			}
			update_buffer(buffer, buffer_size, s - buffer, offset, f);
//printf("line 66: buffer == \n%s%s\noffset == %ld\n\n\n", strncpy(dbg0, buffer + offset, buffer_size - offset), strncpy(dbg1, buffer, offset), offset);
			s = circ_str_chr(buffer, buffer_size, offset, '<');
		}
	}while (!f.eof());
}

void read_osm_xml_attr(char *buffer, char *attr, const size_t buffer_size, char *&s, char *&e, size_t & offset, std::ifstream & f){ 
	char *es; 
	update_buffer(buffer, buffer_size, e - buffer, offset, f);
	s = e; 
	e = circ_str_chr(buffer, buffer_size, offset, ' ');
	es = circ_str_chr(buffer, buffer_size, offset, '/');
	if (es){
		if (!e || es < e){
			e = es;
		}
	}
	es = circ_str_chr(buffer, buffer_size, offset, '>');
	if (es){
		if (!e || es < e){
			e = es;
		}
	}
	if (!e || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
		perror("Error: XML tag name contains too many characters\n");
		exit(-1);
	}
	circ_substr(attr, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
}

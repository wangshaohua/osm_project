/*
 *  code for parsing simple OSM XML file
 *
 *  author: Yitao Li
 *
 */

#ifndef OSM_XML_H
#define OSM_XML_H

#define DEFAULT_BUFFER_SIZE 4096   //4k: the default filesystem block size on most *NIX systems
#define MAX_TL 256   //assuming no XML element name contains more than 32 characters 
#define MAX_OUTPUT_LINE_LEN 256
#define T_NODE "node"  //tag name for definition of a node
#define T_ND "nd"    //tag name for reference of a node
#define T_WAY "way"    //tag name for definition of a way
#define T_ATTR "tag"   //tag name for attribute of a node or a way
#define T_TYPE "highway"   //attribute tag name for type of a way 
#define T_NAME "name"   //attribute tag name  for name of a way
#define T_ONEWAY "oneway"
#define T_MAXSPEED "maxspeed"
#define T_YES "yes"

#include <stdlib.h>
#include <sstream>
#include "circ_buff.h"
#include "osm_parse_result.h"

void read_osm_xml_elem(char *, const size_t, char *, size_t &, std::ifstream &, osm_parse_result &);
void read_osm_xml_attr(char *, char *, const size_t, char *&, char *&, size_t &, std::ifstream &);
char *get_attr_str(char *);
char *get_attr_num(char *);

template <typename T> T get_attr_val(char *attr){   //this is assuming all attributes are delimited by double quotes
	char *s = strchr(attr, '"');
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	T val;
	*(strchr(++s, '"')) = '\0';
	ss<<s;
	ss>>val;
	return val;
}

template <typename T> T get_attr_num(char *attr){   //this is assuming all attributes are delimited by double quotes
	char *s = strchr(attr, '"') + 1, *e = s;
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	T val;
	while (*e && *e >= '0' && *e <= '9'){
		++e;
	}
	*e = '\0';
	ss<<s;
	ss>>val;
	return val;
}

#endif

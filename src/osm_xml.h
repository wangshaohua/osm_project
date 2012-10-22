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
#define W_TYPE "highway"   //attribute name for the type of the way 
#define W_TYPE_LEN 8   //attribute name for the type of the way 

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "circ_buff.h"
#include "osm_parse_result.h"

void read_osm_xml_elem(char *, const size_t, char *, size_t &, std::ifstream &, osm_parse_result &);
void read_osm_xml_attr(char *, char *, const size_t, char *&, char *&, size_t &, std::ifstream &);
char *get_attr_substr(char *);

template <typename T> T get_attr_val(char *attr){   //this is assuming all attributes are delimited by double quotes
	char *s = strchr(attr, '"');
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	T val;
	*(strchr(++s, '"')) = '\0';
	ss<<s;
	ss>>val;
	return val;
}

#endif

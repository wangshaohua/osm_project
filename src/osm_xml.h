/*
 *  code for parsing simple OSM XML file
 *
 *  author: Yitao Li
 *
 */

#ifndef OSM_XML_H
#define OSM_XML_H

#define VERBOSE 1
#define DEFAULT_BUFFER_SIZE 4096   //4k: the default filesystem block size on most *NIX systems
#define MAX_TL 256   //assuming no XML element name contains more than 32 characters 

#include <stdio.h>
#include <stdlib.h>
#include "circ_buff.h"

void read_osm_xml_elem(char *, const size_t, char *, size_t &, std::ifstream &);
void read_osm_xml_attr(char *, char *, const size_t, char *&, char *&, size_t &, std::ifstream &);
char *get_attr_val(char *);

#endif

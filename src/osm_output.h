#ifndef OSM_OUTPUT_H
#define OSM_OUTPUT_H

#include <iostream>
#include <fstream>
#include "osm_parse_result.h"

int create_osm_output(const osm_parse_result &, const char *, const char *);

#endif

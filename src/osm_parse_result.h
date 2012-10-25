/* 
 *  code for converting the XML parse result into a directed graph 
 *
 *  author: Yitao Li
 *
 */

#ifndef OSM_PARSE_RESULT_H
#define OSM_PARSE_RESULT_H

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
#define T_UNCLASSIFIED "unclassified"    //type of a way says "unclassified" if the type is not given in the input file 
#define DEFAULT_DELIM ' '   //default delimiter for all other output files
#define DEFAULT_GEOMETRY_DELIM '^'   //default delimiter for the geometry output file
#define INF 1e37   //cost of traversing an one-way edge in the opposite direction 

#include <map>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "circ_buff.h"

class osm_parse_result{
	private:
		std::map< size_t, std::pair<double, double> > n;   //map from node id to node latitude / longtitude values
		std::map< size_t, std::vector<size_t> > w;    //map from way id to list of node ids contained in that way (in correct order) 
		std::map< size_t, std::map<size_t, size_t> > wi;  //map from node id to the map of way id and index of node on that way   
		std::map< size_t, std::map<size_t, double> > wl;    //map from way id to estimated total length of the way
		std::map< size_t, std::vector< std::pair<size_t, size_t> > > wv;  //map from way id to list of vertices contained in that way 
		std::set<size_t> v, o_w;    //the list of vertices in the abstracted version of the map, list of one-way streets in map
		std::set< std::pair< size_t, std::pair<size_t, size_t> > > e;    //the set of edges in the abstracted version of the map
		std::map<size_t, std::string> w_t, w_n;  //map from way id to name of the way (if any) and map from way id to type of the way (if any)
		std::map<size_t, double> w_ms;  //map from way id to max speed
		ssize_t find_immediate_predecessor(const std::vector< std::pair<size_t, size_t> > *, const size_t) const;
		ssize_t find_immediate_successor(const std::vector< std::pair<size_t, size_t> > *, const size_t) const;
		void read_osm_xml_elem(char *, const size_t, char *, size_t &, std::ifstream &);
	public:
		osm_parse_result(const char *);
		~osm_parse_result(void){};
		void insert_node_ref(const size_t, const double, const double);  //inserting into n_map 	
		void insert_way_ref(const size_t, const size_t);    //inserting into w_map
		void insert_end_pts(void);
		void insert_way_name(const size_t, const char *);
		void insert_way_type(const size_t, const char *);
		void insert_maxspeed(const size_t, const double);
		void insert_oneway(const size_t);
		double get_edge_len(const size_t, const size_t, const size_t) const;  
		double get_edge_cost(const size_t, const size_t, const size_t) const;  
		const std::set<size_t>& get_vertex_set(void) const{
			return v;
		}
		const std::set< std::pair< size_t, std::pair<size_t, size_t> > >& get_edge_set(void) const{
			return e;
		}
		int write_node_file(const char *, const char = DEFAULT_DELIM) const; 
		int write_vertex_file(const char *, const char = DEFAULT_DELIM) const; 
		int write_edge_file(const char *, const char = DEFAULT_DELIM) const; 
		int write_edge_geometry_file(const char *, const char = DEFAULT_GEOMETRY_DELIM) const; 
};

char *get_attr_str(char *);
char *get_attr_num(char *);
void read_osm_xml_attr(char *, char *, const size_t, char *&, char *&, size_t &, std::ifstream &);

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

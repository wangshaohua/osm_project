/* 
 *  code for converting the XML parse result into a directed graph 
 *
 *  author: Yitao Li
 *
 */

#ifndef OSM_PARSE_RESULT_H
#define OSM_PARSE_RESULT_H

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
#include <iostream>
#include <math.h>

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
		ssize_t find_immediate_predecessor(const std::vector< std::pair<size_t, size_t> > *, const size_t) const;
		ssize_t find_immediate_successor(const std::vector< std::pair<size_t, size_t> > *, const size_t) const;
	public:
		osm_parse_result(void){};
		~osm_parse_result(void){};
		void insert_node_ref(const size_t, const double, const double);  //inserting into n_map 	
		void insert_way_ref(const size_t, const size_t);    //inserting into w_map
		void insert_end_pts(void);
		void insert_way_name(const size_t, const char *);
		void insert_way_type(const size_t, const char *);
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

#endif

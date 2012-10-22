/* 
 *  code for converting the XML parse result into a directed graph 
 *
 *  author: Yitao Li
 *
 */

#ifndef OSM_PARSE_RESULT_H
#define OSM_PARSE_RESULT_H

#include <map>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <assert.h>
#include <math.h>

class osm_parse_result{
	private:
		std::map< size_t, std::pair<double, double> > n;   //map from node id to node latitude / size_ttitude values
		std::map< size_t, std::vector<size_t> > w;    //map from way id to list of node ids contained in that way (in correct order) 
		std::map< size_t, std::map<size_t, size_t> > wi;  //map from node id to the map of way id and index of node on that way   
		std::map< size_t, std::map<size_t, double> > wl;    //map from way id to estimated total length of the way
		std::map< size_t, std::vector< std::pair<size_t, size_t> > > wv;  //map from way id to list of vertices contained in that way 
		std::set<size_t> v;    //the list of vertices in the abstracted version of the map
		std::set< std::pair< size_t, std::pair<size_t, size_t> > > e;    //the set of edges in the abstracted version of the map
		std::map<size_t, std::string> w_t;
		long find_immediate_predecessor(std::vector< std::pair<size_t, size_t> > *, const size_t);
		long find_immediate_successor(std::vector< std::pair<size_t, size_t> > *, const size_t);
	public:
		osm_parse_result(void){};
		~osm_parse_result(void){};
		void insert_node_ref(const size_t, const double, const double);  //inserting into n_map 	
		void insert_way_ref(const size_t, const size_t);    //inserting into w_map
		void insert_end_pts(void);
		void insert_way_type(const size_t, const char *);
		double get_edge_cost(const size_t, const size_t, const size_t);  
		const std::set<size_t>& get_vertex_set(void) const{
			return v;
		}
		const std::set< std::pair< size_t, std::pair<size_t, size_t> > >& get_edge_set(void) const{
			return e;
		}
};

#endif

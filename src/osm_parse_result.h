#ifndef OSM_PARSE_RESULT_H
#define OSM_PARSE_RESULT_H

#include <map>
#include <vector>
#include <set>
#include <list>
#include <assert.h>
#include <math.h>

class osm_parse_result{
	private:
		std::map< long, std::pair<double, double> > n;   //map from node id to node latitude / longtitude values
		std::map< long, std::vector<long> > w;    //map from way id to list of node ids contained in that way (in correct order) 
		std::map< long, std::map<long, size_t> > wi;  //map from node id to the map of way id and index of node on that way   
		std::map<long, double> wl;    //map from way id to estimated total length of the way
		std::map< long, std::vector< std::pair<size_t, long> > > wv;  //map from way id to list of vertices contained in that way 
		std::set<long> v;    //the list of vertices in the abstracted version of the map
		std::set< std::pair< long, std::pair<long, long> > > e;    //the set of edges in the abstracted version of the map
		long find_immediate_predecessor(std::vector< std::pair<size_t, long> > *, const size_t);
		long find_immediate_successor(std::vector< std::pair<size_t, long> > *, const size_t);
	public:
		osm_parse_result(void){};
		~osm_parse_result(void){};
		void insert_node_ref(const long, const double, const double);  //inserting into n_map 	
		void insert_way_ref(const long, const long);    //inserting into w_map
		void insert_end_pts(void);
		const std::set<long>& get_vertex_set(void){
			return v;
		}
		const std::set< std::pair< long, std::pair<long, long> > >& get_edge_set(void){
			return e;
		}
};

#endif

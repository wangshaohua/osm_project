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
		std::map< long, std::vector<long> > w;    //map from way id to list of node ids included in that way (in correct order) 
		std::map<long, double> wl;    //map from way id to estimated total length of the way
		std::map< long, std::map<long, size_t> > wi;  //map from way id to the map of node id and index of node on that way   
		std::list<long> v;    //the list of vertices in the abstracted version of the map
		std::set< std::pair<long, long> > e;    //the set of edges in the abstracted version of the map
	public:
		osm_parse_result(void){};
		~osm_parse_result(void){};
		void insert_node_ref(const long, const double, const double);  //inserting into n_map 	
		void insert_node_ref(const long, const long);    //inserting into w_map
};

#endif

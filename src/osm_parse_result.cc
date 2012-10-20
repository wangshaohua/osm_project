#include "osm_parse_result.h"

void osm_parse_result::insert_node_ref(const long node_id, const double lat, const double lon){
	n.insert(std::pair< long, std::pair<double, double> >(node_id, std::pair<double, double>(lat, lon)));
}

void osm_parse_result::insert_node_ref(const long node_id, const long way_id){
	long l_id;
	double lat, lon, l_lon, l_lat, dx, dy;
	std::pair<double, double> *nd, *l_nd; 
	std::vector<long> *w_v = &(w[way_id]);
	std::map<long, double>::iterator l_itr;
	if (w_v -> size()){
		l_id = w_v -> back();
		nd = &(n[node_id]);
		l_nd = &(n[l_id]);
		lat = nd -> first;
		lon = nd -> second;
		l_lat = l_nd -> first;
		l_lon = l_nd -> second;
		dy = lat - l_lat;
		dx = lon - l_lon;
		if ((l_itr = wl.find(way_id)) == wl.end()){    //incrementing the estimated length of the current way
			l_itr = wl.insert(std::pair<long, double>(way_id, 0.0)).first;
		}
		l_itr -> second += sqrt(dy * dy + dx * dx); 
	}
	w_v -> push_back(node_id);
}

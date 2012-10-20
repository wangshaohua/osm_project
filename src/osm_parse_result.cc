#include "osm_parse_result.h"

long osm_parse_result::find_immediate_predecessor(const long way_id, const size_t index){   //find vertex in current way that is the immediate predecessor to the node at current index
	size_t min = 0, max, c, c_i;		
	std::vector< std::pair<size_t, long> > *c_wv = &(wv[way_id]);
	if (((*c_wv)[0]).first >= index){
		return -1;              //no immediate predecessor exists
	}
	max = c_wv -> size() - 1;
	while (max != min){ 	
		c = (max + min) / 2;
		if ((c_i = ((*c_wv)[c]).first) > index){
			max = c - 1;
		}else if (c_i < index){
			min = c;
		}
	}
	return min;   //max is -1 if current way contains only 1 vertex
}

long osm_parse_result::find_immediate_successor(const long way_id, const size_t index){ //find vertex in current way that is the immediate successor to the node at current index
	size_t min = 0, max, c, c_i;		
	std::vector< std::pair<size_t, long> > *c_wv = &(wv[way_id]);
	max = c_wv -> size() - 1;
	if (((*c_wv)[max]).first <= index){
		return -1;              //no immediate successor exists
	}
	while (max != min){ 	
		c = (max + min) / 2;
		if ((c_i = ((*c_wv)[c]).first) < index){
			min = c + 1;
		}else if (c_i > index){
			max = c;
		}
	}
	return max;   //max is -1 if current way contains only 1 vertex
}

void osm_parse_result::insert_node_ref(const long node_id, const double lat, const double lon){
	n.insert(std::pair< long, std::pair<double, double> >(node_id, std::pair<double, double>(lat, lon)));
}

void osm_parse_result::insert_way_ref(const long node_id, const long way_id){
	long l_id, _way_id;
	size_t n_ind, _n_ind, p_n_v, s_n_v;
	double lat, lon, l_lon, l_lat, dx, dy;
	std::pair<double, double> *nd, *l_nd; 
	std::vector<long> *w_nv = &(w[way_id]);
	std::map<long, size_t> *w_ni;
	std::map<long, double>::iterator l_itr;
	std::map<long, size_t>::iterator n_itr;
	if ((n_ind = w_nv -> size())){
		l_id = w_nv -> back();
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
	}else{
		v.insert(node_id);    //the starting point of each way must be a vertex
		wv[way_id].push_back(std::pair<size_t, long>(0, node_id));
	}
	w_nv -> push_back(node_id);   //add the node to current way
	w_ni = &(wi[node_id]);
	if (w_ni -> size()){    //current node is contained in another way 
		if (v.find(node_id) == v.end()){   //current node is not a vertex in the graph yet
			n_itr = w_ni -> begin();
			_way_id = n_itr -> first;    //the way id of another way that contains the current node
			_n_ind = n_itr -> second;    //the index of the current node in another way
			if ((p_n_v = find_immediate_predecessor(_way_id, _n_ind)) != -1){
				//create edge
			}
			if ((s_n_v = find_immediate_successor(_way_id, _n_ind)) != -1){
				//create edge
			}
		}else{
			//create 1 or 2 edges
		}
	}
	w_ni -> insert(std::pair<long, size_t>(way_id, n_ind));

}

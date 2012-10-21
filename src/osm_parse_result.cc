#include "osm_parse_result.h"

//FOR DEBUGGING PURPOSE ONLY:

		#include <stdio.h>


long osm_parse_result::find_immediate_predecessor(std::vector< std::pair<size_t, long> > *c_wv, const size_t index){   //find vertex in current way that is the immediate predecessor to the node at current index
	size_t min = 0, max, c, c_i;		
	if (c_wv -> empty() || (*c_wv)[0].first >= index){
		return -1;              //no immediate predecessor exists
	}
	max = c_wv -> size();
	while (max - min > 1){	
		c = (max + min) / 2;
		if ((c_i = (*c_wv)[c].first) >= index){
			max = c;
		}else if (c_i < index){
			min = c;
		}
	}
	return min;   //max is -1 if current way contains only 1 vertex
}

long osm_parse_result::find_immediate_successor(std::vector< std::pair<size_t, long> > *c_wv, const size_t index){ //find vertex in current way that is the immediate successor to the node at current index
	size_t min = 0, max, c, c_i;		
	if (c_wv -> empty()){ 
		return -1;
	}
	if ((*c_wv)[max].first <= index){
		return -1;              //no immediate successor exists
	}
	max = c_wv -> size();
	while (max - min > 1){ 	
		c = (max + min) / 2;
		if ((c_i = (*c_wv)[c].first) <= index){
			min = c;
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
	long l_id, _way_id, p_n_v, s_n_v = -1;
	size_t pn, sn, n_ind, _n_ind, n_v;
	double lat, lon, l_lon, l_lat, dx, dy;
	std::pair<double, double> *nd, *l_nd; 
	std::vector<long> *c_w = &(w[way_id]);
	std::vector< std::pair<size_t, long> > *_c_wv, *c_wv = &(wv[way_id]);
	std::map<long, size_t> *c_wi;
	std::map<long, double>::iterator l_itr;
	std::map<long, size_t>::iterator n_itr;
	if ((n_ind = c_w -> size())){
		l_id = c_w -> back();
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
	c_w -> push_back(node_id);   //add the node to current way
	c_wi = &(wi[node_id]);
	if (c_wi -> size()){    //current node is contained in another way 
		if (v.find(node_id) == v.end()){   //current node is not a vertex in the graph yet
			v.insert(node_id);
			n_itr = c_wi -> begin();
			_way_id = n_itr -> first;    //the way id of another way that contains the current node
			_n_ind = n_itr -> second;    //the index of the current node in another way
			_c_wv = &(wv[_way_id]);
			if ((p_n_v = find_immediate_predecessor(_c_wv, _n_ind)) != -1){
				e.insert(std::pair<long, long>(pn = (*c_wv)[p_n_v].second, node_id));    //create the required edge
				if (c_wv -> size() > (n_v = p_n_v + 1)){
					e.insert(std::pair<long, long>(sn = (*c_wv)[n_v].second, node_id));    //create the required edge
					s_n_v = n_v;
				}
			}else if ((s_n_v = find_immediate_successor(_c_wv, _n_ind)) != -1){
				e.insert(std::pair<long, long>(node_id, sn = (*c_wv)[s_n_v].second));    //create the required edge
			}
			if (p_n_v != -1 && s_n_v != -1){
				e.erase(std::pair<long, long>(pn, sn));
			}
			_c_wv -> insert(_c_wv -> begin() + p_n_v + 1, std::pair<size_t, long>(_n_ind, node_id));   //insert as a vertex in the intersecting way
		}
		c_wv -> push_back(std::pair<size_t, long>(n_ind, node_id));   //otherwise no need to insert current node as vertex in the intersecting way 
		if (n_ind){
			e.insert(std::pair<long, long>((*c_w)[n_ind - 1], node_id));    //create the required edge
		}
	}
	c_wi -> insert(std::pair<long, size_t>(way_id, n_ind));
}

void osm_parse_result::insert_end_pts(void){
	size_t ind;
	long n_id;
	std::map< long, std::vector<long> >::iterator w_iter;
	std::set<long>::iterator v_iter;
	std::vector< std::pair<size_t, long> > *c_wv;
	for (std::map< long, std::vector<long> >::iterator w_iter = w.begin(); w_iter != w.end(); ++w_iter){
//printf("insert_end_pts: way_id == %ld, size == %ld\n", w_iter -> first, w_iter -> second.size());
		if ((ind = w_iter -> second.size() - 1) > 0){
			n_id = w_iter -> second.back();
			if ((v_iter = v.find(n_id)) == v.end()){     //current end point is not a vertex
				v.insert(n_id);
				c_wv = &(wv[w_iter -> first]);
//printf("insert_end_pts: inserting edge (%ld, %ld)\n", c_wv -> back().second, n_id);
				e.insert(std::pair<long, long>(c_wv -> back().second, n_id));    //create the required edge
				c_wv -> push_back(std::pair<size_t, long>(ind, n_id));
			}		
		}
	}
}

/*
 *   code for converting the XML parse result into a directed graph 
 *
 *   author: Yitao Li
 */

#include "osm_parse_result.h"

ssize_t osm_parse_result::find_immediate_predecessor(const std::vector< std::pair<size_t, size_t> > *c_wv, const size_t index) const{   //find vertex in current way that is the immediate predecessor to the node at current index
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

ssize_t osm_parse_result::find_immediate_successor(const std::vector< std::pair<size_t, size_t> > *c_wv, const size_t index) const{ //find vertex in current way that is the immediate successor to the node at current index
	size_t min = 0, max, c, c_i;		
	if (c_wv -> empty()){ 
		return -1;
	}
	if ((*c_wv)[max].first <= index){
		return -1;              //no immediate successor exists
	}
	max = c_wv -> size() - 1;
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

void osm_parse_result::insert_node_ref(const size_t n_id, const double lat, const double lon){
	n.insert(std::pair< size_t, std::pair<double, double> >(n_id, std::pair<double, double>(lat, lon)));
}

void osm_parse_result::insert_way_ref(const size_t n_id, const size_t way_id){
	size_t l_id, _way_id, n_ind, _n_ind, n_v, pn, sn;
	ssize_t p_n_v, s_n_v = -1;
	double lat, lon, l_lon, l_lat, dx, dy;
	std::pair<double, double> *nd, *l_nd; 
	std::vector<size_t> *c_w = &(w[way_id]);
	std::vector< std::pair<size_t, size_t> > *_c_wv, *c_wv = &(wv[way_id]);
	std::map<size_t, size_t> *c_wi;
	std::map<size_t, size_t>::iterator n_itr;
	if ((n_ind = c_w -> size())){
		l_id = c_w -> back();
		nd = &(n[n_id]);
		l_nd = &(n[l_id]);
		lat = nd -> first;
		lon = nd -> second;
		l_lat = l_nd -> first;
		l_lon = l_nd -> second;
		dy = lat - l_lat;
		dx = lon - l_lon;
		if (wl.find(way_id) == wl.end()){    //incrementing the estimated length of the current way
			//l_itr = wl.insert(std::pair< size_t, std::map<size_t, double> >(way_id, 0.0)).first;
			//l_itr = wl[ 
			wl[way_id][n_id] = 0.0;
		}
		wl[way_id][n_id] += sqrt(dy * dy + dx * dx); 
	}
	c_w -> push_back(n_id);   //add the node to current way
	c_wi = &(wi[n_id]);
	if (c_wi -> size()){    //current node is contained in another way 
		if (v.find(n_id) == v.end()){   //current node is not a vertex in the graph yet
			v.insert(n_id);
			n_itr = c_wi -> begin();
			_way_id = n_itr -> first;    //the way id of another way that contains the current node
			_n_ind = n_itr -> second;    //the index of the current node in another way
			_c_wv = &(wv[_way_id]);
			if ((p_n_v = find_immediate_predecessor(_c_wv, _n_ind)) != -1){
				e.insert(std::pair< size_t, std::pair<size_t, size_t> >(_way_id, std::pair<size_t, size_t>(pn = (*_c_wv)[p_n_v].second, n_id)));    //create the required edge
				if (_c_wv -> size() > (n_v = p_n_v + 1)){
					e.insert(std::pair< size_t, std::pair<size_t, size_t> >(_way_id, std::pair<size_t, size_t>(n_id, sn = (*_c_wv)[n_v].second)));    //create the required edge
					s_n_v = n_v;
				}
			}else if ((s_n_v = find_immediate_successor(_c_wv, _n_ind)) != -1){
				e.insert(std::pair< size_t, std::pair<size_t, size_t> >(_way_id, std::pair<size_t, size_t>(n_id, sn = (*_c_wv)[s_n_v].second)));    //create the required edge
			}
			if (p_n_v != -1 && s_n_v != -1){
				e.erase(std::pair< size_t, std::pair<size_t, size_t> >(_way_id, std::pair<size_t, size_t>(pn, sn)));
			}
			_c_wv -> insert(_c_wv -> begin() + p_n_v + 1, std::pair<size_t, size_t>(_n_ind, n_id));   //insert as a vertex in the intersecting way
		}
		if (c_wv -> size()){
			e.insert(std::pair< size_t, std::pair<size_t, size_t> >(way_id, std::pair<size_t, size_t>(c_wv -> back().second, n_id)));    //create the required edge
		}
		c_wv -> push_back(std::pair<size_t, size_t>(n_ind, n_id));   //otherwise no need to insert current node as vertex in the intersecting way 
		/*
		*/
	}else if (!n_ind){
		v.insert(n_id);    //the starting point of each way must be a vertex
		wv[way_id].push_back(std::pair<size_t, size_t>(0, n_id));
	}
	c_wi -> insert(std::pair<size_t, size_t>(way_id, n_ind));
}

void osm_parse_result::insert_end_pts(void){
	size_t ind;
	size_t n_id;
	std::map< size_t, std::vector<size_t> >::iterator w_iter;
	std::vector< std::pair<size_t, size_t> > *c_wv;
	for (std::map< size_t, std::vector<size_t> >::iterator w_iter = w.begin(); w_iter != w.end(); ++w_iter){
		if ((ind = w_iter -> second.size() - 1) > 0){
			if (v.find(n_id = w_iter -> second.back()) == v.end()){
				v.insert(n_id);
				c_wv = &(wv[w_iter -> first]);
				e.insert(std::pair< size_t, std::pair<size_t, size_t> >(w_iter -> first, std::pair<size_t, size_t>(c_wv -> back().second, n_id)));    //create the required edge
				c_wv -> push_back(std::pair<size_t, size_t>(ind, n_id));
			}
		}
	}
}

void osm_parse_result::insert_way_name(const size_t w_id, const char *name){
	w_n[w_id] = std::string(name);
}

void osm_parse_result::insert_way_type(const size_t w_id, const char *type){
	w_t[w_id] = std::string(type);
}

void osm_parse_result::insert_oneway(const size_t w_id){
	o_w.insert(w_id);
}

double osm_parse_result::get_edge_len(const size_t way_id, const size_t p, const size_t q) const{
	const std::map<size_t, double> *c_wl = &(wl.find(way_id) -> second);
	return fabs(c_wl -> find(q) -> second - c_wl -> find(p) -> second);

}

double osm_parse_result::get_edge_cost(const size_t way_id, const size_t p, const size_t q) const{   //for now edge cost is the same as edge length
	return get_edge_len(way_id, p, q);
}

int osm_parse_result::write_node_file(const char *fn, const char delim) const{
	std::ofstream f(fn, std::ios_base::out);
	if (!f.is_open()){
		std::cerr<<"Error opening "<<fn<<" for writing\n";
		return -1;
	}
	for (std::map< size_t, std::pair<double, double> >::const_iterator iter = n.begin(); iter != n.end(); ++iter){
		f<<iter -> first<<delim<<iter -> second.first<<delim<<iter -> second.second<<'\n';
	}
	f.close();
	return 0;
}

int osm_parse_result::write_vertex_file(const char *fn, const char delim) const{
	std::ofstream f(fn, std::ios_base::out);
	std::map< size_t, std::pair<double, double> >::const_iterator c_v; 
	if (!f.is_open()){
		std::cerr<<"Error opening "<<fn<<" for writing\n";
		return -1;
	}
	for (std::set< size_t >::const_iterator iter = v.begin(); iter != v.end(); ++iter){
		c_v = n.find(*iter);
		f<<*iter<<delim<<c_v -> second.first<<delim<<c_v -> second.second<<'\n';
	}
	f.close();
	return 0;
}

int osm_parse_result::write_edge_file(const char *fn, const char delim) const{
	size_t e_id = 0;
	std::ofstream f(fn, std::ios_base::out);
	if (!f.is_open()){
		std::cerr<<"Error opening "<<fn<<" for writing\n";
		return -1;
	}
	for (std::set< std::pair< size_t, std::pair<size_t, size_t> > >::const_iterator iter = e.begin(); iter != e.end(); ++e_id, ++iter){
		f<<e_id<<delim<<iter -> second.first<<delim<<iter -> second.second<<'\n';
	}
	f.close();
	return 0;
}

int osm_parse_result::write_edge_geometry_file(const char *fn, const char delim) const{
	size_t e_id = 0, s_i, e_i;
	std::string c_n, c_t;
	std::ofstream f(fn, std::ios_base::out);
	std::map<size_t, std::string>::const_iterator a_iter;
	std::map< size_t, std::vector<size_t> >::const_iterator w_iter;
	std::map< size_t, std::pair<double, double> >::const_iterator n_iter;
	if (!f.is_open()){
		std::cerr<<"Error opening "<<fn<<" for writing\n";
		return -1;
	}
	for (std::set< std::pair< size_t, std::pair<size_t, size_t> > >::const_iterator iter = e.begin(); iter != e.end(); ++e_id, ++iter){
		c_n = ((a_iter = w_n.find(iter -> first)) == w_n.end()) ? "" : a_iter -> second;
		c_t = ((a_iter = w_t.find(iter -> first)) == w_t.end()) ? T_UNCLASSIFIED : a_iter -> second;
		f<<e_id<<delim<<c_n<<delim<<c_t<<delim<<get_edge_len(iter -> first, iter -> second.first, iter -> second.second); 
		w_iter = w.find(iter -> first);
		s_i = wi.find(iter -> second.first) -> second.find(iter -> first) -> second;
		e_i = wi.find(iter -> second.second) -> second.find(iter -> first) -> second;
		for (size_t i = s_i; i <= e_i; ++i){ 
			n_iter = n.find(w_iter -> second[i]);
			f<<delim<<(n_iter -> second).first<<delim<<(n_iter -> second).second;
		}
		f<<'\n';
	}
	f.close();
	return 0;
}

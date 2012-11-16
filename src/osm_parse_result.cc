/*
 *   code for converting the XML parse result into a directed graph 
 *
 *   author: Yitao Li
 */

#include "osm_parse_result.h"
#include "alt_cost.h"

osm_parse_result::osm_parse_result(const char *fn){
	char buffer[DEFAULT_BUFFER_SIZE + 1];
	char tag_name[MAX_TL + 1];
	size_t offset = 0;
	buffer[DEFAULT_BUFFER_SIZE] = '\0';
	tag_name[MAX_TL] = '\0';
	std::ifstream f(fn, std::ios_base::in);
	if (!f.is_open()){
		std::cerr<<"Error opening "<<fn<<" for reading"<<std::endl;
		exit(1);
	}
	f.read(buffer, DEFAULT_BUFFER_SIZE);
	read_osm_xml_elem(buffer, DEFAULT_BUFFER_SIZE, tag_name, offset, f);
std::cout<<"# of vert(ices): "<<get_vertex_set().size()<<'\n';
std::cout<<"# of edge(s): "<<get_edge_set().size()<<'\n';
/*
std::cout<<"vert(ices):\n";
for (std::set<size_t>::iterator v_iter = get_vertex_set().begin(); v_iter != get_vertex_set().end(); ++v_iter){
std::cout<<*v_iter<<", ";
}
std::cout<<"\nedge(s):\n";
for (std::set< std::pair< size_t, std::pair<size_t, size_t> > >::iterator e_iter = get_edge_set().begin(); e_iter != get_edge_set().end(); ++e_iter){
std::cout<<"< "<<e_iter -> first<<", ("<<e_iter -> second.first<<", "<<e_iter -> second.second<<") >, ";
}
#std::cout<<'\n';
*/
write_node_file("/tmp/WA_Nodes.txt");
write_vertex_file("/tmp/WA_Vertices.txt");
write_edge_file("/tmp/WA_Edges.txt");
write_edge_geometry_file("/tmp/WA_EdgeGeometry.txt");
	f.close();
}

void osm_parse_result::read_osm_xml_elem(char *buffer, const size_t buffer_size, char *tag_name, size_t &offset, std::ifstream &f){ 
	char c[2], attr[MAX_TL], *s, *e, *attr_n;
	size_t n_id, w_id = 0;
	double lon, lat; 
	do{
		while (!(s = circ_str_chr(buffer, buffer_size, offset, '<'))){
			offset = 0;
			f.read(buffer, buffer_size);
		}
		while (s){
			update_buffer(buffer, buffer_size, s - buffer, offset, f);
			circ_strncpy(c, buffer, buffer_size, offset, 0, 1);	
			if (c[0] != '/'){
				if (!(e = circ_str_chr(buffer, buffer_size, offset, ' ')) || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
					std::cerr<<"Error: XML tag name contains too many characters"<<std::endl;
					exit(-1);
				}
				circ_substr(tag_name, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
				if (!strcmp(tag_name, T_NODE)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node id
					n_id = get_attr_val<size_t>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lat
					lat = get_attr_val<double>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //node lon
					lon = get_attr_val<double>(attr);
					insert_node_ref(n_id, lat, lon);
				}else if (!strcmp(tag_name, T_WAY)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //way id
					w_id = get_attr_val<size_t>(attr);
				}else if (!strcmp(tag_name, T_ND)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);  //nd ref id
					n_id = get_attr_val<size_t>(attr);
					insert_way_ref(n_id, w_id);
				}else if(w_id && !strcmp(tag_name, T_ATTR)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
					attr_n = get_attr_str(attr);
					if (!strcmp(attr_n, T_NAME)){  
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						insert_way_name(w_id, get_attr_str(attr));
					}else if (!strcmp(attr_n, T_TYPE)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						insert_way_type(w_id, get_attr_str(attr));
					}else if (!strcmp(attr_n, T_ONEWAY)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						if (!strcmp(get_attr_str(attr), T_YES)){
							insert_oneway(w_id);
						}
					}else if (!strcmp(attr_n, T_MAXSPEED)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f);
						insert_maxspeed(w_id, get_attr_num<double>(attr));
					}
				}
				update_buffer(buffer, buffer_size, e - buffer, offset, f);
			}
			while (!(s = circ_str_chr(buffer, buffer_size, offset, '>'))){
				offset = 0;
				f.read(buffer, buffer_size);
			}
			update_buffer(buffer, buffer_size, s - buffer, offset, f);
			s = circ_str_chr(buffer, buffer_size, offset, '<');
		}
	}while (!f.eof());
	insert_end_pts();
}

/* same as above, except for the input is passed in as a long string (possibly from a php script): this is for testing purpose only */
void osm_parse_result::read_osm_xml_elem(char *buffer, const size_t buffer_size, char *tag_name, size_t &offset, const char *f, const size_t f_size, size_t &fp){ 
	char c[2], attr[MAX_TL], *s, *e, *attr_n;
	size_t n_id, w_id = 0;
	double lon, lat; 
	do{
		while (!(s = circ_str_chr(buffer, buffer_size, offset, '<'))){
			offset = 0;
			strncpy(buffer, f, buffer_size);
			fp += buffer_size;
		}
		while (s){
			update_buffer(buffer, buffer_size, s - buffer, offset, f, f_size, fp);
			circ_strncpy(c, buffer, buffer_size, offset, 0, 1);	
			if (c[0] != '/'){
				if (!(e = circ_str_chr(buffer, buffer_size, offset, ' ')) || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
					std::cerr<<"Error: XML tag name contains too many characters"<<std::endl;
					exit(-1);
				}
				circ_substr(tag_name, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
				if (!strcmp(tag_name, T_NODE)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);  //node id
					n_id = get_attr_val<size_t>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);  //node lat
					lat = get_attr_val<double>(attr);
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);  //node lon
					lon = get_attr_val<double>(attr);
					insert_node_ref(n_id, lat, lon);
				}else if (!strcmp(tag_name, T_WAY)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);  //way id
					w_id = get_attr_val<size_t>(attr);
				}else if (!strcmp(tag_name, T_ND)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);  //nd ref id
					n_id = get_attr_val<size_t>(attr);
					insert_way_ref(n_id, w_id);
				}else if(w_id && !strcmp(tag_name, T_ATTR)){
					read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);
					attr_n = get_attr_str(attr);
					if (!strcmp(attr_n, T_NAME)){  
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);
						insert_way_name(w_id, get_attr_str(attr));
					}else if (!strcmp(attr_n, T_TYPE)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);
						insert_way_type(w_id, get_attr_str(attr));
					}else if (!strcmp(attr_n, T_ONEWAY)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);
						if (!strcmp(get_attr_str(attr), T_YES)){
							insert_oneway(w_id);
						}
					}else if (!strcmp(attr_n, T_MAXSPEED)){ 
						read_osm_xml_attr(buffer, attr, buffer_size, s, e, offset, f, f_size, fp);
						insert_maxspeed(w_id, get_attr_num<double>(attr));
					}
				}
				update_buffer(buffer, buffer_size, e - buffer, offset, f, f_size, fp);
			}
			while (!(s = circ_str_chr(buffer, buffer_size, offset, '>'))){
				offset = 0;
				strncpy(buffer, f, buffer_size);
				fp += buffer_size;
			}
			update_buffer(buffer, buffer_size, s - buffer, offset, f, f_size, fp);
			s = circ_str_chr(buffer, buffer_size, offset, '<');
		}
	}while (fp < f_size);
	insert_end_pts();
}

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

void osm_parse_result::insert_maxspeed(const size_t w_id, const double ms){
	w_ms[w_id] = ms;
}

void osm_parse_result::insert_oneway(const size_t w_id){
	o_w.insert(w_id);
}

double osm_parse_result::get_edge_len(const size_t way_id, const size_t p, const size_t q) const{
	const std::map<size_t, double> *c_wl = &(wl.find(way_id) -> second);
	
	return fabs(c_wl -> find(q) -> second - c_wl -> find(p) -> second);
}

double osm_parse_result::get_edge_cost(const size_t way_id, const size_t p, const size_t q) const{   //for now edge cost is the same as edge length
	return o_w.find(way_id) == o_w.end() ? get_edge_len(way_id, p, q) : wi.find(p) -> second.find(way_id) -> second <= wi.find(q) -> second.find(way_id) -> second ? get_edge_len(way_id, p, q) : INF; 
}

double osm_parse_result::get_edge_speed(const size_t way_id) const {
	std::map<size_t, double>::const_iterator iter = w_ms.find(way_id);	
	//return iter == w_ms.end() ? DEFAULT_SPEED : iter -> second;
	return iter == w_ms.end() ? determine_unknown_speed(way_id) : iter -> second;
}

double osm_parse_result::determine_unknown_speed(const size_t way_id) const {
    std::string way_type = w_t.find(way_id) -> second;

    if (way_type == H_MOTORWAY)
        return MOTORWAY;
    else if (way_type == H_MOTORWAY_LINK)
        return MOTORWAY_LINK;
    else if (way_type == H_TRUNK)
        return TRUNK;
    else if (way_type == H_TRUNK_LINK)
        return TRUNK_LINK;
    else if (way_type == H_PRIMARY)
        return PRIMARY;
    else if (way_type == H_PRIMARY_LINK)
        return PRIMARY_LINK;
    else if (way_type == H_SECONDARY)
        return SECONDARY;
    else if (way_type == H_SECONDARY_LINK)
        return SECONDARY_LINK;
    else if (way_type == H_TERTIARY)
        return TERTIARY;
    else if (way_type == H_TERTIARY_LINK)
        return TERTIARY_LINK;
    else if (way_type == H_LIVING_STREET)
        return LIVING_STREET;
    else if (way_type == H_PEDESTRIAN)
        return PEDESTRIAN;
    else if (way_type == H_RESIDENTIAL)
        return RESIDENTIAL;
    else if (way_type == H_UNCLASSIFIED)
        return UNCLASSIFIED;
    else if (way_type == H_SERVICE)
        return SERVICE;
    else if (way_type == H_TRACK)
        return TRACK;
    else if (way_type == H_BUS_GUIDEWAY)
        return BUS_GUIDEWAY;
    else if (way_type == H_RACEWAY)
        return RACEWAY;
    else if (way_type == H_ROAD)
        return ROAD;
    else
        return DEFAULT_SPEED;
    
}

int osm_parse_result::write_node_file(const char *fn, const char delim) const{
	std::ofstream f(fn, std::ios_base::out);
	if (!f.is_open()){
		std::cerr<<"Error opening "<<fn<<" for writing"<<std::endl;
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
		std::cerr<<"Error opening "<<fn<<" for writing"<<std::endl;
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
	size_t e_id = 0, bp, ep;
	double speed, ec;
	std::ofstream f(fn, std::ios_base::out);
	if (!f.is_open()){
		std::cerr<<"Error opening "<<fn<<" for writing"<<std::endl;
		return -1;
	}
	
	for (std::set< std::pair< size_t, std::pair<size_t, size_t> > >::const_iterator iter = e.begin(); iter != e.end(); ++e_id, ++iter){
		bp = iter -> second.first;
		ep = iter -> second.second;
		speed = get_edge_speed(iter -> first);
		ec = (get_edge_distance_mi(bp, ep) * 3600.0 / get_edge_speed(iter -> first));    //estimated time cost
		f<<e_id<<delim<<bp<<delim<<ep<<delim<<ec<<'\n';
		if (o_w.find(iter -> first) == o_w.end()){
			f<<++e_id<<delim<<ep<<delim<<bp<<delim<<ec<<'\n';
		}
	}
	f.close();
	return 0;
}

int osm_parse_result::write_edge_geometry_file(const char *fn, const char delim) const{
	size_t e_id = 0, s_i, e_i;
	double e_l;
	std::string c_n, c_t;
	std::ofstream f(fn, std::ios_base::out);
	std::map<size_t, std::string>::const_iterator a_iter;
	std::map< size_t, std::vector<size_t> >::const_iterator w_iter;
	std::map< size_t, std::pair<double, double> >::const_iterator n_iter;
	if (!f.is_open()){
		std::cerr<<"Error opening "<<fn<<" for writing"<<std::endl;
		return -1;
	}
	for (std::set< std::pair< size_t, std::pair<size_t, size_t> > >::const_iterator iter = e.begin(); iter != e.end(); ++e_id, ++iter){
		c_n = ((a_iter = w_n.find(iter -> first)) == w_n.end()) ? "" : a_iter -> second;
		c_t = ((a_iter = w_t.find(iter -> first)) == w_t.end()) ? T_UNCLASSIFIED : a_iter -> second;
		e_l = get_edge_len(iter -> first, iter -> second.first, iter -> second.second);
		f<<e_id<<delim<<c_n<<delim<<c_t<<delim<<e_l; 
		w_iter = w.find(iter -> first);
		s_i = wi.find(iter -> second.first) -> second.find(iter -> first) -> second;
		e_i = wi.find(iter -> second.second) -> second.find(iter -> first) -> second;
		for (size_t i = s_i; i <= e_i; ++i){ 
			n_iter = n.find(w_iter -> second[i]);
			f<<delim<<(n_iter -> second).first<<delim<<(n_iter -> second).second;
		}
		f<<'\n';
		if (o_w.find(iter -> first) == o_w.end()){
			f<<++e_id<<delim<<c_n<<delim<<c_t<<delim<<e_l; 
			for (size_t i = e_i; i > s_i; --i){        //this is in case of s_i == 0 
				n_iter = n.find(w_iter -> second[i]);
				f<<delim<<(n_iter -> second).first<<delim<<(n_iter -> second).second;
			}
			n_iter = n.find(w_iter -> second[s_i]);
			f<<delim<<(n_iter -> second).first<<delim<<(n_iter -> second).second;
		}
		f<<'\n';
	}
	f.close();
	return 0;
}

void read_osm_xml_attr(char *buffer, char *attr, const size_t buffer_size, char *&s, char *&e, size_t & offset, std::ifstream & f){ 
	char *es = NULL; 
	update_buffer(buffer, buffer_size, e - buffer, offset, f);
	s = e; 
	e = circ_str_chr(buffer, buffer_size, offset, ' ');
	es = circ_str_chr(buffer, buffer_size, offset, '/');
	if (es){
		if (!e || (es - buffer - offset + buffer_size) % buffer_size < (e - buffer - offset + buffer_size) % buffer_size){
			e = es;
		}
	}
	es = circ_str_chr(buffer, buffer_size, offset, '>');
	if (es){
		if (!e || (es - buffer - offset + buffer_size) % buffer_size < (e - buffer - offset + buffer_size) % buffer_size){
			e = es;
		}
	}
	if (!e || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
		perror("Error: XML tag name contains too many characters\n");
		exit(-1);
	}
	circ_substr(attr, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
}

void read_osm_xml_attr(char *buffer, char *attr, const size_t buffer_size, char *&s, char *&e, size_t & offset, const char *f, const size_t f_size, size_t &fp){  /* same as the above, except for input is a long string, not a file (note: this is for demo / testing purpose only)*/
	char *es = NULL; 
	update_buffer(buffer, buffer_size, e - buffer, offset, f, f_size, fp);
	s = e; 
	e = circ_str_chr(buffer, buffer_size, offset, ' ');
	es = circ_str_chr(buffer, buffer_size, offset, '/');
	if (es){
		if (!e || (es - buffer - offset + buffer_size) % buffer_size < (e - buffer - offset + buffer_size) % buffer_size){
			e = es;
		}
	}
	es = circ_str_chr(buffer, buffer_size, offset, '>');
	if (es){
		if (!e || (es - buffer - offset + buffer_size) % buffer_size < (e - buffer - offset + buffer_size) % buffer_size){
			e = es;
		}
	}
	if (!e || circ_len(buffer_size, offset, offset, e - buffer) > MAX_TL){
		perror("Error: XML tag name contains too many characters\n");
		exit(-1);
	}
	circ_substr(attr, buffer, buffer_size, offset, offset, (e - buffer + buffer_size - 1) % buffer_size);	
}

char *get_attr_str(char *attr){   //note: the source attribute may contain space in it
	char *s = strchr(attr, '"'), *e = strchr(++s, '"');
	if (e){
		*e = '\0';
	}else{
		*s = '\0';
	}
	return s;	
}

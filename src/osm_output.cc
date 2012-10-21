#include "osm_output.h"

int create_osm_output(const osm_parse_result & res, const char *edge_fn, const char *vertex_fn){
	std::ofstream fp_e(edge_fn, std::ios_base::out), fp_v(vertex_fn, std::ios_base::out); 
	if (!fp_e.is_open()){
		std::cerr<<"error opening edge file for writing\n";
		return -1;
	}
	if (!fp_v.is_open()){
		std::cerr<<"error opening vertex file for writing\n";
		return -1;
	}
	fp_e.close();
	fp_v.close();
	return 0;
}

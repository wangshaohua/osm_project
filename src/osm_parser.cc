/* 
 * author: Yitao Li
 * 
 * note: this will probably only work with XML data from http://api.openstreetmap.org/api/0.6 
 *
 * <!-- the current code does not parse XML comment like this --> 
 *  
 */

#include "osm_xml.h"

int main(int argc, char *argv[]){
	if (argc != 2){
		std::cout<<"Usage: "<<argv[0]<<" <OSM ML data>\n";
		exit(0);
	}
	char buffer[DEFAULT_BUFFER_SIZE + 1];
	char tag_name[MAX_TL + 1];
	size_t offset = 0;
	osm_parse_result res;
	buffer[DEFAULT_BUFFER_SIZE] = '\0';
	tag_name[MAX_TL] = '\0';
	std::ifstream f(argv[1], std::ios_base::in);
	if (!f.is_open()){
		std::cerr<<"Error opening input file\n";
		exit(1);
	}
	f.read(buffer, DEFAULT_BUFFER_SIZE);
	read_osm_xml_elem(buffer, DEFAULT_BUFFER_SIZE, tag_name, offset, f, res);
std::cout<<"# of vert(ices): "<<res.get_vertex_set().size()<<"\n";
std::cout<<"# of edge(s): "<<res.get_edge_set().size()<<"\n";
std::cout<<"vert(ices):\n";
for (std::set<size_t>::iterator v_iter = res.get_vertex_set().begin(); v_iter != res.get_vertex_set().end(); ++v_iter){
std::cout<<*v_iter<<", ";
}
std::cout<<"\nedge(s):\n";
for (std::set< std::pair< size_t, std::pair<size_t, size_t> > >::iterator e_iter = res.get_edge_set().begin(); e_iter != res.get_edge_set().end(); ++e_iter){
std::cout<<"< "<<e_iter -> first<<", ("<<e_iter -> second.first<<", "<<e_iter -> second.second<<") >, ";
}
std::cout<<"\n";
	return 0;
}

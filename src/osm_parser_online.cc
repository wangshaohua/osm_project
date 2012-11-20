/* 
 * author: Yitao Li
 * 
 * note: this will probably only work with XML data from http://api.openstreetmap.org/api/0.6 
 *
 * <!-- the current code does not parse XML comment like this --> 
 *  
 */

#include "osm_parse_result.h"

int main(int argc, char *argv[]){
	if (argc > 1){
		osm_parse_result res(argv[1], true);	
		if (argc == 3 && !strcmp(argv[3], "-g")){
			std::cout<<";";
			res.print_edge_geometry_result();
		}
	}
	return 0;
}

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
	if (argc != 2){
		std::cout<<"Usage: "<<argv[0]<<" <OSM ML data>\n";
		exit(0);
	}
	osm_parse_result res(argv[1]);	
	return 0;
}

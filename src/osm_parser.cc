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
		printf("Usage: %s <OSM ML data>\n", argv[0]);
		exit(0);
	}
	char buffer[DEFAULT_BUFFER_SIZE + 1];
	char tag_name[MAX_TL + 1];
	size_t offset = 0;
	buffer[DEFAULT_BUFFER_SIZE] = '\0';
	tag_name[MAX_TL] = '\0';
	std::ifstream f(argv[1]);
	if (!f.is_open()){
		perror("Error opening input file\n");
		exit(1);
	}
	f.read(buffer, DEFAULT_BUFFER_SIZE);
	read_osm_xml_elem(buffer, DEFAULT_BUFFER_SIZE, tag_name, offset, f);
	return 0;
}

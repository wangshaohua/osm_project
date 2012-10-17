/* 
 * author: Yitao Li
 * 
 * note: this code will probably only work with XML data from http://api.openstreetmap.org/api/0.6 
 *  
 */

#include <fstream>
#include <iostream>
#include <string.h>

#define VERBOSE 1
#define BUFFER_SIZE 4096    //assuming block size of the filesystem is 4k



int main(int argc, char *argv[]){
	if (argc != 2){
		std::cout<<"Usage: "<<argv[0]<<" <OSM ML data>\n";
		return 0;
	}
	std::ifstream f(argv[1]);	
	if (!f.is_open()){
		std::cerr<<"Error opening input file\n";
		return -1;
	}
	char buffer[BUFFER_SIZE];
	buffer[BUFFER_SIZE - 1] = '\0'; 
	unsigned int num_node;
	/* read XML version and encoding info */
	f.getline(buffer, BUFFER_SIZE - 1, '>');
	if (VERBOSE){
		buffer[strlen(buffer) - 1] = '\0';    //omit the trailing '?' in output
		std::cout<<"reading XML version info: "<<(buffer + 2)<<"\n\n";
	}
	f.getline(buffer, BUFFER_SIZE - 1, '>');
	if (VERBOSE){
		std::cout<<"reading OSM version info: "<<(buffer + 2)<<"\n\n";
	}
	f.getline(buffer, BUFFER_SIZE - 1, '>');
	if (VERBOSE){
		buffer[strlen(buffer) - 1] = '\0';    //omit the trailing '/' in output 
		std::cout<<"reading bounding box coordinates: "<<(buffer + 2)<<"\n\n";
	}
	for (num_node = 0, f.getline(buffer, BUFFER_SIZE - 1, '>'); !strncmp(buffer + 3, "node", 4); ++num_node, f.getline(buffer, BUFFER_SIZE - 1, '>')){
		if (strncmp(buffer + strlen(buffer) - 1, "/", 2)){   //check if the current tag is closed
			std::cout<<"unclosed tag\n";
			for (f.getline(buffer, BUFFER_SIZE - 1, '>'); strncmp(buffer + 2, "</node", 6); f.getline(buffer, BUFFER_SIZE - 1, '>'));
		}
	}
	if (VERBOSE){ std::cout<<num_node<<" node(s) found\n\n"; }
	while (!f.eof()){
		f.getline(buffer, BUFFER_SIZE - 1, '>');
	}
	f.close();
	return 0;
}

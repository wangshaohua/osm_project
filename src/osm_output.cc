#include "osm_output.h"

osm_output::osm_output(const char *nodes_file, const char *edges_file) {
    if (nodes_file != NULL)
        open_nodes_file(nodes_file);
    else
        open_nodes_file(DEFAULT_NODES_FILE);
    
    if (edges_file != NULL)
        open_edges_file(edges_file);
    else
        open_edges_file(DEFAULT_EDGES_FILE);
}

osm_output::~osm_output() {
    close_nodes_file();
    close_edges_file();
}

bool osm_output::open_nodes_file(const char *file_path) {
    fp_nodes.open(file_path);
    
    if (fp_nodes.is_open())
        return true;
    else
        return false;
}

bool osm_output::open_edges_file(const char *file_path) {
    fp_edges.open(file_path);
    
    if (fp_edges.is_open())
        return true;
    else
        return false;
}

void osm_output::osm_write_node(const long id, const double lat, const double lon) {
    if (fp_nodes.is_open()) {
        fp_nodes << id << " " << lat << " " << lon << "\n";
    } else {
        std::cout << "Error: Nodes output file is not opened!\n";
    }
}

void osm_output::osm_write_edge(const long id, const long start_id, const long end_id, const int cost) {
    if (fp_edges.is_open()) {
        fp_edges << id << " " << start_id << " " << end_id << " " << cost << "\n";
    } else {
        std::cout << "Error: Edges output file is not opened!\n";
    }
}

int osm_output::osm_write(const osm_parse_result & res, const char *edge_fn, const char *vertex_fn){
	
	return 0;
}

void osm_output::close_nodes_file(void) {
    if (fp_nodes.is_open())
        fp_nodes.close();
}

void osm_output::close_edges_file(void) {
    if (fp_edges.is_open())
        fp_edges.close();
}
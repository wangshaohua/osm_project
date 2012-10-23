#ifndef OSM_OUTPUT_H
#define OSM_OUTPUT_H

#include "osm_parse_result.h"

#define DEFAULT_NODES_FILE "../doc/output/WA_Nodes.txt"
#define DEFAULT_EDGES_FILE "../doc/output/WA_Edges.txt"


class osm_output {
    private:
        std::ofstream fp_nodes;
        std::ofstream fp_edges;
        
        bool open_nodes_file(const char *); // open specified nodes output file, if the first parameter is NULL, default path is opened.
        bool open_edges_file(const char *); // open specified edges output file, if the first parameter is NULL, default path is opened.
        
        void close_nodes_file(void);
        void close_edges_file(void);
    public:
        osm_output(const char *, const char *);
        ~osm_output(void);
        int osm_write(const osm_parse_result &, const char *, const char *);
};

#endif

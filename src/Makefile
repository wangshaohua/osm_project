CC=g++
CFLAGS=-g -c -Wall -O3
LDFLAGS=
OSM_PARSER=circ_buff.o osm_parse_result.o osm_parser.o
OSM_PARSER_ONLINE=circ_buff.o osm_parse_result.o osm_parser_online.o
CIRC_BUFF_TEST=circ_buff.o circ_buff_test.o
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLES=osm_parser osm_parser_online circ_buff_test
BIN_DIR=../bin/

.PHONY: clean

.cc.o:
	$(CC) $(CFLAGS) $<

all: $(EXECUTABLES)

osm_parser: $(OSM_PARSER)
	$(CC) $(LDFLAGS) $(OSM_PARSER) -o $(BIN_DIR)$@

osm_parser_online: $(OSM_PARSER_ONLINE)
	$(CC) $(LDFLAGS) $(OSM_PARSER_ONLINE) -o $(BIN_DIR)$@

circ_buff_test: $(CIRC_BUFF_TEST)
	$(CC) $(LDFLAGS) $(CIRC_BUFF_TEST) -o $(BIN_DIR)$@

clean:
	rm -f *.o; cd $(BIN_DIR); rm -f $(EXECUTABLES); cd -

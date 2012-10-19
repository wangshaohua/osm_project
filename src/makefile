G=g++

target: osm_parser

osm_parser: circ_buff.o osm_xml.o read_osm_xml.o
	$(G) -o $@ $^

circ_buff.o: circ_buff.cc
	$(G) -o $@ -c $<

osm_xml.o: osm_xml.cc
	$(G) -o $@ -c $<

read_osm_xml.o: read_osm_xml.cc
	$(G) -o $@ -c $<

clean:
	rm -rf *.o *~ a.out osm_parser
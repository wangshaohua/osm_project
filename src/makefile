G=g++

target: osm_parser

osm_parser: circ_buff.cc osm_xml.cc read_osm_xml.cc
	$(G) -o $@ $^

buff_test: circ_buff.cc test_circ.cc
	$(G) -o $@ $^

clean:
	rm -rf *.o *~ a.out osm_parser buff_test
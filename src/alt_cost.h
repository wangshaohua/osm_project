/* 
 *  code related to computing alternative cost functions 
 *
 *  author: Lim Kwan Kong
 *
 */

#ifndef ALT_COST_H
#define ALT_COST_H

// Reference: http://wiki.openstreetmap.org/wiki/United_States_roads_tagging
// All speed are in mph. These are just the minimum speed for all different roadway, and may or may not be the same as the actual speed limit, but they are close.
#define MOTORWAY 60
#define MOTORWAY_LINK 25
#define TRUNK 50
#define TRUNK_LINK 25
#define PRIMARY 45
#define PRIMARY_LINK 25
#define SECONDARY 45 
#define SECONDARY_LINK 25
#define TERTIARY 35
#define TERTIARY_LINK 25
#define LIVING_STREET 25
#define PEDESTRIAN 0
#define RESIDENTIAL 25
#define UNCLASSIFIED 10
#define SERVICE 10
#define TRACK 10
#define BUS_GUIDEWAY 0
#define RACEWAY 0
#define ROAD 10

#define H_MOTORWAY "motorway"
#define H_MOTORWAY_LINK "motorway_link"
#define H_TRUNK "trunk"
#define H_TRUNK_LINK "trunk_link"
#define H_PRIMARY "primary"
#define H_PRIMARY_LINK "primary_link"
#define H_SECONDARY "secondary"
#define H_SECONDARY_LINK "secondary_link"
#define H_TERTIARY "tertiary"
#define H_TERTIARY_LINK "tertiary_link"
#define H_LIVING_STREET "living_street"
#define H_PEDESTRIAN "pedestrian"
#define H_RESIDENTIAL "residential"
#define H_UNCLASSIFIED "unclassified"
#define H_SERVICE "service"
#define H_TRACK "track"
#define H_BUS_GUIDEWAY "bus_guideway"
#define H_RACEWAY "raceway"
#define H_ROAD "road"

#define PI 3.14159265358979
#define D2R 0.01745329251    //PI / 180, deg-to-rad conversion ratio
#define EARTH_VOLUMETRIC_MEAN_RADIUS 6371.0    //physical constant needed for distance estimations on surface of earth (in SI unit) 
#define K2M 0.621371   //km-to-mile conversion ratio
#define DEFAULT_SPEED 30   //default speed in absence of speed limit info

double osm_parse_result::get_edge_distance_km(const size_t p, const size_t q) const {  //note: this returns the length of the shortest line on surface of the earth between 2 end points, not the length of the road itself, hence the returned value should only be used as a lowerbound for the length of the road 
	std::pair<double, double> start = n.find(p) -> second;
	std::pair<double, double> end = n.find(q) -> second;
	double dLat = (end.first - start.first) * D2R;
	double dLon = (end.second - start.second) * D2R;
	double rLat1 = start.first * D2R;
	double rLat2 = end.first * D2R;
	double a = sin(dLat / 2) * sin(dLat / 2) + cos(rLat1) * cos(rLat2) * sin(dLon / 2) * sin(dLon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return EARTH_VOLUMETRIC_MEAN_RADIUS * c;   //dist = earth_radius * c; // in SI unit (Kilometer)
}

double osm_parse_result::get_edge_distance_mi(const size_t p, const size_t q) const {  //note: this returns the length of the shortest line on surface of the earth between 2 end points, not the length of the road itself, hence the returned value should only be used as a lowerbound for the length of the road 
	return get_edge_distance_km(p, q) * K2M;
}

#endif

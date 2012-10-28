/* 
 *  code related to computing alternative cost functions 
 *
 *  author: Lim Kwan Kong
 *
 */

#ifndef ALT_COST_H
#define ALT_COST_H

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

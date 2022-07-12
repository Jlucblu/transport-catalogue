#pragma once

#include <string_view>
#include <string>
#include <deque>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <unordered_set>

#include "geo.h"


struct BusStop {
	std::string name_;
	Coordinates coordinates_ { 0, 0 };
};

struct BusRoute {
	std::string name_;
	std::vector<BusStop*> stops_;
	bool circle;
};

struct RouteInfo {
	int number_;
	int unique_stops_;
	double distance_;
};

class Hasher {
public:
	size_t operator() (const std::string name) {
		return hasher_(name);
	}
private:
	std::hash<std::string> hasher_;
};

class TransportCatalogue {

public:

	TransportCatalogue() = default;

	BusRoute* FindRoute(std::string_view name);
	BusStop* FindStop(std::string_view name);
	RouteInfo GetInfo(BusRoute* route);
	RouteInfo GetInfo(std::string_view stop_on_route);
	void UpdateRoute(const BusRoute& bus);
	void UpdateStop(const BusStop& stop);

private:
	std::deque<std::string> bus_name_; 
	std::deque<BusRoute> bus_cat_; 
	std::deque<BusStop> stop_cat_; 
	std::unordered_map<std::string_view, BusStop*> stop_name_; 
	std::unordered_map<std::string_view, BusRoute*> route_name_; 
	std::unordered_map<BusStop*, std::vector<BusRoute*>> route_stop_; 
};
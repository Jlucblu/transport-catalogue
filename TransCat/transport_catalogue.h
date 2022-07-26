#pragma once
#include "geo.h"

#include <string_view>
#include <string>
#include <deque>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <utility>


using namespace std::string_literals;
using DistancePair = std::vector<std::pair<std::string, int>>;


struct BusStop {
	std::string name_;
	Coordinates coordinates_ { 0, 0 };
};

struct BusRoute {
	std::string number_;
	std::vector<BusStop*> stops_;
	bool circle;
};

struct RouteInfo {
	int stops_ { 0 };
	int unique_stops_ { 0 };
	double distance_ { 0.0 };
	double curvature_ { 0.0 };
};

class Hasher {
public:
	size_t operator() (const std::pair<BusStop*, BusStop*>& stops) const {
		return hasher_(stops.first) + hasher_(stops.second);
	}
private:
	std::hash<const void*> hasher_;
};

class TransportCatalogue {

public:

	TransportCatalogue() = default;

	BusRoute* FindRoute(std::string_view name);
	BusStop* FindStop(std::string_view name);
	RouteInfo GetStopInfo(BusRoute* route);
	RouteInfo GetStopInfo(std::string_view stop_on_route);
	std::unordered_set<BusRoute*> GetBusInfo(const std::string& stop_name);
	void UpdateRoute(const BusRoute& bus);
	void UpdateStop(const BusStop& stop);
	void UpdateStopDictance(const std::string& from, const DistancePair& to);
	int GetDistance(const std::string& from, const std::string& to);

private:
	std::deque<BusRoute> bus_cat_; 
	std::deque<BusStop> stop_cat_; 
	std::unordered_map<std::string_view, BusStop*> stop_name_; 
	std::unordered_map<std::string_view, BusRoute*> route_name_; 
	std::unordered_map<BusStop*, std::unordered_set<BusRoute*>> route_stop_;
	std::unordered_map<std::pair<BusStop*, BusStop*>, int, Hasher> stop_distance_;
};
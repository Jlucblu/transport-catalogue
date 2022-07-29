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
using DistancePair = std::unordered_map<std::string_view, int>;


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
		return hasher_(stops.first) + (hasher_(stops.second) * 37^3);
	}
private:
	std::hash<const void*> hasher_;
};

class TransportCatalogue {

public:
	TransportCatalogue() = default;

	void UpdateRoute(const BusRoute& bus);
	void UpdateStop(const BusStop& stop);
	BusRoute* FindRoute(std::string_view name) const;
	BusStop* FindStop(std::string_view name) const;
	RouteInfo GetStopInfo(BusRoute* route) const;
	RouteInfo GetStopInfo(std::string_view stop_on_route) const;
	std::unordered_set<BusRoute*> GetBusInfo(const std::string& stop_name) const;
	void UpdateStopDictance(std::string_view from, const DistancePair& to);
	double GetDistance(std::string_view from, std::string_view to) const;

private:
	std::deque<BusRoute> bus_cat_; 
	std::deque<BusStop> stop_cat_; 
	std::unordered_map<std::string_view, BusStop*> stop_name_; 
	std::unordered_map<std::string_view, BusRoute*> route_name_; 
	std::unordered_map<BusStop*, std::unordered_set<BusRoute*>> route_stop_;
	std::unordered_map<std::pair<BusStop*, BusStop*>, double, Hasher> stop_distance_;
};
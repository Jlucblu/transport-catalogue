#pragma once
#include "geo.h"
#include "domain.h"

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

using namespace geo;
using namespace std::string_literals;
using namespace domain;
using DistancePair = std::unordered_map<std::string_view, int>;

namespace transport_catalogue {

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
		void UpdateStopDistance(std::string_view from, const DistancePair& to);
		double GetDistance(std::string_view from, std::string_view to) const;

	private:
		std::deque<BusRoute> bus_cat_;
		std::deque<BusStop> stop_cat_;
		std::unordered_map<std::string_view, BusStop*> stop_name_;
		std::unordered_map<std::string_view, BusRoute*> route_name_;
		std::unordered_map<BusStop*, std::unordered_set<BusRoute*>> route_stop_;
		std::unordered_map<std::pair<BusStop*, BusStop*>, double, Hasher> stop_distance_;
	};
}
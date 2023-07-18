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
using DistancePair = std::unordered_map<std::string_view, double>;


namespace transport_catalogue {

	class TransportCatalogue {
	public:
		TransportCatalogue() = default;

		void UpdateRoute(const BusRoute& bus);
		void UpdateStop(const BusStop& stop);
		void UpdateStopDistance(std::string_view from, const DistancePair& to);
		BusRoute* FindRoute(std::string_view name) const;
		BusStop* FindStop(std::string_view name) const;
		std::vector<BusStop*> MakeUniqueStops(const std::vector<BusStop*> route) const;

		RouteStats GetBusInfo(BusRoute* route) const;
		RouteStats GetBusInfo(std::string_view stop_on_route) const;
		std::unordered_set<BusRoute*> GetStopInfo(std::string_view stop_name) const;
		double GetDistance(std::string_view from, std::string_view to) const;
		std::vector<BusStop*> GetAllUniqueStops() const;
		std::vector<BusRoute*> GetAllRoutes() const;
		const std::deque<BusStop>& GetStopList() const;
		const std::deque<BusRoute>& GetBusList() const;

	private:
		std::deque<BusRoute> buses_;
		std::deque<BusStop> stops_;
		std::unordered_map<std::string_view, BusStop*> stop_info_map_;
		std::unordered_map<std::string_view, BusRoute*> route_info_map_;
		std::unordered_map<BusStop*, std::unordered_set<BusRoute*>> route_stop_map_;
		std::unordered_map<std::pair<BusStop*, BusStop*>, double, Hasher> stop_distance_map_;
	};

} // namespace transport_catalogue
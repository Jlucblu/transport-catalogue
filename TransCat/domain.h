#pragma once
#include <string>
#include <vector>
#include "geo.h"

using namespace geo;

namespace domain {

	struct BusStop {
		std::string name_;
		Coordinates coordinates_{ 0, 0 };
	};

	struct BusRoute {
		std::string number_;
		std::vector<BusStop*> stops_;
		bool circle;
	};

	struct RouteInfo {
		int stops_{ 0 };
		int unique_stops_{ 0 };
		double distance_{ 0.0 };
		double curvature_{ 0.0 };
	};

	class Hasher {
	public:
		size_t operator() (const std::pair<BusStop*, BusStop*>& stops) const {
			return hasher_(stops.first) + (hasher_(stops.second) * 37 ^ 3);
		}
	private:
		std::hash<const void*> hasher_;
	};

} // namespace domain
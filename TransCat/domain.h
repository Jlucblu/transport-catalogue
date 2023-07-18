#pragma once

#include "geo.h"

#include <string>
#include <vector>


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

	struct RouteStats {
		int stops_{ 0 };
		int unique_stops_{ 0 };
		double distance_{ 0.0 };
		double curvature_{ 0.0 };
	};

	struct RoutingSettings {
		double bus_velocity_{ 0.0 };
		double bus_wait_time_{ 0.0 };
	};

	struct RouteItem {
		std::string_view bus_name_;
		double time_;
		int span_count_;

		bool operator==(const RouteItem& other) const {
			return bus_name_ == other.bus_name_ && time_ == other.time_ && span_count_ == other.span_count_;
		}

		bool operator!=(const RouteItem& other) const {
			return !(*this == other);
		}

		bool operator<(const RouteItem& other) const {
			if (time_ == other.time_) {
				return span_count_ < other.span_count_;
			}
			return time_ < other.time_;
		}

		bool operator>(const RouteItem& other) const {
			return other < *this;
		}

		RouteItem& operator+=(const RouteItem& other) {
			time_ += other.time_;
			 span_count_ += other.span_count_;
			return *this;
		}

		RouteItem operator+(const RouteItem& other) const {
			RouteItem result = *this;
			result += other;
			return result;
		}
	};

	struct RouteResponse {
		double total_time_{ 0.0 };
		std::vector<RouteItem> items_;
		std::vector<std::pair<std::string_view, double>> stop_;
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
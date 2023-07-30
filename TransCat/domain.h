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

	struct RouteSettings {
		double bus_velocity_{ 0.0 };
		double bus_wait_time_{ 0.0 };
	};

	struct RouteItem {
		std::string_view bus_name_;
		std::string_view from_stop_;
		std::string_view to_stop_;
		double route_time_;
		double wait_time_;
		int span_count_;

		bool operator==(const RouteItem& other) const {
			return bus_name_ == other.bus_name_ && from_stop_ == other.from_stop_ && to_stop_ == other.to_stop_
				&& route_time_ == other.route_time_ && span_count_ == other.span_count_ && wait_time_ == other.wait_time_;
		}

		bool operator!=(const RouteItem& other) const {
			return !(*this == other);
		}

		bool operator<(const RouteItem& other) const {
			if (route_time_ == other.route_time_) {
				return span_count_ < other.span_count_;
			}
			return route_time_ < other.route_time_;
		}

		bool operator>(const RouteItem& other) const {
			return other < *this;
		}

		RouteItem& operator+=(const RouteItem& other) {
			route_time_ += other.route_time_;
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
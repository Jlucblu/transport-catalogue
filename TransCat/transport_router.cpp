#include "transport_router.h"


namespace transport_router {

	void TransportRouter::SetSettings(const domain::RoutingSettings settings) {
		settings_ = settings;
	}

	RoutingSettings TransportRouter::GetSettings() {
		return settings_;
	}

	std::string_view TransportRouter::FindStopByIndex(VertexId index) {
		for (auto const& stop : stop_id_) {
			if (stop.second == index) {
				return stop.first;
			}
		}

		return {};
	}

	void TransportRouter::BuildGraph() {
		const auto& stops = tc_.GetAllUniqueStops();
		const auto& buses = tc_.GetAllRoutes();
		graph_ = DirectedWeightedGraph<RouteItem>(stops.size());

		for (const auto& stop : stops) {
			if (stop_id_.count(stop->name_) == 0) {
				stop_id_[stop->name_] = stop_id_.size();
			}
		}

		for (const auto& bus : buses) {
			if (!bus->circle) {
				auto middle_it = std::next(bus->stops_.begin(), bus->stops_.size() / 2);
				MakeEdges(bus->stops_.begin(), middle_it + 1, bus->number_);
				MakeEdges(middle_it, bus->stops_.end(), bus->number_);
			}
			else {
				MakeEdges(bus->stops_.begin(), bus->stops_.end(), bus->number_);
			}
		}

		router_.emplace(graph_);
	}

	std::optional<RouteResponse> TransportRouter::MakeRouteResponse(const std::string_view from, const std::string_view to) {
		RouteResponse response;
		const auto& valid_stop_1 = tc_.GetStopInfo(from);
		const auto& valid_stop_2 = tc_.GetStopInfo(to);

		if (valid_stop_1.empty() || valid_stop_2.empty()) {
			return std::nullopt;
		}

		auto const& way = router_->BuildRoute(stop_id_.at(from), stop_id_.at(to));
		if (!way.has_value()) {
			return std::nullopt;
		}

		response.total_time_ = way->weight.route_time_;

		for (auto it = way->edges.begin(); it != way->edges.end(); it++) {
			const auto& edge = graph_.GetEdge(*it);
			response.items_.push_back(edge.weight);
		}

		return response;
	}

} // namespace transport_router
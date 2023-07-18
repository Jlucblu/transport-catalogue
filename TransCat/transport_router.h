#pragma once

#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "domain.h"
#include "graph.h"
#include "router.h"
#include "transport_catalogue.h"


using namespace domain;
using namespace graph;
namespace tc = transport_catalogue;

const double METERS_PER_MINUTE = 1000 / 60.0;


namespace transport_router {

	class TransportRouter {
	public:
		TransportRouter(tc::TransportCatalogue& tc)
			: tc_(tc)
		{}

		void SetSettings(const domain::RoutingSettings settings) {
			settings_ = settings;
		}

		RoutingSettings GetSettings() {
			return settings_;
		}

		std::string_view FindStopByIndex(VertexId index) {
			for (auto const& stop : stop_id_) {
				if (stop.second == index) {
					return stop.first;
				}
			}

			return {};
		}

		template <typename Iter>
		void MakeEdges(Iter begin, Iter end, const BusRoute* route);

		void BuildGraph() {
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
					MakeEdges(bus->stops_.begin(), middle_it + 1, bus);
					MakeEdges(middle_it, bus->stops_.end(), bus);
				}
				else {
					MakeEdges(bus->stops_.begin(), bus->stops_.end(), bus);
				}
			}

			router_.emplace(graph_);
		}

		std::optional<RouteResponse> MakeRouteResponse(const std::string_view from, const std::string_view to) {
			RouteResponse response;
			auto const& way = router_->BuildRoute(stop_id_.at(from), stop_id_.at(to));
			if (!way.has_value()) {
				return std::nullopt;
			}
			
			response.total_time_ = way->weight.time_;
			
			for (auto it = way->edges.begin(); it != way->edges.end(); it++) {
				const auto& edge = graph_.GetEdge(*it);
				response.stop_.push_back({ FindStopByIndex(edge.from), settings_.bus_wait_time_ });
				response.items_.push_back(edge.weight);
			}

			return response;
		}

	private:
		tc::TransportCatalogue& tc_;
		RoutingSettings settings_;
		DirectedWeightedGraph<RouteItem> graph_;
		std::optional<graph::Router<RouteItem>> router_;
		std::unordered_map<std::string_view, VertexId> stop_id_;
	};


	template <typename Iter>
	void TransportRouter::MakeEdges(Iter begin, Iter end, const BusRoute* route) {
		for (auto it = begin; it != end - 1; it++) {
			int span_count = 1;
			double distance = 0.0;
			auto curr_it = it;

			for (auto next_it = std::next(it); next_it != end; next_it++) {
				distance += tc_.GetDistance((*curr_it)->name_, (*next_it)->name_);
				double transit_time = settings_.bus_wait_time_ + (distance / (settings_.bus_velocity_ * METERS_PER_MINUTE));
				RouteItem item{ route->number_ , transit_time, span_count++ };
				graph_.AddEdge({ stop_id_[(*it)->name_], stop_id_[(*next_it)->name_], item });
				curr_it = next_it;
			}
		}
	}

} // namespace transport_router
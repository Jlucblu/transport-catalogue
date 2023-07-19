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

		void SetSettings(const domain::RoutingSettings settings);
		RoutingSettings GetSettings();
		void BuildGraph();

		template <typename Iter>
		void MakeEdges(Iter begin, Iter end, const std::string& number);
		std::string_view FindStopByIndex(VertexId index);
		std::optional<RouteResponse> MakeRouteResponse(const std::string_view from, const std::string_view to);

	private:
		tc::TransportCatalogue& tc_;
		RoutingSettings settings_;
		DirectedWeightedGraph<RouteItem> graph_;
		std::optional<graph::Router<RouteItem>> router_;
		std::unordered_map<std::string_view, VertexId> stop_id_;
	};


	template <typename Iter>
	void TransportRouter::MakeEdges(Iter begin, Iter end, const std::string& number) {
		for (auto it = begin; it != end - 1; it++) {
			int span_count = 1;
			double distance = 0.0;
			auto curr_it = it;

			for (auto next_it = std::next(it); next_it != end; next_it++) {
				distance += tc_.GetDistance((*curr_it)->name_, (*next_it)->name_);
				double transit_time = settings_.bus_wait_time_ + (distance / (settings_.bus_velocity_ * METERS_PER_MINUTE));
				RouteItem item{ number , (*it)->name_, (*next_it)->name_, transit_time, span_count++ };
				graph_.AddEdge({ stop_id_[(*it)->name_], stop_id_[(*next_it)->name_], item });
				curr_it = next_it;
			}
		}
	}

} // namespace transport_router
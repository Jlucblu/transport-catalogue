#include "request_handler.h"

namespace request_handler {

	RequestHandler::RequestHandler(TransportCatalogue& tc)
		: tc_(tc)
	{}

	std::optional<RouteInfo> RequestHandler::GetBusStat(const std::string_view& bus_name) const {
		return tc_.GetBusInfo(bus_name);
	}

	std::unordered_set<BusRoute*> RequestHandler::GetBusesByStop(const std::string_view& stop_name) const {
		return tc_.GetStopInfo(stop_name);
	}

}
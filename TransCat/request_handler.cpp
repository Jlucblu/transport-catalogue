#include "request_handler.h"


namespace request_handler {

	RequestHandler::RequestHandler(tc::TransportCatalogue& tc)
		: tc_(tc)
	{}

	std::optional<RouteInfo> RequestHandler::GetBusStat(const std::string_view& bus_name) const {
		return tc_.GetBusInfo(bus_name);
	}

	std::unordered_set<BusRoute*> RequestHandler::GetBusesByStop(const std::string_view& stop_name) const {
		return tc_.GetStopInfo(stop_name);
	}
	
	std::vector<BusRoute*> RequestHandler::GetRoutes() const {
		return tc_.GetRoutes();
	}

} // namespace request_handler
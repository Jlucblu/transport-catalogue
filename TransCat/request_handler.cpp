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

	std::vector<BusStop*> RequestHandler::MakeUniqueStops(const std::vector<BusStop*> stops) const {
		return tc_.MakeUniqueStops(stops);
	}

	std::vector<BusRoute*> RequestHandler::GetRoutesWithUniqueStops(const std::vector<BusRoute*> routes) const {
		return tc_.GetRoutesWithUniqueStops(routes);
	}

	std::vector<BusStop*> RequestHandler::GetAllUniqueStops() const {
		return tc_.GetAllUniqueStops();
	}

	std::vector<BusRoute*> RequestHandler::GetAllRoutes() const {
		return tc_.GetAllRoutes();
	}

} // namespace request_handler
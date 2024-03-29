#include "transport_catalogue.h"


namespace transport_catalogue {

	// Обновление маршрута автобуса
	void TransportCatalogue::UpdateRoute(const BusRoute& bus) {
		BusRoute* fb = FindRoute(bus.number_);
		if (fb == nullptr) {
			buses_.emplace_back(bus);
			BusRoute* ptr_bus = &buses_.back();
			route_info_map_[ptr_bus->number_] = ptr_bus;
			for (BusStop* ptr_stop : bus.stops_) {
				route_stop_map_[ptr_stop].emplace(ptr_bus);
			}
		}
		else {
			*fb = bus;
		}
	}

	// Обновление остановки и координат
	void TransportCatalogue::UpdateStop(const BusStop& stop) {
		BusStop* fs = FindStop(stop.name_);
		if (fs == nullptr) {
			stops_.emplace_back(stop);
			BusStop* ptr_stop = &stops_.back();
			stop_info_map_[ptr_stop->name_] = ptr_stop;
			route_stop_map_[ptr_stop] = {};
		}
		else {
			*fs = stop;
		}
	}

	// Поиск маршрута по имени
	BusRoute* TransportCatalogue::FindRoute(std::string_view name) const {
		if (route_info_map_.count(name) != 0) {
			return route_info_map_.at(name);
		}
		return nullptr;
	}

	// Поиск остановки по имени
	BusStop* TransportCatalogue::FindStop(std::string_view name) const {
		if (stop_info_map_.count(name) != 0) {
			return stop_info_map_.at(name);
		}
		return nullptr;
	}

	// Получение информации о маршруте автобуса
	RouteStats TransportCatalogue::GetBusInfo(BusRoute* route) const {
		RouteStats info = {};
		double length = 0.0;
		auto unique = MakeUniqueStops(route->stops_);

		const BusStop* ptr_start = nullptr;
		for (const BusStop* prt_end : route->stops_) {
			if (ptr_start) {
				length += ComputeDistance(ptr_start->coordinates_, prt_end->coordinates_);
				info.distance_ += GetDistance(ptr_start->name_, prt_end->name_);
			}
			ptr_start = prt_end;
		}

		info.stops_ = route->stops_.size();
		info.unique_stops_ = unique.size();
		info.curvature_ = info.distance_ / length;
		return info;
	}

	RouteStats TransportCatalogue::GetBusInfo(const std::string_view name) const {
		return GetBusInfo(FindRoute(name));
	}

	// Получение информации об автобусах проходящих через остановку
	std::unordered_set<BusRoute*> TransportCatalogue::GetStopInfo(std::string_view stop_name) const {
		BusStop* stop = FindStop(stop_name);
		return std::unordered_set<BusRoute*>(route_stop_map_.at(stop));
	}

	// Обновление расстояния между двумя точками (остановками)
	void TransportCatalogue::UpdateStopDistance(std::string_view from, const DistancePair& to) {
		for (auto& [stop, distance] : to) {
			BusStop* stopTo = FindStop(stop);
			if (stopTo == nullptr) {
				UpdateStop({ std::string(stop), {0.0, 0.0} });
			}

			std::pair<BusStop*, BusStop*> forward = { stop_info_map_.at(from), stop_info_map_.at(stop) };
			stop_distance_map_[forward] = distance;
		}
	}

	// Получение информации о расстоянии между двумя точками (остановками)
	double TransportCatalogue::GetDistance(std::string_view from, std::string_view to) const {
		BusStop* stopFrom = FindStop(from);
		BusStop* stopTo = FindStop(to);

		if (stop_distance_map_.count({ stopFrom, stopTo })) {
			return stop_distance_map_.at({ stopFrom, stopTo });
		}
		else if (stop_distance_map_.count({ stopTo, stopFrom })) {
			return stop_distance_map_.at({ stopTo, stopFrom });
		}
		else {
			return 0.0;
		}
	}

	// Уникальные остановки на маршруте с сохранением порядка
	std::vector<BusStop*> TransportCatalogue::MakeUniqueStops(const std::vector<BusStop*> route) const {
		std::vector<BusStop*> unique = {};
		std::unordered_set<BusStop*> seenElements = {};

		for (const auto& stops : route) {
			if (seenElements.insert(stops).second) {
				unique.push_back(stops);
			}
		}

		return unique;
	}

	// Получение всех уникальных остановок на всех маршрутах отсортированных в лексографическом порядке
	std::vector<BusStop*> TransportCatalogue::GetAllUniqueStops() const {
		std::vector<BusStop*> allStops = {};

		for (const auto& [_, route] : route_info_map_) {
			for (const auto& stop : route->stops_) {
				allStops.push_back(stop);
			}
		}

		auto uniqueStops = MakeUniqueStops(allStops);

		std::sort(uniqueStops.begin(), uniqueStops.end(),
			[](const domain::BusStop* a, const domain::BusStop* b) {
				return a->name_ < b->name_;
			});

		return uniqueStops;
	}

	// Получение всех маршрутов, отсортированных в лексографическом порядке
	std::vector<BusRoute*> TransportCatalogue::GetAllRoutes() const {
		std::vector<BusRoute*> routes = {};

		for (const auto& route : route_info_map_) {
			routes.push_back(route.second);
		}

		std::sort(routes.begin(), routes.end(),
			[](const domain::BusRoute* a, const domain::BusRoute* b) {
				return a->number_ < b->number_;
			});

		return routes;
	}


	const std::deque<BusStop>& TransportCatalogue::GetStopList() const {
		return stops_;
	}

	const std::deque<BusRoute>& TransportCatalogue::GetBusList() const {
		return buses_;
	}

	const std::unordered_map<std::pair<BusStop*, BusStop*>, double, Hasher>& TransportCatalogue::GetDistanceList() const {
		return stop_distance_map_;
	}

} // namespace transport_catalogue
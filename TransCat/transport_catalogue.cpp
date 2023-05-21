#include "transport_catalogue.h"

namespace transport_catalogue {

	// Обновление маршрута автобуса
	void TransportCatalogue::UpdateRoute(const BusRoute& bus) {
		BusRoute* fb = FindRoute(bus.number_);
		if (fb == nullptr) {
			bus_cat_.emplace_back(bus);
			BusRoute* ptr_bus = &bus_cat_.back();
			route_name_[ptr_bus->number_] = ptr_bus;
			for (BusStop* ptr_stop : bus.stops_) {
				route_stop_[ptr_stop].emplace(ptr_bus);
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
			stop_cat_.emplace_back(stop);
			BusStop* ptr_stop = &stop_cat_.back();
			stop_name_[ptr_stop->name_] = ptr_stop;
			route_stop_[ptr_stop] = {};
		}
		else {
			*fs = stop;
		}
	}

	// Поиск маршрута по имени
	BusRoute* TransportCatalogue::FindRoute(std::string_view name) const {
		if (route_name_.count(name) != 0) {
			return route_name_.at(name);
		}
		return nullptr;
	}

	// Поиск остановки по имени
	BusStop* TransportCatalogue::FindStop(std::string_view name) const {
		if (stop_name_.count(name) != 0) {
			return stop_name_.at(name);
		}
		return nullptr;
	}

	// Получение информации о маршруте автобуса
	RouteInfo TransportCatalogue::GetBusInfo(BusRoute* route) const {
		RouteInfo info = {};
		std::unordered_set<std::string_view> unique = {};
		double length = 0.0;
		const BusStop* ptr_start = nullptr;

		for (const BusStop* prt_end : route->stops_) {
			unique.insert(prt_end->name_);
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

	RouteInfo TransportCatalogue::GetBusInfo(const std::string_view name) const {
		return GetBusInfo(FindRoute(name));
	}

	// Получение информации об автобусах проходящих через остановку
	std::unordered_set<BusRoute*> TransportCatalogue::GetStopInfo(std::string_view stop_name) const {
		BusStop* stop = FindStop(stop_name);
		return std::unordered_set<BusRoute*>(route_stop_.at(stop));
	}

	// Обновление расстояния между двумя точками (остановками)
	void TransportCatalogue::UpdateStopDistance(std::string_view from, const DistancePair& to) {
		for (auto& [stop, distance] : to) {
			BusStop* stopTo = FindStop(stop);
			if (stopTo == nullptr) {
				UpdateStop({ std::string(stop), {0.0, 0.0} });
			}

			std::pair<BusStop*, BusStop*> forward = { stop_name_.at(from), stop_name_.at(stop) };
			stop_distance_[forward] = distance;
		}
	}

	// Получение информации о расстоянии между двумя точками (остановками)
	double TransportCatalogue::GetDistance(std::string_view from, std::string_view to) const {
		BusStop* stopFrom = FindStop(from);
		BusStop* stopTo = FindStop(to);

		if (stop_distance_.count({ stopFrom, stopTo })) {
			return stop_distance_.at({ stopFrom, stopTo });
		}
		else if (stop_distance_.count({ stopTo, stopFrom })) {
			return stop_distance_.at({ stopTo, stopFrom });
		}
		else {
			return 0.0;
		}
	}

} // namespace transport_catalogue
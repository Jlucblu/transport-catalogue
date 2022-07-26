#include "transport_catalogue.h"


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

// Поиск маршрута по имени
BusRoute* TransportCatalogue::FindRoute(std::string_view name) {
	if (route_name_.count(name) != 0) {
		return route_name_.at(name);
	}
	return nullptr;
}

// Поиск остановки по имени
BusStop* TransportCatalogue::FindStop(std::string_view name) {
	if (stop_name_.count(name) != 0) {
		return stop_name_.at(name);
	}
	return nullptr;
}

// Получение информации о маршруте
RouteInfo TransportCatalogue::GetStopInfo(BusRoute* route) {
	RouteInfo info;
	std::unordered_set<std::string_view> unique;
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

RouteInfo TransportCatalogue::GetStopInfo(const std::string_view name) {
	return GetStopInfo(FindRoute(name));
}

std::unordered_set<BusRoute*> TransportCatalogue::GetBusInfo(const std::string& stop_name) {
	BusStop* stop = FindStop(stop_name);
	return std::unordered_set<BusRoute*>(route_stop_.at(stop));
}

void TransportCatalogue::UpdateStopDictance(const std::string& from, const DistancePair& to) {
	for (auto& [stop, distance] : to) {
		BusStop* tostop = FindStop(stop);
		if (tostop == nullptr) {
			UpdateStop({ stop, {0.0, 0.0} });;
		}

		std::pair<BusStop*, BusStop*> forward = { stop_name_.at(from), stop_name_.at(stop) };
		std::pair<BusStop*, BusStop*> backward = { stop_name_.at(stop), stop_name_.at(from) };

		stop_distance_[forward] = distance;
		if (!stop_distance_.count(backward)) {
			stop_distance_.emplace(backward, distance);
		}
	}
}

int TransportCatalogue::GetDistance(const std::string& from, const std::string& to) {
	const std::pair<BusStop*, BusStop*> stops = { stop_name_.at(from), stop_name_.at(to) };
	return stop_distance_.at(stops);
}
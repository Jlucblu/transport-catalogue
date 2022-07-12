#include "transport_catalogue.h"
#include "iostream"

using namespace std::string_literals;

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
	BusRoute* fb = FindRoute(bus.name_);
	if (fb == nullptr) {
		bus_cat_.emplace_back(bus);
		BusRoute* ptr_bus = &bus_cat_.back();
		route_name_[ptr_bus->name_] = ptr_bus;
		for (BusStop* ptr_stop : bus.stops_) {
			route_stop_[ptr_stop].push_back(ptr_bus);
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
RouteInfo TransportCatalogue::GetInfo(BusRoute* route) {
	std::unordered_set<std::string_view> unique;
	double length = 0.0;
	const BusStop* previous = nullptr;
	for (const BusStop* current : route->stops_) {
		unique.insert(current->name_);
		if (previous) {
			length += ComputeDistance(previous->coordinates_, current->coordinates_);
		}
		previous = current;
	}
	return { static_cast<int>(route->stops_.size()), static_cast<int>(unique.size()), length };
}

RouteInfo TransportCatalogue::GetInfo(const std::string_view name) {
	return GetInfo(FindRoute(name));
}
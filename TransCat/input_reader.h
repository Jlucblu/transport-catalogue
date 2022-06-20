#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <tuple>

#include "geo.h"
#include "transport_catalogue.h"

using namespace std::literals;

// ввод запросов
std::string ReadLine() {
	std::string s;
	std::getline(std::cin, s);
	return s;
}
 
// ввод кол-ва запросов
int ReadNumber() {
	int x;
	std::cin >> x;
	ReadLine();
	return x;
}

// парсинг ключа
std::string ParseKey(std::string& request) {
	std::string key(request.begin(), std::find(request.begin(), request.end(), ' '));
	request.erase(0, key.size());
	request.erase(0, request.find_first_not_of(' '));
	return key;
}

// маршрут автобуса
std::vector<std::string> ParseRoute(std::string& str, bool type) {
	std::vector<std::string> single_stops = {};
	auto it = str.begin();
	while (it != str.end()) {
		if (!type) {
			if (std::count(str.begin(), str.end(), '-') == 0) {
				std::string word(it, str.end());
				single_stops.push_back(word);
				str.erase(0, word.size());
			}
			else {
				std::string word(it, std::find(str.begin(), str.end(), '-') - 1);
				single_stops.push_back(word);
				str.erase(0, word.size() + 3);
			}
			it = str.begin();

		}
		else {
			if (std::count(str.begin(), str.end(), '>') == 0) {
				std::string word(it, str.end());
				single_stops.push_back(word);
				str.erase(0, word.size());
			}
			else {
				std::string word(it, std::find(str.begin(), str.end(), '>') - 1);
				single_stops.push_back(word);
				str.erase(0, word.size() + 3);
			}
			it = str.begin();
		}
	}

	return single_stops;
}

// парсинг номера автобуса
BusRoute ParseBus(std::string& request) {
	std::string number(request.begin(), std::find(request.begin(), request.end(), ':'));
	request.erase(0, number.size() + 2);
	bool circle = (std::count(request.begin(), request.end(), '>') ? true : false);
	std::vector<std::string> stops = ParseRoute(request, circle);

	return { number, stops, circle };
}

// сопоставление остановки и координат
BusStop UpdateStop(std::string& request) {
	double latitude = 0.0;
	double longitude = 0.0;
	std::string name(request.begin(), std::find(request.begin(), request.end(), ':'));
	request.erase(0, name.size() + 2);
	auto pos = request.find(',');
	if (pos < request.size()) {
		latitude = std::stod(request.substr(0, pos));
	}
	if ((pos + 2) < request.size()) {
		longitude = std::stod(request.substr(pos + 2, request.size() - 1));
	}
	request.clear();

	return { name, latitude, longitude };
}

// обновление каталога (остановки / маршрут автобуса)
void UpdateCat() {
	int count = ReadNumber();
	std::string key;
	std::deque<BusRoute> bus_cat;
	std::deque<BusStop> stop_cat;

	while (count != 0) {
		std::string request = ReadLine();

		if (request.size() == 0) {
			continue;
		}

		key = ParseKey(request);

		if (key == "Bus") {
			BusRoute route = ParseBus(request);
			bus_cat.push_back(route);
			std::cout << route.number_ << std::endl;
			for (auto& b : route.stops_) {
				std::cout << b << std::endl;
			}
		} 
		else if (key == "Stop") {
			BusStop stop = UpdateStop(request);
			stop_cat.push_back(stop);
			std::cout << stop.name_ << ' ' << stop.latitude_ << ' ' << stop.longitude_ << std::endl;
		}
		--count;
	}
}
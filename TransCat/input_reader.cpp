#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>

#include "input_reader.h"
#include "transport_catalogue.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

// Ввод запросов
std::string ReadLine(std::istream& in) {
    std::string s;
    std::getline(in, s);
    return s;
}

// Парсинг ключа
std::string_view ParseKey(std::string_view request) {
    auto pos = request.find(' ');
    std::string_view key = request.substr(0, pos);
    return key;
}

// Парсинг остановки и координат
BusStop ParseStop(const std::string& str) {
    auto pos_name = str.find(' ');
    auto pos_lat = str.find(':');
    auto pos_lng = str.find(',');
    std::string stop_name = str.substr(str.find_first_not_of(' ', pos_name), pos_lat - (pos_name + 1));
    double latitude = std::stod(std::string(str.substr(str.find_first_not_of(": "s, pos_lat), pos_lng - (pos_lat + 2))));
    double longitude = std::stod(std::string(str.substr(str.find_first_not_of(", "s, pos_lng))));
    return { std::move(stop_name), { std::move(latitude), std::move(longitude) } };
}

// Парсинг маршрута автобуса
std::vector<std::string> ParseRoute(const std::string& str, bool circle) {
    std::vector<std::string> route = {};
    auto pos_start = str.find(':');

    while (true) {
        BusStop stop = {};
        if (circle) {
            auto first = str.find_first_not_of(' ', pos_start + 1);
            auto last = str.find_first_of('>', first);
            std::string name = str.substr(first, last - first - 1);
            route.push_back(name);
            if (last == str.npos) {
                break;
            }
            pos_start = last + 1;
        }
        else {
            auto first = str.find_first_not_of(' ', pos_start + 1);
            auto last = str.find_first_of('-', first);
            std::string name = str.substr(first, last - first - 1);
            route.push_back(name);
            if (last == str.npos) {
                break;
            }
            pos_start = last + 1;
        }
    }

    if (!circle) {
        for (int i = route.size() - 2; i >= 0; --i) {
            route.push_back(route[i]);
        }
    }

    return route;
}

// Обновление каталога
std::istream& UpdateCat(std::istream& in, TransportCatalogue& cat) {
    int count = std::stoi(ReadLine(in));

    while (count != 0) {
        const std::string& request = ReadLine(in);
        std::string_view key = ParseKey(request);

        if (key == "Stop"sv) {
            BusStop query = ParseStop(request);
            cat.UpdateStop(query);
        }
        else if (key == "Bus"sv) {
            BusRoute route = {};
            route.circle = (std::count(request.begin(), request.end(), '>') ? true : false);
            route.name_ = std::string(std::next(std::find(request.begin(), request.end(), ' ')), std::find(request.begin(), request.end(), ':'));
            std::vector<std::string> stops = ParseRoute(request, route.circle);

            for (const std::string& stop : stops) {
                BusStop* found = cat.FindStop(stop);
                if (!found) {
                    cat.UpdateStop({ stop, { 0.0, 0.0 } });
                }
                route.stops_.push_back(cat.FindStop(stop));
            }

            cat.UpdateRoute(route);
        }

        --count;
    }

    return in;
}
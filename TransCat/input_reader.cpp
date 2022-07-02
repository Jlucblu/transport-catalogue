#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "input_reader.h"
#include "transport_catalogue.h"

using namespace std::string_literals;


// Ввод запросов
std::string ReadLine(std::istream& in) {
    std::string s;
    std::getline(in, s);
    return s;
}

// Парсинг ключа
std::string_view ParseKey(std::string_view request) {
    auto pos = request.find(' ');
    std::string_view key(request.substr(0, pos));
    return key;
}

// Парсинг остановки и координат
InputStops ParseStop(std::string& str) {
    auto pos_name = str.find(' ');
    auto pos_lat = str.find(':');
    auto pos_lng = str.find(',');
    std::string stop_name = str.substr(str.find_first_not_of(' ', pos_name), pos_lat - (pos_name + 1));
    double latitude = std::stod(str.substr(str.find_first_not_of(": "s, pos_lat), pos_lng - (pos_lat + 2)));
    double longitude = std::stod(str.substr(str.find_first_not_of(", "s, pos_lng)));
    return { stop_name, { latitude, longitude } };
}

// Парсинг маршрута автобуса
InputRoute ParseRoute(std::string& str) {
    std::vector<std::string> single_stops = {};
    bool circle = (std::count(str.begin(), str.end(), '>') ? true : false);
    std::string number(std::next(std::find(str.begin(), str.end(), ' ')), std::find(str.begin(), str.end(), ':'));
    auto pos_start = str.find(':');

    while (true) {
        if (circle) {
            auto first = str.find_first_not_of(' ', pos_start + 1);
            auto last = str.find_first_of('>', first);
            std::string name(str.substr(first, last - first - 1));
            single_stops.push_back(name);
            if (last == str.npos) {
                break;
            }
            pos_start = last + 1;
        }
        else {
            auto first = str.find_first_not_of(' ', pos_start + 1);
            auto last = str.find_first_of('-', first);
            std::string name(str.substr(first, last - first - 1));
            single_stops.push_back(name);
            if (last == str.npos) {
                break;
            }
            pos_start = last + 1;
        }
    }

    if (!circle) {
        for (int i = single_stops.size() - 2; i >= 0; --i) {
            single_stops.push_back(single_stops[i]);
        }
    }

    return { number, single_stops };
}

// Обновление каталога
std::istream& UpdateCat(std::istream& in, TransportCatalogue& cat) {
    using namespace std::string_view_literals;
    int count = std::stoi(ReadLine(in));
    std::vector<InputRoute> queue;

    while(count != 0) {
        std::string request = ReadLine(in);
        std::string_view key = ParseKey(request);

        if (key == "Stop"sv) {
            InputStops query = ParseStop(request);
            cat.UpdateStop(query.name, query.coordinates);
        }
        else if (key == "Bus"sv) {
            queue.push_back(ParseRoute(request));
            //Route b_s = ParseRoute(request);
            //cat.UpdateRoute((b_s.name), b_s.stopNames);
        }
        --count;
    }

    for (auto& parse : queue) {
        cat.UpdateRoute(parse.name, parse.stops);
    }

    return in;
}
#include "input_reader.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

// Ввод запросов
std::string ReadLine(std::istream& in) {
    std::string s;
    std::getline(in, s);
    return s;
}

// Парсинг ключа
std::string_view ParseKey(std::string_view str) {
    return std::string_view (str.substr(0, str.find(' ')));;
}

// Парсинг остановки и координат
// Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino
// Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka, 100m to Marushkino
std::pair <BusStop, DistancePair> ParseStop(std::string_view str) {
    BusStop stop = {};
    DistancePair distance_to_stop = {};
    auto pos_name = str.find(' ');
    auto pos_start = str.find(':');
    auto pos_lat_end = str.find(',');
    auto pos_lng_end = str.find(',', pos_lat_end + 1);
    stop.name_ = std::string(str.substr(str.find_first_not_of(' ', pos_name), pos_start - (pos_name + 1)));
    stop.coordinates_.lat = std::stod(std::string(str.substr(str.find_first_not_of(": "s, pos_start), pos_lat_end - (pos_start + 2))));
    stop.coordinates_.lng = std::stod(std::string(str.substr(str.find_first_not_of(", "s, pos_lat_end), pos_lng_end - (pos_lat_end + 2))));
    while (pos_lng_end != str.npos) {
        auto pos_distance = str.find("m to "s, pos_lng_end + 1);
        auto last = str.find(',', pos_lng_end + 1);
        double dist = std::stoi(std::string(str.substr(str.find_first_not_of(", "s, pos_lng_end), pos_distance - (pos_lng_end + 2))));
        std::string_view name = str.substr(str.find_first_not_of("m to "s, pos_distance), last - (pos_distance + 5));
        distance_to_stop.emplace(name, dist);
        pos_lng_end = last;
    } 
    
    return { stop, distance_to_stop };
}

// Парсинг маршрута автобуса
std::deque<std::string> ParseRoute(std::string_view str, bool circle) {
    std::deque<std::string> route = {};
    auto pos_start = str.find(':');
    while (pos_start != str.npos) {
        if (circle) {
            auto first = str.find_first_not_of(' ', pos_start + 1);
            auto last = str.find_first_of('>', first);
            std::string name = std::string(str.substr(first, last - first - 1));
            route.push_back(name);
            pos_start = last;
        }
        else {
            auto first = str.find_first_not_of(' ', pos_start + 1);
            auto last = str.find_first_of('-', first);
            std::string name = std::string(str.substr(first, last - first - 1));
            route.push_back(name);
            pos_start = last;
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

        if (key == "Stop"s) {
            std::pair<BusStop, DistancePair> query = ParseStop(request);
            cat.UpdateStop(std::move(query.first));
            if (!query.second.empty()) {
                cat.UpdateStopDictance(std::move(query.first.name_), std::move(query.second));
            }
        }
        else if (key == "Bus"s) {
            BusRoute route = {};
            route.circle = (std::count(request.begin(), request.end(), '>') ? true : false);
            route.number_ = std::string(std::next(std::find(request.begin(), request.end(), ' ')), std::find(request.begin(), request.end(), ':'));
            auto stops = ParseRoute(request, route.circle);

            for (auto& stop : stops) {
                BusStop* found = cat.FindStop(stop);
                if (found == nullptr) {
                    cat.UpdateStop({ stop, { 0.0, 0.0 } });
                }
                route.stops_.push_back(cat.FindStop(stop));
            }

            cat.UpdateRoute(std::move(route));
        }

        --count;
    }

    return in;
}
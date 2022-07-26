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
std::pair <BusStop, DistancePair> ParseStop(const std::string& str) {
    BusStop stop = {};
    DistancePair distance_to_stop = {};
    auto pos_name = str.find(' ');
    auto pos_start = str.find(':');
    auto pos_lat_end = str.find(',');
    auto pos_lng_end = str.find(',', pos_lat_end + 1);
    stop.name_ = str.substr(str.find_first_not_of(' ', pos_name), pos_start - (pos_name + 1));
    stop.coordinates_.lat = std::stod(str.substr(str.find_first_not_of(": "s, pos_start), pos_lat_end - (pos_start + 2)));
    stop.coordinates_.lng = std::stod(str.substr(str.find_first_not_of(", "s, pos_lat_end), pos_lng_end - (pos_lat_end + 2)));
    while (pos_lng_end != str.npos) {
        std::pair<std::string, int> dts = {};
        auto pos_distance = str.find("m to "s, pos_lng_end + 1);
        auto last = str.find(',', pos_lng_end + 1);
        dts.second = std::stoi(str.substr(str.find_first_not_of(", "s, pos_lng_end), pos_distance - (pos_lng_end + 2)));
        dts.first = str.substr(str.find_first_not_of("m to "s, pos_distance), last - (pos_distance+ 5));
        distance_to_stop.push_back(dts);
        pos_lng_end = last;
    }
    
    return { stop, distance_to_stop };
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

        if (key == "Stop"s) {
            std::pair<BusStop, DistancePair> query = ParseStop(request);
            cat.UpdateStop(query.first);
            if (!query.second.empty()) {
                cat.UpdateStopDictance(query.first.name_, query.second);
            }
        }
        else if (key == "Bus"s) {
            BusRoute route = {};
            route.circle = (std::count(request.begin(), request.end(), '>') ? true : false);
            route.number_ = std::string(std::next(std::find(request.begin(), request.end(), ' ')), std::find(request.begin(), request.end(), ':'));
            std::vector<std::string> stops = ParseRoute(request, route.circle);

            for (const std::string& stop : stops) {
                BusStop* found = cat.FindStop(stop);
                if (found == nullptr) {
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
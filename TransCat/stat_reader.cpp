#include "stat_reader.h"
#include "input_reader.h"

#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace std::string_literals;
using namespace std::string_view_literals;


// Вывод информации по запросу
void PrintInfo(std::istream& in, std::ostream& out, TransportCatalogue& cat) {
    int count = std::stoi(ReadLine(in));

    while(count != 0) { 
        const std::string& request = ReadLine(in);
        std::string_view key = ParseKey(request);
        if (key == "Bus"sv) {
            const std::string& number = request.substr(request.find_first_not_of(' ', key.size()), request.npos);
            const BusRoute* bus = cat.FindRoute(number);
            if (!bus) {
                out << "Bus "s << number << ": not found\n"s;
            }
            else {
                RouteInfo info = cat.GetInfo(number);
                out << "Bus "s << number << ": "s << info.number_ << " stops on route, "s << info.unique_stops_
                    << " unique stops, "s << info.distance_ << " route length\n"s;
            }
        }
        else if (key == "Stop"sv) {
            const std::string& name = request.substr(request.find_first_not_of(' ', key.size()), request.npos);
            const BusStop* stop = cat.FindStop(name);
            if (!stop) {
                out << "Stop "s << name << ": not found\n"s;
            }
            else {
                out << "Stop "s << name << ": "s << stop->coordinates_.lat << " "s << stop->coordinates_.lng << "\n"s;
            }
        }

        --count;
    }
}
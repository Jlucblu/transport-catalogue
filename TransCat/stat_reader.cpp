#include "stat_reader.h"


using namespace std::string_literals;
using namespace std::string_view_literals;


// Вывод информации по запросу
void PrintInfo(std::istream& in, std::ostream& out, TransportCatalogue& cat) {
    int count = std::stoi(ReadLine(in));

    while(count != 0) { 
        const std::string& request = ReadLine(in);
        std::string_view key = ParseKey(request);
        if (key == "Bus"s) {
            const std::string& number = request.substr(request.find_first_not_of(' ', key.size()), request.npos);
            const BusRoute* bus = cat.FindRoute(number);
            if (!bus) {
                out << "Bus "s << number << ": not found\n"s;
            }
            else {
                RouteInfo info = cat.GetStopInfo(number);
                out << "Bus "s << number << ": "s << info.stops_ << " stops on route, "s << info.unique_stops_
                    << " unique stops, "s << info.distance_ << " route length, "s << info.curvature_ << " curvature\n"s;
            } 
        }
        else if (key == "Stop"s) {
            const std::string& name = request.substr(request.find_first_not_of(' ', key.size()), request.npos);
            const BusStop* stop = cat.FindStop(name);
            if (!stop) {
                out << "Stop "s << name << ": not found\n"s;
            }
            else {
                std::unordered_set<BusRoute*> numofbus = cat.GetBusInfo(name);
                std::set<std::string> bus_numbers;
                for (auto& num : numofbus) {
                    bus_numbers.emplace(num->number_);
                }
                
                if (numofbus.empty()) {
                    out << "Stop "s << name << ": no buses\n"s;
                }
                else {
                    out << "Stop "s << name << ": buses"s;
                        for (auto& bus : bus_numbers) {
                            out << ' ' << bus;
                        }
                        out << "\n";
                }
            }
        }

        --count;
    }
}
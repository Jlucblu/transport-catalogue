#include <cassert>
#include <iostream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

/*
//tests for TransportCatalogue class
void TCTest1() {
	using namespace std::string_literals;
	TransportCatalogue tc;
	std::vector<std::string> stops{ "stop1"s, "stop2"s };
	tc.UpdateStop({ "stop1", { 0.1, 0.2 } });
	tc.UpdateStop({ "stop2", { 0.2, 0.3 } });
	const BusStop* stop = tc.FindStop("stop1"s);
	assert(stop->name_ == "stop1"s);
	assert(stop->coordinates_.lat == 0.1);
	assert(stop->coordinates_.lng == 0.2);
	stop = tc.FindStop("no such stop"s);
	assert(!stop);
	std::string busName = "bus1";
	tc.UpdateRoute( { busName, { { "stop1"sv, {0.1, 0.2} }, {"stop2"sv, {0.2, 0.3}} }, false});
	const BusRoute* rt = tc.FindRoute("bus1");
	assert(rt->circle == false);
	assert(rt->stops_[0] == tc.FindStop(stops[0]));
	try {
		stops.push_back("stop3"s);
		std::string busName2 = "bus2"s;
		tc.UpdateRoute(busName2, stops, false);
		//assert(false);
	}
	catch (std::invalid_argument) {
		//std::cout << e.what() << std::endl;
	}
	catch (std::exception) {
		assert(false);
	}
	rt = tc.FindRoute("no such bus"s);
	assert(!rt);
}
void TCTest2() {
	using namespace std::string_literals;
	TransportCatalogue tc;
	std::vector<std::string> stops{ "stop1"s, "stop2"s, "stop3"s, "stop4"s };
	std::vector<Coordinates> coord{ {0.0, 0.0}, {0.0, 1.0}, {0.0, 2.0}, {0.0, 3.0} };
	std::vector<std::string> routeNames{ "route 1"s, "route2"s };
	for (size_t i = 0; i < stops.size(); i++)
	{
		tc.UpdateStop(stops[i], coord[i]);
	}
	tc.UpdateRoute(routeNames[0], stops, false);
	stops.resize(2);
	tc.UpdateRoute(routeNames[1], stops, false);
	RouteInfo info1 = tc.GetInfo(routeNames[0]);
	assert(info1.number_ == 4);
	assert(info1.unique_stops_ == 4);
	double len = ComputeDistance(coord[0], coord[1]) + ComputeDistance(coord[2], coord[1]) + ComputeDistance(coord[2], coord[3]);
	assert(info1.distance_ == len);
	RouteInfo info2 = tc.GetInfo("route2"s);
	assert(info2.number_ == 2);
	assert(info2.unique_stops_ == 2);
	len = ComputeDistance(coord[0], coord[1]);
	assert(info2.distance_ == len);
}
void testTransportCatalogue() {
	TCTest1();
	TCTest2();

}

//tests for input_reader functions

TransportCatalogue makeSimpleCatalogue() {
	using namespace std::string_literals;
	std::stringstream ss;
	ss << 10 << "\n";
	ss << "Stop Tolstopaltsevo: 55.611087, 37.208290"s << "\n";
	ss << "Stop Marushkino: 55.595884, 37.209755"s << "\n";
	ss << "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye"s << "\n";
	ss << "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka"s << "\n";
	ss << "Stop Rasskazovka: 55.632761, 37.333324"s << "\n";
	ss << "Stop Biryulyovo Zapadnoye: 55.574371, 37.651700"s << "\n";
	ss << "Stop Biryusinka: 55.581065, 37.648390"s << "\n";
	ss << "Stop Universam: 55.587655, 37.645687"s << "\n";
	ss << "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656"s << "\n";
	ss << "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164"s << "\n";
	TransportCatalogue tc;
	UpdateCat(ss, tc);
	return tc;
}

void IRTest1() {
	using namespace std::string_literals;
	std::stringstream ss;
	ss << "NotANumber"s << "\n";
	TransportCatalogue tc;
	try {
		UpdateCat(ss, tc);
		assert(false);
	}
	catch (std::invalid_argument) {

	}
	catch (std::exception) {
		assert(false);
	}
}

void IRTest2() {
	using namespace std::string_literals;
	std::stringstream ss;
	ss << 10 << "\n";
	ss << "Stop Tolstopaltsevo: 55.611087, 37.208290"s << "\n";
	ss << "Stop Marushkino: 55.595884, 37.209755"s << "\n";
	ss << "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye"s << "\n";
	ss << "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka"s << "\n";
	ss << "Stop Rasskazovka: 55.632761, 37.333324"s << "\n";
	ss << "Stop Biryulyovo Zapadnoye: 55.574371, 37.651700"s << "\n";
	ss << "Stop Biryusinka: 55.581065, 37.648390"s << "\n";
	ss << "Stop Universam: 55.587655, 37.645687"s << "\n";
	ss << "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656"s << "\n";
	ss << "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164"s << "\n";
	TransportCatalogue tc;
	UpdateCat(ss, tc);
	auto routeInfo = tc.GetInfo("256"s);
	assert(routeInfo.number_ == 6);
	assert(routeInfo.unique_stops_ == 5);
	assert(floor(routeInfo.distance_) == 4371.);
	routeInfo = tc.GetInfo("750"s);
	assert(routeInfo.number_ == 5);
	assert(routeInfo.unique_stops_ == 3);
	assert(floor(routeInfo.distance_) == 20939.);
	try {
		routeInfo = tc.GetInfo("noSuchRoute"s);
		assert(false);
	}
	catch (std::invalid_argument) {

	}
	catch (std::exception) {
		assert(false);
	}
}

void IRTest3() {
	TransportCatalogue tc = makeSimpleCatalogue();
	using namespace std::string_literals;
	std::stringstream ss;
	ss << 3 << "\n";
	ss << "Bus 256"s << "\n";
	ss << "Bus 750"s << "\n";
	ss << "Bus 751"s << "\n";
	std::stringstream so;
	PrintInfo(ss, so, tc);
	const std::vector<std::string> answers = { "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length"s,
		"Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length"s,
		"Bus 751: not found"s
	};
	for (const std::string& ans : answers) {
		std::string line;
		std::getline(so, line);
		assert(line == ans);
	}
}

void testInputReader() {
	IRTest1();
	IRTest2();
	IRTest3();
}
*/

using namespace std;

int main() {
	//testTransportCatalogue();
	//testInputReader();
	//cout << "all tests passed successfully"s;

	TransportCatalogue tc;
	UpdateCat(cin, tc);
	PrintInfo(cin, cout, tc);
	
}
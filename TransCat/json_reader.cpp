#include "json_reader.h"


namespace json_reader {

	JSONReader::JSONReader(TransportCatalogue& tc, std::istream& input, std::ostream& output)
		: tc_(tc)
		, request_(tc)
		, doc_(Load(input))
		, output_(output)
	{}

	// Парсинг входящей запроса
	void JSONReader::ParseBaseRequest() const {
		const auto& load = doc_.GetRoot().AsMap();

		if (load.count("base_requests")){
			const auto& base = load.at("base_requests").AsArray();

			for (const auto& request : base) {
				const auto& type = request.AsMap().at("type").AsString();

				if (type == "Bus") {
					tc_.UpdateRoute(ParseBus(request.AsMap()));
				}

				if (type == "Stop") {
					const auto& stopInfo = ParseStop(request.AsMap());
					tc_.UpdateStop(stopInfo.first);
					if (!stopInfo.second.empty()) {
						tc_.UpdateStopDistance(stopInfo.first.name_, stopInfo.second);
					}
				}
			}
		}

		if (load.count("stat_requests")) {
			const auto& base = load.at("stat_requests").AsArray();
			Array answer = {};

			for (const auto& request : base) {
				const auto& type = request.AsMap().at("type").AsString();

				if (type == "Bus") {
					const auto& busAnswer = GetBusAnswer(request.AsMap());
					answer.emplace_back(busAnswer);
				}

				if (type == "Stop") {
					const auto& stopAnswer = GetStopAnswer(request.AsMap());
					answer.emplace_back(stopAnswer);
				}
			}

			Print(Document(answer), output_);
		}
	}


	// Парсинг маршрута автобуса
	BusRoute JSONReader::ParseBus(const Dict& businfo) const {
		BusRoute route = {};
		route.number_ = businfo.at("name").AsString();
		route.circle = businfo.at("is_roundtrip").IsBool();
		const auto& stops = businfo.at("stops").AsArray();
		for (auto& stop : stops) {
			BusStop* found = tc_.FindStop(stop.AsString());
			if (found == nullptr) {
				tc_.UpdateStop({ stop.AsString(), {0.0, 0.0} });
			}
			route.stops_.push_back(tc_.FindStop(stop.AsString()));
		}

		if (route.circle) {
			for (int i = route.stops_.size() - 2; i >= 0; --i) {
				route.stops_.push_back(route.stops_[i]);
			}
		}

		return route;
	}

	// Парсинг остановок на маршруте
	std::pair<BusStop, DistancePair> JSONReader::ParseStop(const Dict& stopinfo) const {
		BusStop stop = {};
		DistancePair distance = {};
		stop.name_ = stopinfo.at("name").AsString();
		stop.coordinates_.lat = stopinfo.at("latitude").AsDouble();
		stop.coordinates_.lng = stopinfo.at("longitude").AsDouble();
		
		for (const auto& [name, interval] : stopinfo.at("road_distances").AsMap()) {
			distance.insert({ name, interval.AsInt() });
		}

		return { stop, distance };
	}


	// Формирование ответа на запрос по номеру автобуса
	Dict JSONReader::GetBusAnswer(const Dict& request) const {
		Dict dict = {};
		dict.emplace("request_id"s, request.at("id").AsInt());
		const auto& name = request.at("name").AsString();
		const auto& valid = tc_.FindRoute(name);

		if (!valid) {
			dict.emplace("error_message"s, "not found"s);
		}
		else {
			const auto& businfo = request_.GetBusStat(name);
			dict.emplace("curvature"s, businfo->curvature_);
			dict.emplace("route_length"s, businfo->distance_);
			dict.emplace("stop_count"s, businfo->stops_);
			dict.emplace("unique_stop_count"s, businfo->unique_stops_);
		}

		return dict;
	}

	// Формирование ответа на запрос по названию остановки
	Dict JSONReader::GetStopAnswer(const Dict& request) const {
		Dict dict = {};
		dict.emplace("request_id"s, request.at("id").AsInt());
		const auto& name = request.at("name").AsString();
		const auto& valid = tc_.FindStop(name);
		if (!valid) {
			dict.emplace("error_message"s, "not found"s);
		}
		else {
			Array set = {};
			const auto& businfo = request_.GetBusesByStop(name);
			for (const auto& bus : businfo) {
				set.push_back(bus->number_);
			}
			dict.emplace("buses"s, set);
		}

		return dict;
	}

} // namespace reading_queries

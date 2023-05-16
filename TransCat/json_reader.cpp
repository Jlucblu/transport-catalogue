#include "json_reader.h"


namespace reading_queries {

	//JSONReader::JSONReader(const Document& doc, TransportCatalogue& tc)
	//	: doc_(doc)
	//	, tc_(tc)
	//{}

	JSONReader::JSONReader(TransportCatalogue& tc)
		: tc_(tc)
	{}

	void JSONReader::ParseBaseRequest(const Document& doc) {
		const auto& load = doc.GetRoot().AsMap().at("base_requests").AsArray();

		for (const auto& request : load) {
			if (request.AsMap().at("type").AsString() == "Bus") {
				tc_.UpdateRoute(ParseBus(request.AsMap()));
			}
		}

		for (const auto& request : load) {
			if (request.AsMap().at("type").AsString() == "Stop") {
				tc_.UpdateStop(ParseStop(request.AsMap()));

				if (request.AsMap().count("road_distances")) {
					DistancePair to = {};
					const auto& from = request.AsMap().at("name").AsString();
					
					for (const auto& [name, distance] : request.AsMap().at("road_distances").AsMap()) {
						to.insert({ name, distance.AsInt() });
					}

					tc_.UpdateStopDistance(from, to);
				}
			}
		}
	}


	BusRoute JSONReader::ParseBus(const json::Dict& businfo) {
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

		if (!route.circle) {
			for (int i = route.stops_.size() - 2; i >= 0; --i) {
				route.stops_.push_back(route.stops_[i]);
			}
		}

		return route;
	}


	BusStop JSONReader::ParseStop(const json::Dict& stopinfo) {
		BusStop stop = {};
		stop.name_ = stopinfo.at("name").AsString();
		stop.coordinates_.lat = stopinfo.at("latitude").AsDouble();
		stop.coordinates_.lng = stopinfo.at("longitude").AsDouble();
		return stop;
	}


} // namespace reading_queries
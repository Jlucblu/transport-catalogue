#include "json_reader.h"


namespace json_reader {

	JSONReader::JSONReader(tc::TransportCatalogue& tc, rh::RequestHandler& rh, mr::MapRenderer& mr, tr::TransportRouter& tr, std::istream& input, std::ostream& output)
		: tc_(tc)
		, request_(rh)
		, render_(mr)
		, router_(tr)
		, doc_(json::Load(input))
		, output_(output)
	{}


	// ------------------------ Parsing ------------------------ //
	
	// Парсинг входящего запроса
	void JSONReader::ParseBaseRequest() const {
		const auto& load = doc_.GetRoot().AsDict();

		if (load.count("base_requests")){
			const auto& base = load.at("base_requests").AsArray();

			for (const auto& request : base) {
				const auto& type = request.AsDict().at("type").AsString();

				if (type == "Bus") {
					tc_.UpdateRoute(ParseBus(request.AsDict()));
				}

				if (type == "Stop") {
					const auto& stopInfo = ParseStop(request.AsDict());
					tc_.UpdateStop(stopInfo.first);
					if (!stopInfo.second.empty()) {
						tc_.UpdateStopDistance(stopInfo.first.name_, stopInfo.second);
					}
				}
			}
		}

		if (load.count("render_settings")) {
			const auto& render = load.at("render_settings").AsDict();
			render_.SetSettings(ParseMapSettings(render));
		}

		if (load.count("routing_settings")) {
			const auto& routing = load.at("routing_settings").AsDict();

			RoutingSettings settings = {};
			settings.bus_velocity_ = routing.at("bus_velocity").AsDouble();
			settings.bus_wait_time_ = routing.at("bus_wait_time").AsDouble();
			router_.SetSettings(settings);
		}

		if (load.count("stat_requests")) {
			const auto& stat = load.at("stat_requests").AsArray();
			router_.BuildGraph();
			json::Array answer = {};
			json::Builder builder {};

			for (const auto& request : stat) {
				const auto& type = request.AsDict().at("type").AsString();

				if (type == "Bus") {
					const auto& busAnswer = GetBusAnswer(request.AsDict());
					answer.emplace_back(busAnswer);
				}

				if (type == "Stop") {
					const auto& stopAnswer = GetStopAnswer(request.AsDict());
					answer.emplace_back(stopAnswer);
				}
				
				if (type == "Map") {
					const auto& mapAnswer = GetMapAnswer(request.AsDict());
					answer.emplace_back(mapAnswer);
				}

				if (type == "Route") {
					const auto& routeAnswer = GetRouteAnswer(request.AsDict());
					answer.emplace_back(routeAnswer);
				}
			}

			if (!answer.empty()) {
				json::Print(json::Document(answer), output_);
			}
		}
	}

	// Парсинг маршрута автобуса
	BusRoute JSONReader::ParseBus(const json::Dict& businfo) const {
		BusRoute route = {};
		route.number_ = businfo.at("name").AsString();
		route.circle = businfo.at("is_roundtrip").AsBool();
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

	// Парсинг остановок на маршруте
	std::pair<BusStop, DistancePair> JSONReader::ParseStop(const json::Dict& stopinfo) const {
		BusStop stop = {};
		DistancePair distance = {};
		stop.name_ = stopinfo.at("name").AsString();
		stop.coordinates_.lat = stopinfo.at("latitude").AsDouble();
		stop.coordinates_.lng = stopinfo.at("longitude").AsDouble();
		
		for (const auto& [name, interval] : stopinfo.at("road_distances").AsDict()) {
			distance.insert({ name, interval.AsInt() });
		}

		return { stop, distance };
	}


	// ------------------------ Answers ------------------------ //

	// Формирование ответа на запрос Bus
	json::Node JSONReader::GetBusAnswer(const json::Dict& request) const {
		json::Builder builder;
		const auto& name = request.at("name"s).AsString();
		const auto& valid = tc_.FindRoute(name);

		builder.StartDict().Key("request_id"s).Value(request.at("id"s).AsInt());
		if (!valid) {
			builder.Key("error_message"s).Value("not found"s);
		}
		else {
			const auto& businfo = request_.GetBusStat(name);
			builder.Key("curvature"s).Value(businfo->curvature_)
				.Key("route_length"s).Value(businfo->distance_)
				.Key("stop_count"s).Value(businfo->stops_)
				.Key("unique_stop_count"s).Value(businfo->unique_stops_);
		}

		return builder.EndDict().Build();
	}

	// Формирование ответа на запрос Stop
	json::Node JSONReader::GetStopAnswer(const json::Dict& request) const {
		json::Builder builder;
		const auto& name = request.at("name"s).AsString();
		const auto& valid = tc_.FindStop(name);

		builder.StartDict().Key("request_id"s).Value(request.at("id"s).AsInt());
		if (!valid) {
			builder.Key("error_message"s).Value("not found"s);
		}
		else {
			json::Array set = {};
			std::vector<std::string> forSort = {};
			const auto& businfo = request_.GetBusesByStop(name);
			for (const auto& bus : businfo) {
				forSort.push_back(bus->number_);
			}
			std::sort(forSort.begin(), forSort.end());

			for (const auto& fS : forSort) {
				set.push_back(fS);
			}

			builder.Key("buses"s).Value(set);
		}

		return builder.EndDict().Build();
	}

	// Формирование ответа на запрос Map
	json::Node JSONReader::GetMapAnswer(const json::Dict& request) const {
		json::Builder builder;
		builder.StartDict().Key("request_id"s).Value(request.at("id"s).AsInt());

		std::ostringstream oss;
		RendererMap(oss);

		builder.Key("map").Value(oss.str());
		return builder.EndDict().Build();
	}

	// Формирование ответа на запрос Route
	json::Node JSONReader::GetRouteAnswer(const json::Dict& request) const {
		json::Builder builder;
		const auto& from = request.at("from"s).AsString();
		const auto& to = request.at("to"s).AsString();
		const auto& wait_time = router_.GetSettings().bus_wait_time_;
		const auto& response = router_.MakeRouteResponse(from, to);

		builder.StartDict().Key("request_id"s).Value(request.at("id"s).AsInt());
		if (!response.has_value()) {
			builder.Key("error_message"s).Value("not found"s);
		}
		else {
			builder.Key("total_time"s).Value(response->total_time_).Key("items"s).StartArray();

			for (auto it = response->items_.begin(); it != response->items_.end(); it++) {
				builder.StartDict().Key("type"s).Value("Wait"s)
					.Key("stop_name"s).Value(std::string((*it).from_stop_))
					.Key("time"s).Value(wait_time).EndDict();

				builder.StartDict().Key("type"s).Value("Bus"s)
					.Key("bus"s).Value(std::string((*it).bus_name_))
					.Key("time"s).Value((*it).time_ - wait_time)
					.Key("span_count"s).Value((*it).span_count_).EndDict();
			}

			builder.EndArray();
		}

		return builder.EndDict().Build();
	}


	// ------------------------ Drawing ------------------------ //

	// Парсинг и заполнения данных для отрисовки
	mr::RenderSettings JSONReader::ParseMapSettings(const json::Dict& request) const {
		mr::RenderSettings settings = {};
		settings.width = request.at("width"s).AsDouble();
		settings.height = request.at("height"s).AsDouble();
		settings.padding = request.at("padding"s).AsDouble();
		settings.stop_radius = request.at("stop_radius"s).AsDouble();
		settings.line_width = request.at("line_width"s).AsDouble();
		settings.bus_label_font_size = request.at("bus_label_font_size"s).AsInt();
		settings.bus_label_offset = { request.at("bus_label_offset"s).AsArray()[0].AsDouble(), 
			request.at("bus_label_offset"s).AsArray()[1].AsDouble() };
		settings.stop_label_font_size = request.at("stop_label_font_size"s).AsInt();
		settings.stop_label_offset = { request.at("stop_label_offset"s).AsArray()[0].AsDouble(), 
			request.at("stop_label_offset"s).AsArray()[1].AsDouble() };
		settings.underlayer_color = GetColor(request.at("underlayer_color"));
		settings.underlayer_width = request.at("underlayer_width"s).AsDouble();
		for (const auto& palette : request.at("color_palette").AsArray()) {
			settings.color_palette.push_back(GetColor(palette));
		}

		return settings;
	}

	// Возвращаем структуру Color
	svg::Color JSONReader::GetColor(const json::Node& node) const  {
		if (node.IsArray()) {
			if (node.AsArray().size() == 3) {
				svg::Rgb rgb ( node.AsArray()[0].AsInt(), node.AsArray()[1].AsInt(),
					node.AsArray()[2].AsInt() );
				return rgb;
			}
			else if (node.AsArray().size() == 4) {
				svg::Rgba rgba( node.AsArray()[0].AsInt(), node.AsArray()[1].AsInt(),
					node.AsArray()[2].AsInt(), node.AsArray()[3].AsDouble() );
				return rgba;
			}
		}

		return node.AsString();
	}

	// Отрисовка маршрутов
	void JSONReader::RendererMap(std::ostream& output) const {
		const auto& routes = request_.GetAllRoutes();
		render_.SetProjector(routes);
		render_.RendererPolyline(routes);
		render_.RendererRouteName(routes);
		render_.RendererStopSymbols(request_.GetAllUniqueStops());
		render_.RendererStopNames(request_.GetAllUniqueStops());
		render_.RendererXML(output);
	}

} // namespace reading_queries

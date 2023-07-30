#include "json_reader.h"


namespace json_reader {

	JSONReader::JSONReader(tc::TransportCatalogue& tc, rh::RequestHandler& rh, 
		mr::MapRenderer& mr, tr::TransportRouter& tr, sr::Serialization& ser, 
		sr::Deserialization deser, std::istream& input, std::ostream& output)
		: tc_(tc)
		, request_(rh)
		, render_(mr)
		, router_(tr)
		, ser_(ser)
		, deser_(deser)
		, doc_(json::Load(input))
		, output_(output)
	{}


	// ------------------------ Parsing ------------------------ //
	
	// Парсинг входящего запроса - заполнение базы / сериализация
	void JSONReader::MakeBase() const {
		const auto& load = doc_.GetRoot().AsDict();

		if (load.count("base_requests"s)) {
			const auto& base = load.at("base_requests"s).AsArray();

			for (const auto& request : base) {
				const auto& type = request.AsDict().at("type"s).AsString();

				if (type == "Bus"s) {
					tc_.UpdateRoute(ParseBus(request.AsDict()));
				}

				if (type == "Stop"s) {
					const auto& stopInfo = ParseStop(request.AsDict());
					tc_.UpdateStop(stopInfo.first);
					if (!stopInfo.second.empty()) {
						tc_.UpdateStopDistance(stopInfo.first.name_, stopInfo.second);
					}
				}
			}
		}

		if (load.count("render_settings"s)) {
			const auto& render = load.at("render_settings"s).AsDict();
			render_.SetSettings(ParseMapSettings(render));
		}

		if (load.count("routing_settings")) {
			const auto& routing = load.at("routing_settings"s).AsDict();

			domain::RouteSettings settings = {};
			settings.bus_velocity_ = routing.at("bus_velocity"s).AsDouble();
			settings.bus_wait_time_ = routing.at("bus_wait_time"s).AsDouble();
			router_.SetSettings(settings);
		}

		if (load.count("serialization_settings"s)) {
			const auto& ser = load.at("serialization_settings"s).AsDict();
			ser_.SetPath(ser.at("file"s).AsString());
			ser_.SerializeTransportCatalogue();
		}
	}

	// Парсинг входящего запроса - ответ из базы / десериализация
	void JSONReader::ProcessRequests() const {
		const auto& load = doc_.GetRoot().AsDict();

		if (load.count("serialization_settings"s)) {
			const auto& deser = load.at("serialization_settings"s).AsDict();
			deser_.SetPath(deser.at("file"s).AsString());
			deser_.DeserializeTransportCatalogue();
		}

		if (load.count("stat_requests"s)) {
			const auto& stat = load.at("stat_requests"s).AsArray();
			router_.BuildGraph();
			json::Array answer = {};
			json::Builder builder {};

			for (const auto& request : stat) {
				const auto& type = request.AsDict().at("type"s).AsString();

				if (type == "Bus"s) {
					const auto& busAnswer = GetBusAnswer(request.AsDict());
					answer.emplace_back(busAnswer);
				}

				if (type == "Stop"s) {
					const auto& stopAnswer = GetStopAnswer(request.AsDict());
					answer.emplace_back(stopAnswer);
				}

				if (type == "Map"s) {
					const auto& mapAnswer = GetMapAnswer(request.AsDict());
					answer.emplace_back(mapAnswer);
				}

				if (type == "Route"s) {
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
	domain::BusRoute JSONReader::ParseBus(const json::Dict& businfo) const {
		domain::BusRoute route = {};
		route.number_ = businfo.at("name"s).AsString();
		route.circle = businfo.at("is_roundtrip"s).AsBool();
		const auto& stops = businfo.at("stops"s).AsArray();
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
	std::pair<domain::BusStop, DistancePair> JSONReader::ParseStop(const json::Dict& stopinfo) const {
		domain::BusStop stop = {};
		DistancePair distance = {};
		stop.name_ = stopinfo.at("name"s).AsString();
		stop.coordinates_.lat = stopinfo.at("latitude"s).AsDouble();
		stop.coordinates_.lng = stopinfo.at("longitude"s).AsDouble();
		
		for (const auto& [name, interval] : stopinfo.at("road_distances"s).AsDict()) {
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

		builder.Key("map"s).Value(oss.str());
		return builder.EndDict().Build();
	}

	// Формирование ответа на запрос Route
	json::Node JSONReader::GetRouteAnswer(const json::Dict& request) const {
		json::Builder builder;
		const auto& from = request.at("from"s).AsString();
		const auto& to = request.at("to"s).AsString();
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
					.Key("time"s).Value((*it).wait_time_).EndDict();
				
				builder.StartDict().Key("type"s).Value("Bus"s)
					.Key("bus"s).Value(std::string((*it).bus_name_))
					.Key("time"s).Value((*it).route_time_ - (*it).wait_time_)
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
		settings.underlayer_color = GetColor(request.at("underlayer_color"s));
		settings.underlayer_width = request.at("underlayer_width"s).AsDouble();
		for (const auto& palette : request.at("color_palette"s).AsArray()) {
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

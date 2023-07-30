#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"
#include "request_handler.h"
#include "map_renderer.h"
#include "json_builder.h"
#include "transport_router.h"
#include "serialization.h"

#include <sstream>


namespace tc = transport_catalogue;
namespace rh = request_handler;
namespace mr = map_renderer;
namespace tr = transport_router;
namespace sr = serialization;


namespace json_reader {

	class JSONReader {
	public:
		JSONReader() = default;
		JSONReader(tc::TransportCatalogue& tc, rh::RequestHandler& rh,
			mr::MapRenderer& mr, tr::TransportRouter& tr, sr::Serialization& ser,
			sr::Deserialization deser, std::istream& input = std::cin, std::ostream& output = std::cout);

		void MakeBase() const;
		void ProcessRequests() const;
		BusRoute ParseBus(const json::Dict& businfo) const;
		std::pair<BusStop, DistancePair> ParseStop(const json::Dict& stopinfo) const;

		json::Node GetBusAnswer(const json::Dict& request) const;
		json::Node GetStopAnswer(const json::Dict& request) const;
		json::Node GetMapAnswer(const json::Dict& request) const;
		json::Node GetRouteAnswer(const json::Dict& request) const;
		
		mr::RenderSettings ParseMapSettings(const json::Dict& request) const;
		svg::Color GetColor(const json::Node& node) const;
		void RendererMap(std::ostream& output) const;

	private:
		tc::TransportCatalogue& tc_;
		rh::RequestHandler& request_;
		mr::MapRenderer& render_;
		tr::TransportRouter& router_;
		sr::Serialization& ser_;
		sr::Deserialization& deser_;
		const json::Document doc_;
		std::ostream& output_;
	};

} // namespace reading_queries
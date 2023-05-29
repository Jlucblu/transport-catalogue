#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"
#include "request_handler.h"
#include "map_renderer.h"


namespace tc = transport_catalogue;
namespace rh = request_handler;
namespace mr = map_renderer;


namespace json_reader {

	class JSONReader {
	public:
		JSONReader() = default;
		JSONReader(tc::TransportCatalogue& tc, rh::RequestHandler& rh, mr::MapRender& mr, std::istream& input, std::ostream& output);

		void ParseBaseRequest() const;
		BusRoute ParseBus(const json::Dict& businfo) const;
		std::pair<BusStop, DistancePair> ParseStop(const json::Dict& stopinfo) const;

		json::Dict GetBusAnswer(const json::Dict& request) const;
		json::Dict GetStopAnswer(const json::Dict& request) const;
		

		mr::RenderSettings ParseMapSettings(const json::Dict& request) const;
		svg::Color GetColor(const json::Node& node) const;

	private:
		tc::TransportCatalogue& tc_;
		mr::MapRender& render_;
		rh::RequestHandler& request_;
		const json::Document doc_;
		std::ostream& output_;
	};

} // namespace reading_queries
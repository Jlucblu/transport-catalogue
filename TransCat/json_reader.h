#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"
#include "request_handler.h"

using namespace json;
using namespace transport_catalogue;
using namespace request_handler;

namespace json_reader {

	class JSONReader {
	public:
		JSONReader() = default;
		JSONReader(TransportCatalogue& tc, std::istream& input, std::ostream& output);

		void ParseBaseRequest() const;
		BusRoute ParseBus(const json::Dict& businfo) const;
		std::pair<BusStop, DistancePair> ParseStop(const json::Dict& stopinfo) const;

		Dict GetBusAnswer(const Dict& request) const;
		Dict GetStopAnswer(const Dict& request) const;

	private:
		TransportCatalogue& tc_;
		RequestHandler request_;
		const Document doc_;
		std::ostream& output_;
	};

} // namespace reading_queries
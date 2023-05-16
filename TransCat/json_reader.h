#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"

using namespace json;
using namespace transport_catalogue;

namespace reading_queries {
	class JSONReader {
	public:
		JSONReader() = default;
		JSONReader(TransportCatalogue& tc);
		//JSONReader(const Document& doc, TransportCatalogue& tc);

		void ParseBaseRequest(const Document& doc);

		BusRoute ParseBus(const json::Dict& businfo);
		BusStop ParseStop(const json::Dict& businfo);


	private:
		//const Document& doc_;
		TransportCatalogue& tc_;
	};

} // namespace reading_queries
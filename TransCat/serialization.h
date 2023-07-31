#pragma once
#include "transport_catalogue.h"
#include "transport_router.h"
#include "map_renderer.h"
#include "domain.h"

#include <transport_catalogue.pb.h>
#include <filesystem>
#include <fstream>


namespace tc = transport_catalogue;
namespace mr = map_renderer;
namespace tr = transport_router;

namespace serialization {

    class Serialization {
    public:
        Serialization(tc::TransportCatalogue& tc, mr::MapRenderer& mr, tr::TransportRouter& tr);

        // ----------- Serialize -----------

        void SerializeTransportCatalogue();
        void SetPath(const std::filesystem::path& path);
        void SerializeStops();
        void SerializeBuses();
        void SerializeDistance();
        void SerializeMapSettings();
        serializator::Color SerializedColor(const svg::Color color);
        void SerializeRoute();


        // ----------- Deserialize -----------

        void DeserializeTransportCatalogue();
        void DeserializeStops();
        void DeserializeBuses();
        void DeserializeDistance();
        void DeserializeMapSettings();
        svg::Color DeserializedColor(const serializator::Color& color);
        void DeserializeRoute();

    private:
        tc::TransportCatalogue& catalogue_;
        tr::TransportRouter& router_;
        mr::MapRenderer& map_render_;
        serializator::Base base_;
        std::filesystem::path path_;
    };

} // namespace serialization
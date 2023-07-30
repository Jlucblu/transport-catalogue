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
        Serialization(const tc::TransportCatalogue& tc, const mr::MapRenderer& mr, const tr::TransportRouter& tr)
            : catalogue_(tc)
            , map_render_(mr)
            , router_(tr) {}

        void SerializeTransportCatalogue() {
            std::ofstream out_file(path_, std::ios::binary);
            SerializeStops();
            SerializeBuses();
            SerializeDistance();
            base_.SerializePartialToOstream(&out_file);
        }

        void SetPath(const std::filesystem::path& path) {
            path_ = path;
        }

        void SerializeStops() {
            const auto& all_stops = catalogue_.GetStopList();
            for (const auto& stop : all_stops) {
                auto* new_stop = base_.add_stops();
                new_stop->set_name(stop.name_);
                auto* coordinates = new_stop->mutable_coordinates();
                coordinates->set_latitude(stop.coordinates_.lat);
                coordinates->set_longitude(stop.coordinates_.lng);
            }
        }

        void SerializeBuses() {
            const auto& all_buses = catalogue_.GetBusList();
            for (const auto& bus : all_buses) {
                auto* new_bus = base_.add_buses();
                new_bus->set_number(bus.number_);
                new_bus->set_circle(bus.circle);
                for (const auto& stop : bus.stops_) {
                    auto* new_stop = new_bus->add_stops();
                    new_stop->set_name(stop->name_);
                }
            }
        }

        void SerializeDistance() {
            const auto& distance_map = catalogue_.GetDistanceList();
            for (const auto& [stops, distance] : distance_map) {
                auto* new_distance_map = base_.add_stop_distance_map();
                new_distance_map->set_from_stop(stops.first->name_);
                new_distance_map->set_to_stop(stops.second->name_);
                new_distance_map->set_distance(distance);
            }
        }

    private:
        const tc::TransportCatalogue& catalogue_;
        const tr::TransportRouter& router_;
        const mr::MapRenderer& map_render_;
        serializator::Base base_;
        std::filesystem::path path_;
    };

    class Deserialization {
    public:
        Deserialization(tc::TransportCatalogue& tc, mr::MapRenderer& mr, tr::TransportRouter& tr)
            : catalogue_(tc)
            , map_render_(mr)
            , router_(tr) {}

        void SetPath(const std::filesystem::path& path) {
            path_ = path;
        }

        void DeserializeTransportCatalogue() {
            std::ifstream in_file(path_, std::ios::binary);
            base_.ParseFromIstream(&in_file);
            DeserializeStops();
            DeserializeBuses();
            DeserializeDistance();
        }

        void DeserializeStops() {
            for (int i = 0; i < base_.stops_size(); ++i) {
                auto& stop = base_.stops(i);
                catalogue_.UpdateStop({ stop.name(), { stop.coordinates().latitude(), stop.coordinates().longitude()} });
            }
        }

        void DeserializeBuses() {
            for (int i = 0; i < base_.buses_size(); ++i) {
                auto& bus = base_.buses(i);
                domain::BusRoute route = {};
                route.number_ = bus.number();
                route.circle = bus.circle();
                for (int j = 0; j < bus.stops_size(); ++j) {
                    domain::BusStop* found = catalogue_.FindStop(bus.stops()[j].name());
                    if (found == nullptr) {
                        catalogue_.UpdateStop({ bus.stops()[j].name(), {0.0, 0.0} });
                    }

                    route.stops_.push_back(catalogue_.FindStop(bus.stops()[j].name()));
                }

                catalogue_.UpdateRoute(route);
            }
        }

        void DeserializeDistance() {
            for (int i = 0; i < base_.stop_distance_map_size(); ++i) {
                auto& distance_map = base_.stop_distance_map(i);
                std::string_view from = catalogue_.FindStop(distance_map.from_stop())->name_;
                std::string_view to = catalogue_.FindStop(distance_map.to_stop())->name_;
                DistancePair pair = {};
                pair.insert({ to, distance_map.distance() });
                catalogue_.UpdateStopDistance(from, pair);
            }
        }

    private:
        tc::TransportCatalogue& catalogue_;
        tr::TransportRouter& router_;
        mr::MapRenderer& map_render_;
        serializator::Base base_;
        std::filesystem::path path_;
    };

} // namespace serialization
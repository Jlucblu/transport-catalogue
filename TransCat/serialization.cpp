#include "serialization.h"


namespace serialization {

    Serialization::Serialization(tc::TransportCatalogue& tc, mr::MapRenderer& mr, tr::TransportRouter& tr)
        : catalogue_(tc)
        , map_render_(mr)
        , router_(tr) {}

    // ----------- Serialize -----------

    void Serialization::SerializeTransportCatalogue() {
        std::ofstream out_file(path_, std::ios::binary);
        SerializeStops();
        SerializeBuses();
        SerializeDistance();
        SerializeMapSettings();
        SerializeRoute();
        base_.SerializePartialToOstream(&out_file);
    }

    void Serialization::SetPath(const std::filesystem::path& path) {
        path_ = path;
    }

    void Serialization::SerializeStops() {
        const auto& all_stops = catalogue_.GetStopList();
        for (const auto& stop : all_stops) {
            auto* new_stop = base_.add_stops();
            new_stop->set_name(stop.name_);
            auto* coordinates = new_stop->mutable_coordinates();
            coordinates->set_latitude(stop.coordinates_.lat);
            coordinates->set_longitude(stop.coordinates_.lng);
        }
    }

    void Serialization::SerializeBuses() {
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

    void Serialization::SerializeDistance() {
        const auto& distance_map = catalogue_.GetDistanceList();
        for (const auto& [stops, distance] : distance_map) {
            auto* new_distance_map = base_.add_stop_distance_map();
            new_distance_map->set_from_stop(stops.first->name_);
            new_distance_map->set_to_stop(stops.second->name_);
            new_distance_map->set_distance(distance);
        }
    }

    void Serialization::SerializeMapSettings() {
        const auto& settings = map_render_.GetSettings();
        auto* render_settings = base_.mutable_map_settings();

        render_settings->set_width(settings.width);
        render_settings->set_height(settings.height);
        render_settings->set_padding(settings.padding);
        render_settings->set_line_width(settings.line_width);
        render_settings->set_stop_radius(settings.stop_radius);
        render_settings->set_bus_label_font_size(settings.bus_label_font_size);
        render_settings->mutable_bus_label_offset()->set_x(settings.bus_label_offset.x);
        render_settings->mutable_bus_label_offset()->set_y(settings.bus_label_offset.y);
        render_settings->set_stop_label_font_size(settings.stop_label_font_size);
        render_settings->mutable_stop_label_offset()->set_x(settings.stop_label_offset.x);
        render_settings->mutable_stop_label_offset()->set_y(settings.stop_label_offset.y);
        render_settings->set_underlayer_width(settings.underlayer_width);
        *render_settings->mutable_underlayer_color() = SerializedColor(settings.underlayer_color);

        for (const auto& color : settings.color_palette) {
            *render_settings->add_color_palette() = SerializedColor(color);
        }
    }

    serializator::Color Serialization::SerializedColor(const svg::Color color) {
        serializator::Color serialized_color;

        if (std::holds_alternative<std::string>(color)) {
            serialized_color.set_name(std::get<std::string>(color));
        }
        else if (std::holds_alternative<svg::Rgb>(color)) {
            svg::Rgb rgb = std::get<svg::Rgb>(color);
            serialized_color.mutable_rgb()->set_red(rgb.red);
            serialized_color.mutable_rgb()->set_green(rgb.green);
            serialized_color.mutable_rgb()->set_blue(rgb.blue);
        }
        else if (std::holds_alternative<svg::Rgba>(color)) {
            svg::Rgba rgba = std::get<svg::Rgba>(color);
            serialized_color.mutable_rgba()->set_red(rgba.red);
            serialized_color.mutable_rgba()->set_green(rgba.green);
            serialized_color.mutable_rgba()->set_blue(rgba.blue);
            serialized_color.mutable_rgba()->set_opacity(rgba.opacity);
        }

        return serialized_color;
    }

    void Serialization::SerializeRoute() {
        const auto& settings = router_.GetSettings();
        auto* route_settings = base_.mutable_route_settings();
        route_settings->set_bus_wait_time(settings.bus_wait_time_);
        route_settings->set_bus_velocity(settings.bus_velocity_);
    }


    // ----------- Deserialize -----------

    void Serialization::DeserializeTransportCatalogue() {
        std::ifstream in_file(path_, std::ios::binary);
        base_.ParseFromIstream(&in_file);
        DeserializeStops();
        DeserializeBuses();
        DeserializeDistance();
        DeserializeMapSettings();
        DeserializeRoute();
    }

    void Serialization::DeserializeStops() {
        for (int i = 0; i < base_.stops_size(); ++i) {
            auto& stop = base_.stops(i);
            domain::BusStop update_stop = {};
            update_stop.name_ = stop.name();
            update_stop.coordinates_.lat = stop.coordinates().latitude();
            update_stop.coordinates_.lng = stop.coordinates().longitude();
            catalogue_.UpdateStop(update_stop);
        }
    }

    void Serialization::DeserializeBuses() {
        for (int i = 0; i < base_.buses_size(); ++i) {
            const auto& bus = base_.buses(i);
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

    void Serialization::DeserializeDistance() {
        for (int i = 0; i < base_.stop_distance_map_size(); ++i) {
            const auto& distance_map = base_.stop_distance_map(i);
            std::string_view from = catalogue_.FindStop(distance_map.from_stop())->name_;
            std::string_view to = catalogue_.FindStop(distance_map.to_stop())->name_;
            DistancePair pair = {};
            pair.insert({ to, distance_map.distance() });
            catalogue_.UpdateStopDistance(from, pair);
        }
    }

    void Serialization::DeserializeMapSettings() {
        const auto& map_settings = base_.map_settings();
        map_renderer::RenderSettings settings = {};

        settings.width = map_settings.width();
        settings.height = map_settings.height();
        settings.padding = map_settings.padding();
        settings.stop_radius = map_settings.stop_radius();
        settings.line_width = map_settings.line_width();
        settings.bus_label_font_size = map_settings.bus_label_font_size();
        settings.bus_label_offset.x = map_settings.bus_label_offset().x();
        settings.bus_label_offset.y = map_settings.bus_label_offset().y();
        settings.stop_label_font_size = map_settings.stop_label_font_size();
        settings.stop_label_offset.x = map_settings.stop_label_offset().x();
        settings.stop_label_offset.y = map_settings.stop_label_offset().y();
        settings.underlayer_width = map_settings.underlayer_width();
        settings.underlayer_color = DeserializedColor(map_settings.underlayer_color());

        for (int i = 0; i < map_settings.color_palette_size(); ++i) {
            settings.color_palette.push_back(DeserializedColor(map_settings.color_palette(i)));
        }

        map_render_.SetSettings(settings);
    }

    svg::Color Serialization::DeserializedColor(const serializator::Color& color) {
        svg::Color deserialized_color;

        if (color.color_case() == serializator::Color::kName) {
            deserialized_color = color.name();
        }
        else if (color.color_case() == serializator::Color::kRgb) {
            const auto& rgb = color.rgb();
            deserialized_color = svg::Rgb(rgb.red(), rgb.green(), rgb.blue());
        }
        else if (color.color_case() == serializator::Color::kRgba) {
            const auto& rgba = color.rgba();
            deserialized_color = svg::Rgba(rgba.red(), rgba.green(), rgba.blue(), rgba.opacity());
        }

        return deserialized_color;
    }

    void Serialization::DeserializeRoute() {
        domain::RouteSettings settings;
        const auto& route_settings = base_.route_settings();
        settings.bus_wait_time_ = route_settings.bus_wait_time();
        settings.bus_velocity_ = route_settings.bus_velocity();
        router_.SetSettings(settings);
    }

} // namespace serialization
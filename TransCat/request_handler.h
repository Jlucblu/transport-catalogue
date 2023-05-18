#pragma once

#include <optional>

#include "transport_catalogue.h"
#include "domain.h"


using namespace transport_catalogue;


namespace request_handler {

    class RequestHandler {
    public:
        // MapRenderer понадобится в следующей части итогового проекта
        RequestHandler(TransportCatalogue& tc);
        //RequestHandler(const TransportCatalogue& db, const renderer::MapRenderer& renderer);

        // Возвращает информацию о маршруте (запрос Bus)
        std::optional<RouteInfo> GetBusStat(const std::string_view& bus_name) const;

        // Возвращает маршруты, проходящие через
        std::unordered_set<BusRoute*> GetBusesByStop(const std::string_view& stop_name) const;

        // Этот метод будет нужен в следующей части итогового проекта
        //svg::Document RenderMap() const;

    private:
        // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
        TransportCatalogue& tc_;
        //const renderer::MapRenderer& renderer_;
    };

} // namespace request_handler
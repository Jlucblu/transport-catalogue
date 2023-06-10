#pragma once

#include "transport_catalogue.h"
#include "domain.h"
#include "map_renderer.h"

#include <optional>


namespace tc = transport_catalogue;
namespace mr = map_renderer;


namespace request_handler {

    class RequestHandler {
    public:
        RequestHandler() = default;
        RequestHandler(tc::TransportCatalogue& tc);

        // Возвращает информацию о маршруте (запрос Bus)
        std::optional<RouteInfo> GetBusStat(const std::string_view& bus_name) const;

        // Возвращает маршруты, проходящие через остановку
        std::unordered_set<BusRoute*> GetBusesByStop(const std::string_view& stop_name) const;

        // Уникальные остановки на маршруте с сохранением порядка
        std::vector<BusStop*> MakeUniqueStops(const std::vector<BusStop*> stops) const;

        // Получение маршрута с уникальными остановками
        std::vector<BusRoute*> GetRoutesWithUniqueStops(const std::vector<BusRoute*> routes) const;

        // Получение всех уникальных остановок на всех маршрутах отсортированных в лексографическом порядке
        std::vector<BusStop*> GetAllUniqueStops() const;

        // Получение всех маршрутов, отсортированных в лексографическом порядке
        std::vector<BusRoute*> GetAllRoutes() const;

    private:
        tc::TransportCatalogue& tc_;
    };

} // namespace request_handler
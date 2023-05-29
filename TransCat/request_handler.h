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

        // Возвращает маршруты, проходящие через
        std::unordered_set<BusRoute*> GetBusesByStop(const std::string_view& stop_name) const;

        // Получение всех маршрутов
        std::vector<BusRoute*> GetRoutes() const;

    private:
        tc::TransportCatalogue& tc_;
    };

} // namespace request_handler
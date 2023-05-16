#pragma once
#include "transport_catalogue.h"

#include <sstream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>


namespace transport_catalogue {
    namespace input_handler {
        // Ввод запросов
        std::string ReadLine(std::istream& in);
        // Парсинг ключа
        std::string_view ParseKey(std::string_view str);
        // Парсинг маршрута автобуса
        std::deque<std::string> ParseRoute(std::string_view, bool);
        // Парсинг остановки и координат
        std::pair <BusStop, DistancePair> ParseStop(std::string_view str);
        // Обновление каталога
        void UpdateCatalogue(std::istream& in, TransportCatalogue& cat);
    }
}
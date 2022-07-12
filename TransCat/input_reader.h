#pragma once
#include "transport_catalogue.h"
#include <sstream>

struct InputRoute {
    std::string name;
    std::vector<BusStop> stops;
    bool circle;
};

// Ввод запросов
std::string ReadLine(std::istream& in);
// Парсинг ключа
std::string_view ParseKey(std::string_view request);
// Парсинг маршрута автобуса
std::vector<std::string> ParseRoute(std::string& str, bool);
// Парсинг остановки и координат
BusStop ParseStop(std::string& str);
// Обновление каталога
std::istream& UpdateCat(std::istream& in, TransportCatalogue& cat);
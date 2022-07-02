#pragma once
#include "transport_catalogue.h"
#include <sstream>


struct InputRoute {
    std::string name;
    std::vector<std::string> stops;
};

struct InputStops {
    std::string name;
    Coordinates coordinates{ 0, 0 };
};

// Ввод запросов
std::string ReadLine(std::istream& in);
// Парсинг ключа
std::string_view ParseKey(std::string_view request);
// Парсинг остановки и координат
InputStops ParseStop(std::string& str);
// Парсинг маршрута автобуса
InputRoute ParseRoute(std::string& str);
// Обновление каталога
std::istream& UpdateCat(std::istream& in, TransportCatalogue& cat);
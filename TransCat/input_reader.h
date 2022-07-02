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

// ���� ��������
std::string ReadLine(std::istream& in);
// ������� �����
std::string_view ParseKey(std::string_view request);
// ������� ��������� � ���������
InputStops ParseStop(std::string& str);
// ������� �������� ��������
InputRoute ParseRoute(std::string& str);
// ���������� ��������
std::istream& UpdateCat(std::istream& in, TransportCatalogue& cat);
#pragma once
#include "transport_catalogue.h"
#include <sstream>

struct InputRoute {
    std::string name;
    std::vector<BusStop> stops;
    bool circle;
};

// ���� ��������
std::string ReadLine(std::istream& in);
// ������� �����
std::string_view ParseKey(std::string_view request);
// ������� �������� ��������
std::vector<std::string> ParseRoute(std::string& str, bool);
// ������� ��������� � ���������
BusStop ParseStop(std::string& str);
// ���������� ��������
std::istream& UpdateCat(std::istream& in, TransportCatalogue& cat);
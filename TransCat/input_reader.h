#pragma once
#include "transport_catalogue.h"

#include <sstream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>


// ���� ��������
std::string ReadLine(std::istream& in);
// ������� �����
std::string_view ParseKey(std::string_view str);
// ������� �������� ��������
std::vector<std::string> ParseRoute(const std::string& str, bool);
// ������� ��������� � ���������
std::pair <BusStop, DistancePair> ParseStop(const std::string& str);
// ���������� ��������
std::istream& UpdateCat(std::istream& in, TransportCatalogue& cat);
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
std::deque<std::string> ParseRoute(std::string_view, bool);
// ������� ��������� � ���������
std::pair <BusStop, DistancePair> ParseStop(std::string_view str);
// ���������� ��������
std::istream& UpdateCat(std::istream& in, TransportCatalogue& cat);
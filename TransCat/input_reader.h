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
        // ���� ��������
        std::string ReadLine(std::istream& in);
        // ������� �����
        std::string_view ParseKey(std::string_view str);
        // ������� �������� ��������
        std::deque<std::string> ParseRoute(std::string_view, bool);
        // ������� ��������� � ���������
        std::pair <BusStop, DistancePair> ParseStop(std::string_view str);
        // ���������� ��������
        void UpdateCatalogue(std::istream& in, TransportCatalogue& cat);
    }
}
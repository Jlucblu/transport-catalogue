#pragma once
#include "transport_catalogue.h"
#include "input_reader.h"

#include <sstream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <set>
#include <algorithm>

namespace transport_catalogue {
    namespace statistics {
        void PrintInfo(std::istream& in, std::ostream& out, TransportCatalogue& cat);
        void PrintBusInfo(const std::string& str, std::ostream& out, TransportCatalogue& cat);
        void PrintStopInfo(const std::string& str, std::ostream& out, TransportCatalogue& cat);
    }
}
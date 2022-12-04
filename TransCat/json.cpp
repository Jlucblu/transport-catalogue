#include "json.h"

using namespace std;

namespace json {

    namespace {

        string ParseWord(istream& input) {
            string str;
            for (char ch; input >> ch;) {
                if (ch == input.eof() || ch == ',' || ch == ']' || ch == '}') {
                    input.putback(ch);
                    break;
                }
                str += ch;
            }
            return str;
        }

        Node LoadNode(istream& input);

        Node LoadArray(istream& input) {
            Array result;
            if (input.peek() == -1) {
                throw ParsingError("Array parsing error");
            }

            for (char ch; input >> ch && ch != ']';) {
                if (ch != ',') {
                    input.putback(ch);
                }
                result.push_back(LoadNode(input));
            }

            return Node(move(result));
        }

        using Number = std::variant<int, double>;
        Node LoadNumber(istream& input) {
            using namespace std::literals;

            std::string parsed_num;

            auto read_char = [&parsed_num, &input] {
                parsed_num += static_cast<char>(input.get());
                if (!input) {
                    throw ParsingError("Failed to read number from stream"s);
                }
            };

            auto read_digits = [&input, read_char] {
                if (!std::isdigit(input.peek())) {
                    throw ParsingError("A digit was expected");
                }
                while (std::isdigit(input.peek())) {
                    read_char();
                }
            };

            if (input.peek() == '-') {
                read_char();
            }

            if (input.peek() == '0') {
                read_char();
            }
            else {
                read_digits();
            }

            bool is_int = true;
            if (input.peek() == '.') {
                read_char();
                read_digits();
                is_int = false;
            }

            if (int ch = input.peek(); ch == 'e' || ch == 'E') {
                read_char();
                if (ch = input.peek(); ch == '+' || ch == '-') {
                    read_char();
                }
                read_digits();
                is_int = false;
            }

            try {
                if (is_int) {
                    try {
                        return Node(std::stoi(parsed_num));
                    }
                    catch (...) {

                    }
                }
                return Node(std::stod(parsed_num));
            }
            catch (...) {
                throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
            }

        }

        Node LoadString(istream& input) {
            using namespace std::literals;

            auto it = std::istreambuf_iterator<char>(input);
            auto end = std::istreambuf_iterator<char>();

            std::string s;
            while (true) {
                if (it == end) {
                    throw ParsingError("String parsing error"s);
                }
                const char ch = *it;
                if (ch == '"') {
                    ++it;
                    break;
                }
                else if (ch == '\\') {
                    ++it;
                    if (it == end) {
                        throw ParsingError("String parsing error"s);
                    }
                    const char escaped_char = *it;
                    switch (escaped_char) {
                    case 'n':
                        s.push_back('\n');
                        break;
                    case 't':
                        s.push_back('\t');
                        break;
                    case 'r':
                        s.push_back('\r');
                        break;
                    case '"':
                        s.push_back('"');
                        break;
                    case '\\':
                        s.push_back('\\');
                        break;
                    default:
                        throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
                    }
                }
                else if (ch == '\n' || ch == '\r') {
                    throw ParsingError("Unexpected end of line"s);
                }
                else {
                    s.push_back(ch);
                }
                ++it;
            }

            return Node(s);
        }

        Node LoadDict(istream& input) {
            Dict result;
            if (input.peek() == -1) {
                throw ParsingError("Dict parsing error"s);
            }

            for (char ch; input >> ch && ch != '}';) {
                if (ch == ',') {
                    input >> ch;
                }

                string key = LoadString(input).AsString();
                input >> ch;
                result.insert({ move(key), LoadNode(input) });
            }

            return Node(move(result));
        }

        Node LoadBool(istream& input) {
            string str = ParseWord(input);

            if (str == "true"s) {
                return Node(true);
            }
            else if (str == "false"s) {
                return Node(false);
            }
            else {
                throw ParsingError("Couldn't parse bool value"s);
            }
        }

        Node LoadNull(istream& input) {
            string str = ParseWord(input);

            if (str == "null"s) {
                return Node(nullptr);
            }
            else {
                throw ParsingError("Couldn't parse null value");
            }
        }

        Node LoadNode(istream& input) {
            char ch;
            input >> ch;

            if (ch == '[') {
                return LoadArray(input);
            }
            else if (ch == '{') {
                return LoadDict(input);
            }
            else if (ch == '"') {
                return LoadString(input);
            }
            else if (ch == 'n') {
                input.putback(ch);
                return LoadNull(input);
            }
            else if (ch == 't' || ch == 'f') {
                input.putback(ch);
                return LoadBool(input);
            }
            else {
                input.putback(ch);
                return LoadNumber(input);
            }

        }

    }  // namespace

    Node::Node(Array array)
        : data_(move(array))
    {}

    Node::Node(Dict map)
        : data_(move(map))
    {}

    Node::Node(int value)
        : data_(value)
    {}

    Node::Node(string value)
        : data_(move(value))
    {}

    Node::Node(double value)
        : data_(value)
    {}

    Node::Node(bool value)
        : data_(value)
    {}

    Node::Node(std::nullptr_t)
        : data_(nullptr)
    {}

    bool Node::IsInt() const {
        return holds_alternative<int>(data_);
    }

    bool Node::IsDouble() const {
        return holds_alternative<int>(data_) || holds_alternative<double>(data_);
    }

    bool Node::IsPureDouble() const {
        return holds_alternative<double>(data_);
    }

    bool Node::IsBool() const {
        return holds_alternative<bool>(data_);
    }

    bool Node::IsString() const {
        return holds_alternative<string>(data_);
    }

    bool Node::IsNull() const {
        return holds_alternative<nullptr_t>(data_);
    }

    bool Node::IsArray() const {
        return holds_alternative<Array>(data_);
    }

    bool Node::IsMap() const {
        return holds_alternative<Dict>(data_);
    }

    int Node::AsInt() const {
        if (!IsInt()) {
            throw logic_error("logic error");
        }
        return get<int>(data_);
    }

    bool Node::AsBool() const {
        if (!IsBool()) {
            throw logic_error("logic error");
        }
        return get<bool>(data_);
    }

    double Node::AsDouble() const {
        if (!IsDouble()) {
            throw logic_error("logic error");
        }
        try {
            return get<double>(data_);
        }
        catch (...) {
            return get<int>(data_);
        }
    }

    const string& Node::AsString() const {
        if (!IsString()) {
            throw logic_error("logic error");
        }
        return get<string>(data_);
    }

    const Array& Node::AsArray() const {
        if (!IsArray()) {
            throw logic_error("logic error");
        }
        return get<Array>(data_);
    }

    const Dict& Node::AsMap() const {
        if (!IsMap()) {
            throw logic_error("logic error");
        }
        return get<Dict>(data_);
    }

    Document::Document(Node root)
        : root_(move(root)) {
    }

    const Node& Document::GetRoot() const {
        return root_;
    }

    Document Load(istream& input) {
        return Document{ LoadNode(input) };
    }

    // ------------- Print ---------------

    void PrintContext::operator()(std::nullptr_t) {
        using namespace std::literals;
        output << "null"sv;
    }

    void PrintContext::operator()(std::string value) {
        output << '\"';
        for (char ch : value) {
            if (ch == '"') {
                output << '\\';
            }
            if (ch == '/') {
                output << '\\';
            }
            if (ch == '\\') {
                output << '\\';
            }
            if (ch == '\n') {
                output << "\\n"sv;
                continue;
            }
            if (ch == '\t') {
                output << "\t"sv;
                continue;
            }
            if (ch == '\r') {
                output << "\\r"sv;
                continue;
            }

            output << ch;
        }
        output << '"';
    }

    void PrintContext::operator()(double value) {
        output << value;
    }

    void PrintContext::operator()(int value) {
        output << value;
    }

    void PrintContext::operator()(bool value) {
        output << (value ? "true"s : "false"s);
    }

    void PrintContext::operator()(Array arr) {
        output << "["s;
        bool first = true;

        for (const Node& node : arr) {
            if (first) {
                first = false;
            }
            else {
                output << ", "s;
            }
            visit(PrintContext{ output }, node.GetValue());
        }

        output << ']';
    }

    void PrintContext::operator()(Dict map) {
        output << "{ "s;
        bool first = true;
        for (auto& [key, node] : map) {
            if (first) {
                first = false;
            }
            else {
                output << ", "s;
            }
            output << '"' << key << "\": "s;
            visit(PrintContext{ output }, node.GetValue());
        }

        output << '}';
    }

    void Print(const Document& doc, std::ostream& output) {
        visit(PrintContext{ output }, doc.GetRoot().GetValue());
    }

}  // namespace json
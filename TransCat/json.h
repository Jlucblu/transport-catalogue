#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>


namespace json {

    class Node;
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;
    using Value = std::variant<std::nullptr_t, int, double, std::string, bool, Array, Dict>;

    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node : private Value {
    public:

        using variant::variant;
        using InputValue = variant;

        Node(Node::InputValue& value) {
            this->swap(value);
        }

        bool IsInt() const;
        bool IsDouble() const;
        bool IsPureDouble() const;
        bool IsBool() const;
        bool IsString() const;
        bool IsNull() const;
        bool IsArray() const;
        bool IsMap() const;

        int AsInt() const;
        bool AsBool() const;
        double AsDouble() const;
        const std::string& AsString() const;
        const Array& AsArray() const;
        const Dict& AsMap() const;

        Value GetValue() const {
            return *this;
        }

        bool operator==(const Node& other) const {
            return *this == other;
        }

        bool operator!=(const Node& other) const {
            return (*this == other);
        }
    };

    class Document {
    public:
        Document() = default;
        explicit Document(Node root);

        const Node& GetRoot() const;
        bool operator==(const Document& other) const {
            return root_ == other.root_;
        }
        bool operator!=(const Document& other) const {
            return !(root_ == other.root_);
        }

    private:
        Node root_;
    };

    Document Load(std::istream& input);

    struct PrintContext {
        std::ostream& output;
        void operator()(std::nullptr_t);
        void operator()(std::string value);
        void operator()(double value);
        void operator()(int value);
        void operator()(bool value);
        void operator()(Array arr);
        void operator()(Dict map);
    };

    void Print(const Document& doc, std::ostream& output);

}  // namespace json
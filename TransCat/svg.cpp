#include "svg.h"

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);
    context.out << std::endl;
}

// ------------ Color ------------

Rgb::Rgb(uint8_t R, uint8_t G, uint8_t B)
    : red(R)
    , green(G)
    , blue(B) {}

Rgba::Rgba(size_t R, size_t G, size_t B, double op)
    : red(R)
    , green(G)
    , blue(B)
    , opacity(op) {}

void pColor::operator()(std::monostate) const {
    out << "none"s;
}
void pColor::operator()(const std::string& str) const {
    out << str;
}
void pColor::operator()(const Rgb& rgb) const {
    out << "rgb("s << static_cast<int>(rgb.red)
        << ","s << static_cast<int>(rgb.green)
        << ","s << static_cast<int>(rgb.blue) << ")"s;
}
void pColor::operator()(const Rgba& rgba) const {
    out << "rgba("s << static_cast<int>(rgba.red)
        << ","s << static_cast<int>(rgba.green)
        << ","s << static_cast<int>(rgba.blue)
        << ","s << rgba.opacity << ")"s;
}

std::ostream& operator<<(std::ostream& stm, const svg::Color& color) {
    std::visit(svg::pColor{ stm }, color);
    return stm;
}

// ---------- StrokeLineCap ------------------   

std::ostream& operator<<(std::ostream& stream, const StrokeLineCap& stroke_line_cap) {
    switch (stroke_line_cap) {
    case StrokeLineCap::BUTT:
        stream << "butt"sv;
        break;
    case StrokeLineCap::ROUND:
        stream << "round"sv;
        break;
    case StrokeLineCap::SQUARE:
        stream << "square"sv;
        break;
    }
    return stream;
}

// ---------- StrokeLineJoin ------------------

std::ostream& operator<<(std::ostream& stream, const StrokeLineJoin& stroke_line_join) {
    switch (stroke_line_join) {
    case StrokeLineJoin::ARCS:
        stream << "arcs"sv;
        break;
    case StrokeLineJoin::BEVEL:
        stream << "bevel"sv;
        break;
    case StrokeLineJoin::MITER:
        stream << "miter"sv;
        break;
    case StrokeLineJoin::MITER_CLIP:
        stream << "miter-clip"sv;
        break;
    case StrokeLineJoin::ROUND:
        stream << "round"sv;
        break;
    }
    return stream;
}
// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    RenderAttrs(out);
    out << "/>"sv;
}

// ---------- Polyline ------------------

// Добавляет очередную вершину к ломаной линии
Polyline& Polyline::AddPoint(Point point) {
    points_.push_back(point);
    return *this;
 }

void Polyline::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<polyline points=\""sv;
    for (auto it = points_.begin(); it != points_.end(); ++it) {
        out << it->x << ","sv << it->y;
        if (it + 1 != points_.end()) {
            out << " "sv;
        }
    }

    out << "\""sv;
    RenderAttrs(out);
    out << "/>"sv;
 }

// ---------- Text ------------------

    // Задаёт координаты опорной точки (атрибуты x и y)
Text& Text::SetPosition(Point pos) {
    pos_ = pos;
    return *this;
}

// Задаёт смещение относительно опорной точки (атрибуты dx, dy)
Text& Text::SetOffset(Point offset) {
    offset_ = offset;
    return *this;
}

// Задаёт размеры шрифта (атрибут font-size)
Text& Text::SetFontSize(uint32_t size) {
    size_ = size;
    return *this;
}

// Задаёт название шрифта (атрибут font-family)
Text& Text::SetFontFamily(std::string font_family) {
    font_family_ = font_family;
    return *this;
}

// Задаёт толщину шрифта (атрибут font-weight)
Text& Text::SetFontWeight(std::string font_weight) {
    font_weight_ = font_weight;
    return *this;
}

// Задаёт текстовое содержимое объекта (отображается внутри тега text)
Text& Text::SetData(std::string data) {
    for (auto [ch, spec_ch] : spec_symbols_) {
        auto founded_ch = data.find(ch);
        while (founded_ch != std::string::npos) {
            data.replace(founded_ch, 1, spec_ch);
            founded_ch = data.find(ch);
        }
    }

    data_ = data;
    return *this;
}

void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text x=\"" << pos_.x << "\" y=\"" << pos_.y << "\" ";
    out << "dx=\"" << offset_.x << "\" dy=\"" << offset_.y << "\" ";
    out << "font-size=\"" << size_ << "\"";

    if (font_family_) {
        out << " font-family=\"" << *font_family_ << "\"";
    }
    if (font_weight_) {
        out << " font-weight=\"" << *font_weight_ << "\"";
    }

    RenderAttrs(out);
    out << ">" << data_ << "</text>";
}

// ---------- Document ------------------

// Добавляет в svg-документ объект-наследник svg::Object
void Document::AddPtr(std::unique_ptr<Object>&& obj) {
    objects_.emplace_back(std::move(obj));
}

// Выводит в ostream svg-представление документа
void Document::Render(std::ostream& out) const {
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"sv;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"sv;
    for (auto& obj : objects_) {
        obj->Render(RenderContext(out, 2, 2));
    }

    out << "</svg>"sv;
}

}  // namespace svg

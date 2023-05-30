#include "map_renderer.h"


namespace map_renderer {

	bool IsZero(double value) {
		return std::abs(value) < EPSILON;
	}

	MapRenderer::MapRenderer(const RenderSettings& settings)
		: settings_(settings)
	{}

	void MapRenderer::SetSettings(const RenderSettings& settings) {
		settings_ = settings;
	}

	void MapRenderer::SetProjector(const std::vector<domain::BusRoute*> routes) {
		std::vector<geo::Coordinates> coords = {};
		for (const auto& route : routes) {
			for (const auto& stop : route->stops_) {
				coords.push_back(stop->coordinates_);
			}
		}

		projector_ = SphereProjector{ coords.begin(), coords.end(),
			settings_.width, settings_.height, settings_.padding };
	}

	void MapRenderer::RendererPolyline(const domain::BusRoute* route, size_t number)	{
		svg::Polyline polyline;
		size_t colorIndex = number % settings_.color_palette.size();

		for (const auto& stop : route->stops_) {
			polyline.AddPoint(projector_(stop->coordinates_));
		}

		polyline.SetFillColor(svg::NoneColor)
			.SetStrokeColor(settings_.color_palette[colorIndex])
			.SetStrokeWidth(settings_.line_width)
			.SetStrokeLineCap(svg::StrokeLineCap::ROUND)
			.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

		doc_.Add(polyline);
	}

	void MapRenderer::RendererRouteName(const domain::BusRoute* route, size_t number) {
		svg::Text text;
		svg::Text back;
		size_t colorIndex = number % settings_.color_palette.size();

		text.SetFillColor(settings_.color_palette[colorIndex])
			.SetPosition(projector_(route->stops_.front()->coordinates_))
			.SetOffset(settings_.bus_label_offset)
			.SetFontSize(settings_.bus_label_font_size)
			.SetFontFamily("Verdana"s)
			.SetFontWeight("bold"s)
			.SetData(route->number_);

		back.SetFillColor(settings_.underlayer_color)
			.SetStrokeColor(settings_.underlayer_color)
			.SetPosition(projector_(route->stops_.front()->coordinates_))
			.SetStrokeWidth(settings_.underlayer_width)
			.SetStrokeLineCap(svg::StrokeLineCap::ROUND)
			.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
			.SetOffset(settings_.bus_label_offset)
			.SetFontSize(settings_.bus_label_font_size)
			.SetFontFamily("Verdana"s)
			.SetFontWeight("bold"s)
			.SetData(route->number_);

		doc_.Add(back);
		doc_.Add(text);

		if (!route->circle && route->stops_.size() > 1) {
			int peak = route->stops_.size() / 2;
			text.SetPosition(projector_(route->stops_[peak]->coordinates_));
			back.SetPosition(projector_(route->stops_[peak]->coordinates_));
			doc_.Add(back);
			doc_.Add(text);
		}
	}

	void MapRenderer::RendererStopSymbols(const std::vector<domain::BusStop*> stops) {
		for (const auto& stop : stops) {
			doc_.Add(svg::Circle().SetCenter(projector_(stop->coordinates_))
				.SetRadius(settings_.stop_radius)
				.SetFillColor("white"s));
		}
	}

	void MapRenderer::RendererStopNames(const std::vector<domain::BusStop*> stops) {
		for (const auto& stop : stops) {
			svg::Text text;
			svg::Text back;
			text.SetFillColor("black"s)
				.SetPosition(projector_(stop->coordinates_))
				.SetOffset(settings_.stop_label_offset)
				.SetFontSize(settings_.stop_label_font_size)
				.SetFontFamily("Verdana"s)
				.SetData(stop->name_);

			back.SetFillColor("black"s)
				.SetFillColor(settings_.underlayer_color)
				.SetStrokeColor(settings_.underlayer_color)
				.SetStrokeWidth(settings_.underlayer_width)
				.SetPosition(projector_(stop->coordinates_))
				.SetOffset(settings_.stop_label_offset)
				.SetFontSize(settings_.stop_label_font_size)
				.SetFontFamily("Verdana"s)
				.SetData(stop->name_)
				.SetStrokeLineCap(svg::StrokeLineCap::ROUND)
				.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

			doc_.Add(back);
			doc_.Add(text);
		}
	}

	void MapRenderer::MakeSVGDoc(const std::vector<domain::BusRoute*> routes, std::ostream& output) {
		{
			size_t number = 0;
			for (const auto& route : routes) {
				RendererPolyline(route, number);
				number++;
			}
		}
		{
			size_t number = 0;
			for (const auto& route : routes) {
				RendererRouteName(route, number);
				number++;
			}
		}
		{
			std::unordered_set<domain::BusStop*> uniqueStops = {};
			for (const auto& route : routes) {
				for (const auto& stops : route->stops_) {
					uniqueStops.insert(stops);
				}
			}

			std::vector<domain::BusStop*> sortedStops(uniqueStops.begin(), uniqueStops.end());
			std::sort(sortedStops.begin(), sortedStops.end(),
				[](const domain::BusStop* a, const domain::BusStop* b) {
					return a->name_ < b->name_;
				});

			RendererStopSymbols(sortedStops);
			RendererStopNames(sortedStops);
		}

		doc_.Render(output);
	}

} // namespace map_renderer
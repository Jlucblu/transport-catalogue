#include "map_renderer.h"


namespace map_renderer {

	bool IsZero(double value) {
		return std::abs(value) < EPSILON;
	}

	MapRender::MapRender(const RenderSettings& settings)
		: settings_(settings)
	{}

	void MapRender::SetSettings(const RenderSettings& settings) {
		settings_ = settings;
	}

	void MapRender::SetProjector(const std::vector<geo::Coordinates>&points) {
		projector_ = SphereProjector{ points.begin(), points.end(),
			settings_.width, settings_.height, settings_.padding };
	}

	svg::Polyline MapRender::RenderRoute(const domain::BusRoute* route, int number)	{
		svg::Polyline polyline;

		for (const auto& stop : route->stops_) {
			polyline.AddPoint(projector_(stop->coordinates_));
		}

		size_t colorIndex = number % settings_.color_palette.size();

		polyline.SetFillColor(svg::NoneColor);
		polyline.SetStrokeColor(settings_.color_palette[colorIndex]);
		polyline.SetStrokeWidth(settings_.line_width);
		polyline.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
		polyline.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

		return polyline;
	}

	void MapRender::MakeSVGDoc(const std::vector<domain::BusRoute*> routes, std::ostream& output) {
		std::vector<geo::Coordinates> coords = {};
		for (const auto& route : routes) {
			for (const auto& stops : route->stops_) {
				coords.push_back(stops->coordinates_);
			}
		}
		SetProjector(coords);

		int number = 0;
		for (const auto& bus : routes) {
			doc_.Add(RenderRoute(bus, number));
			number++;
		}

		doc_.Render(output);
	}

} // namespace map_renderer
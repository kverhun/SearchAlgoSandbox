#include "TSPDrawingWidget.h"

#include <QPainter>

using namespace TSP_GUI;

namespace
{
    using TPoint = std::pair<double, double>;

    TPoint operator + (const TPoint& i_pt1, const TPoint& i_pt2)
    {
        return TPoint{ i_pt1.first + i_pt2.first, i_pt1.second + i_pt2.second };
    }

    TPoint operator - (const TPoint& i_pt1, const TPoint& i_pt2)
    {
        return TPoint{ i_pt1.first - i_pt2.first, i_pt1.second - i_pt2.second };
    }

    std::pair<TPoint, TPoint> _GetPointsBounds(const TSP::TPoints& i_points)
    {
        TPoint pt_min{ i_points.front().x, i_points.front().y };
        TPoint pt_max{ i_points.front().x, i_points.front().y };

        for (const auto& pt : i_points)
        {
            if (pt.x < pt_min.first)
                pt_min.first = pt.x;
            if (pt.x > pt_max.first)
                pt_max.first = pt.x;
            if (pt.y < pt_min.second)
                pt_min.second = pt.y;
            if (pt.y > pt_max.second)
                pt_max.second = pt.y;
        }

        return std::make_pair(pt_min, pt_max);
    }

    std::pair<double, double> _GetScale(const TPoint& i_max_point, const TPoint& i_max_widget_point)
    {
        double scale_x = i_max_point.first / i_max_widget_point.first;
        double scale_y = i_max_point.second / i_max_widget_point.second;
        return std::make_pair(scale_x, scale_y);
    }
}

DrawingWidget::DrawingWidget(const TSP& i_tsp)
    : m_tsp(i_tsp)
{
    setStyleSheet("background-color: white");
    _CalculatePointsToDraw();
}

void DrawingWidget::UpdatePath(const TSP::TPath& i_path)
{
    m_current_path = i_path;
    update();
}

void DrawingWidget::_CalculatePointsToDraw()
{
    m_points_to_draw = m_tsp.GetPoints();

    // shift points
    auto points_bounds = _GetPointsBounds(m_tsp.GetPoints());
    for (auto& pt : m_points_to_draw)
    {
        pt.x -= points_bounds.first.first;
        pt.y -= points_bounds.first.second;
    }

    // scale points
    auto widget_size = TPoint{ width() - 2 * m_margin_x, height() - 2 * m_margin_y };
    auto scale = _GetScale(points_bounds.second - points_bounds.first, widget_size);

    double max_scale = std::max(scale.first, scale.second);
    for (auto& pt : m_points_to_draw)
    {
        pt.x /= max_scale;
        pt.y /= max_scale;
    }

    // apply margin
    for (auto& pt : m_points_to_draw)
    {
        pt.x += m_margin_x;
        pt.y += m_margin_y;
    }

}

void DrawingWidget::paintEvent(QPaintEvent* ip_event)
{
    QPainter painter(this);
    painter.setBrush(Qt::blue);

    _DrawPoints(painter);
    _DrawPath(painter);
}

void DrawingWidget::_DrawPoints(QPainter& i_painter)
{
    for (const auto& pt : m_points_to_draw)
        i_painter.drawEllipse(QPoint(pt.x, pt.y), 3, 3);
}

void DrawingWidget::_DrawPath(QPainter& i_painter)
{
    if (m_current_path.size() != m_points_to_draw.size())
        return;

    i_painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 1; i < m_current_path.size(); ++i)
        i_painter.drawLine(
            QPoint(m_points_to_draw[m_current_path[i - 1]].x, m_points_to_draw[m_current_path[i - 1]].y),
            QPoint(m_points_to_draw[m_current_path[i]].x, m_points_to_draw[m_current_path[i]].y)
            );
    i_painter.drawLine(
        QPoint(m_points_to_draw[m_current_path.front()].x, m_points_to_draw[m_current_path.front()].y),
        QPoint(m_points_to_draw[m_current_path.back()].x, m_points_to_draw[m_current_path.back()].y)
        );
}

void DrawingWidget::resizeEvent(QResizeEvent* ip_resize_event)
{
    QWidget::resizeEvent(ip_resize_event);
    _CalculatePointsToDraw();
}

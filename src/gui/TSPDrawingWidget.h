#pragma once

#include <08-TSP\impl\TSP.h>

#include <QWidget>

namespace TSP_GUI
{
    class DrawingWidget : public QWidget
    {
    public:
        DrawingWidget(const TSP& i_tsp);

        void UpdatePath(const TSP::TPath& i_path);

    private:
        void _CalculatePointsToDraw();
        void paintEvent(QPaintEvent* ip_event) override;
        void _DrawPoints(QPainter& i_painter);
        void _DrawPath(QPainter& i_painter);
        void resizeEvent(QResizeEvent* ip_resize_event) override;

    private:
        const TSP& m_tsp;
        const size_t m_margin_x = 10;
        const size_t m_margin_y = 10;

        TSP::TPoints m_points_to_draw;
        TSP::TPath m_current_path;

    };

}

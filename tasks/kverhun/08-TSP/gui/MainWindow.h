#pragma once

#include "TSPSolver.h"

#include <QMainWindow>
#include <QPointer>
class TSP;

namespace TSP_GUI
{
    class DrawingWidget;

    class MainWindow : public QMainWindow
    {
    public:
        MainWindow(const TSP& i_tsp);

        QPointer<DrawingWidget> GetDrawingWidget();

    private:
        QPointer<DrawingWidget> mp_drawing_widget;

        Solver m_solver;

    };
}
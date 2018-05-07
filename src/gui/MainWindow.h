#pragma once

#include "TSPGeneticSolver.h"

#include <QMainWindow>
#include <QPointer>

#include <memory>

class TSP;
class QTextEdit;
class QSpinBox;

namespace TSP_GUI
{
    class DrawingWidget;

    class MainWindow : public QMainWindow
    {
    public:
        MainWindow(const TSP& i_tsp);

        QPointer<DrawingWidget> GetDrawingWidget();

    private:
        void _InitUi();

        void _StartGeneticAlgoFromScratch();
        void _Start2OptAlgoFromScratch();
        void _PauseAlgo();
        void _ResumeAlgo();

    private:
        QPointer<DrawingWidget> mp_drawing_widget;
        QPointer<QTextEdit>     mp_log_widget;
        QPointer<QSpinBox>      mp_population_size_spinbox;

        const TSP& m_tsp;
        std::unique_ptr<GeneticSolver> mp_solver;

    };
}
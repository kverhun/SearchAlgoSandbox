#include "MainWindow.h"

#include "TSPDrawingWidget.h"

#include <QWidget>
#include <QLayout>

using namespace TSP_GUI;

MainWindow::MainWindow(const TSP& i_tsp)
    : mp_drawing_widget(new DrawingWidget(i_tsp))
    , m_solver(i_tsp, [this](const TSP::TPath& i_path) 
    {
        mp_drawing_widget->UpdatePath(i_path);
    })
{
    
    mp_drawing_widget->setFixedSize(1920, 1080);
    setFixedSize(1920, 1080);
    layout()->addWidget(mp_drawing_widget);

    m_solver.Start();
}

QPointer<DrawingWidget> MainWindow::GetDrawingWidget()
{
    return mp_drawing_widget;
}
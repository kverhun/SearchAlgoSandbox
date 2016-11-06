#include "MainWindow.h"

#include "TSPDrawingWidget.h"

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QHBoxLayout>

using namespace TSP_GUI;

MainWindow::MainWindow(const TSP& i_tsp)
    : m_tsp(i_tsp)
    , mp_drawing_widget(new DrawingWidget(i_tsp))
{
    _InitUi();
}

QPointer<DrawingWidget> MainWindow::GetDrawingWidget()
{
    return mp_drawing_widget;
}

void MainWindow::_InitUi()
{
    //mp_drawing_widget->setFixedSize(800, 600);
    setFixedSize(1920, 1080);

    auto* p_hor_layout = new QHBoxLayout;
    p_hor_layout->addWidget(mp_drawing_widget);
    
    auto* p_button_panel_widget = new QWidget;
    p_button_panel_widget->setMaximumWidth(200);
    auto* p_ver_layout = new QVBoxLayout;
    
    auto* p_start_from_scratch_button = new QPushButton;
    p_start_from_scratch_button->setText("Start");
    QObject::connect(p_start_from_scratch_button, &QPushButton::clicked, this, &MainWindow::_StartAlgoFromScratch);
    p_ver_layout->addWidget(p_start_from_scratch_button);

    auto* p_pause_button = new QPushButton;
    p_pause_button->setText("Pause");
    QObject::connect(p_pause_button, &QPushButton::clicked, this, &MainWindow::_PauseAlgo);
    p_ver_layout->addWidget(p_pause_button);

    auto* p_resume_button = new QPushButton;
    p_resume_button->setText("Resume");
    QObject::connect(p_resume_button, &QPushButton::clicked, this, &MainWindow::_ResumeAlgo);
    p_ver_layout->addWidget(p_resume_button);

    p_button_panel_widget->setLayout(p_ver_layout);

    p_hor_layout->addWidget(p_button_panel_widget);

    auto* p_central_widget = new QWidget;
    p_central_widget->setLayout(p_hor_layout);
    
    setCentralWidget(p_central_widget);

}

void MainWindow::_StartAlgoFromScratch()
{
    mp_solver = std::make_unique<Solver>(m_tsp, [this](const TSP::TPath& i_path)
    {
        mp_drawing_widget->UpdatePath(i_path);
    });
    mp_solver->Start();
}

void MainWindow::_PauseAlgo()
{
    mp_solver->Pause();
}

void MainWindow::_ResumeAlgo()
{
    mp_solver->Resume();
}
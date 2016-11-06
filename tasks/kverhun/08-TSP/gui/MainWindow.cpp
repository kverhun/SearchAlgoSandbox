#include "MainWindow.h"

#include "TSPDrawingWidget.h"

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QString>
#include <QSpinBox>

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
    auto* p_main_vertical_layout = new QVBoxLayout;

    {
        auto* p_view_and_buttons_widget = new QWidget;

        auto* p_hor_layout = new QHBoxLayout;
        
        {
            p_hor_layout->addWidget(mp_drawing_widget);
        }

        {
            auto* p_button_panel_widget = new QWidget;
            p_button_panel_widget->setMaximumWidth(200);
            
            {
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

                mp_population_size_spinbox = new QSpinBox;
                mp_population_size_spinbox->setMinimum(2);
                mp_population_size_spinbox->setMaximum(1000000);
                mp_population_size_spinbox->setValue(200000);
                p_ver_layout->addWidget(mp_population_size_spinbox);

                p_button_panel_widget->setLayout(p_ver_layout);
            }


            p_hor_layout->addWidget(p_button_panel_widget);
        }
        p_view_and_buttons_widget->setLayout(p_hor_layout);
        
        p_main_vertical_layout->addWidget(p_view_and_buttons_widget);
    }

    {
        mp_log_widget = new QTextEdit;
        mp_log_widget->setMaximumHeight(150);
        p_main_vertical_layout->addWidget(mp_log_widget);
    }

    auto* p_central_widget = new QWidget;
    p_central_widget->setLayout(p_main_vertical_layout);
    setCentralWidget(p_central_widget);
}

void MainWindow::_StartAlgoFromScratch()
{
    mp_solver = std::make_unique<Solver>(m_tsp, [this](std::shared_ptr<const TSP_GUI::SolverStatusReporter::GeneticAlgorithmIterationInfo> ip_info)
    {
        mp_drawing_widget->UpdatePath(ip_info->m_current_path);
        mp_log_widget->append(QString::fromStdString(std::to_string(ip_info->m_current_path_length)));
    });
    mp_solver->SetPopulationSize(mp_population_size_spinbox->value());
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
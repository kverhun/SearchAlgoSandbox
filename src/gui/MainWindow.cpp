#include "MainWindow.h"

#include "TSPDrawingWidget.h"

#include "08-TSP/impl/GeneticAlgo.h"
#include "08-TSP/impl/TSP2OptSolver.h"

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QString>
#include <QSpinBox>

using namespace TSP_GUI;

namespace
{
    QString _GetLogStringForIterationInfo(const SolverStatusReporter::GeneticAlgorithmIterationInfo& i_info)
    {
        std::string iteration_number_str = std::to_string(i_info.m_iteration_number);
        std::string path_length_str = std::to_string(i_info.m_current_path_length);
        std::string milliseconds_str = std::to_string(i_info.m_iteration_duration_milliseconds);

        QString log_string;
        log_string
            .append("Iteration: ")
            .append(QString::fromStdString(iteration_number_str))
            .append("; Path length: ")
            .append(QString::fromStdString(path_length_str))
            .append("; Duration: ")
            .append(QString::fromStdString(milliseconds_str));

        return log_string;
    }
}

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

                auto* p_start_genetic_from_scratch_button = new QPushButton;
                p_start_genetic_from_scratch_button->setText("Start genetic algo");
                QObject::connect(p_start_genetic_from_scratch_button, &QPushButton::clicked, this, &MainWindow::_StartGeneticAlgoFromScratch);
                p_ver_layout->addWidget(p_start_genetic_from_scratch_button);

                auto* p_start_2opt_from_scratch_button = new QPushButton;
                p_start_2opt_from_scratch_button->setText("Start 2-opt algo");
                QObject::connect(p_start_2opt_from_scratch_button, &QPushButton::clicked, this, &MainWindow::_Start2OptAlgoFromScratch);
                p_ver_layout->addWidget(p_start_2opt_from_scratch_button);

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

void MainWindow::_StartGeneticAlgoFromScratch()
{
    auto p_genetic_solver = std::make_shared<TSPGenetic::GeneticSolver>(m_tsp);
    
    auto setup_solver = [&]() {p_genetic_solver->InitializePopulation(mp_population_size_spinbox->value()); };

    mp_solver = std::make_unique<GeneticSolver>(m_tsp, p_genetic_solver, [this](std::shared_ptr<const TSP_GUI::SolverStatusReporter::GeneticAlgorithmIterationInfo> ip_info)
    {
        mp_drawing_widget->UpdatePath(ip_info->m_current_path);
        mp_log_widget->append(_GetLogStringForIterationInfo(*ip_info.get()));
    }, setup_solver);
    
    mp_solver->Start();
}

void MainWindow::_Start2OptAlgoFromScratch()
{
    auto starting_path = m_tsp.GenerateSomePath();
    auto p_2opt_solver = std::make_shared<TSP2OptSolver>(m_tsp, starting_path);

    mp_solver = std::make_unique<GeneticSolver>(m_tsp, p_2opt_solver, [this](std::shared_ptr<const TSP_GUI::SolverStatusReporter::GeneticAlgorithmIterationInfo> ip_info)
    {
        mp_drawing_widget->UpdatePath(ip_info->m_current_path);
        mp_log_widget->append(_GetLogStringForIterationInfo(*ip_info.get()));
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
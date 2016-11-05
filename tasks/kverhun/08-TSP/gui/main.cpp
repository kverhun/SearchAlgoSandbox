#include "TSPDrawingWidget.h"

#include <iostream>

#include <QApplication>
#include <QtWidgets>
#include <QPainter>
#include <Qfuture>
#include <QFutureWatcher>
#include <QtConcurrent/qtconcurrentrun.h>

#include <08-TSP\impl\TSP.h>
#include <08-TSP\impl\PredefinedConfigurations.h>
#include <08-TSP\impl\GeneticAlgo.h>

#include <thread>

using namespace TSP_GUI;

namespace
{
    TSP::TPoints _TransposePoints(const TSP::TPoints& i_points)
    {
        auto result = i_points;
        for (auto& pt : result)
        {
            std::swap(pt.x, pt.y);
        }
        return result;
    }
    


    TSP::TPath _PerformNextStepAndReturnBestPath(std::shared_ptr<TSPGenetic::GeneticSolver> ip_solver)
    {
        ip_solver->PerformSingleStep();
        return ip_solver->GetCurrentPopulation().front();
    }

}

int main(int i_argc, char** i_argv)
{
    std::cout << "Start application..." << std::endl;

    TSP::TPoints points = _TransposePoints(PredefiedConfigurations::g_config_states);
    TSP tsp(points);

    QApplication app(i_argc, i_argv);
    
    QPointer<DrawingWidget> p_widget = new DrawingWidget(tsp);
    p_widget->setFixedSize(1920, 1080);

    QMainWindow main_wnd;
    main_wnd.setFixedSize(1920, 1080);
    main_wnd.layout()->addWidget(p_widget);

    main_wnd.show();

    //auto path = tsp.GenerateSomePath();
    //auto best_path = tsp.GenerateBestPathStartingFrom(path);
    //p_widget->UpdatePath(best_path);

    auto p_solver = std::make_shared<TSPGenetic::GeneticSolver>(tsp);
    p_solver->InitializePopulation(200000);
    
    const size_t iterations = 100;

    QFutureWatcher<TSP::TPath> watcher;
    QObject::connect(&watcher, &QFutureWatcher<TSP::TPath>::finished, [&]() {
        auto result = watcher.result();
        p_widget->UpdatePath(result);
        watcher.setFuture(QtConcurrent::run([&]() -> TSP::TPath 
        {
            //std::this_thread::sleep_for(std::chrono::microseconds(250));
            return _PerformNextStepAndReturnBestPath(p_solver);
        }));
    });

    watcher.setFuture(QtConcurrent::run([&]() -> TSP::TPath
    {
        return _PerformNextStepAndReturnBestPath(p_solver);
    }));

    auto return_code = app.exec();
    std::cout << "Terminating application. Exit code: " << return_code << std::endl;
    
    return return_code;
}
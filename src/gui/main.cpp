#include "TSPDrawingWidget.h"
#include "MainWindow.h"

#include <iostream>

#include <QApplication>
#include <QtWidgets>
#include <QPainter>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/qtconcurrentrun.h>

#include "../impl/TSP.h"
#include "../impl/PredefinedConfigurations.h"
#include "../impl/GeneticAlgo.h"

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
    
}

int main(int i_argc, char** i_argv)
{
    std::cout << "Start application..." << std::endl;

    TSP::TPoints points = _TransposePoints(PredefiedConfigurations::g_predefined_configuration_2000);
    TSP tsp(points);

    QApplication app(i_argc, i_argv);
    
    MainWindow main_wnd(tsp);
    main_wnd.show();

    //auto path = tsp.GenerateSomePath();
    //auto best_path = tsp.GenerateBestPathStartingFrom(path);
    //p_widget->UpdatePath(best_path);

    auto return_code = app.exec();
    std::cout << "Terminating application. Exit code: " << return_code << std::endl;
    
    return return_code;
}
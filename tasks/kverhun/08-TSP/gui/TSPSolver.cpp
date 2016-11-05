#include "TSPSolver.h"

#include <08-TSP/impl/GeneticAlgo.h>

#include <Qfuture>
#include <QFutureWatcher>
#include <QtConcurrent/qtconcurrentrun.h>

#include <thread>

using namespace TSP_GUI;

namespace
{
    TSP::TPath _PerformNextStepAndReturnBestPath(std::shared_ptr<TSPGenetic::GeneticSolver> ip_solver)
    {
        ip_solver->PerformSingleStep();
        return ip_solver->GetCurrentPopulation().front();
    }
}

Solver::Solver(const TSP& i_tsp, SolverStatusReporter::TReportFunctor i_report_functor)
    : m_reporter(i_report_functor)
    , m_tsp(i_tsp)
{
    mp_solver = std::make_shared<TSPGenetic::GeneticSolver>(m_tsp);
    mp_solver->InitializePopulation(200000);

    QObject::connect(&m_future_watcher, &QFutureWatcher<TSP::TPath>::finished, [&]() {
        auto result = m_future_watcher.result();
        m_reporter.Report(result);
        m_future_watcher.setFuture(QtConcurrent::run([&]() -> TSP::TPath
        {
            //std::this_thread::sleep_for(std::chrono::microseconds(50));
            return _PerformNextStepAndReturnBestPath(mp_solver);
        }));
    });
}

void Solver::Start()
{
    m_future_watcher.setFuture(QtConcurrent::run([&]() -> TSP::TPath
    {
        return _PerformNextStepAndReturnBestPath(mp_solver);
    }));
}

void Solver::Pause()
{
}

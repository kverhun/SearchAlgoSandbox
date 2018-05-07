#include "TSPGeneticSolver.h"

#include "../impl/GeneticAlgo.h"

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/qtconcurrentrun.h>

#include <thread>
#include <chrono>

using namespace TSP_GUI;

namespace
{

    void _PerformGeneticAlgoSingleStep(std::shared_ptr<TSPGenetic::GeneticSolver> ip_solver)
    {
        ip_solver->PerformSingleStep();
    }

    TSP::TPath _PerformNextStepAndReturnBestPath(std::shared_ptr<TSPGenetic::GeneticSolver> ip_solver)
    {
        ip_solver->PerformSingleStep();
        return ip_solver->GetCurrentPopulation().front();
    }

    const size_t g_default_population_size = 200000;
}

GeneticSolver::GeneticSolver(const TSP& i_tsp, std::shared_ptr<ITSPSolver> ip_solver, SolverStatusReporter::TReportFunctor i_report_functor, std::function<void()> ip_setup_function)
    : m_reporter(i_report_functor)
    , m_tsp(i_tsp)
    , mp_solver(ip_solver)
    , mp_setup_function(ip_setup_function)
{
    QObject::connect(&m_future_watcher, &QFutureWatcher<void>::finished, [&]() 
    {
        auto p_iteration_info = std::make_shared<TSP_GUI::SolverStatusReporter::GeneticAlgorithmIterationInfo>();
        p_iteration_info->m_current_path = mp_solver->RetrieveCurrentBestPath();
        p_iteration_info->m_current_path_length = m_tsp.GetPathLength(p_iteration_info->m_current_path);
        p_iteration_info->m_iteration_duration_milliseconds = m_last_iteration_duration_milliseconds;
        p_iteration_info->m_iteration_number = m_last_iteration_number;

        m_reporter.Report(p_iteration_info);
        
        if (m_is_running)
            _TriggerSolverIteration();
    });
}

void GeneticSolver::Start()
{
    m_last_iteration_number = 0;

    m_is_running = true;
    _TriggerSolverIteration();
}

void GeneticSolver::Pause()
{
    m_is_running = false;
}

void GeneticSolver::Resume()
{
    m_is_running = true;
    _TriggerSolverIteration();
}

void GeneticSolver::_TriggerSolverIteration()
{
    m_future_watcher.setFuture(QtConcurrent::run([&]() 
    { 
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        if (mp_setup_function)
        {
            mp_setup_function();
            mp_setup_function = nullptr;
        }

        mp_solver->PerformSingleStep();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        
        m_last_iteration_duration_milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        ++m_last_iteration_number;
    }));
}

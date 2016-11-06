#include "TSPSolver.h"

#include <08-TSP/impl/GeneticAlgo.h>

#include <Qfuture>
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

Solver::Solver(const TSP& i_tsp, SolverStatusReporter::TReportFunctor i_report_functor)
    : m_reporter(i_report_functor)
    , m_tsp(i_tsp)
    , m_population_size(g_default_population_size)
{
    QObject::connect(&m_future_watcher, &QFutureWatcher<void>::finished, [&]() 
    {
        auto p_iteration_info = std::make_shared<TSP_GUI::SolverStatusReporter::GeneticAlgorithmIterationInfo>();
        p_iteration_info->m_current_path = mp_solver->GetCurrentPopulation().front();
        p_iteration_info->m_current_path_length = m_tsp.GetPathLength(p_iteration_info->m_current_path);
        p_iteration_info->m_iteration_duration_milliseconds = m_last_iteration_duration_milliseconds;
        p_iteration_info->m_iteration_number = m_last_iteration_number;

        m_reporter.Report(p_iteration_info);
        
        if (m_is_running)
            _TriggerSolverIteration();
    });
}

void Solver::Start()
{
    mp_solver = std::make_shared<TSPGenetic::GeneticSolver>(m_tsp);
    mp_solver->InitializePopulation(m_population_size);
    m_last_iteration_number = 0;

    m_is_running = true;
    _TriggerSolverIteration();
}

void Solver::Pause()
{
    m_is_running = false;
}

void Solver::Resume()
{
    m_is_running = true;
    _TriggerSolverIteration();
}

void Solver::_TriggerSolverIteration()
{
    m_future_watcher.setFuture(QtConcurrent::run([&]() 
    { 
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        _PerformGeneticAlgoSingleStep(mp_solver); 

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        
        m_last_iteration_duration_milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        ++m_last_iteration_number;
    }));
}

void Solver::SetPopulationSize(size_t i_population_size)
{
    m_population_size = i_population_size;
}
#pragma once

#include "TSPSolverStatusReporter.h"

#include <08-TSP/impl/TSP.h>

#include <memory>
#include <functional>
#include <QFutureWatcher>

class ITSPSolver;

namespace TSP_GUI
{
    class GeneticSolver
    {

    public:
        GeneticSolver(const TSP& i_tsp, std::shared_ptr<ITSPSolver> ip_solver, SolverStatusReporter::TReportFunctor i_report_functor, std::function<void()> ip_setup_function = nullptr);

        void Start();
        void Pause();
        void Resume();
    
    private:
        void _TriggerSolverIteration();

    private:
        SolverStatusReporter m_reporter;
        const TSP& m_tsp;

        std::shared_ptr<ITSPSolver> mp_solver;

        QFutureWatcher<void> m_future_watcher;

        bool m_is_running = false;

        double m_last_iteration_duration_milliseconds;
        size_t m_last_iteration_number = 0;
        std::function<void()> mp_setup_function = nullptr;
    };
}
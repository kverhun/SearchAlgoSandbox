#pragma once

#include "TSPSolverStatusReporter.h"

#include <08-TSP/impl/TSP.h>

#include <memory>
#include <QFutureWatcher>

namespace TSPGenetic
{
    class GeneticSolver;
}

namespace TSP_GUI
{
    class Solver
    {
    public:
        Solver(const TSP& i_tsp, SolverStatusReporter::TReportFunctor i_report_functor);

        void Start();
        void Pause();
        void Resume();
        
    private:
        SolverStatusReporter m_reporter;
        const TSP& m_tsp;

        std::shared_ptr<TSPGenetic::GeneticSolver> mp_solver;

        QFutureWatcher<TSP::TPath> m_future_watcher;

        bool m_is_running = false;
    };
}
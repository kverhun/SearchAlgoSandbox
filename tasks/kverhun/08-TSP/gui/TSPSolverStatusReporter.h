#pragma once

#include <08-TSP/impl/TSP.h>

#include <functional>

namespace TSP_GUI
{
    class SolverStatusReporter
    {
    public:
        using TReportFunctor = std::function<void(const TSP::TPath&)>;

    public:
        SolverStatusReporter(TReportFunctor i_report_functor);

        void Report(const TSP::TPath& i_path);

    private:
        TReportFunctor m_report_functor;

    };
}
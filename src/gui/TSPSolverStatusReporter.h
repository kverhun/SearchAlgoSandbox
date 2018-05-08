#pragma once

#include "../impl/TSP.h"

#include <functional>
#include <memory>

namespace TSP_GUI
{
    class SolverStatusReporter
    {
    public:
        struct GeneticAlgorithmIterationInfo
        {
            TSP::TPath m_current_path;
            double     m_current_path_length;
            size_t     m_iteration_number;
            size_t     m_iteration_duration_milliseconds;
        };

        using TReportFunctor = std::function<void(std::shared_ptr<const GeneticAlgorithmIterationInfo>)>;

    public:
        SolverStatusReporter(TReportFunctor i_report_functor);

        void Report(std::shared_ptr<const GeneticAlgorithmIterationInfo> ip_info);

    private:
        TReportFunctor m_report_functor;

    };
}
#include "TSPSolverStatusReporter.h"

using namespace TSP_GUI;

SolverStatusReporter::SolverStatusReporter(SolverStatusReporter::TReportFunctor i_report_functor)
    : m_report_functor(i_report_functor)
{
}

void TSP_GUI::SolverStatusReporter::Report(std::shared_ptr<const GeneticAlgorithmIterationInfo> i_info)
{
    m_report_functor(i_info);
}

#include "TSPSolverStatusReporter.h"

using namespace TSP_GUI;

SolverStatusReporter::SolverStatusReporter(SolverStatusReporter::TReportFunctor i_report_functor)
    : m_report_functor(i_report_functor)
{
}

void TSP_GUI::SolverStatusReporter::Report(const TSP::TPath& i_path)
{
    m_report_functor(i_path);
}

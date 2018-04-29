#include "TSP2OptSolver.h"

TSP2OptSolver::TSP2OptSolver(const TSP& i_tsp, const TSP::TPath& i_starting_path)
    : m_tsp(i_tsp)
    , m_current_best_path(i_starting_path)
{

}

void TSP2OptSolver::PerformSingleStep()
{
    m_current_best_path = m_tsp.GenerateBetterPath(m_current_best_path);
}

TSP::TPath TSP2OptSolver::RetrieveCurrentBestPath() const
{
    return m_current_best_path;
}

void TSP2OptSolver::FindBestPossiblePath()
{
    m_current_best_path = m_tsp.GenerateBestPathStartingFrom(m_current_best_path);
}

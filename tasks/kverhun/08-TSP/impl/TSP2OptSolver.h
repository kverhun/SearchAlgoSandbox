#pragma once

#include "_Library.h"

#include "ITSPSolver.h"
#include "TSP.h"

class TSP_API TSP2OptSolver : public ITSPSolver
{
public:
    TSP2OptSolver(const TSP& i_tsp, const TSP::TPath& i_starting_path);

    virtual void PerformSingleStep() override;
    virtual TSP::TPath RetrieveCurrentBestPath() const override;
    
    void FindBestPossiblePath();

private:
    const TSP& m_tsp;
    TSP::TPath m_current_best_path;

};
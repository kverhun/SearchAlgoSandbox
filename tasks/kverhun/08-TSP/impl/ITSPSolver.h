#pragma once

#include "TSP.h"

class ITSPSolver
{
public:
    virtual ~ITSPSolver() = 0 { }

    virtual void PerformSingleStep() = 0;

    virtual TSP::TPath RetrieveCurrentBestPath() const = 0;
};
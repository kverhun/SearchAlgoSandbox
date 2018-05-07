#pragma once

#include "TSP.h"

class ITSPSolver
{
public:
    virtual ~ITSPSolver() { }

    virtual void PerformSingleStep() = 0;

    virtual TSP::TPath RetrieveCurrentBestPath() const = 0;
};
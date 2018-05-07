#pragma once

#include "_Library.h"
#include "TSP.h"
#include "ITSPSolver.h"

namespace TSPGenetic
{

    class TSP_API GeneticSolver : public ITSPSolver
    {
    public:
        using TPopulation = std::vector<TSP::TPath>;

    public:
        GeneticSolver(const TSP& i_tsp);

        void InitializePopulation(size_t i_size);

        const TPopulation& GetCurrentPopulation() const;

        virtual void PerformSingleStep() override;

        virtual TSP::TPath RetrieveCurrentBestPath() const override;

    private:
        TPopulation _GenerateRandomPopulation(size_t i_population_size);
        TPopulation _GenerateSomePopulation(size_t i_population_size);

        TSP::TPath _Crossover(const TSP::TPath& i_first, const TSP::TPath& i_second);
        TSP::TPath _Mutation(const TSP::TPath& i_path);

        void _SortByFitness();
    private:
        const TSP& m_tsp;

        TPopulation m_current_population;
    };

}
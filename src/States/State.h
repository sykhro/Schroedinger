#ifndef STATE_H
#define STATE_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include <spdlog/fmt/ostr.h>
#include "Base.h"
#include "Potential.h"


class State {
  public:

    State() {}
    State(std::vector<double> wavefunction, std::vector<double> probability, std::vector<std::vector<double>> potential, double energy, Base base, int nbox);
    State(std::vector<State>);
    
    const std::vector<double>& getWavefunction();
    const std::vector<double>& getProbability();
    const Potential& getPotential();
    const double& getEnergy();
    Base getBase() const { return base; };

    void printToFile();

    friend std::ostream& operator<<(std::ostream& stream, const State& state);

    int nbox;
    Base base;
    Potential potential;
    std::vector<double> wavefunction;
    std::vector<double> probability;
    double energy;
};

#endif

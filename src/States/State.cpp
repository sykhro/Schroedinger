#include "State.h"

#include <utility>

State::State(std::vector<double> wavefunction, std::vector<double> probability, std::vector<std::vector<double>> potential, double energy, Base base, int nbox) {

    double norm = 0.0;
    this->potential = Potential(base, potential);
    this->nbox = nbox;
    this->probability  = probability;
    this->wavefunction = wavefunction;
    this->base         = base;
    this->energy       = energy;
}

State::State(std::vector<State> states) {
    std::vector<Base> bases;
    std::vector<std::vector<double>> wavefunctions = std::vector<std::vector<double>>();
    std::vector<std::vector<double>> probabilities = std::vector<std::vector<double>>();
    std::vector<Potential> potentials = std::vector<Potential>();

    for (State local_state : states) {
        bases.push_back(local_state.getBase());
        wavefunctions.push_back(local_state.getWavefunction());
        potentials.push_back(local_state.getPotential());
        probabilities.push_back(local_state.getProbability());
    }

    for (Base b : bases) {
        this->base += b;
    }

    for (Potential p: potentials) {
        this->potential += p;
    }

    // Save W(a, ..., z)
    int n = wavefunctions.size(); 
    int* indices = new int[n]; 
  
    for (int i = 0; i < n; i++) 
        indices[i] = 0; 
  
    while (1) { 
  
        // print current combination 
        double sum = 0;
        for (int i = 0; i < n; i++) {
            sum += wavefunctions[i][indices[i]];
        } 
        this->wavefunction.push_back(sum);
        
        int next = n - 1; 
        while (next >= 0 &&  
              (indices[next] + 1 >= wavefunctions[next].size())) 
            next--; 
  
        if (next < 0) 
            break; 
  
        indices[next]++; 
  
        for (int i = next + 1; i < n; i++) 
            indices[i] = 0; 
    } 

    // Maybe we should check probability here
    this->probability = probabilities.at(0);
}

const std::vector<double> &State::getWavefunction() { return this->wavefunction; }
const std::vector<double> &State::getProbability() { return this->probability; }
const Potential &State::getPotential() { return this->potential; }

const double &State::getEnergy() { return this->energy; }

void State::printToFile() {
    std::ofstream basefile("base.dat");
    std::ofstream wavefunctionfile("wavefunction.dat");
    std::ofstream probabilityfile("probability.dat");

    if (wavefunctionfile.is_open() && probabilityfile.is_open() && basefile.is_open()) {

        for (double wavefunction_element : wavefunction)
            wavefunctionfile << wavefunction_element << std::endl;
        
        for (double probability_element : probability)
            probabilityfile << probability_element << std::endl;

        basefile << base;

        wavefunctionfile.close();
        probabilityfile.close();
        basefile.close();
    }
}

std::ostream &operator<<(std::ostream &stream, const State &st) {
    std::vector<double> base_coords = st.getBase().getCoords();

    stream << std::setw(20) << std::right << "Basis coordinates";
    stream << std::setw(20) << std::right << "Wavefunction";
    stream << std::setw(20) << std::right << "Probability" << std::endl;

    for (int i = 0; i < base_coords.size(); i++) {

        // Printing coord
        Base base = st.getBase();
        stream << base;

        // Printing wavefunction
        stream << std::setprecision(3) << std::setw(20) << std::right << st.wavefunction[i];

        // Printing probability
        stream << std::setprecision(3) << std::setw(20) << std::right << st.probability[i]
               << std::endl;
    }

    return stream;
}



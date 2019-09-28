#include "Potential.h"

#include <utility>

Potential::Potential() = default;
Potential::Potential(Base base, std::vector<std::vector<double>> potentialValues) {
    this->base      = std::move(base);
    this->values    = std::move(potentialValues);
}

Potential::Potential(Base base, PotentialType type, double k, double width, double height) {
    this->base      = base;
    this->k         = k;
    this->width     = width;
    this->height    = height;
    this->type      = type;

    // Evaluation
    switch (type) {
        case BOX_POTENTIAL:
            this->box_potential();
            break;
        case HARMONIC_OSCILLATOR:
            this->ho_potential();
            break;
        case FINITE_WELL_POTENTIAL:
            this->finite_well_potential();
            break;
        default:
            throw std::invalid_argument("Wrong potential type or initialization meaningless!");
    }
    this->printToFile();
}

void Potential::ho_potential() {

    for (ContinuousBase b : this->base.getContinuous()) {
        std::vector<double> x = b.getCoords();
        std::vector<double> v = std::vector<double>(b.getCoords().size());
        std::fill(v.begin(), v.end(), 0.0);
        int i = 0;
        for (double value : x) {
            v[i] = (value * value * this->k);
            i++;
        }

        this->values.push_back(v);
    }

    for (DiscreteBase b : this->base.getDiscrete()) {
        std::vector<int> x = b.getCoords();
        std::vector<double> v(x.begin(), x.end());
        std::fill(v.begin(), v.end(), 0.0);
        int i = 0;
        for (double value : x) {
            v[i] = (value * value * this->k);
            i++;
        }

        this->values.push_back(v);
    }
}

void Potential::box_potential() {

    for (ContinuousBase b : this->base.getContinuous()) {
        std::vector<double> v = std::vector<double>(b.getCoords().size());
        std::fill(v.begin(), v.end(), 0.0);
        this->values.push_back(v);
    }

    for (DiscreteBase b : this->base.getDiscrete()) {
        std::vector<int> x = b.getCoords();
        std::vector<double> v(x.begin(), x.end());
        std::fill(v.begin(), v.end(), 0.0);
        this->values.push_back(v);
    }

}

void Potential::finite_well_potential() {

    for (ContinuousBase b : this->base.getContinuous()) {
        std::vector<double> x = b.getCoords();
        std::vector<double> v = std::vector<double>(b.getCoords().size(), 0);

        int i = 0;
        for (double value : x) {
            v[i] = (value > -this->width / 2.0 && value < this->width / 2.0) ? 0.0 : this->height;
            i++;
        }
        this->values.push_back(v);
    }

    for (DiscreteBase b : this->base.getDiscrete()) {
        std::vector<int> x = b.getCoords();
        std::vector<double> v(x.begin(), x.end());
        std::fill(v.begin(), v.end(), 0.0);
        int i = 0;
        for (double value : x) {
            v[i] = (value > -this->width / 2.0 && value < this->width / 2.0) ? 0.0 : this->height;
            i++;
        }
        this->values.push_back(v);
    }
}

void Potential::printToFile() {
    std::ofstream myfile("potential.dat");
    if (myfile.is_open()) {
        myfile << *this;
        myfile.close();
    }
}

std::ostream& operator<<(std::ostream& stream, Potential& potential) {
    std::vector<std::vector<double>> arr = potential.getValues();
    // number of arrays 
    int n = arr.size(); 
  
    // to keep track of next element in each of 
    // the n arrays 
    int* indices = new int[n]; 
  
    // initialize with first element's index 
    for (int i = 0; i < n; i++) 
        indices[i] = 0; 
  
    while (1) { 
  
        // print current combination 
        double sum = 0;
        for (int i = 0; i < n; i++) {
            sum += arr[i][indices[i]];
        } 
        stream << sum << " " << std::endl; 
  
        // find the rightmost array that has more 
        // elements left after the current element  
        // in that array 
        int next = n - 1; 
        while (next >= 0 &&  
              (indices[next] + 1 >= arr[next].size())) 
            next--; 
  
        // no such array is found so no more  
        // combinations left 
        if (next < 0) 
            break; 
  
        // if found move to next element in that  
        // array 
        indices[next]++; 
  
        // for all arrays to the right of this  
        // array current index again points to  
        // first element 
        for (int i = next + 1; i < n; i++) 
            indices[i] = 0; 
    } 

    return stream;
}

Base Potential::getBase() { return this->base; }

// Create a potential having all vectors of both potentials
const Potential operator+(Potential& potential1, Potential& potential2) {
    std::vector<std::vector<double>> potential_values = std::vector<std::vector<double>>();

    for (std::vector<double> dimensions : potential1.getValues())
        potential_values.push_back(dimensions);

    for (std::vector<double> dimensions : potential2.getValues())
        potential_values.push_back(dimensions);

    Base base1 = potential1.getBase();
    Base base2 = potential2.getBase();
    Base final_base = base1 + base2;

    const Potential& final_potential = Potential(final_base, potential_values);
    return final_potential;
}

    Potential& Potential::operator+=(Potential& potential2) {

        for (std::vector<double> vals : potential2.getValues())
            this->values.push_back(vals);

        return *this;
    }
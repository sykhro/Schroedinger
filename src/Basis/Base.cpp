#include "Base.h"
#include "BasisManager.h"
#include "LogManager.h"

Base::Base(const std::vector<double>& coords) {
    this->dimensions = 1;
    this->continuous.emplace_back(coords);
    this->boundary = ZEROEDGE;
}

Base::Base(basePreset t, int n_dimension, std::vector<ContinuousBase> c_base,
           std::vector<DiscreteBase> d_base) {

    switch (t) {
        // TODO: add here, for each base type, a control for dimensions
        case Custom:
            INFO("Initializing custom basis");
            break;
        case Cartesian:
            INFO("Initializing cartesian basis");
            break;
        case Spherical:
            INFO("Initializing Spherical Basis");
            break;
        case Cylindrical:
            INFO("Initializing Cylindrical Basis");
            break;
        default:
            throw std::invalid_argument("Wrong basis type or initialization meaningless!");
            break;
    }

    this->dimensions = n_dimension;
    this->continuous.insert(continuous.end(), c_base.begin(), c_base.end());
    this->discrete.insert(discrete.end(), d_base.begin(), d_base.end());
    this->boundary = ZEROEDGE;

    BasisManager::getInstance()->selectBase(*this);
};


std::ostream& operator<<(std::ostream& stream, Base& base) {

    std::vector<std::vector<double>> arr;
    for (ContinuousBase c : base.getContinuous()) {
        arr.push_back(c.getCoords());
    }

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
        
        for (int i = 0; i < n; i++) 
            stream << arr[i][indices[i]] << " "; 
        stream << "\n"; 
  
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

    const Base operator+(Base& base1, Base& base2) {
        std::vector<DiscreteBase> discrete_dimension = std::vector<DiscreteBase>();
        std::vector<ContinuousBase> continuous_dimension = std::vector<ContinuousBase>();
        
        for (int i = 0; i < base1.getDiscrete().size(); i++)
            discrete_dimension.push_back(base1.getDiscrete().at(i));

        for (int i = 0; i < base2.getDiscrete().size(); i++)
            discrete_dimension.push_back(base2.getDiscrete().at(i));

        for (int i = 0; i < base1.getContinuous().size(); i++)
            continuous_dimension.push_back(base1.getContinuous().at(i));

        for (int i = 0; i < base2.getContinuous().size(); i++)
            continuous_dimension.push_back(base2.getContinuous().at(i));

        const Base& base = Base(Base::basePreset::Custom, 
                        (base1.getDim() + base2.getDim()), 
                        continuous_dimension, 
                        discrete_dimension);
        return base;

    }

    Base& Base::operator += (Base& base2) {
        for (int i = 0; i < base2.getDiscrete().size(); i++)
            this->discrete.push_back(base2.getDiscrete().at(i));

        for (int i = 0; i < base2.getContinuous().size(); i++)
            this->continuous.push_back(base2.getContinuous().at(i));  

        // You have to upload also the dimension HERE
        return *this;
}

// This method let you get basis coords when it has only one dimension
std::vector<double> Base::getCoords() {
    if (this->getContinuous().size() == 1) {
        std::vector<double> toreturn = this->getContinuous().at(0).getCoords();
        return toreturn;
    } else if (this->getDiscrete().size() == 1) {
        // tricky conversion taking each std::vector<int> value and returning a final
        // std::vector<double>
        std::vector<int> original_coords = this->getDiscrete().at(0).getCoords();
        std::vector<double> toreturn =
            std::vector<double>(original_coords.begin(), original_coords.end());
        return toreturn;
    } else {
        throw std::runtime_error(
            "Can't get coords from multidimensional basis using this method. ");
    }
}
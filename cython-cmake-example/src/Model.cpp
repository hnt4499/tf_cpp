#include "Model.h"
#include <iostream>

using namespace std;

namespace model {
    // Overloaded constructor
    Model::Model (string& msg) {
        cout << "Initializing Model object with message: " << msg << endl;
        this->msg = msg;

        cout << "Initializing TensorflowShape object" << endl;
        this->shape = tensorflow::TensorShape();
        this->shape.AddDim(128);
        this->shape.AddDim(128);
        this->shape.AddDim(3);

        cout << "Done initializing" << endl;
    }

    // Destructor
    Model::~Model () {}

    void Model::say () {
        cout << msg << endl;
    }
}

#include "Model.h"
#include <iostream>

using namespace cv;
using namespace std;

namespace model {
    Model::Model () {}

    Model::Model (string& msg) {
        cout << "Initializing Model object" << endl;        
        this->msg = msg;
        
        cout << "Initializing TensorflowShape object" << endl;
        this->shape.AddDim(128);
        this->shape.AddDim(128);
        this->shape.AddDim(3);
        
        cout << "Done initializing" << endl;
    }

    Model::~Model () {}

    void Model::say () {
        cout << msg << endl;
    }
}
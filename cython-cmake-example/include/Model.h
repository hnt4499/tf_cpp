#ifndef _Model_h
#define _Model_h

#include <iostream>

#include "tensorflow/cc/client/client_session.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/cc/framework/gradients.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/cc/tools/freeze_saved_model.h"

using namespace std;
using namespace tensorflow;

namespace model {
    class Model {
        private:
            tensorflow::TensorShape shape;
            string msg;
        public:
            Model(string& message);
            ~Model(); // destructor
            void say();
    };
}

#endif

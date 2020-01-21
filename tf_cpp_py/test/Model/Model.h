#ifndef MODEL_H
#define MODEL_H

#include <opencv2/core/mat.hpp>
#include <iostream>

#include "tensorflow/cc/client/client_session.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/cc/framework/gradients.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/summary/summary_file_writer.h"
#include "tensorflow/cc/tools/freeze_saved_model.h"

using namespace cv;
using namespace std;
using namespace tensorflow;
using namespace tensorflow::ops;

namespace model {
    class Model {
        private:
            Mat mat;
            tensorflow::TensorShape shape = tensorflow::TensorShape();
            string msg;
        public:
            Model(); // nullary constructor
            Model(string& message);
            ~Model(); // destructor
            void say();
    };
}

#endif
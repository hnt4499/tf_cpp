#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <map>
#include <fstream>
#include "tensorflow/cc/client/client_session.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/cc/framework/gradients.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/summary/summary_file_writer.h"
#include "tensorflow/cc/tools/freeze_saved_model.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <regex>

using namespace std;
using namespace cv;
using namespace tensorflow;
using namespace tensorflow::ops;


namespace model {
    class Model {
        private:
            // Image reading
            Scope input_scope; // scope for loading images into tensors
            const int image_height;
            const int image_width;
            const int image_channels; // number of channels
            tensorflow::TensorShape image_shape = tensorflow::TensorShape();
            cv::Size image_size;

            // Making inference
            Scope inf_scope; // scope for making predictions
            unique_ptr<Session> test_session;


        public:
            Model(int img_height, int img_width, int channels): input_scope(Scope::NewRootScope()), inf_scope(Scope::NewRootScope()), image_height(img_height), image_width(img_width), image_channels(channels) {
                image_shape.AddDim(image_height);
                image_shape.AddDim(image_width);
                image_shape.AddDim(3);

                image_size = cv::Size(image_width, image_height);
            }

            tensorflow::TensorShape get_shape();

            Status mat2tensor(const Mat &mat, Tensor &outTensor);
            Status imread_from_file(string& filename, Tensor &out_tensor);
            Status imread_from_folder(string& foldername, vector<Tensor>& image_tensors);
            Status imread_batches_from_folder(string& foldername, vector<Tensor>& image_batches, int batch_size);

            Status load_model(string& filepath);
            Status load_labelmap(const string &filepath, map<int, string> &labelmap);
            Status predict(vector<pair<String, Tensor>> input_layers, vector<String> output_layers, vector<Tensor>& results);
    };
}

#endif
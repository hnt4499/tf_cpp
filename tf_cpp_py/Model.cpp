#include "Model.h"

using namespace std;
using namespace cv;
using namespace tensorflow;
using namespace tensorflow::ops;


/** Get image shape
 */
tensorflow::TensorShape Model::get_shape() {
    return image_shape;
}

/** Convert Mat image into Tensor of shape (image_height, image_width, 3)
 */
Status Model::mat2tensor(const Mat &mat, Tensor &out_tensor) {
    // Trick from https://github.com/tensorflow/tensorflow/issues/8033
    float *p = out_tensor.flat<float>().data();
    Mat fakeMat(mat.rows, mat.cols, CV_32FC3, p);
    mat.convertTo(fakeMat, CV_32FC3);
    
    // Cast to type UINT8
    auto input_tensor = Placeholder(input_scope.WithOpName("input"), tensorflow::DT_FLOAT);
    auto uint8Caster = Cast(input_scope.WithOpName("uint8_Cast"), out_tensor, tensorflow::DT_UINT8);
    // Container for the output
    vector<Tensor> out_tensors;
    ClientSession session(input_scope);

    TF_CHECK_OK(session.Run({{input_tensor, out_tensor}}, {uint8Caster}, &out_tensors));

    out_tensor = out_tensors[0];
    return Status::OK();
}

/** Load image from a single file.
 */
Status Model::imread_from_file(string& filename, Tensor &out_tensor)
{
    Mat image = cv::imread(filename);
    Mat image_resized;
    cv::resize(image, image_resized, image_size);
    // Convert mat to tensor
    mat2tensor(image_resized, out_tensor); 
    return Status::OK();
}

/** Load all images in a folder to a vector (list)
 */
Status Model::imread_from_folder(string& foldername, vector<Tensor>& image_tensors)
{
    // Validate the folder
    Env* penv = Env::Default();
    TF_RETURN_IF_ERROR(penv->IsDirectory(foldername));
    // Get children files
    vector<string> filenames;
    TF_RETURN_IF_ERROR(penv->GetChildren(foldername, &filenames));
    
    for(string filename: filenames)
    {
        // Check if valid image file
        if (!absl::EndsWith(filename, ".jpg") && !absl::EndsWith(filename, ".jpeg") && !absl::EndsWith(filename, ".png")) {
            continue;
        }
        string filepath = io::JoinPath(foldername, filename);
        // Load image
        Tensor image_tensor = Tensor(tensorflow::DT_FLOAT, image_shape);
        TF_RETURN_IF_ERROR(imread_from_file(filepath, image_tensor));
        
        image_tensors.push_back(image_tensor);
    }
    return Status::OK();
}

/** Load image from a folder in batches.
 */
Status Model::imread_batches_from_folder(string& foldername, vector<Tensor>& image_batches, int batch_size)
{
    // First load all images to a vector (list)
    vector<Tensor> image_tensors;
    TF_RETURN_IF_ERROR(imread_from_folder(foldername, image_tensors));
    // Parameters
    auto start_i = image_tensors.begin();
    auto end_i = image_tensors.begin() + batch_size;
    size_t batches = image_tensors.size() / batch_size;
    if(batches * batch_size < image_tensors.size()) {
        batches++;
    }
    // Generate batches
    for (int b = 0; b < batches; b++)
    {
        if(end_i > image_tensors.end())
            end_i = image_tensors.end();
        // Convert type Tensor to type Input
        vector<Tensor> one_batch(start_i, end_i);
        vector<Input> one_batch_;
        for (auto p: one_batch) {
            one_batch_.push_back(Input(p));
        }
        InputList one_batch_inputs(one_batch_);
        // Stack images
        auto stacked_images = Stack(input_scope, one_batch_inputs);
        TF_CHECK_OK(input_scope.status());
        
        ClientSession session(input_scope);
        vector<Tensor> out_tensors;
        TF_CHECK_OK(session.Run({}, {stacked_images}, &out_tensors));
        image_batches.push_back(out_tensors[0]);
        
        start_i = end_i;
        if(start_i == image_tensors.end())
            break;
        end_i = start_i + batch_size;
    }
    return Status::OK();
}


/** Read either a single image file or all image files in a folder
 */
// Status Model::imread(string& path, Tensor& out_tensors)
// {
//     // Validate the folder
//     Env* penv = Env::Default();
//     if (penv->IsDirectory(file_name).ok()) {
//         cout << "Reading folder." << endl;
//     } else {
//         cout << "Reading file." << endl;
//     }
//     return Status::OK();
//     // Convert mat to tensor
//     Tensor tensor = Tensor(tensorflow::DT_FLOAT, shape);
//     Status readTensorStatus = readTensorFromMat(frame, tensor);
//     if (!readTensorStatus.ok()) {
//         LOG(ERROR) << "Mat->Tensor conversion failed: " << readTensorStatus;
//         return -1;
//     }
//     ClientSession session(i_root);
//     TF_CHECK_OK(session.Run({{file_name_var, file_name}}, {image_tensor_var}, &out_tensors));
//     outTensor = out_tensors[0]; // shallow copy
//     return Status::OK();
// }


/** Load model *.pb
 */
Status Model::load_model(string& filepath)
{
    std::unique_ptr<GraphDef> graph_def;
    SessionOptions options;
    Status s;
    
    test_session.reset(NewSession(options));
    graph_def.reset(new GraphDef());
    TF_CHECK_OK(ReadBinaryProto(Env::Default(), filepath, graph_def.get()));
    s = test_session->Create(*graph_def.get());
    return s;
}

/** Read a labelmap file *.pbtxt
 */
Status Model::load_labelmap(const string &filepath, map<int, string> &labelmap) {

    // Read file into a string
    ifstream t(filepath);
    if (t.bad())
        return tensorflow::errors::NotFound("Failed to load labels map at '", filepath, "'");
    stringstream buffer;
    buffer << t.rdbuf();
    string file_string = buffer.str();

    // Search entry patterns of type 'item { ... }' and parse each of them
    smatch matcher_entry;
    smatch matcher_id;
    smatch matcher_name;
    
    const regex re_entry("item \\{([\\S\\s]*?)\\}");
    const regex re_id("[0-9]+");
    const regex re_name("\'.+\'");
    string entry;

    auto string_begin = sregex_iterator(file_string.begin(), file_string.end(), re_entry);
    auto string_end = sregex_iterator();

    int id;
    string name;
    for (sregex_iterator i = string_begin; i != string_end; i++) {
        matcher_entry = *i;
        entry = matcher_entry.str();
        regex_search(entry, matcher_id, re_id);
        if (!matcher_id.empty())
            id = stoi(matcher_id[0].str());
        else
            continue;
        regex_search(entry, matcher_name, re_name);
        if (!matcher_name.empty())
            name = matcher_name[0].str().substr(1, matcher_name[0].str().length() - 2);
        else
            continue;
        labelmap.insert(pair<int, string>(id, name));
    }
    return Status::OK();
}



/** Predict single image or a batch of images
 */
Status Model::predict(vector<pair<String, Tensor>> input_layers, vector<String> output_layers, vector<Tensor>& results)
{
    TF_CHECK_OK(test_session->Run(input_layers, output_layers, {}, &results));
    return Status::OK();
}
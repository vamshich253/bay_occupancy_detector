// vehicle_detector.h
// Runs a YOLO11 ONNX model through OpenCV's DNN module and returns
// the vehicles (car, motorcycle, bus, truck) found in an image.

#pragma once

#include <string>
#include <vector>

#include <opencv2/dnn.hpp>

#include "detection.h"

struct DetectorConfig {
    int inputSize = 640;         // size the model was exported with
    float confThreshold = 0.35f; // minimum score to accept a detection
    float nmsThreshold = 0.45f;  // overlap above which duplicate boxes are removed
};

class VehicleDetector {
public:
    explicit VehicleDetector(const DetectorConfig& config = DetectorConfig());

    // Loads the ONNX model. Returns false and fills error if it fails.
    bool load(const std::string& modelPath, std::string& error);

    // Detects vehicles in a BGR image. Boxes are in original image pixels.
    std::vector<Detection> detect(const cv::Mat& img);

    // Time taken by the network itself in the last detect() call, in milliseconds.
    double lastInferenceMs() const { return lastInferenceMs_; }

private:
    DetectorConfig config_;
    cv::dnn::Net net_;
    double lastInferenceMs_ = 0.0;
};

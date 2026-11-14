// main.cpp
// Detects cars (and buses, trucks, motorcycles) in one image with YOLO11
// through OpenCV's DNN module, draws boxes, saves and shows the result.
//
// Usage:
//   bay_occupancy_detector.exe <image> [model.onnx] [output.jpg]
//   bay_occupancy_detector.exe samples\Car_Image.jpg models\yolo11n.onnx result.jpg
//
// Relative paths are taken from the current working directory.
// Needs: OpenCV 5 (or 4.8+) and yolo11n.onnx exported at 640x640.

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "drawing.h"
#include "vehicle_detector.h"

int main(int argc, char** argv) {
    const std::string imagePath = argc > 1 ? argv[1] : "samples/Car_Image.jpeg";
    const std::string modelPath = argc > 2 ? argv[2] : "models/yolo11n.onnx";
    const std::string outPath = argc > 3 ? argv[3] : "result.jpeg";

    std::cout << "OpenCV " << CV_VERSION << "\n";

    // 1. Load the image
    cv::Mat img = cv::imread(imagePath);
    if (img.empty()) {
        std::cerr << "Cannot read image: " << imagePath << "\n"
            << "Working directory: " << std::filesystem::current_path().string() << "\n";
        return 1;
    }

    // 2. Load the model
    VehicleDetector detector;  // default settings: 640 input, 0.35 confidence, 0.45 NMS
    std::string error;
    if (!detector.load(modelPath, error)) {
        std::cerr << "Cannot load model " << modelPath << "\n"
            << "Working directory: " << std::filesystem::current_path().string() << "\n"
            << error << "\n";
        return 1;
    }

    // 3. Detect vehicles
    const std::vector<Detection> detections = detector.detect(img);

    // 4. Report
    std::cout << detections.size() << " vehicle(s) found in " << cvRound(detector.lastInferenceMs()) << " ms\n";
    for (const Detection& d : detections) {
        const cv::Rect& b = d.box;
        std::cout << "  " << makeLabel(d) << "  at x=" << b.x << " y=" << b.y << " w=" << b.width
            << " h=" << b.height << "\n";
    }

    // 5. Draw and save
    drawDetections(img, detections);
    cv::imwrite(outPath, img);
    std::cout << "Saved " << outPath << "\n";

    // 6. Show it (skipped if OpenCV has no window support)
    try {
        cv::imshow("Car detection - press any key", img);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    catch (const cv::Exception&) {
    }
    return 0;
}

// vehicle_detector.cpp

#include "vehicle_detector.h"

#include "coco_classes.h"
#include "letterbox.h"

VehicleDetector::VehicleDetector(const DetectorConfig& config) : config_(config) {}

bool VehicleDetector::load(const std::string& modelPath, std::string& error) {
    try {
        net_ = cv::dnn::readNetFromONNX(modelPath);
    }
    catch (const cv::Exception& e) {
        error = e.what();
        return false;
    }
    return true;
}

std::vector<Detection> VehicleDetector::detect(const cv::Mat& img) {
    // 1. Letterbox to a square input
    LetterboxInfo lb;
    const cv::Mat input = letterbox(img, config_.inputSize, lb);

    // 2. Run the network. blobFromImage scales pixels to 0..1 and swaps BGR to RGB.
    const cv::Mat blob = cv::dnn::blobFromImage(input, 1.0 / 255.0, cv::Size(), cv::Scalar(), true, false);
    net_.setInput(blob);
    const int64 t0 = cv::getTickCount();
    cv::Mat out = net_.forward();
    lastInferenceMs_ = (cv::getTickCount() - t0) * 1000.0 / cv::getTickFrequency();

    // 3. Output is [1, 84, 8400]: for each of 8400 candidate boxes,
    //    4 numbers (centre x, centre y, width, height) then 80 class scores.
    //    Transpose to 8400 rows of 84 numbers to read it easily.
    const int channels = out.size[1];
    const int candidates = out.size[2];
    const cv::Mat rows = cv::Mat(channels, candidates, CV_32F, out.ptr<float>()).t();

    std::vector<cv::Rect> boxes;
    std::vector<float> scores;
    std::vector<int> classes;
    for (int i = 0; i < rows.rows; ++i) {
        const float* r = rows.ptr<float>(i);
        const cv::Mat classScores(1, channels - 4, CV_32F, (void*)(r + 4));
        cv::Point best;
        double bestScore;
        cv::minMaxLoc(classScores, nullptr, &bestScore, nullptr, &best);
        if (bestScore < config_.confThreshold || !coco::isVehicle(best.x)) continue;

        boxes.push_back(unletterbox(r[0], r[1], r[2], r[3], lb));
        scores.push_back((float)bestScore);
        classes.push_back(best.x);
    }

    // 4. Remove duplicate boxes for the same vehicle
    std::vector<int> keep;
    cv::dnn::NMSBoxes(boxes, scores, config_.confThreshold, config_.nmsThreshold, keep);

    // 5. Build the result, clipping each box to the image
    const cv::Rect imageRect(0, 0, img.cols, img.rows);
    std::vector<Detection> result;
    result.reserve(keep.size());
    for (int idx : keep) {
        result.push_back({ boxes[idx] & imageRect, scores[idx], classes[idx] });
    }
    return result;
}

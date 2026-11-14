// letterbox.cpp

#include "letterbox.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

cv::Mat letterbox(const cv::Mat& img, int size, LetterboxInfo& info) {
    info.scale = std::min(size / (float)img.cols, size / (float)img.rows);
    const int newW = cvRound(img.cols * info.scale);
    const int newH = cvRound(img.rows * info.scale);
    info.padX = (size - newW) / 2;
    info.padY = (size - newH) / 2;

    cv::Mat out(size, size, CV_8UC3, cv::Scalar(114, 114, 114));
    cv::Mat resized;
    cv::resize(img, resized, cv::Size(newW, newH));
    resized.copyTo(out(cv::Rect(info.padX, info.padY, newW, newH)));
    return out;
}

cv::Rect unletterbox(float cx, float cy, float w, float h, const LetterboxInfo& info) {
    const float x = (cx - info.padX) / info.scale;
    const float y = (cy - info.padY) / info.scale;
    const float bw = w / info.scale;
    const float bh = h / info.scale;
    return cv::Rect(cvRound(x - bw / 2), cvRound(y - bh / 2), cvRound(bw), cvRound(bh));
}

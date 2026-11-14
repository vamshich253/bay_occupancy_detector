// drawing.cpp

#include "drawing.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

#include "coco_classes.h"

std::string makeLabel(const Detection& d) {
    return std::string(coco::className(d.classId)) + " " + std::to_string(cvRound(d.score * 100)) + "%";
}

void drawDetections(cv::Mat& img, const std::vector<Detection>& detections) {
    const cv::Scalar green(0, 255, 0);
    const cv::Scalar black(0, 0, 0);
    const int font = cv::FONT_HERSHEY_SIMPLEX;
    const double fontScale = 0.6;
    const int thickness = 2;

    for (const Detection& d : detections) {
        const cv::Rect& b = d.box;
        const std::string label = makeLabel(d);

        cv::rectangle(img, b, green, 2);

        int base = 0;
        const cv::Size ts = cv::getTextSize(label, font, fontScale, thickness, &base);
        const cv::Point tl(b.x, std::max(b.y - ts.height - 6, 0));
        cv::rectangle(img, tl, tl + cv::Point(ts.width + 6, ts.height + 6), green, -1);
        cv::putText(img, label, tl + cv::Point(3, ts.height + 2), font, fontScale, black, thickness);
    }
}

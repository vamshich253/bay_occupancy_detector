// detection.h
// One detected object: where it is, how sure the model is, and what it is.

#pragma once

#include <opencv2/core.hpp>

struct Detection {
    cv::Rect box;     // box in original image pixels (already clipped to the image)
    float score;      // confidence, 0..1
    int classId;      // COCO class index, see coco_classes.h
};

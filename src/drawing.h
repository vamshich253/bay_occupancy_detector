// drawing.h
// Turning detections into text labels and drawing them on the image.

#pragma once

#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "detection.h"

// e.g. "car 87%"
std::string makeLabel(const Detection& d);

// Draws a green box and a label above it for each detection.
void drawDetections(cv::Mat& img, const std::vector<Detection>& detections);

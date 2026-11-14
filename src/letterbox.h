// letterbox.h
// YOLO expects a square input (e.g. 640x640). Letterboxing resizes the image
// without stretching it and fills the empty space with grey. The model then
// gives boxes in letterboxed coordinates, so we also need to undo it.

#pragma once

#include <opencv2/core.hpp>

struct LetterboxInfo {
    float scale = 1.0f;  // resize factor applied to the original image
    int padX = 0;        // grey padding added on the left
    int padY = 0;        // grey padding added on the top
};

// Resizes img to fit in a size x size square, keeping its shape, and pads it with grey.
// Fills info so that boxes can be mapped back later.
cv::Mat letterbox(const cv::Mat& img, int size, LetterboxInfo& info);

// Maps a box given as centre x, centre y, width, height in letterboxed pixels
// back to a rectangle in original image pixels.
cv::Rect unletterbox(float cx, float cy, float w, float h, const LetterboxInfo& info);

// coco_classes.h
// Names of the 80 COCO classes the YOLO model was trained on,
// and a check for the vehicle classes we care about.

#pragma once

namespace coco {

constexpr int kNumClasses = 80;

// Returns the class name, or "unknown" if classId is out of range.
const char* className(int classId);

// True for car, motorcycle, bus and truck.
bool isVehicle(int classId);

}  // namespace coco

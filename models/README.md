# Model

`yolo11n.onnx` is the YOLO11 nano detection model by Ultralytics,
exported to ONNX at 640x640. It is licensed under AGPL-3.0
(https://github.com/ultralytics/ultralytics/blob/main/LICENSE).

To export it again yourself:

    pip install ultralytics
    yolo export model=yolo11n.pt format=onnx imgsz=640
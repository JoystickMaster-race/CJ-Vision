#include "Camera.h"

namespace CJ {
  void Camera::camSetup(Image *image, Cam *cam) {

    cam->cap.open(cam->config.CamPort, cam->config.apiID);

    if (!cam->cap.isOpened()) {
      std::cout << cam->config.CamName << " Failed to open camera on port: " << cam->config.CamPort << std::endl;
      PROG::set_PROG_RUNNING(false);
    }
    
    cam->cap.set(cv::CAP_PROP_FPS, cam->config.FPS);

    cam->cap.set(cv::CAP_PROP_FRAME_HEIGHT, cam->config.ResHeight);
    cam->cap.set(cv::CAP_PROP_FRAME_WIDTH, cam->config.ResWidth);
    
    std::thread capture_t(capture, image, cam);
    capture_t.detach();
  }

  void Camera::capture(Image *image, Camera::Cam *cam) {
    cam->cap.read(image->data);
    if (cam->config.AutoExposure) {
      cam->cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75);
    } else {
      cam->cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
      cam->cap.set(cv::CAP_PROP_EXPOSURE, cam->config.Exposure);
    }
    while (PROG::THREADS_RUNNING()) {
      cam->cap.read(image->data);
      if (image->data.empty()) {
        PROG::set_PROG_RUNNING(false);
      }
    }
  }
}

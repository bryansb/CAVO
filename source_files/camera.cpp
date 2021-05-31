#include "../header_files/header.hpp"

Camera::Camera() : Frame(0){
        // set(CAP_PROP_FPS, 30);
        set(CAP_PROP_BUFFERSIZE, 4);
}
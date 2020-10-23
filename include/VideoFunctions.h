/**
 * @file   	VideoFunctions.h
 * @brief  	Includes all the function prototype
 */


/*! Include Guard */
#ifndef CAMERA_H
#define CAMERA_H

/*! CPP guard */
#ifdef camera
extern "C" {
#endif


/* --- Project Includes --- */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <fstream>
#include <string>
#include <assert.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <linux/videodev2.h>
#include <linux/videodev.h>
#include <glob.h>
#include <unistd.h>
#include <stdint.h>
#include <cstdlib>
#include <string>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <vector>

/* --- OpenCV Includes --- */

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

/**
 *  @brief List all the active cameras present
 *
 *  @return -1 If device is not found and else 0 on Success
 */

extern int ListActiveCameras();


// creating a structure for capturing image and saving it into file

struct initCapture {

    int fd;                                     // integer variable to open the camera
    char loc[100];                               //  Char Member which stores the device location
    v4l2_buffer bufferinfo;
    char* buffer;                                               // To store the buffer for frame to disk
    Mat Frame;
    std::vector<cv::Mat>frames;                                // Vector array to store the Mat frames

};



/**
 * @brief Parse and store JSON document into global variable
 *
 * @return int8_t 0 on SUCCESS and -1 on FAILURE
 *
 */

extern int8_t loadJsonConfig(struct initCapture *device, char* name);

/**
 *  @brief Initialises the Camera by opening the device location
 *
 *  @param  device the object
 *
 *  @return -1 on failure and fd on success
 */

extern int initCamera(struct initCapture *device);


/**
 *  @brief Reads the respective Camera settings of a particular device
 *  @param  fd integer Variable for opening camera
 *  @param  device the object
 *
 *  @return -1 on failure and 0 on success
 */

extern int ReadCameraSettings(struct initCapture *device);

/**
 *  @brief  Capture Image and save it into memory
 *  @param  device the object
 *
 *  @return 0 on success and 1 on failure
 */

extern int CaptureFrame(struct initCapture *device);


/**
 * @brief CloseCapture Closes Streaming
 * @param device
 * @return
 */

extern int CloseCapture(struct initCapture *device);
/**
 * @brief Capture frame to memory
 * 
 * @param obj Capture object 
 * @return cv::Mat Frame on success
 * @return cv::Mat Frame on Failure
 */

extern Mat FrametoMem(struct initCapture *obj);

/**
 *  @brief  Take 10 frames at a time and save it into an array
 *  @param  device the object
 *
 *
 *  @return 0 on success and 1 on failure
 */

extern int CaptureStreamtoMem(struct initCapture *device);


/**
 *  @brief  Save the frame to Disk
 *  @param  device  object
 *
 *  @return 0 
 */

extern int CaptureFrametoDisk(struct initCapture *device);


/**
 *  @brief  Save the Stream of frames to Disk
 *  @param  device  object
 *
 *  @return 0 
 */

extern int CaptureStreamtoDisk(struct initCapture *device);

#ifdef camera
}
#endif /* End of CPP guard */

/* End of Include Guard */
#endif



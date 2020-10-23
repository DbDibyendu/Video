/**
* @file VideoFunctions.cpp
* @brief
* Contains all the functions for Camera Initialisation, load JSON config
* and Capture Frame and stream of frames and save them into Disk
*
* @author Dibyendu Biswas
* @bug No bugs from my side
*/

/*
 *#####################################################################
 *  Initialization block
 *  ---------------------
 *  This block contains initialization code for this particular file.
 *  It typically contains Includes, constants or global variables used
 *  throughout the file.
 *#####################################################################
 */

/* --- Standard Includes --- */

#include "VideoFunctions.h"
#include <fstream>
#include <cstdlib>
#include <string>
#include <errno.h>

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
#include <tsanalyser/tsanalyser.h>

/* --- OpenCV Includes --- */

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

/* --- rapidjson Includes --- */

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

/* Defines for JSON file */

#define SI_CONFIG_FILE "home/video-component/config.json"                       // Location of the Json File
#define JSON_MAX_READ_BUF_SIZE 65536

using namespace rapidjson;
using namespace cv;
using namespace std;

/* Declare a JSON document. JSON document parsed will be stored in this variable */
static Document config;


/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here
 *#####################################################################
 */


/**
 *  @brief List all the active cameras present
 *
 *  @return -1 If device is not found and else 0 on Success
 */

int ListActiveCameras() {
    int i;
    glob_t globbuf;
    if (glob("/dev/v4l/by-path/*", 0, NULL, &globbuf) != 0) {
        perror("glob");
        return -1;
    }
    for (i=0; i < globbuf.gl_pathc; i++) {
        char buf[256] = {};
        if (readlink(globbuf.gl_pathv[i], buf, sizeof(buf)-1) > 0) {
            puts(buf);
        }
    }
    return 0;
}


/**
 * @brief Parse and store JSON document into global variable
 *
 * @return int8_t 0 on SUCCESS and -1 on FAILURE
 *
 */

int8_t loadJsonConfig(struct initCapture *device, char* name)
{

    char device_id[100];                        // char to store the device id

    int8_t ret = -1;
    /* Open the example.json file in read mode */
    FILE *fp = fopen(SI_CONFIG_FILE, "rb");

    if (fp != NULL) {
        /* Declare read buffer */
        char readBuffer[JSON_MAX_READ_BUF_SIZE];
        /* Declare stream for reading the example stream */
        FileReadStream frstream(fp, readBuffer, sizeof(readBuffer));
        /* Parse example.json and store it in `d` */
        config.ParseStream(frstream);

        Value& eStatus = config[name];                                      // Gets the device number from it

        strcpy(device_id,"/dev/video");

        strcat(device_id, eStatus.GetString());                             // Adding the device number in the end of the string

        strcpy(device->loc,device_id);                                       // copying the device id in device1.loc

        ret = 0;

    } else {
        fprintf(stderr,"Error Reading JSON config file: %s \n", strerror(errno));
        fclose(fp);
        return ret;
    }

    /* Close the example.json file*/
    fclose(fp);
    return ret;
}

/**
 *  @brief Initialises the Camera by opening the device location
 *  @param  device the object
 *
 *  @return -1 on failure and fd on success
 */


int initCamera(struct initCapture *device) {

    // 1.  Open the device

    device->fd = open("/dev/video0",O_RDWR);
    if(device->fd < 0) {
        perror("Failed to open device, OPEN");
        return -1;
    }
    // 2. Ask the device if it can capture frames
    v4l2_capability capability;
    if(ioctl(device->fd, VIDIOC_QUERYCAP, &capability) < 0) {
        // something went wrong... exit
        perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
        return -1;
    }

    // 3. Set Image format
    v4l2_format imageFormat= {0};
    imageFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    imageFormat.fmt.pix.width = 1024;
    imageFormat.fmt.pix.height = 1024;
    imageFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    imageFormat.fmt.pix.field = V4L2_FIELD_NONE;

    // tell the device you are using this format
    if(ioctl(device->fd, VIDIOC_S_FMT, &imageFormat) < 0) {
        perror("Device could not set format, VIDIOC_S_FMT");
        return 1;
    }

    // 4. Request Buffers from the device
    v4l2_requestbuffers requestBuffer = {0};
    requestBuffer.count = 1; // one request buffer
    requestBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // request a buffer wich we an use for capturing frames
    requestBuffer.memory = V4L2_MEMORY_MMAP;

    if(ioctl(device->fd, VIDIOC_REQBUFS, &requestBuffer) < 0) {
        perror("Could not request buffer from device, VIDIOC_REQBUFS");
        return 1;
    }


    // 5. Quety the buffer to get raw data ie. ask for the you requested buffer
    // and allocate memory for it
    v4l2_buffer queryBuffer;
    queryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queryBuffer.memory = V4L2_MEMORY_MMAP;
    queryBuffer.index = 0;
    if(ioctl(device->fd, VIDIOC_QUERYBUF, &queryBuffer) < 0) {
        perror("Device did not return the buffer information, VIDIOC_QUERYBUF");
        return 1;
    }
    // use a pointer to point to the newly created buffer
    // mmap() will map the memory address of the device to
    // an address in memory
    device->buffer = (char*)mmap(NULL, queryBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED,
                                 device->fd,queryBuffer.m.offset);
    memset(device->buffer, 0, queryBuffer.length);

    // 6. Get a frame
    // Create a new buffer type so the device knows whichbuffer we are talking about
    v4l2_buffer bufferinfo;

    memset(&device->bufferinfo, 0, sizeof(device->bufferinfo));
    device->bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    device->bufferinfo.memory = V4L2_MEMORY_MMAP;
    device->bufferinfo.index = 0;

    // Activate streaming
    int type = device->bufferinfo.type;
    if(ioctl(device->fd, VIDIOC_STREAMON, &type) < 0) {
        perror("Could not start streaming, VIDIOC_STREAMON");
        return 1;
    }

    return 0;
}


/**
 *  @brief Reads the respective Camera settings of a particular device and prints them out
 *  @param  device the object
 *
 *  @return 0 on success
 */

int ReadCameraSettings(struct initCapture *device) {

    VideoCapture cap(0);

    /* Get all the Camera Settings for the device */
    int Brightness = cap.get((int)CV_CAP_PROP_BRIGHTNESS);
    int Fps=cap.get(CAP_PROP_FPS);
    int Height=cap.get(CAP_PROP_FRAME_HEIGHT);
    int Width=cap.get(CAP_PROP_FRAME_WIDTH);
    int value = cap.get(CAP_PROP_FOURCC);

    /* reinterpret_cast for image format */
    char* Format = (char*) (&value);

    /* Print all the camera Settings  */

    cout<<"===================================="<<endl<<endl;
    cout<<"Default Height----------> "<<Height<<endl;
    cout<<"Default Width-----------> "<<Width<<endl;
    cout<<"Default Brightness------> "<<Brightness<<endl;
    cout<<"Default FPS-------------> "<<Fps<<endl;
    cout<<"Default Image format----> \'"<<Format<<"\'"<<endl<<endl;
    cout<<"===================================="<<endl;

    return 0;

}

/**
 *  @brief  Capture Image and save it into memory
 *  @param  device the object
 *
 *
 *  @return 0 on success and 1 on failure
 */

int CaptureFrame(struct initCapture *device) {


    /***************************** Begin looping here *********************/
    // Queue the buffer
    if(ioctl(device->fd, VIDIOC_QBUF, &device->bufferinfo) < 0) {
        perror("Could not queue buffer, VIDIOC_QBUF");
        return 1;
    }

    // Dequeue the buffer
    if(ioctl(device->fd, VIDIOC_DQBUF, &device->bufferinfo) < 0) {
        perror("Could not dequeue the buffer, VIDIOC_DQBUF");
        return 1;
    }
    // Frames get written after dequeuing the buffer

    cout << "Buffer has: " << (double)device->bufferinfo.bytesused / 1024
         << " KBytes of data" << endl;

    /* Convert the array data into Mat format */
    Mat rawData(1280,720,CV_8UC3, device->buffer);

    /* Decode the frames into proper format to be able to save in Mat array  */
    device->Frame=imdecode(rawData,1);

}


/**
 * @brief CloseCapture Closes Streaming
 * @param device
 * @return
 */

int CloseCapture(struct initCapture *device){

       // end streaming
    if(ioctl(device->fd, VIDIOC_STREAMOFF, &device->bufferinfo.type) < 0) {
        perror("Could not end streaming, VIDIOC_STREAMOFF");
        return 1;
    }

    close(device->fd);
}


/**
 * @brief Capture frame to memory
 * 
 * @param obj Capture object 
 * @return cv::Mat Frame on success
 * @return cv::Mat Frame on Failure
 */

Mat FrametoMem(struct initCapture *obj)
{
    /* Read frame from the capture */
    Mat frame=obj->Frame.clone();

    /*Check if the frame is empty */
    if(frame.empty()) {
        fprintf(stderr,"Error: captureFrameToMem: Frame captured is empty!\n");
    }

    return frame;
}

/**
 *  @brief  Take 10 frames at a time and save it into an array
 *  @param  fd integer Variable for opening camera
 *  @param  device the object
 *
 *  @return 0 on success and 1 on failure
 */

int CaptureStreamtoMem(struct initCapture *device) {

   /* declaring increamental for the loop  */
    int i=0;
    while(i<20) {

        /***************************** Begin looping here *********************/

        // Queue the buffer
        if(ioctl(device->fd, VIDIOC_QBUF, &device->bufferinfo) < 0) {
            perror("Could not queue buffer, VIDIOC_QBUF");
            return 1;
        }

        // Dequeue the buffer
        if(ioctl(device->fd, VIDIOC_DQBUF, &device->bufferinfo) < 0) {
            perror("Could not dequeue the buffer, VIDIOC_DQBUF");
            return 1;
        }
        /* Frames get written after dequeuing the buffer */

        /* Convert the array data into Mat format */
        Mat rawData(1280,720,CV_8UC3,device->buffer);

        /* Decode the frames into proper format to be able to save in Mat array  */
        Mat image= imdecode(rawData,1);

        /* Push the frames into Mat vector */
        device->frames.push_back(image);

        /* increament the integer */
        i++;

    }

}


/**
 *  @brief  Save the frame to Disk
 *  @param  device  object
 *
 *  @return 0
 */

int CaptureFrametoDisk(struct initCapture *device) {

    /* Declare size to store the image */
    double size=10000000;

    /* Open the file */
    ofstream outFile;
    outFile.open("webcam_output.jpeg", ios::binary| ios::app);

    /* Write the data out to file */
    outFile.write(device->buffer, (double)size);

    /* Close the file */
    outFile.close();

    return 0;
}


/**
 *  @brief  Save the Stream of frames to Disk
 *  @param  device  object
 *
 *  @return 0
 */

int CaptureStreamtoDisk(struct initCapture *device) {

    VideoWriter video("MyVideo.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cvSize(1280,720), true);

    int i=0;

    while(!device->frames[i].empty()) {

        /* Write the frames into the video file */
        video.write(device->frames[i]);

        /* increament i */
        i++;

    }

    /* Flush and close the video file */
    video.release();

    return 0;

}


/**
* @file example1.cpp
* @brief
*
*  Lists all the devices, Load device number from JSON file and print the settings of
*  selected Camera and then Initialise it and Capture Frame and save it into memory
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

#include <iostream>
#include <cstdlib>

/* --- Project Includes --- */

#include "dispay.h"
#include <QApplication>
#include "VideoFunctions.h"

/* --- OpenCV Includes --- */

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;

/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here
 *#####################################################################
 */


/**
 *  @brief
 *
 *  Lists all the devices, loads device loc from JSON and print the settings of
 *  selected Camera and then Initialise it and Capture Frame and save it into memory
 *
 *  @return 0 on success and -1 on failure
 */


int main() {

        /* creates a new object device1 */
        initCapture device1;                             

        /* Read Camera settings from kernel  */
        ReadCameraSettings(&device1);                               

        /* Initialise Camera and opens it  */
        initCamera(&device1);                                           

        /* Captures frame and save it into memory  */
        CaptureFrame(&device1);     

        Mat image=FrametoMem(&device1);

        /* Saves frame to disk */        
        CaptureFrametoDisk(&device1);  

        /*Display one image */
        DisplayImage("Name",image);

        /* Captures frame and save it into memory  */
        CaptureFrame(&device1);

        /* load the new frame into Mat image2 */
        Mat image2=FrametoMem(&device1);

        /*  Display the new frame into the new Window */
        DisplayImage("Window",image2);

        /* Stop Capturing frames */
        CloseCapture(&device1);


        return 0;
}



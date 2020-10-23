/**
* @file example2.cpp
* @brief
*   Captures to Stream of frames into OpenCV array
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
#include <string>
#include <errno.h>
#include "VideoFunctions.h"


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
 *  Captures to Stream of frames into Mat array
 *
 *  @return 0 on success and -1 on failure
 */

int main() {

    /* creates a new object device1 */
    initCapture device1;                  

    cout<<"Number of cameras available: \n";

    /*  Checks if device is present or not Prints and proceed on success else Abort */
    if(ListActiveCameras()==0) {                                             

        /* Read Camera settings from kernel  */
        ReadCameraSettings(&device1);                               

        /* Initialise Camera and opens it  */
        initCamera(&device1);                                           

        /* call function to CaptureStreamtoMem   */
        CaptureStreamtoMem(&device1);                             

        /* Saves all the frames to video file  */
        CaptureStreamtoDisk(&device1);  

         /* Stop Capturing frames */
        CloseCapture(&device1);        
                             

    }
    else
    {
        /* Prints Error if no divice is found */
        cout<<"Error:No Device Found!!!!"<<endl;             
        return -1;
    }

    return 0;

}
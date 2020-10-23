/**
* @file DisplayFunctions.cpp
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

#include <fstream>
#include <cstdlib>
#include <string>
#include <errno.h>

/* -----Project Includes ---- */

#include "dispay.h"
#include "VideoFunctions.h"
#include "ui_dispay.h"
#include <QPixmap>
#include <QApplication>


/* --- OpenCV Includes --- */

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"


using namespace cv;
using namespace std;

/* Declare a JSON document. JSON document parsed will be stored in this variable */


/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here
 *#####################################################################
 */

Dispay::Dispay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dispay)
{
    ui->setupUi(this);

}

/**
 * @brief Dispay::Window
 *  Calls the Qt operators through this function and pass the parameters of DisplayImage into it
 * @param windowname
 */

void Dispay::Window(char *windowname,cv::Mat frame){

        this->setWindowTitle(windowname);
        
        /* Load the Mat frame into Qimage format */
        QImage image = QImage((const unsigned char *) (frame.data), frame.cols, frame.rows,
                      QImage::Format_RGB888);
        /* Get the height and width of the label */
        int w = ui->label->width();
        int h = ui->label->height();
        /* Set the Image accodording to the height and width of the label */
        ui->label->setPixmap(QPixmap::fromImage(image).scaled(w,h,Qt::KeepAspectRatio));

        
}

/**
 * @brief DisplayImage
 *  The main function where the Windowname gets changed and frame gets loaded
 * @param windowname
 * @return a.exec()
 */

int DisplayImage(char *windowname,Mat frame)
{
    int argc;
    char *argv[1000];
    QApplication a(argc, argv);

    /* Declare new Object w */
    Dispay w;
    /* Call the member Function Window */
    w.Window(windowname, frame);
    /* Show the window */
    w.show();

    return a.exec();
}

Dispay::~Dispay()
{
    delete ui;
}

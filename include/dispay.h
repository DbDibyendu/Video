#ifndef DISPAY_H
#define DISPAY_H

#include <QMainWindow>

/* --- OpenCV Includes --- */

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

namespace Ui {
class Dispay;
}

class Dispay : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dispay(QWidget *parent = 0);
    ~Dispay();

private:
    Ui::Dispay *ui;

public slots:
    void Window(char *windowname,cv::Mat frame);

};

extern int DisplayImage(char *windowname,cv::Mat frame);

#endif // DISPAY_H

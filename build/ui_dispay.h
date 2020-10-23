/********************************************************************************
** Form generated from reading UI file 'dispay.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPAY_H
#define UI_DISPAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dispay
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Dispay)
    {
        if (Dispay->objectName().isEmpty())
            Dispay->setObjectName(QStringLiteral("Dispay"));
        Dispay->resize(675, 452);
        centralWidget = new QWidget(Dispay);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, -10, 671, 411));
        Dispay->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Dispay);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 675, 28));
        Dispay->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Dispay);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Dispay->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Dispay);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Dispay->setStatusBar(statusBar);

        retranslateUi(Dispay);

        QMetaObject::connectSlotsByName(Dispay);
    } // setupUi

    void retranslateUi(QMainWindow *Dispay)
    {
        Dispay->setWindowTitle(QApplication::translate("Dispay", "Dispay", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dispay: public Ui_Dispay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPAY_H

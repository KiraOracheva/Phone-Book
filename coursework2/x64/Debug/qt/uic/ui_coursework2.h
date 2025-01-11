/********************************************************************************
** Form generated from reading UI file 'coursework2.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COURSEWORK2_H
#define UI_COURSEWORK2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_coursework2Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *coursework2Class)
    {
        if (coursework2Class->objectName().isEmpty())
            coursework2Class->setObjectName(QString::fromUtf8("coursework2Class"));
        coursework2Class->resize(600, 400);
        menuBar = new QMenuBar(coursework2Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        coursework2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(coursework2Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        coursework2Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(coursework2Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        coursework2Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(coursework2Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        coursework2Class->setStatusBar(statusBar);

        retranslateUi(coursework2Class);

        QMetaObject::connectSlotsByName(coursework2Class);
    } // setupUi

    void retranslateUi(QMainWindow *coursework2Class)
    {
        coursework2Class->setWindowTitle(QApplication::translate("coursework2Class", "coursework2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class coursework2Class: public Ui_coursework2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COURSEWORK2_H

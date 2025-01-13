/********************************************************************************
** Form generated from reading UI file 'coursework3.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COURSEWORK3_H
#define UI_COURSEWORK3_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_coursework3Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *coursework3Class)
    {
        if (coursework3Class->objectName().isEmpty())
            coursework3Class->setObjectName(QString::fromUtf8("coursework3Class"));
        coursework3Class->resize(600, 400);
        menuBar = new QMenuBar(coursework3Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        coursework3Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(coursework3Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        coursework3Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(coursework3Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        coursework3Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(coursework3Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        coursework3Class->setStatusBar(statusBar);

        retranslateUi(coursework3Class);

        QMetaObject::connectSlotsByName(coursework3Class);
    } // setupUi

    void retranslateUi(QMainWindow *coursework3Class)
    {
        coursework3Class->setWindowTitle(QApplication::translate("coursework3Class", "coursework3", nullptr));
    } // retranslateUi

};

namespace Ui {
    class coursework3Class: public Ui_coursework3Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COURSEWORK3_H

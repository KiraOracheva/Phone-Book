#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_coursework3.h"

class QtWidgetsApplication : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication(QWidget* parent = nullptr);
    ~QtWidgetsApplication();

private:
    Ui::coursework3Class ui;
};

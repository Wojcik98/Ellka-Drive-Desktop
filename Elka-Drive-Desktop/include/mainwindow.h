#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "include/controller.h"
#include "include/view.h"

class Controller;
class View;

namespace Ui {
class MainWindow;
}

class MainWindow : public View {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setController(Controller *controller);
    void setFileList(QList<QStandardItem*> files);

private:
    Ui::MainWindow *ui;

    virtual void settingsButtonClicked();
};

#endif // MAINWINDOW_H

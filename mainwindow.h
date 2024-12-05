#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingswindow.h"
#include <QMainWindow>
#include <qpushbutton.h>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showContextMenu(QPushButton *button, const QPoint &pos);

private slots:
    void removeButton(QPushButton *button);

    void on_pushButton_clicked();

    void openSettings();

private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;
};
#endif // MAINWINDOW_H

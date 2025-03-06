#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingswindow.h"
#include <QMainWindow>
#include <qpushbutton.h>
#include <QMenu>
#include <QSettings>
#include <QDir>
#include <QMessageBox>

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
    void contextMenuConnectButton(QPushButton *button);
    void contextMenuChangeButton(QPushButton *button);
    void contextMenuRemoveButton(QPushButton *button);


    void openSettings();

    void on_mainWindowServerAddButton_clicked();

private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;
    VoiceClient *client;

};
#endif // MAINWINDOW_H

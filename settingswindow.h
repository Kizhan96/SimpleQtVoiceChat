#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "VoiceClient.h"
#include <QDialog>
#include <QAudioDevice>
#include <QTcpSocket>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

signals:
    void inputDeviceChanged(const QAudioDevice &device);
    void outputDeviceChanged(const QAudioDevice &device);
    void connectedToServer();

private:
    Ui::SettingsWindow *ui;
    void saveSettings();
    void connectToServer();
    VoiceClient *client;

private slots:
    // void onInputDeviceChanged(int index);
    // void onOutputDeviceChanged(int index);
    // void onConnectButtonClicked();
    void onSettingsApplyButtonClicked();
};

#endif // SETTINGSWINDOW_H

#include "settingswindow.h"
#include "./ui_settingswindow.h"
#include <QMediaDevices>
#include <QDebug>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsWindow) {
    ui->setupUi(this);

    // Инициализируем клиент
    // client = new VoiceClient("172.16.90.193", 12345, this);

    // Получаем список устройств ввода
    for (const QAudioDevice &device : QMediaDevices::audioInputs()) {
        ui->settingsInputDeviceComboBox->addItem(device.description(), QVariant::fromValue(device));
    }

    // Получаем список устройств вывода
    for (const QAudioDevice &device : QMediaDevices::audioOutputs()) {
        ui->settingsOutputDeviceComboBox->addItem(device.description(), QVariant::fromValue(device));
    }

    // Установим стандартные устройства по умолчанию
    int defaultInputIndex = ui->settingsInputDeviceComboBox->findText(QMediaDevices::defaultAudioInput().description());
    if (defaultInputIndex >= 0)
        ui->settingsInputDeviceComboBox->setCurrentIndex(defaultInputIndex);

    int defaultOutputIndex = ui->settingsOutputDeviceComboBox->findText(QMediaDevices::defaultAudioOutput().description());
    if (defaultOutputIndex >= 0)
        ui->settingsOutputDeviceComboBox->setCurrentIndex(defaultOutputIndex);

}

SettingsWindow::~SettingsWindow() {
    delete ui;
}

void SettingsWindow::onSettingsApplyButtonClicked()
{
    // Получаем выбранные устройства
    QAudioDevice inputDevice = ui->settingsInputDeviceComboBox->currentData().value<QAudioDevice>();
    QAudioDevice outputDevice = ui->settingsOutputDeviceComboBox->currentData().value<QAudioDevice>();

    // Передаем их в клиент
    client->setInputDevice(inputDevice);
    client->setOutputDevice(outputDevice);

    qDebug() << "Audio settings applied: Input ->" << inputDevice.description()
             << ", Output ->" << outputDevice.description();
}


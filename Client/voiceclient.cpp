#include "VoiceClient.h"
#include <QAudioFormat>
#include <QMediaDevices>
#include <QDebug>

VoiceClient::VoiceClient(const QString &serverAddress, quint16 port, QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this)), audioSource(nullptr), audioSink(nullptr) {
    // Подключение к серверу
    socket->connectToHost(serverAddress, port);
    if (!socket->waitForConnected(3000)) {
        qCritical() << "Failed to connect to server!";
        emit connectionStatusChanged(false); // Сообщаем о неудачном подключении
        return;
    }

    emit connectionStatusChanged(true); // Сообщаем об успешном подключении
    // Установка стандартных устройств
    inputDevice = QMediaDevices::defaultAudioInput();
    outputDevice = QMediaDevices::defaultAudioOutput();

    // Создаем аудиоисточник и приемник
    setInputDevice(inputDevice);
    setOutputDevice(outputDevice);

    // Обработка данных, полученных от сервера
    connect(socket, &QTcpSocket::readyRead, this, &VoiceClient::onIncomingData);
}

VoiceClient::~VoiceClient() {
    if (audioSource)
        audioSource->stop();
    if (audioSink)
        audioSink->stop();
    socket->disconnectFromHost();
}

void VoiceClient::setInputDevice(const QAudioDevice &device) {
    inputDevice = device;

    // Настройка формата аудиоданных
    QAudioFormat format = inputDevice.preferredFormat();
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16); // Заменяет setSampleType

    // Пересоздаем аудиоисточник
    delete audioSource;
    audioSource = new QAudioSource(inputDevice, format, this);

    // Подключаем поток данных к серверу
    audioInputDevice = audioSource->start();
    connect(audioInputDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray audioData = audioInputDevice->readAll();
        socket->write(audioData); // Отправляем данные на сервер
    });
}

void VoiceClient::setOutputDevice(const QAudioDevice &device) {
    outputDevice = device;

    // Настройка формата аудиоданных
    QAudioFormat format = outputDevice.preferredFormat();
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16); // Заменяет setSampleType

    // Пересоздаем аудиоприемник
    delete audioSink;
    audioSink = new QAudioSink(outputDevice, format, this);

    // Подключаем поток данных для воспроизведения
    audioOutputDevice = audioSink->start();
}

void VoiceClient::onIncomingData() {
    QByteArray data = socket->readAll();
    audioOutputDevice->write(data); // Воспроизводим полученные данные
}

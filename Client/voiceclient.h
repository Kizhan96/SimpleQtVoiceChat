#ifndef VOICECLIENT_H
#define VOICECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAudioSource>
#include <QAudioSink>
#include <QAudioDevice>
#include <QIODevice>

class VoiceClient : public QObject {
    Q_OBJECT

public:
    explicit VoiceClient(const QString &serverAddress, quint16 port, QObject *parent = nullptr);
    ~VoiceClient();

    void setInputDevice(const QAudioDevice &device);  // Установить микрофон
    void setOutputDevice(const QAudioDevice &device); // Установить устройство воспроизведения
signals:
    void connectionStatusChanged(bool success); // Сигнал изменения состояния подключения

private:
    QTcpSocket *socket;
    QAudioSource *audioSource;
    QAudioSink *audioSink;
    QIODevice *audioInputDevice;
    QIODevice *audioOutputDevice;

    QAudioDevice inputDevice;   // Текущий микрофон
    QAudioDevice outputDevice;  // Текущее устройство воспроизведения

private slots:
    void onIncomingData();
};

#endif // VOICECLIENT_H

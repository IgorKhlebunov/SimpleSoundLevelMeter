#pragma once
#include <QObject>
#include <qaudioinput.h>

class SoundWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float db READ db NOTIFY dbChanged)
    Q_PROPERTY(QStringList inputDevices READ inputDevices NOTIFY inputDevicesChanged)
public:
    explicit SoundWrapper(QObject *parent = 0);
    ~SoundWrapper() override;
    void initUi();
    Q_INVOKABLE void changeInputDevice(const QString &name);
    Q_INVOKABLE void updateInputDevices();

private:
    void init();
    void createAudioInput();
    inline float db() const { return m_db; }
    float calculateDecibels(qint16 *data, qint32 dataSize);
    QStringList inputDevices() const;

Q_SIGNALS:
    void setQmlVariantProperty(const QString &name, const QVariant &value);
    void setQmlObjectProperty(const QString &name, QObject *value);
    void dbChanged(float level);
    void sendToServer(const QString &message);
    void inputDevicesChanged();

public Q_SLOTS:
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

private Q_SLOTS:
    void readMore();

private:
    QAudioDeviceInfo m_Inputdevice;
    QAudioFormat m_format;
    QAudioInput *m_audioInput = nullptr;
    QIODevice *m_input = nullptr;
    float m_db = 0.0f;
};

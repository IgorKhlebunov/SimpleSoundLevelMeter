#pragma once
#include <QObject>
#include <QByteArray>
#include <qaudioinput.h>
#include<qaudiooutput.h>
#include <QBuffer>

class SoundWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float db READ db NOTIFY dbChanged)
public:
    explicit SoundWrapper(QObject *parent = 0);
    ~SoundWrapper() override;

private:
    void init();
    void createAudioInput();
    inline float db() const { return m_db; }
signals:
    void setQmlVariantProperty(const QString &name, const QVariant &value);
    void setQmlObjectProperty(const QString &name, QObject *value);
    void dbChanged(float level);

public slots:
    void start();
private slots:
    void readMore();
private:
    QAudioDeviceInfo m_Inputdevice;
    QAudioFormat m_format;
    QAudioInput *m_audioInput = nullptr;
    QIODevice *m_input = nullptr;
    QByteArray m_buffer;
    int m_iVolume = 0;
    float m_db = 0.0f;
};

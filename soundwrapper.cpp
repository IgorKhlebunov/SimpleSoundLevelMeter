#include "soundwrapper.h"
#include "settings.h"
#include <math.h>
#include <QByteArray>

#include <QDebug>

SoundWrapper::SoundWrapper(QObject *parent)
    : QObject(parent)
    ,   m_Inputdevice(QAudioDeviceInfo::defaultInputDevice())
{
    const auto deviceName = Settings::instance().deviceName();

    if (m_Inputdevice.deviceName() != deviceName) {
        foreach (const auto dev, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
            if (dev.deviceName() == deviceName) {
                m_Inputdevice = dev;
                break;
            }
        }
    }

    init();
}

SoundWrapper::~SoundWrapper()
{
    if (m_audioInput->state() != QAudio::StoppedState)
        m_audioInput->stop();

    delete m_audioInput;
}

void SoundWrapper::init()
{
    m_format.setSampleRate(Settings::instance().rate());
    m_format.setChannelCount(Settings::instance().channelCount());
    m_format.setSampleSize(Settings::instance().sampleSize());
    m_format.setSampleType(QAudioFormat::UnSignedInt );
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec(Settings::instance().codec());

    QAudioDeviceInfo infoIn(m_Inputdevice);

    if (!infoIn.isFormatSupported(m_format))
        m_format = infoIn.nearestFormat(m_format);

    createAudioInput();
}

void SoundWrapper::createAudioInput()
{
    if (m_input) {
        disconnect(m_input, nullptr, this, nullptr);
        m_input = nullptr;
    }

    m_audioInput = new QAudioInput(m_Inputdevice, m_format, this);
}

void SoundWrapper::start()
{
    emit setQmlObjectProperty("wrapper", this);
    m_input = m_audioInput->start();
    connect(m_input, &QIODevice::readyRead, this, &SoundWrapper::readMore);
}

void SoundWrapper::readMore()
{
    if (!m_audioInput)
        return;

    const qint64 len = m_audioInput->bytesReady();
    QByteArray buffer(len, 0);

    const qint64 l = m_input->read(buffer.data(), len);

    if (l <= 0)
        return;

    short *data = reinterpret_cast<short*>(buffer.data());
    float sum = 0.0f;

    for (qint64 i = 0; i < len/2; ++i) {
        float sample = data[i] / 32768.0f;
        sum += (sample * sample);
    }

    const double rms = sqrt(sum / (len/2));
    m_db = 20 * log10(rms);

    emit dbChanged(m_db);
    emit sendToServer(QString::number(m_db));
}

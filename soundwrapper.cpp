#include "soundwrapper.h"
#include <math.h>
#include <QDebug>

static const int bufferSize = 14096;

SoundWrapper::SoundWrapper(QObject *parent)
    : QObject(parent)
    ,   m_Inputdevice(QAudioDeviceInfo::defaultInputDevice())
    ,   m_buffer(bufferSize, 0)
{
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
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::UnSignedInt );
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());

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

    qint64 len = m_audioInput->bytesReady();

    if (len > 4096)
        len = 4096;

    qint64 l = m_input->read(m_buffer.data(), len);

    if (l <= 0)
        return;

    short* resultingData = reinterpret_cast<short*>(m_buffer.data());

    float sum = 0;
    for (qint64 i = 0; i < l; ++i) {
        float sample = resultingData[i] / 32768.0f;
        sum += (sample * sample);
    }

    double rms = sqrt(sum / l);
    m_db = 20 * log10(rms);

    emit dbChanged(m_db);
}

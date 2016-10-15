#include "soundwrapper.h"
#include "settings.h"
#include <math.h>
#include <QByteArray>

#include <QDebug>

static const float maxValue = 32768.0f;

SoundWrapper::SoundWrapper(QObject *parent)
    : QObject(parent)
    , m_Inputdevice(QAudioDeviceInfo::defaultInputDevice())
    , m_timer(this)
{
    init();
}

SoundWrapper::~SoundWrapper()
{
    if (!m_audioInput)
        return;

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

    m_timer.setInterval(300);
    connect(&m_timer, &QTimer::timeout, this, [this] () {
        if (m_count)
            emit sendToServer(QString::number(m_sumPercents/m_count));
        m_count = 0;
        m_sumPercents = 0;
    });
}

void SoundWrapper::initUi()
{
    emit setQmlObjectProperty("wrapper", this);
}

void SoundWrapper::createAudioInput()
{
    if (m_input) {
        disconnect(m_input, nullptr, this, nullptr);
        m_input = nullptr;
    }

    m_audioInput = new QAudioInput(m_Inputdevice, m_format, this);
}

float SoundWrapper::calculateDecibels(qint16 *data, qint32 dataSize)
{
    float sum = 0.0f;

    for (qint32 i = 0; i < dataSize; ++i) {
        float sample = data[i] / maxValue;
        sum += (sample * sample);
    }

    const qreal rms = sqrt(sum / dataSize);

    return (20 * log10(rms));
}

QStringList SoundWrapper::inputDevices() const
{
    QStringList list;

    foreach (const auto dev, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        list << dev.deviceName();
    }

    return list;
}

void SoundWrapper::changeInputDevice(const QString &name)
{
    if (m_Inputdevice.deviceName() != name) {
        foreach (const auto dev, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
            if (dev.deviceName() == name) {
                m_Inputdevice = dev;
                break;
            }
        }
    }
}

void SoundWrapper::updateInputDevices()
{
    emit inputDevicesChanged();
}

void SoundWrapper::start()
{
    QAudioDeviceInfo infoIn(m_Inputdevice);

    if (!infoIn.isFormatSupported(m_format))
        m_format = infoIn.nearestFormat(m_format);

    createAudioInput();

    m_input = m_audioInput->start();
    connect(m_input, &QIODevice::readyRead, this, &SoundWrapper::readMore);

    m_timer.start();
}

void SoundWrapper::stop()
{
    m_timer.stop();
    m_count = 0;
    m_sumPercents = 0;
    m_audioInput->stop();

    if (m_input) {
        disconnect(m_input, nullptr, this, nullptr);
        m_input = nullptr;
        delete m_audioInput;
        m_audioInput = nullptr;
    }
}

void SoundWrapper::readMore()
{
    if (!m_audioInput)
        return;

    const qint64 len = m_audioInput->bytesReady();
    QByteArray buffer(len, 0);

    if (m_input->read(buffer.data(), len) <= 0)
        return;

    m_db = calculateDecibels(reinterpret_cast<short*>(buffer.data()), len/2);

    emit dbChanged(m_db);

    qint32 percents = (qint32)(m_db * 10) + 100;

    if (percents < 0)
        percents = 0;

    if (percents > 100)
        percents = 100;

    m_sumPercents += percents;
    ++m_count;
    //    emit sendToServer(QString::number(procents));
}

void SoundWrapper::timeOut()
{

}

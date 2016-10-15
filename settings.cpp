#include "settings.h"

Settings &Settings::instance()
{
    static Settings instance;

    return instance;
}

qint32 Settings::port() const
{
    return m_settings.value("Connection/port", 1234).toInt();
}

qint32 Settings::rate() const
{
    return m_settings.value("Sound/rate", 8000).toInt();
}

qint32 Settings::channelCount() const
{
    return m_settings.value("Sound/channel_count", 1).toInt();
}

qint32 Settings::sampleSize() const
{
    return m_settings.value("Sound/sample_size", 16).toInt();
}

QString Settings::codec() const
{
    return m_settings.value("Sound/codec", "audio/pcm").toString();
}

QString Settings::deviceName() const
{
    return m_settings.value("Sound/device", "H2n").toString();
}

Settings::Settings()
    : m_settings("config.ini", QSettings::IniFormat)
{
}

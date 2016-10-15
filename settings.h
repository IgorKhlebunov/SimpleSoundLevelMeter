#pragma once
#include <QSettings>

class Settings
{
public:
    static Settings &instance();

    qint32 port() const;
    qint32 rate() const;
    qint32 channelCount() const;
    qint32 sampleSize() const;
    QString codec() const;
    QString deviceName() const;

private:
    Settings();
    QSettings m_settings;
};

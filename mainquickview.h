#pragma once

#include <QObject>
#include <QQuickView>
#include <QQmlContext>

class MainQuickView : public QQuickView
{
    Q_OBJECT
public:
    explicit MainQuickView(QWindow *parent = nullptr);

public slots:
    void setQmlVariantProperty(const QString &name, const QVariant &value);
    void setQmlObjectProperty(const QString &name, QObject *value);
private:
    template<class T>
    void setQmlPrivateProperty(const QString &name, const T& value) {
        auto root = rootContext();
        root->setContextProperty(name, value);
    }

private:
    void init();
};

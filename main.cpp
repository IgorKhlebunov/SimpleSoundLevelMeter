#include <QApplication>
#include <QQmlApplicationEngine>
#include "mainquickview.h"
#include "soundwrapper.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SoundWrapper soundWrapper;
    Server server;

    MainQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.resize(720, 720);

    QObject::connect(&soundWrapper, &SoundWrapper::setQmlObjectProperty,
                     &view, &MainQuickView::setQmlObjectProperty);
    QObject::connect(&soundWrapper, &SoundWrapper::setQmlVariantProperty,
                     &view, &MainQuickView::setQmlVariantProperty);
    QObject::connect(&soundWrapper, &SoundWrapper::sendToServer,
                     &server, &Server::sendToClient);

    soundWrapper.start();
    view.show();
    return app.exec();
}

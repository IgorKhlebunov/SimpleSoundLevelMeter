#include <QApplication>
#include "mainquickview.h"
#include "soundwrapper.h"
#include "server.h"

static const int defaultHeight = 720;
static const int defaultWidth = 720;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainQuickView view;
    SoundWrapper soundWrapper;
    Server server;

    QObject::connect(&soundWrapper, &SoundWrapper::setQmlObjectProperty,
                     &view, &MainQuickView::setQmlObjectProperty);
    QObject::connect(&soundWrapper, &SoundWrapper::sendToServer,
                     &server, &Server::sendToClient);

    view.setQmlObjectProperty("server", &server);
    soundWrapper.initUi();
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.resize(defaultWidth, defaultHeight);
    view.show();

    return app.exec();
}

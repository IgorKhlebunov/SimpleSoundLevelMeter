#include "server.h"
#include "settings.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QDebug>

QT_USE_NAMESPACE

Server::Server(QObject *parent)
        : QObject(parent)
        , m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Sound Server"),
                                                  QWebSocketServer::NonSecureMode, this))
        , m_clients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, Settings::instance().port())) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }
}

Server::~Server()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void Server::sendToClient(const QString &message)
{
    foreach (const auto &c, m_clients) {
        c->sendTextMessage(message);
    }
}

void Server::onNewConnection()
{
    qDebug() << __func__;
    auto socket = m_pWebSocketServer->nextPendingConnection();

    emit consoleMessage("New connection!");

    connect(socket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(socket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    m_clients << socket;
}

void Server::processTextMessage(const QString &message)
{
    qDebug() << __func__ << message;

    auto client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        qDebug() << __func__ << "try to send message";
        client->sendTextMessage(message + " from server");
    }
}

void Server::processBinaryMessage(const QByteArray &message)
{
    qDebug() << __func__ << message;
    auto client = qobject_cast<QWebSocket *>(sender());

    if (client) {
        qDebug() << __func__ << "try to send message";
        client->sendBinaryMessage(message);
    }
}

void Server::socketDisconnected()
{
    qDebug() << __func__;
    emit consoleMessage("disconnect!");

    auto client = qobject_cast<QWebSocket *>(sender());

    if (client) {
        m_clients.removeAll(client);
        client->deleteLater();
    }
}

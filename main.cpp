#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mpris2.h"

Mpris2Player::Mpris2Player(QString serviceName)
    : QDBusInterface(serviceName,
                     "/org/mpris/MediaPlayer2",
                     "org.mpris.MediaPlayer2.Player") {
        qDebug() << "Connect to player :" << serviceName;
}

Mpris2Player::~Mpris2Player() {
        qDebug() << "Disonnect from player :" << service();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Mpris2Connection mediaControler;
    mediaControler.playerList.first()->Next();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

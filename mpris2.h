#ifndef MPRIS2
#define MPRIS2

#include <QDBusServiceWatcher>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QDBusConnectionInterface>

class Mpris2Player : public QDBusInterface
{
    Q_OBJECT
public:
    Mpris2Player(QString serviceName);
    ~Mpris2Player();
    void Play() { this->call("Play"); }
    void PlayPause() { this->call("PlayPause"); }
    void Next() { this->call("Next"); }
    void Previous() { this->call("Previous"); }
};

class Mpris2Connection : public QObject
{
    Q_OBJECT
    QDBusServiceWatcher *watcher;
public:
    QList<Mpris2Player*> playerList;
    Mpris2Connection() {
        QDBusConnection bus = QDBusConnection::sessionBus();
        const QStringList services = bus.interface()->registeredServiceNames();

        foreach(QString name, services.filter("org.mpris.MediaPlayer2")) {
            playerList.append(new Mpris2Player(name));
        }
        watcher = new QDBusServiceWatcher(QString(), bus);

        connect(watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
                this, SLOT(serviceOwnerChanged(QString,QString,QString)));
    }
    ~Mpris2Connection() {
        while (!playerList.isEmpty())
            delete playerList.takeFirst();
    }
private slots:
    void serviceOwnerChanged(const QString &serviceName,
                             const QString &oldOwner,
                             const QString &newOwner) {
        if (oldOwner.isEmpty() && serviceName.startsWith("org.mpris.MediaPlayer2.")) {
            playerList.append(new Mpris2Player(serviceName));
        } else if (newOwner.isEmpty() && serviceName.startsWith("org.mpris.MediaPlayer2.")) {
            for (int i = 0; i < playerList.size(); ++i) {
                if (playerList.at(i)->service() == serviceName) {
                    delete playerList.at(i);
                    playerList.removeAt(i);
                }
            }
        }
    }
};

#endif // MPRIS2


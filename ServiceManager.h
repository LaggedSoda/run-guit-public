#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <QString>
#include <QVector>
#include <QMap>

enum class ServiceStatus {
    Running,
    Down,
    Error,
    Restarting,
    Starting,
    Stopping,
    Unknown
};

struct ServiceInfo {
    QString name;
    ServiceStatus status;
};

class ServiceManager {
public:
    QVector<ServiceInfo> getAllServices();
    ServiceStatus getServiceStatus(const QString& serviceName);

private:
    QString serviceDir = "/var/service"; // alternatively, /etc/sv all of them but aren't linked'
};

#endif // SERVICEMANAGER_H

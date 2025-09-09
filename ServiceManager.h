#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <QString>
#include <QVector>
#include <QMap>

enum class ServiceStatus {
    Running,
    Down,
    Error,
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
    QString serviceDir = "/etc/sv";
};

#endif // SERVICEMANAGER_H

#include "ServiceManager.h"
#include <QDir>
#include <QProcess>

QVector<ServiceInfo> ServiceManager::getAllServices() {
    QVector<ServiceInfo> services;
    QDir dir(serviceDir);

    for (const QString& entry : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        ServiceInfo info;
        info.name = entry;
        info.status = getServiceStatus(entry);
        services.append(info);  // <-- was service.append(info);
    }

    return services;
}

ServiceStatus ServiceManager::getServiceStatus(const QString& serviceName) {
    QProcess process;
    process.start("sv", { "status", serviceDir + "/" + serviceName });
    process.waitForFinished();

    QString output = process.readAllStandardOutput();

    if (output.contains("run:"))                       return ServiceStatus::Running;
    if (output.contains("down:"))                      return ServiceStatus::Down;
    if (output.contains("supervise not running"))      return ServiceStatus::Error;

    return ServiceStatus::Unknown;
}

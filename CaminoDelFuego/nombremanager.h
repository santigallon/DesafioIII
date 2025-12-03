#ifndef NOMBREMANAGER_H
#define NOMBREMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include "npc.h"

class NombreManager : public QObject {
    Q_OBJECT
public:
    explicit NombreManager(QObject* parent = nullptr);

    bool cargarJSON(const QString& ruta);

    QString nombreAleatorio(Faccion faccion);
    QString nombreRomanoCompleto();

private:
    QJsonObject m_data;

    QString randomFromArray(const QString& key);
};

#endif

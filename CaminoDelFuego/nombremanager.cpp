#include "NombreManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRandomGenerator>
#include "npc.h"

NombreManager::NombreManager(QObject* parent)
    : QObject(parent)
{
}

bool NombreManager::cargarJSON(const QString& ruta) {
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    auto raw = file.readAll();
    auto doc = QJsonDocument::fromJson(raw);
    if (!doc.isObject())
        return false;

    m_data = doc.object();
    return true;
}

QString NombreManager::randomFromArray(const QString& key) {
    if (!m_data.contains(key))
        return "Desconocido";

    QJsonArray arr = m_data[key].toArray();
    if (arr.isEmpty())
        return "Vacío";

    int idx = QRandomGenerator::global()->bounded(arr.size());
    return arr[idx].toString();
}

QString NombreManager::nombreRomanoCompleto() {
    QString pra = randomFromArray("romanos_praenomen");
    QString nom = randomFromArray("romanos_nomen");
    QString cog = randomFromArray("romanos_cognomen");

    return pra + " " + nom + " " + cog;
}

QString NombreManager::nombreAleatorio(Faccion faccion) {
    switch (faccion) {
    case Faccion::JUDIO:
        return randomFromArray("judios");

    case Faccion::CRISTIANO: {
        int r = rand() % 3;
        return (r == 0) ? "judios"
               : (r == 1) ? "griegos"
                          : nombreRomanoCompleto();
    }

    case Faccion::POSEIDO:
        return randomFromArray("demonios");

    case Faccion::ROMANO:
        return nombreRomanoCompleto();

    case Faccion::ENDEMONIADO:
        return randomFromArray("griegos");

    case Faccion::PAGANO:
    {
        int r = rand() % 3;
        return (r == 0) ? "judios"
               : (r == 1) ? "griegos"
                          : nombreRomanoCompleto();
    }
    }

    return "SinNombre";
}

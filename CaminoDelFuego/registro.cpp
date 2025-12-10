#include "registro.h"
#include <QDebug>

Registro::Registro(QObject* parent)
    : QObject(parent)
{}

void Registro::agregar(const QString& texto) {
    eventos.append(texto);
    emit nuevoMensaje(texto);
}

const QList<QString>& Registro::lista() const {
    return eventos;
}
void Registro::log(const QString& texto) {
    qDebug() << "[REGISTRO] " << texto;
    emit nuevoEvento(texto);
}

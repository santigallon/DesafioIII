#include "registro.h"

Registro::Registro(QObject* parent)
    : QObject(parent)
{
}

void Registro::agregar(const QString& texto) {
    eventos.append(texto);
    emit nuevoMensaje(texto);
}

const QList<QString>& Registro::lista() const {
    return eventos;
}

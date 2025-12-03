#ifndef REGISTRO_H
#define REGISTRO_H

#include <QObject>
#include <QString>
#include <QList>

class Registro : public QObject {
    Q_OBJECT

public:
    explicit Registro(QObject* parent = nullptr);
    void agregar(const QString& texto);
    const QList<QString>& lista() const;

signals:
    void nuevoMensaje(const QString& texto);

private:
    QList<QString> eventos;
};

#endif // REGISTRO_H

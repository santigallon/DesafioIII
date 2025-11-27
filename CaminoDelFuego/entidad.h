#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QString>

class Entidad : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Entidad(QGraphicsItem* parent = nullptr);
    virtual ~Entidad() override;

    // Atributos básicos
    QString nombre() const;
    void setNombre(const QString& n);

    QPointF posicion() const;
    void setPosicion(const QPointF& p);

    int vida() const;
    void setVida(int v);
    bool estaVivo() const;

    // Métodos virtuales para sobreescribir
    virtual void actualizar(float dt);   // lógica por frame
    virtual void interactuar(Entidad* otro); // interacción genérica

    // Acciones básicas
    virtual void recibirDanio(int cantidad, const QString& razon = QString());
    virtual void curar(int cantidad);

signals:
    void murio(Entidad* quien);
    void vidaCambiada(int nuevaVida);

protected:
    QString m_nombre;
    int m_vida;
    bool m_vivo;
};

#endif // ENTIDAD_H

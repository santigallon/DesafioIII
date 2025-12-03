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

    bool esJugador(Entidad* e);
    float distanciaA(Entidad* e) const;

    void moverPor(float dx, float dy);
    void moverHacia(const QPointF& destino, float velocidad);

    void emitirDialogo(const QString& texto);
    void emitirEfectoLuz(const QColor& color);

    void modificarVida(int delta);
    void moverAleatorio(float velocidad);

signals:
    void murio(Entidad* quien);
    void vidaCambiada(int nuevaVida);
    void dialogo(const QString& texto);
    void efectoLuz(const QColor& color);

protected:
    QString m_nombre;
    int m_vida;
    bool m_vivo;
};

#endif // ENTIDAD_H

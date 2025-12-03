


#ifndef POSEIDO_H
#define POSEIDO_H

#include "Endemoniado.h"
#include <QString>

/**
 * Poseido - subclase de Endemoniado con demonio dominante.
 */
class Poseido : public Endemoniado {
    Q_OBJECT
public:
    explicit Poseido(QGraphicsItem* parent = nullptr);
    ~Poseido() override;

    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;

    void desatarPoder();
    void setNombreDemonio(const QString& nombre);

private:
    QString m_nombreDemonio;
    int m_rabia;
};
#endif // POSEIDO_H

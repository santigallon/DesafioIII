#ifndef NPC_H
#define NPC_H

#include "Entidad.h"
#include <QString>

/**
 * NPC - base para cualquier personaje no jugable.
 * Define facción y comportamiento básico.
 */
enum class Faccion { PAGANO, JUDIO, CRISTIANO, ROMANO, ENDEMONIADO, POSEIDO, ANGEl};

class NPC : public Entidad {
    Q_OBJECT
public:
    explicit NPC(Faccion f = Faccion::PAGANO, QGraphicsItem* parent = nullptr);
    ~NPC() override;

    Faccion faccion() const;
    int nivelPecado() const;
    bool esHostil() const;
    void setHostil(bool state);

    // Lógica base
    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;

    // Transformaciones
    virtual void transformarA(Faccion nuevaFaccion);

signals:
    void cambioDeFaccion(NPC* quien, Faccion nuevaFaccion);

protected:
    Faccion m_faccion;
    int m_nivelPecado;
    bool m_hostil;
};
#endif // NPC_H


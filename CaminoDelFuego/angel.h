#ifndef ANGEL_H
#define ANGEL_H
#include <QString>

class Angel
{
private:
    QString mensage;
    bool visiblePlayer;

public:
    void actualizar (float deltaTime);
    void interactuar();
    void mensajeParaPlayer();
};

#endif // ANGEL_H

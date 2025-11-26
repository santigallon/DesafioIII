#ifndef ROMANO_H
#define ROMANO_H

class Romano
{
private:
    int autoridadLevel;
    bool estaPersiguiendo;

public:
    void actualizar(float deltaTime);
    void interactua();
    void persigue(Player* player);
    void stopPersecucion();
};

#endif // ROMANO_H

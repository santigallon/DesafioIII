#ifndef ENDEMONIADO_H
#define ENDEMONIADO_H

class Endemoniado
{
protected:
    int possessionLevel;
    bool exorcised;

public:
    void update(floatdeltaTime);
    void interact();
    void possess(int level);
    void exorcise();
};

#endif // ENDEMONIADO_H

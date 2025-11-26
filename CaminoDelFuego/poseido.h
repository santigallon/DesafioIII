#ifndef POSEIDO_H
#define POSEIDO_H
#include <QString>
class poseido
{
protected:
    QString demonName;
    int rageLevel;

public:
    void update(float deltaTime);
    void unleashPower();
};

#endif // POSEIDO_H

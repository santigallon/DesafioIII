#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QString>
#include <QPointF>

class Entidad
{
protected:
    QString name;
    QPointF position;
    int health;
    bool alive;

public:
    QString getName();
    QPointF getPosition();
    bool isAlive();
    void setPosition(QPointF newPos);
    void takeDamage(int amount);
    void heal(int amount);
};

#endif // ENTIDAD_H

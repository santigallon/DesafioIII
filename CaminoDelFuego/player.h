#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <QPointF>
class Player
{
private:
    void onRegenTick();
    void onStateTimerTimeout();
    void enterState(const QString& st, float duration = 0.0f);
    void leaveCurrentState();
    void clampSpirituals();
    void tryProvidenceIfNeeded();
    void onExorcismComplete();
    void onExorcismFail();
    void onCartaComplete(const QString& contenido);
    void onCartaFail();
    void onOratoriaSuccess();
    void onOratoriaFail();

public:
    void updateEntity(float dt);
    void moveByInput(bool up, bool down, bool left, bool right, float dt);
    void pray(int intensidad = 1);
    void rest(float seconds);
    void interactWith(NPC* npc, bool help);
    void throwProjectile(const QPointF& dir);
    void startExorcism();
    void startWriting(const QString& texto);
    void startOratory(const QString& tema);
    void receiveProvidence();
    void applyDamage(int amount, const QString& reason = QString());
    void heal(int amount);
    void increaseFe(double amount);
    void decreaseFe(double amount);
    void addPecado(double amount);
    void reducePecado(double amount);
    bool isFaithHigh(double threshold = 75.0);
    bool isCorrupted(double threshold = 60.0);
    void healthChanged(int newHealth);
    void energyChanged(float newEnergy);
    void faithChanged(double newFaith);
    void pecadoChanged(double newPecado);
    void stateChanged(const QString& newState);
    void exorcismSucceeded();
    void exorcismFailed();
    void cartaCompleted(const QString& contenido);
    void oratoriaSucceeded();
    void oratoriaFailed();
    void died();

};

#endif // PLAYER_H

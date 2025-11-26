#ifndef CRISTIANO_H
#define CRISTIANO_H


class Cristiano
{
private:
    bool convertedRecently;
    int prayerStrength;

public:
    void update(float deltaTime);
    void interact() ;
    void prayFor(Player* player);
};

#endif // CRISTIANO_H

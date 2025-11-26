#ifndef JUDIO_H
#define JUDIO_H

class Judio
{
private:
    bool fariseo;
    bool curiocidad;

public:
    void update(float deltaTime);
    void interact();
    void discussScripture(Player* player);
};

#endif // JUDIO_H

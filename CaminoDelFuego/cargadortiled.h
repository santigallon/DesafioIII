#ifndef CARGADORTILED_H
#define CARGADORTILED_H

#include <QString>
#include <vector>

class CargadorTiled {
public:
    static bool cargarJSON(
        const QString& ruta,
        std::vector<std::vector<int>>& matriz,
        int& width,
        int& height,
        int& tileW,
        int& tileH
        );
};

#endif //CARGADORTILED_H


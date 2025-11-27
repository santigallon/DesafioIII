#ifndef GESTORSONIDO_H
#define GESTORSONIDO_H

#pragma once
#include <QObject>
#include <QMap>
#include <QString>
#include <QMediaPlayer>
#include <QSoundEffect>

/**
 * GestorSonido
 * - Carga y reproduce efectos cortos (QSoundEffect) y música de fondo (QMediaPlayer).
 * - API simple para pedir reproducción por nombre.
 *
 * NOTA: para compilar con QSoundEffect y QMediaPlayer asegúrate de tener enlazadas las
 * bibliotecas de multimedia en tu proyecto (.pro o CMake).
 */

class GestorSonido : public QObject {
    Q_OBJECT
public:
    explicit GestorSonido(QObject* parent = nullptr);
    ~GestorSonido() override;

    // carga un efecto simple
    void cargarEfecto(const QString& nombre, const QString& ruta);
    // reproduce efecto al instante
    void reproducirEfecto(const QString& nombre, float volumen = 1.0f);

    // música de fondo (un canal)
    void reproducirMusica(const QString& ruta, bool loop = true);
    void detenerMusica();

    // atajos semánticos
    void inicioExorcismo();
    void inicioOratoria();
    void inicioSanacion();
    void sonidoAmbiente(const QString& zona);

private:
    QMap<QString, QSoundEffect*> m_efectos;
    QMediaPlayer* m_musicPlayer;
};


#endif // GESTORSONIDO_H

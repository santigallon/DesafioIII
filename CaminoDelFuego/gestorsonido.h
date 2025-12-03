#ifndef GESTORSONIDO_H
#define GESTORSONIDO_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QHash>
#include <QString>

class GestorSonido : public QObject {
    Q_OBJECT
public:
    explicit GestorSonido(QObject* parent = nullptr);
    ~GestorSonido();

    // CARGA DE EFECTOS
    void cargarEfecto(const QString& nombre, const QString& ruta);

    // REPRODUCCIÓN DE EFECTOS
    void reproducirEfecto(const QString& nombre, float volumen = 1.0f);

    // MÚSICA DE FONDO
    void reproducirMusica(const QString& ruta, bool loop = true);
    void detenerMusica();

    // EVENTOS ESPECIALES
    void inicioExorcismo();
    void inicioOratoria();
    void inicioSanacion();

    // AUDIO SEGÚN ZONA
    void sonidoAmbiente(const QString& zona);

private:
    // Música
    QMediaPlayer* m_musicPlayer;
    QAudioOutput* m_musicOutput;

    // Efectos
    QHash<QString, QMediaPlayer*> m_efectos;
};

#endif // GESTORSONIDO_H

#include "GestorSonido.h"
#include <QUrl>

GestorSonido::GestorSonido(QObject* parent)
    : QObject(parent)
{
    m_musicOutput = new QAudioOutput(this);
    m_musicPlayer = new QMediaPlayer(this);
    m_musicPlayer->setAudioOutput(m_musicOutput);
    m_musicOutput->setVolume(0.4f);
}

GestorSonido::~GestorSonido() {
    qDeleteAll(m_efectos);
    m_efectos.clear();
}

void GestorSonido::cargarEfecto(const QString& nombre, const QString& ruta) {
    if (m_efectos.contains(nombre)) return;

    QMediaPlayer* efecto = new QMediaPlayer(this);
    QAudioOutput* out = new QAudioOutput(this);

    efecto->setAudioOutput(out);
    efecto->setSource(QUrl::fromLocalFile(ruta));
    out->setVolume(1.0f);

    m_efectos.insert(nombre, efecto);
}

void GestorSonido::reproducirEfecto(const QString& nombre, float volumen) {
    if (!m_efectos.contains(nombre)) return;
    QMediaPlayer* e = m_efectos.value(nombre);

    e->audioOutput()->setVolume(volumen);
    e->stop();  // por si está reproduciendo
    e->play();
}

void GestorSonido::reproducirMusica(const QString& ruta, bool loop) {
    m_musicPlayer->setSource(QUrl::fromLocalFile(ruta));
    m_musicPlayer->setLoops(loop ? QMediaPlayer::Infinite : 1);
    m_musicPlayer->play();
}

void GestorSonido::detenerMusica() {
    m_musicPlayer->stop();
}

// ------------------------------
// EVENTOS ESPECIALES
// ------------------------------

void GestorSonido::inicioExorcismo() {
    reproducirEfecto("exorcismo_start", 1.0f);
    reproducirMusica("audio/exorcismo_loop.mp3", true);
}

void GestorSonido::inicioOratoria() {
    reproducirEfecto("oratoria_start", 1.0f);
    reproducirMusica("audio/oratoria_loop.mp3", true);
}

void GestorSonido::inicioSanacion() {
    reproducirEfecto("sanacion_start", 1.0f);
    reproducirMusica("audio/sanacion_loop.mp3", true);
}



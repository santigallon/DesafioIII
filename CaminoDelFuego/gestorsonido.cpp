#include "GestorSonido.h"
#include <QUrl>
#include <QDir>

GestorSonido::GestorSonido(QObject* parent)
    : QObject(parent),
    m_musicPlayer(new QMediaPlayer(this))
{
    // configuración básica
    m_musicPlayer->setVolume(30);
}

GestorSonido::~GestorSonido() {
    // limpiar efectos
    qDeleteAll(m_efectos);
    m_efectos.clear();
}

void GestorSonido::cargarEfecto(const QString& nombre, const QString& ruta) {
    if (m_efectos.contains(nombre)) return;
    QSoundEffect* eff = new QSoundEffect(this);
    eff->setSource(QUrl::fromLocalFile(ruta));
    eff->setVolume(0.8f);
    m_efectos.insert(nombre, eff);
}

void GestorSonido::reproducirEfecto(const QString& nombre, float volumen) {
    if (!m_efectos.contains(nombre)) return;
    QSoundEffect* eff = m_efectos.value(nombre);
    eff->setVolume(volumen);
    eff->play();
}

void GestorSonido::reproducirMusica(const QString& ruta, bool loop) {
    m_musicPlayer->setMedia(QUrl::fromLocalFile(ruta));
    m_musicPlayer->setLoops(loop ? QMediaPlayer::Infinite : 1);
    m_musicPlayer->play();
}

void GestorSonido::detenerMusica() {
    if (m_musicPlayer->state() == QMediaPlayer::PlayingState) {
        m_musicPlayer->stop();
    }
}

void GestorSonido::inicioExorcismo() {
    reproducirEfecto("exorcismo_start", 1.0f);
    reproducirMusica("audio/exorcismo_loop.mp3", true);
}

void GestorSonido::inicioOratoria() {
    reproducirEfecto("oratoria_start", 0.9f);
    reproducirMusica("audio/oratoria_bajo.mp3", true);
}

void GestorSonido::inicioSanacion() {
    reproducirEfecto("sanacion_start", 1.0f);
    reproducirMusica("audio/sanacion_loop.mp3", true);
}

void GestorSonido::sonidoAmbiente(const QString& zona) {
    // ejemplo: reproducir audio segun zona
    Q_UNUSED(zona);
    // m_musicPlayer->setMedia(...)
}

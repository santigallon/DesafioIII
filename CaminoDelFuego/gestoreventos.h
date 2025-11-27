#ifndef GESTOREVENTOS_H
#define GESTOREVENTOS_H

#pragma once
#include <QObject>
#include <functional>
#include <vector>

/**
 * GestorEventos
 * - Maneja eventos aleatorios y periódicos del mundo.
 * - Permite registrar callbacks que serán ejecutados cuando ocurra el evento.
 */

class GestorEventos : public QObject {
    Q_OBJECT
public:
    explicit GestorEventos(QObject* parent = nullptr);
    ~GestorEventos() override;

    // Agregar evento aleatorio (callback)
    void agregarEventoAleatorio(const std::function<void()>& evento);

    // Ejecutar actualización (por ejemplo cada frame o cada X segundos)
    void actualizar(float dt);

    // Forzar ejecución de evento por índice
    void ejecutarEvento(int indice);

    int cantidadEventos() const;

signals:
    void eventoEjecutado(int indice);

private:
    std::vector<std::function<void()>> m_eventos;
    float m_temporizador;
    float m_intervalo; // tiempo entre eventos aleatorios
};

#endif // GESTOREVENTOS_H

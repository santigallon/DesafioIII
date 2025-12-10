#pragma once
#ifndef VENTANACARTASPULINAS_H
#define VENTANACARTASPULINAS_H

#include <QDialog>
#include <QStringList>

class QListWidget;
class QPushButton;

class VentanaCartasPulinas : public QDialog {
    Q_OBJECT
public:
    explicit VentanaCartasPulinas(QWidget* parent = nullptr);
    ~VentanaCartasPulinas() override;

    // Iniciar la ventana con una lista de "cartas" (strings)
    void iniciar(const QStringList& cartas);

signals:
    void cartaSeleccionada(const QString& carta);
    void ventanaCerrada();

private slots:
    void onSeleccionar();
    void onCancelar();
    void onDobleClickLista();

private:
    QListWidget* m_lista;
    QPushButton* m_btnSeleccionar;
    QPushButton* m_btnCancelar;
};

#endif // VENTANACARTASPULINAS_H

#include "VentanaExorcismo.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QDebug>

VentanaExorcismo::VentanaExorcismo(QWidget* parent)
    : QWidget(parent), indice(0)
{
    setWindowTitle("Exorcismo");
    setFixedSize(300,200);

    auto* lay = new QVBoxLayout(this);
    auto* lbl = new QLabel("Sigue la secuencia de teclas (K,O,L,Ñ)", this);
    lbl->setAlignment(Qt::AlignCenter);
    lay->addWidget(lbl);
}

VentanaExorcismo::~VentanaExorcismo() = default;

void VentanaExorcismo::generarSecuencia(int longitud) {
    secuencia.clear();
    const QVector<QChar> teclas = { QChar('K'), QChar('O'), QChar('L'), QChar('N') }; // 'N' representará Ñ
    for (int i = 0; i < longitud; ++i) {
        int idx = QRandomGenerator::global()->bounded(teclas.size());
        secuencia.append(teclas[idx]);
    }
}

void VentanaExorcismo::iniciarSecuencia() {
    indice = 0;
    generarSecuencia(4);
    show();
    raise();
    activateWindow();
    qDebug() << "Exorcismo - secuencia generada:" << QString::fromUtf16(reinterpret_cast<const ushort*>(secuencia.constData()), secuencia.size());
}

void VentanaExorcismo::keyPressEvent(QKeyEvent* event) {
    QString t = event->text().toUpper();
    if (t.isEmpty()) {
        // ignorar teclas sin texto (flechas, etc.)
        return;
    }
    QChar c = t.at(0);
    // normalizar Ñ: si el usuario teclea 'N' lo interpretamos como posible 'Ñ' también
    if (c == QChar('Ñ')) c = QChar('N');

    if (indice < 0 || indice >= secuencia.size()) {
        emit exorcismoFallado();
        close();
        return;
    }

    if (c == secuencia[indice]) {
        ++indice;
        if (indice >= secuencia.size()) {
            emit exorcismoCompletado();
            close();
            return;
        }
    } else {
        emit exorcismoFallado();
        close();
    }
}

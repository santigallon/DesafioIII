#include "HouseWindow.h"
#include "LevelManager.h"
#include "PatternWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>
#include <QGridLayout>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

HouseWindow::HouseWindow(LevelManager* level, const QRect& worldDoorTileRect, QWidget* parent)
    : QDialog(parent), m_level(level), m_worldDoor(worldDoorTileRect), m_areaW(4), m_areaH(4)
{
    setModal(true);
    setWindowTitle("Interior de casa");
    setMinimumSize(480, 360);
    construirUI();
    generarNPCs();
}

HouseWindow::~HouseWindow() = default;

void HouseWindow::construirUI() {
    auto* mainV = new QVBoxLayout(this);
    mainV->addWidget(new QLabel("Casa - interactúa con los NPCs (clic) para acciones."));

    QWidget* gridWidget = new QWidget(this);
    QGridLayout* grid = new QGridLayout(gridWidget);
    grid->setSpacing(10);

    for (int ry = 0; ry < 2; ++ry) {
        for (int rx = 0; rx < 2; ++rx) {

            QWidget* area = new QWidget(this);
            QVBoxLayout* lay = new QVBoxLayout(area);
            lay->addWidget(new QLabel(QString("Cuarto %1").arg(ry*2 + rx + 1)));

            grid->addWidget(area, ry, rx);
        }
    }
    mainV->addWidget(gridWidget);


    auto* btn = new QPushButton("Salir", this);
    connect(btn, &QPushButton::clicked, this, &HouseWindow::close);
    mainV->addWidget(btn, 0, Qt::AlignRight);
}

void HouseWindow::generarNPCs() {
    m_npcs.clear();

    QStringList facs = {"cristiano", "judio", "romano", "endemoniado"};
    for (int cuarto = 0; cuarto < 4; ++cuarto) {
        QString fac = facs.at(QRandomGenerator::global()->bounded(facs.size()));
        int count = 3 + QRandomGenerator::global()->bounded(2); // 3 o 4
        for (int i = 0; i < count; ++i) {
            HouseNPC hn;
            hn.faction = fac;
            hn.name = QString("%1_%2").arg(fac).arg(i+1);
            hn.vida = 3 + QRandomGenerator::global()->bounded(3);

            int rx = QRandomGenerator::global()->bounded(m_areaW);
            int ry = QRandomGenerator::global()->bounded(m_areaH);

            int cuartoX = (cuarto % 2) * m_areaW;
            int cuartoY = (cuarto / 2) * m_areaH;
            hn.pos = QPoint(cuartoX + rx, cuartoY + ry);
            m_npcs.append(hn);
        }
    }


    QList<QLabel*> labels = findChildren<QLabel*>();

    QList<QWidget*> areaWidgets;

    QVBoxLayout* mainLay = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLay) return;


    QWidget* central = new QWidget(this);
    QGridLayout* grid = new QGridLayout(central);

    int idx = 0;
    for (int cuarto = 0; cuarto < 4; ++cuarto) {
        QWidget* area = new QWidget(this);
        QVBoxLayout* lay = new QVBoxLayout(area);
        lay->addWidget(new QLabel(QString("Cuarto %1").arg(cuarto+1)));

        for (const HouseNPC& hn : m_npcs.mid(idx, 4)) {
            QPushButton* b = new QPushButton(QString("%1 (%2) ").arg(hn.name).arg(hn.faction), area);

            connect(b, &QPushButton::clicked, this, [this, hn]() {
                abrirInteraccionConNPC(hn);
            });
            lay->addWidget(b);
        }
        idx += 4;
        grid->addWidget(area, cuarto/2, cuarto%2);
    }


    while (mainLay->count() > 1) {
        QLayoutItem* it = mainLay->takeAt(1);
        if (it->widget()) {
            delete it->widget();
        } else {
            delete it;
        }
    }
    mainLay->addWidget(central);
}

void HouseWindow::abrirInteraccionConNPC(const HouseNPC& npc) {

    QString accion;
    QString pattern;
    if (npc.faction == "endemoniado" || npc.faction == "poseido") {
        accion = "Exorcismo";

        int len = 4 + QRandomGenerator::global()->bounded(5); // 4..8
        QStringList keys = {"K","L","O","Ñ"};
        for (int i = 0; i < len; ++i) pattern.append(keys.at(QRandomGenerator::global()->bounded(keys.size())));
    } else if (npc.faction == "cristiano" || npc.faction == "judio" || npc.faction == "romano") {
        accion = "Oratoria/Sanación";
        int len = 4 + QRandomGenerator::global()->bounded(5);
        QStringList keys = {"K","L","O","Ñ"};
        for (int i = 0; i < len; ++i) pattern.append(keys.at(QRandomGenerator::global()->bounded(keys.size())));
    } else {
        accion = "Interacción";
        pattern = "KKLL";
    }

    PatternWindow pw(pattern, this);
    bool ok = pw.execPattern();
    emit accionRealizada(accion, ok);


    if (ok) {

        QLabel* l = new QLabel(QString("%1 realizado con éxito").arg(accion), this);
        l->show();
    } else {
        QLabel* l = new QLabel(QString("%1 falló").arg(accion), this);
        l->show();
    }
}
bool HouseWindow::eventFilter(QObject* obj, QEvent* ev) {

    if (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease) {

        QKeyEvent* kev = static_cast<QKeyEvent*>(ev);
        if (kev && kev->key() == Qt::Key_Escape) {
            this->reject();
            return true;
        }
    }

    return QObject::eventFilter(obj, ev);
}

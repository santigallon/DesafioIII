#include "PatternWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>

PatternWindow::PatternWindow(const QString& pattern, QWidget* parent)
    : QDialog(parent), m_pattern(pattern.toUpper())
{
    setModal(true);
    setWindowTitle("Secuencia");
    setMinimumSize(360, 120);
    auto* v = new QVBoxLayout(this);
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    v->addWidget(m_label);
    normalizePattern();
    refreshLabel();
}

PatternWindow::~PatternWindow() = default;

void PatternWindow::normalizePattern() {

    QString out;
    for (QChar c : m_pattern) {
        if (c.isSpace()) continue;
        if (c == 'N') out.append(QChar(0x00D1));
        else out.append(c);
    }
    m_pattern = out;
}

void PatternWindow::refreshLabel() {
    QString show = QString("Patrón: %1\nIngresado: %2").arg(m_pattern).arg(m_inputSoFar);
    m_label->setText(show);
}

bool PatternWindow::execPattern() {
    m_inputSoFar.clear();
    refreshLabel();
    int r = exec();
    return r == QDialog::Accepted;
}

void PatternWindow::keyPressEvent(QKeyEvent* ev) {
    QString text = ev->text().toUpper();
    if (text.isEmpty()) return;
    QChar c = text.at(0);
    if (c == 'N') c = QChar(0x00D1);
    if (!(c == 'K' || c == 'L' || c == 'O' || c == QChar(0x00D1))) {
        return;
    }
    m_inputSoFar.append(c);
    refreshLabel();
    if (m_inputSoFar.length() == m_pattern.length()) {
        bool success = (m_inputSoFar == m_pattern);
        if (success) accept();
        else reject();
        emit patternCompleted(success);
    }
}

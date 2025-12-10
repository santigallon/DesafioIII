#pragma once
#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include <QDialog>
#include <QString>

class QLabel;
class QHBoxLayout;

class PatternWindow : public QDialog {
    Q_OBJECT
public:
    // pattern example: "K K L O Ñ" (no spaces required, but we store as string)
    explicit PatternWindow(const QString& pattern, QWidget* parent=nullptr);
    ~PatternWindow() override;

    // starts the modal and returns true if correct
    bool execPattern(); // shows modal, returns success

signals:
    void patternCompleted(bool success);

protected:
    void keyPressEvent(QKeyEvent* ev) override;

private:
    QString m_pattern;      // normalized uppercase sequence without spaces, e.g. "KKLOÑK"
    QString m_inputSoFar;
    QLabel* m_label;
    void refreshLabel();
    QHBoxLayout* m_layout;
    void normalizePattern();
};


#endif // PATTERNWINDOW_H

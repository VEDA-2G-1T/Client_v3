#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QFrame>
#include <QPoint>

class CustomMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit CustomMessageBox(const QString &text, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint m_dragPosition;
    bool m_isDragging = false;
    QFrame* m_titleBar;
};

#endif // CUSTOMMESSAGEBOX_H
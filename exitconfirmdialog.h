#ifndef EXITCONFIRMDIALOG_H
#define EXITCONFIRMDIALOG_H

#include <QDialog>

// 전방 선언 (Forward Declarations)
class QFrame;
class QMouseEvent;
class QPoint;

class ExitConfirmDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExitConfirmDialog(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint m_dragPosition;
    bool m_isDragging = false;
    QFrame* titleBar;
};

#endif // EXITCONFIRMDIALOG_H
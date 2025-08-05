#ifndef BRIGHTNESSDIALOG_H
#define BRIGHTNESSDIALOG_H

#include <QDialog>

// 클래스 전방 선언
class QSlider;
class QLabel;
class QFrame;
class QMouseEvent;
class QPoint;

class BrightnessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrightnessDialog(const QString &cameraName, int initialValue, QWidget *parent = nullptr);

signals:
    void brightnessChanged(int value);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QSlider *brightnessSlider;
    QLabel *valueLabel;
    QFrame* titleBar;
    QPoint m_dragPosition;
    bool m_isDragging = false;
};

#endif // BRIGHTNESSDIALOG_H
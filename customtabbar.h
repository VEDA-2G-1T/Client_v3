#ifndef CUSTOMTABBAR_H
#define CUSTOMTABBAR_H

#include <QTabBar>
#include <QIcon>
#include <QPoint>

// 클래스 전방 선언
class QPaintEvent;
class QMouseEvent;

class CustomTabBar : public QTabBar
{
    Q_OBJECT

public:
    explicit CustomTabBar(QWidget *parent = nullptr);

    void setFixedTabWidth(int w);

protected:
    QSize tabSizeHint(int index) const override;
    void paintEvent(QPaintEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

private:
    int m_padding;
    int m_radius;
    int m_fixedTabWidth;
    QIcon m_closeIcon;
    QPoint m_dragStartPos;

    QRect closeIconRect(int idx) const;
};

#endif // CUSTOMTABBAR_H
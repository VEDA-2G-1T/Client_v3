#include "customtabbar.h"

// 구현에 필요한 헤더 포함
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOptionTab>

CustomTabBar::CustomTabBar(QWidget *parent)
    : QTabBar(parent),
    m_padding(12),
    m_radius(0),
    m_fixedTabWidth(120),
    m_closeIcon(QStringLiteral(":/resources/icons/close_small_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"))
{
    setExpanding(false);
    setMovable(true);
    setUsesScrollButtons(true);
}

void CustomTabBar::setFixedTabWidth(int w)
{
    m_fixedTabWidth = w;
    updateGeometry();
}

QSize CustomTabBar::tabSizeHint(int index) const
{
    QSize s = QTabBar::tabSizeHint(index);
    s.setWidth(m_fixedTabWidth);
    s.setHeight(height());
    return s;
}

void CustomTabBar::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < count(); ++i) {
        QStyleOptionTab opt;
        initStyleOption(&opt, i);
        QRect r = tabRect(i);
        bool sel = (currentIndex() == i);

        QColor textC = sel ? Qt::white : QColor(200, 200, 200);

        p.setPen(textC);
        QRect textRect = r.adjusted(m_padding, 0, -m_padding - 16, 0);
        p.drawText(textRect, Qt::AlignCenter, opt.text);

        QFont closeFont = font();
        closeFont.setPointSize(10);
        p.setFont(closeFont);
        p.drawText(closeIconRect(i), Qt::AlignCenter, QStringLiteral("✕"));

        if (sel) {
            QColor underlineColor = QColor("#f4731f");
            int underlineHeight = 4;
            QRect underlineRect(r.left(), r.bottom() - underlineHeight, r.width(), underlineHeight);
            p.fillRect(underlineRect, underlineColor);
        }
    }
}

void CustomTabBar::mousePressEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::LeftButton) {
        m_dragStartPos = evt->globalPosition().toPoint();
    }

    int idx = tabAt(evt->pos());
    if (idx >= 0) {
        QRect icr = closeIconRect(idx);
        if (icr.contains(evt->pos())) {
            emit tabCloseRequested(idx);
            return;
        }
    }
    QTabBar::mousePressEvent(evt);
}

void CustomTabBar::mouseMoveEvent(QMouseEvent *evt)
{
    if ((evt->buttons() & Qt::LeftButton)) {
        int startTabIndex = tabAt(mapFromGlobal(m_dragStartPos));
        if (startTabIndex == -1) {
            if (QWidget *w = window()) {
                QPoint newPos = evt->globalPosition().toPoint();
                w->move(w->pos() + (newPos - m_dragStartPos));
                m_dragStartPos = newPos;
            }
        } else {
            QTabBar::mouseMoveEvent(evt);
        }
    }
}

void CustomTabBar::mouseReleaseEvent(QMouseEvent *evt)
{
    QTabBar::mouseReleaseEvent(evt);
}

QRect CustomTabBar::closeIconRect(int idx) const
{
    QRect r = tabRect(idx).adjusted(1, 0, -1, 0);
    constexpr int iconSz = 14;
    int x = r.right() - m_padding - iconSz;
    int y = r.top() + (r.height() - iconSz) / 2;
    return QRect(x, y, iconSz, iconSz);
}
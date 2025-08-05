#include "aspectratiowidget.h"

AspectRatioWidget::AspectRatioWidget(QWidget *parent, qreal ratio)
    : QWidget(parent), m_ratio(ratio)
{
    // This policy is key. It tells the layout to expand horizontally
    // but to determine the height based on our custom heightForWidth function.
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void AspectRatioWidget::setAspectRatio(qreal ratio)
{
    if (m_ratio != ratio) {
        m_ratio = ratio;
        updateGeometry(); // Tell the layout to recalculate our size
    }
}

int AspectRatioWidget::heightForWidth(int w) const
{
    if (m_ratio <= 0) {
        return QWidget::heightForWidth(w);
    }
    return static_cast<int>(w / m_ratio);
}
#ifndef ASPECTRATIOWIDGET_H
#define ASPECTRATIOWIDGET_H

#include <QWidget>
#include <QSizePolicy>

class AspectRatioWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal aspectRatio READ aspectRatio WRITE setAspectRatio)

public:
    explicit AspectRatioWidget(QWidget *parent = nullptr, qreal ratio = 4.0/3.0);

    qreal aspectRatio() const { return m_ratio; }
    void setAspectRatio(qreal ratio);

    int heightForWidth(int w) const override;

private:
    qreal m_ratio;
};

#endif // ASPECTRATIOWIDGET_H
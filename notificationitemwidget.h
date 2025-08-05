#ifndef NOTIFICATIONITEMWIDGET_H
#define NOTIFICATIONITEMWIDGET_H

#include <QFrame>
#include <QDateTime>

class NotificationItemWidget : public QFrame
{
    Q_OBJECT

public:
    explicit NotificationItemWidget(const QString &title, const QString &message, const QDateTime &timestamp, bool hasCloseBtn, QWidget *parent = nullptr);
};

#endif // NOTIFICATIONITEMWIDGET_H

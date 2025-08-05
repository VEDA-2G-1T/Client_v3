#ifndef NOTIFICATIONLISTPANEL_H
#define NOTIFICATIONLISTPANEL_H

#include <QFrame>

// 클래스 전방 선언
class QVBoxLayout;
class QFocusEvent;
class QDateTime;

class NotificationListPanel : public QFrame
{
    Q_OBJECT

public:
    explicit NotificationListPanel(QWidget *parent = nullptr);
    ~NotificationListPanel();

    void addNotification(const QString &title, const QString &message, const QDateTime &timestamp, bool hasCloseBtn);

protected:
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void checkItemCount();

private:
    QVBoxLayout *m_listLayout;
};

#endif // NOTIFICATIONLISTPANEL_H
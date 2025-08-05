#include "notificationlistpanel.h"
#include "notificationitemwidget.h" // NotificationItemWidget을 직접 생성하므로 헤더 포함

// 구현에 필요한 헤더 포함
#include <QVBoxLayout>
#include <QFocusEvent>
#include <QTimer>

NotificationListPanel::NotificationListPanel(QWidget *parent) : QFrame(parent)
{
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose, false);

    setFocusPolicy(Qt::StrongFocus);

    setStyleSheet(R"(
        QFrame {
            background-color: #333333;
            border: 1px solid #555555;
            border-radius: 4px;
        }
    )");

    m_listLayout = new QVBoxLayout(this);
    m_listLayout->setContentsMargins(12, 8, 12, 8);
    m_listLayout->setSpacing(0);
    m_listLayout->addStretch();

    setFixedWidth(350);
}

NotificationListPanel::~NotificationListPanel()
{
    QList<NotificationItemWidget*> items = findChildren<NotificationItemWidget*>();
    for (NotificationItemWidget* item : items) {
        disconnect(item, &QWidget::destroyed, this, &NotificationListPanel::checkItemCount);
    }
}

void NotificationListPanel::addNotification(const QString &title, const QString &message, const QDateTime &timestamp, bool hasCloseBtn)
{
    auto *item = new NotificationItemWidget(title, message, timestamp, hasCloseBtn);
    connect(item, &QWidget::destroyed, this, &NotificationListPanel::checkItemCount);
    m_listLayout->insertWidget(m_listLayout->count() - 1, item);
}

void NotificationListPanel::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    hide();
}

void NotificationListPanel::checkItemCount()
{
    QTimer::singleShot(0, this, [this]() {
        int itemCount = 0;
        for (int i = 0; i < m_listLayout->count(); ++i) {
            if (m_listLayout->itemAt(i)->widget()) {
                itemCount++;
            }
        }
        if (itemCount == 0) {
            hide();
        }
    });
}
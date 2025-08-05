#include "notificationitemwidget.h"

// 구현에 필요한 헤더 포함
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

NotificationItemWidget::NotificationItemWidget(const QString &title, const QString &message, const QDateTime &timestamp, bool hasCloseBtn, QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet("background-color: transparent; border: none;");

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 10);
    mainLayout->setSpacing(4);

    auto *topLayout = new QHBoxLayout();
    auto *titleLabel = new QLabel(title, this);
    titleLabel->setStyleSheet("color: #F4731F; font-weight: bold;");

    topLayout->addWidget(titleLabel);
    topLayout->addStretch();

    if (hasCloseBtn) {
        auto *closeButton = new QToolButton(this);
        closeButton->setText("✕");
        closeButton->setStyleSheet("color: #AAAAAA; border: none; font-size: 14px;");
        connect(closeButton, &QToolButton::clicked, this, &QWidget::deleteLater);
        topLayout->addWidget(closeButton);
    }

    auto *msgLabel = new QLabel(message, this);
    msgLabel->setWordWrap(true);
    msgLabel->setStyleSheet("color: #FFFFFF;");

    auto *tsLabel = new QLabel(timestamp.toString("yyyy.MM.dd. hh:mm:ss"), this);
    tsLabel->setStyleSheet("color: #AAAAAA;");

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(msgLabel);
    mainLayout->addWidget(tsLabel);
    mainLayout->addSpacing(10);

    auto *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFixedHeight(1);
    separator->setStyleSheet("background-color: #444444; border: none;");
    mainLayout->addWidget(separator);
}
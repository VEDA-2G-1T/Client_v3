#include "loginpopup.h"

// 구현에 필요한 헤더 포함
#include <QPushButton>
#include <QVBoxLayout>
#include <QFocusEvent>

LoginPopup::LoginPopup(QWidget *parent) : QFrame(parent)
{
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setFocusPolicy(Qt::StrongFocus); // 포커스 설정
    setStyleSheet("border: 1px solid #333; border-radius: 4px;");

    auto *button = new QPushButton("로그아웃(admin)", this);
    button->setStyleSheet(R"(
      QPushButton {
        background-color: #F4731F;
        color: white;
        font-weight: bold;
        border: none;
        padding: 6px 12px;
        border-radius: 4px;
      }
      QPushButton:hover {
        background-color: #E06010;
      }
    )");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(button);

    // 버튼 클릭 시 (실제 로그아웃 로직) 또는 포커스 잃을 시 창이 닫히도록 연결
    connect(button, &QPushButton::clicked, this, &LoginPopup::logoutRequested);
}

void LoginPopup::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    hide();
}
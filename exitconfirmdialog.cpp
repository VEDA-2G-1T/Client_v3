#include "exitconfirmdialog.h"

// 필요한 헤더들을 여기에 포함시킵니다.
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

#include "debug_utils.h"


ExitConfirmDialog::ExitConfirmDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(530, 230);

    // --- Main Background ---
    QFrame *background = new QFrame(this);
    background->setStyleSheet(R"(
        QFrame {
            background-color: #232323;
            border: 1px solid #909090;
        }
    )");

    // [Req 4] Create Title Bar
    titleBar = new QFrame(background);
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("background-color: transparent; border: none;");

    auto *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(0, 0, 5, 0);
    titleLayout->addStretch();

    QToolButton *closeButton = new QToolButton(titleBar);
    closeButton->setText("✕");
    closeButton->setStyleSheet(R"(
        QToolButton { color: #aaaaaa; background-color: transparent; border: none; font-size: 16px; }
        QToolButton:hover { color: white; }
    )");
    connect(closeButton, &QToolButton::clicked, this, &QDialog::reject);
    titleLayout->addWidget(closeButton);


    // --- Content Widgets ---
    QLabel *messageLabel = new QLabel("프로그램을 종료하시겠습니까?", background);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("color: #ffffff; font-size: 11px; border: none;");

    QPushButton *exitButton = new QPushButton("프로그램 종료", background);
    exitButton->setFixedSize(140, 40);
    exitButton->setStyleSheet(R"(
        QPushButton {
            background-color: #232323;
            color: #ffffff;
            border: 1px solid #909090;
            font-size: 11px;
        }
        QPushButton:hover { border-color: #ffffff; }
        QPushButton:pressed { background-color: #3c3c3c; }
    )");
    connect(exitButton, &QPushButton::clicked, this, &QDialog::accept);

    // --- Main Layout ---
    QVBoxLayout *mainLayout = new QVBoxLayout(background);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);

    mainLayout->addStretch(5);
    mainLayout->addWidget(messageLabel);
    mainLayout->addSpacing(60);
    mainLayout->addWidget(exitButton, 0, Qt::AlignHCenter);
    mainLayout->addStretch(1);

    // --- Dialog Layout ---
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(background);

    // debug zone
    applyDebugBorder(background, "red");         // 전체 배경 프레임 (빨간색)
    applyDebugBorder(titleBar, "lime");          // 드래그 가능한 타이틀 바 (라임색)
    applyDebugBorder(messageLabel, "yellow");    // 메시지 레이블 (노란색)
    applyDebugBorder(exitButton, "red");        // 종료 버튼 (청록색)


}

void ExitConfirmDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && titleBar->geometry().contains(event->pos())) {
        m_isDragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void ExitConfirmDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_isDragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void ExitConfirmDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isDragging = false;
}

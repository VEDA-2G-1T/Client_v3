#include "custommessagebox.h"
#include "debug_utils.h" 
CustomMessageBox::CustomMessageBox(const QString &text, QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    QFrame *background = new QFrame(this);
    background->setStyleSheet("background-color: #232323; border: 1px solid #909090;");

    m_titleBar = new QFrame(background);
    m_titleBar->setStyleSheet("background-color: transparent; border: none;");

    QLabel *msgLabel = new QLabel(text, background);
    msgLabel->setAlignment(Qt::AlignCenter);
    msgLabel->setStyleSheet("color: #E0E0E0; border: none; padding: 20px;");

    QPushButton *closeButton = new QPushButton("닫기", background);
    closeButton->setFixedSize(80, 30);
    connect(closeButton, &QPushButton::clicked, this, &CustomMessageBox::accept);

    QToolButton *xButton = new QToolButton(m_titleBar);
    xButton->setText("✕");
    xButton->setStyleSheet("color: #AAAAAA; border: none; font-size: 14px;");
    connect(xButton, &QToolButton::clicked, this, &CustomMessageBox::accept);

    QHBoxLayout *topBarLayout = new QHBoxLayout(m_titleBar);
    topBarLayout->setContentsMargins(0, 0, 5, 0);
    topBarLayout->addStretch();
    topBarLayout->addWidget(xButton);

    QVBoxLayout *vbox = new QVBoxLayout(background);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    vbox->addWidget(m_titleBar);
    vbox->addStretch();
    vbox->addWidget(msgLabel, 0, Qt::AlignCenter);
    vbox->addStretch();
    vbox->addWidget(closeButton, 0, Qt::AlignCenter);
    vbox->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(background);

    setFixedSize(360, 160);

    closeButton->setStyleSheet(R"(
        QPushButton {
            background-color: #232323;
            color: #ffffff;
            border: 1px solid #909090;
            font-size: 11px;
        }
        QPushButton:hover { border-color: #ffffff; }
        QPushButton:pressed { background-color: #3c3c3c; }
    )");

    applyDebugBorder(m_titleBar, "lime"); // 타이틀 바 영역 (라임색)
    applyDebugBorder(background, "red"); // 전체 배경 영역 (빨간색)
}

void CustomMessageBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_titleBar->geometry().contains(event->pos())) {
        m_isDragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void CustomMessageBox::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_isDragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void CustomMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isDragging = false;
}
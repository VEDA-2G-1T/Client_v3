#include "loginwidget.h"
#include "custommessagebox.h" // 에러 메시지 표시에 필요

// 구현에 필요한 헤더 포함
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    auto *formFrame = new QFrame(this);
    formFrame->setObjectName("formFrame");
    formFrame->setFixedWidth(600);
    formFrame->setStyleSheet("background-color: transparent;");

    auto *formLayout = new QVBoxLayout(formFrame);
    formLayout->setContentsMargins(120, 70, 120, 70);
    formLayout->setSpacing(0);

    auto *titleContainer = new QWidget(formFrame);
    auto *titleLayout = new QHBoxLayout(titleContainer);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(8);
    titleLayout->setAlignment(Qt::AlignCenter);

    auto *logoLabel = new QLabel(titleContainer);
    QPixmap logoPixmap(":/resources/icons/45cac18a-7e5b-44a9-8a11-faf5878934f3-removebg-preview_cut_2.png");
    logoLabel->setPixmap(logoPixmap.scaledToHeight(32, Qt::SmoothTransformation));

    auto *viewerLabel = new QLabel("Viewer", titleContainer);
    viewerLabel->setObjectName("viewerLabel");
    viewerLabel->setStyleSheet(R"(
        #viewerLabel {
            color: #E0E0E0;
            font-size: 28px;
            font-weight: bold;
        }
    )");

    titleLayout->addWidget(logoLabel);
    titleLayout->addWidget(viewerLabel);
    auto *versionLabel = new QLabel("1.01.00", formFrame);
    versionLabel->setObjectName("versionLabel");

    idLineEdit = new QLineEdit(formFrame);
    idLineEdit->setPlaceholderText("ID");

    passwordLineEdit = new QLineEdit(formFrame);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("비밀번호");

    auto *saveIdCheckBox = new QCheckBox("ID 저장", formFrame);
    auto *autoLoginCheckBox = new QCheckBox("자동 로그인", formFrame);
    saveIdCheckBox->setChecked(true);
    autoLoginCheckBox->setChecked(true);

    auto *loginButton = new QPushButton("로그인", formFrame);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setFixedHeight(40);

    formLayout->addWidget(titleContainer, 0, Qt::AlignCenter);
    formLayout->addSpacing(5);
    formLayout->addWidget(versionLabel, 0, Qt::AlignCenter);
    formLayout->addSpacing(50);
    formLayout->addWidget(idLineEdit);
    formLayout->addSpacing(25);
    formLayout->addWidget(passwordLineEdit);
    formLayout->addSpacing(35);
    formLayout->addWidget(saveIdCheckBox);
    formLayout->addSpacing(15);
    formLayout->addWidget(autoLoginCheckBox);
    formLayout->addSpacing(15);
    formLayout->addWidget(loginButton);

    mainLayout->addWidget(formFrame);

    const QString styleSheet = R"(
        QWidget { background-color: #121212; }
        #formFrame { border: 1px solid #909090; border-radius: 4px; background-color: #121212; }
        #titleLabel { color: #E0E0E0; font-size: 28px; font-weight: bold; }
        #versionLabel { color: #888888; font-size: 14px; }
        QLineEdit { color: #E0E0E0; font-size: 12px; background-color: transparent; border: none; border-bottom: 1px solid #5c5c5c; padding: 6px 2px; }
        QLineEdit:focus { border-bottom: 1px solid #f4731f; }
        QCheckBox { color: #B0B0B0; font-size: 12px; spacing: 12px; }
        QCheckBox::indicator { width: 18px; height: 18px; border: 1px solid #909090; background-color: transparent; }
        QCheckBox::indicator:checked { image: url(:/resources/icons/check_16dp_E5E5E5_FILL0_wght400_GRAD0_opsz20.svg); }
        QPushButton { color: #AAAAAA; background-color: transparent; border: 1px solid #5c5c5c; font-size: 14px; font-weight: bold; padding: 8px 16px; text-align: center; }
        QPushButton:pressed { background-color: rgb(224, 0, 0); }
        QPushButton:hover { color: #FFFFFF !important; background-color: #909090 !important; border-color: #888888 !important; }
    )";
    this->setStyleSheet(styleSheet);

    idLineEdit->setFocus();
    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::attemptLogin);
    connect(passwordLineEdit, &QLineEdit::returnPressed, this, &LoginWidget::attemptLogin);
    connect(idLineEdit, &QLineEdit::returnPressed, [this]() {
        passwordLineEdit->setFocus();
    });
}

void LoginWidget::attemptLogin()
{
    if (idLineEdit->text() == "admin" && passwordLineEdit->text() == "admin") {
        emit loginSuccessful();
    } else {
        CustomMessageBox msg("ID 또는 비밀번호가 잘못되었습니다.", this);
        msg.exec();
        passwordLineEdit->clear();
        passwordLineEdit->setFocus();
    }
}
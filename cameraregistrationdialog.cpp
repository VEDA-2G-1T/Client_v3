#include "cameraregistrationdialog.h"
#include "debug_utils.h"
#include "custommessagebox.h"

// 구현에 필요한 헤더 포함
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QLabel>
#include <QRegularExpression>

CameraRegistrationDialog::CameraRegistrationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(530, 400);

    QFrame *background = new QFrame(this);
    background->setObjectName("background");
    background->setStyleSheet(R"(
        #background {
            background-color: #232323;
            border: 1px solid #909090;
        }
    )");

    titleBar = new QFrame(background);
    titleBar->setFixedHeight(35);
    titleBar->setStyleSheet("background-color: transparent; border: none;");

    auto *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(10, 0, 5, 0);

    auto *titleLabel = new QLabel("카메라 등록/편집", titleBar);
    titleLabel->setStyleSheet("color: #ffffff; font-size: 11px; font-weight: bold;");

    QToolButton *closeButton = new QToolButton(titleBar);
    closeButton->setText("✕");
    closeButton->setStyleSheet(R"(
        QToolButton { color: #aaaaaa; background-color: transparent; border: none; font-size: 16px; }
        QToolButton:hover { color: white; }
    )");
    connect(closeButton, &QToolButton::clicked, this, &QDialog::reject);

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton);

    QWidget* contentContainer = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(contentContainer);
    contentLayout->setContentsMargins(40, 20, 40, 20);
    contentLayout->setSpacing(20);

    nameEdit = new QLineEdit(this);
    ipEdit = new QLineEdit(this);
    portEdit = new QLineEdit(this);

    nameEdit->setPlaceholderText("장치명");
    ipEdit->setPlaceholderText("IP");
    portEdit->setPlaceholderText("포트");

    nameEdit->setFixedWidth(250);
    ipEdit->setFixedWidth(250);
    portEdit->setFixedWidth(250);

    contentLayout->addWidget(nameEdit, 0, Qt::AlignHCenter);
    contentLayout->addWidget(ipEdit, 0, Qt::AlignHCenter);
    contentLayout->addWidget(portEdit, 0, Qt::AlignHCenter);

    QPushButton *okButton = new QPushButton("확인", background);
    okButton->setObjectName("okButton");
    okButton->setFixedSize(200, 40);
    connect(okButton, &QPushButton::clicked, this, &CameraRegistrationDialog::onAcceptClicked);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(background);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addStretch(1);
    mainLayout->addWidget(contentContainer);
    mainLayout->addStretch(2);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(25);

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(background);

    const QString qss = R"(
        QLineEdit {
            background-color: transparent;
            border: none;
            border-bottom: 1px solid #909090;
            color: #e0e0e0;
            padding: 8px 0px;
            min-height: 28px;
            font-size: 11px;
        }
        #okButton {
            background-color: #232323;
            color: #ffffff;
            border: 1px solid #909090;
            font-size: 11px;
        }
        #okButton:hover { border-color: #ffffff; }
        #okButton:pressed { background-color: #3c3c3c; }
    )";
    this->setStyleSheet(qss);

    applyDebugBorder(titleBar, "red");
    applyDebugBorder(contentContainer, "lime");
    applyDebugBorder(okButton, "cyan");
}

QString CameraRegistrationDialog::deviceName() const { return nameEdit->text(); }
QString CameraRegistrationDialog::ip() const { return ipEdit->text(); }
QString CameraRegistrationDialog::port() const { return portEdit->text(); }

void CameraRegistrationDialog::setInitialData(const QString &name, const QString &ip, const QString &port)
{
    nameEdit->setText(name);
    ipEdit->setText(ip);
    portEdit->setText(port);
}

void CameraRegistrationDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && titleBar->geometry().contains(event->pos())) {
        m_isDragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void CameraRegistrationDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_isDragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void CameraRegistrationDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isDragging = false;
}

void CameraRegistrationDialog::onAcceptClicked()
{
    if (validateInput()) {
        QDialog::accept();
    }
}

bool CameraRegistrationDialog::validateInput()
{
    if (nameEdit->text().trimmed().isEmpty()) {
        CustomMessageBox msg("장치명을 입력해주세요.", this);
        msg.exec();
        return false;
    }
    if (nameEdit->text().length() > 20) {
        CustomMessageBox msg("장치명은 20자 이내로 입력해주세요.", this);
        msg.exec();
        return false;
    }

    QRegularExpression ipRegex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    if (!ipRegex.match(ipEdit->text()).hasMatch()) {
        CustomMessageBox msg("IP 주소 형식이 올바르지 않습니다.", this);
        msg.exec();
        return false;
    }

    bool isPortNumber;
    int portValue = portEdit->text().toInt(&isPortNumber);
    if (!isPortNumber || portValue < 1 || portValue > 65535) {
        CustomMessageBox msg("포트는 1에서 65535 사이의 숫자만 입력 가능합니다.", this);
        msg.exec();
        return false;
    }

    return true;
}
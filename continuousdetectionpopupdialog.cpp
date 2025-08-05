#include "continuousdetectionpopupdialog.h"

// 구현에 필요한 헤더 포함
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QDateTime>

ContinuousDetectionPopupDialog::ContinuousDetectionPopupDialog(const QString& cameraName,
                                                               const QString& imageUrl,
                                                               QWidget *parent)
    : QDialog(parent),
    m_cameraName(cameraName),
    m_imageUrl(imageUrl),
    m_networkManager(new QNetworkAccessManager(this)),
    isDragging(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(false);
    setFixedSize(600, 600);

    setupUi();
    loadImage();
}

void ContinuousDetectionPopupDialog::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *background = new QFrame(this);
    background->setObjectName("backgroundFrame");
    background->setStyleSheet(R"(
        #backgroundFrame {
            background-color: #2b2b2b;
            border: 1px solid #5c5c5c;
            border-radius: 8px;
        }
    )");

    auto *contentLayout = new QVBoxLayout(background);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(15);

    auto *titleBar = new QFrame(background);
    auto *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    auto *titleLabel = new QLabel(QString("🚨 연속 감지 알림 - %1").arg(m_cameraName), titleBar);
    titleLabel->setStyleSheet("color: #f4731f; font-size: 16px; font-weight: bold;");

    auto *closeBtn = new QToolButton(titleBar);
    closeBtn->setText("✕");
    closeBtn->setFixedSize(28, 28);
    closeBtn->setStyleSheet(R"(
        QToolButton { background-color: transparent; color: #d0d0d0; border: none; font-size: 14px; font-weight: bold; }
        QToolButton:hover { background-color: red; color: white; }
    )");

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeBtn);

    auto *warningFrame = new QFrame(background);
    warningFrame->setStyleSheet("background-color: #ff4444; border-radius: 4px; padding: 10px;");
    auto *warningLayout = new QVBoxLayout(warningFrame);
    warningLayout->setContentsMargins(15, 15, 15, 15);
    warningLayout->setSpacing(8);

    auto *warningLabel = new QLabel("⚠️ 20초 안에 4번 연속 PPE 위반 감지되었습니다!", warningFrame);
    warningLabel->setStyleSheet("color: white; font-size: 14px; font-weight: bold;");
    warningLabel->setAlignment(Qt::AlignCenter);

    auto *timeLabel = new QLabel(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), warningFrame);
    timeLabel->setStyleSheet("color: white; font-size: 12px;");
    timeLabel->setAlignment(Qt::AlignCenter);

    warningLayout->addWidget(warningLabel);
    warningLayout->addWidget(timeLabel);

    imagePreview = new QLabel(background);
    imagePreview->setFixedSize(400, 300);
    imagePreview->setAlignment(Qt::AlignCenter);
    imagePreview->setStyleSheet("background-color: #000000; border: 1px solid #555; border-radius: 4px;");
    imagePreview->setScaledContents(false);

    contentLayout->addWidget(titleBar);
    contentLayout->addWidget(warningFrame);

    auto *imageContainer = new QFrame(background);
    auto *imageContainerLayout = new QHBoxLayout(imageContainer);
    imageContainerLayout->setContentsMargins(0, 0, 0, 0);
    imageContainerLayout->addStretch();
    imageContainerLayout->addWidget(imagePreview);
    imageContainerLayout->addStretch();

    contentLayout->addWidget(imageContainer, 1);
    mainLayout->addWidget(background);

    connect(closeBtn, &QToolButton::clicked, this, &QDialog::accept);
}

void ContinuousDetectionPopupDialog::loadImage()
{
    if (m_imageUrl.isEmpty()) {
        QPixmap p(":/resources/images/no_camera.png");
        m_originalPixmap = p;
        imagePreview->setPixmap(p.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        return;
    }

    QNetworkRequest request{QUrl(m_imageUrl)};
    QNetworkReply *reply = m_networkManager->get(request);
    reply->ignoreSslErrors();

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QPixmap pixmap;
            if (pixmap.loadFromData(reply->readAll())) {
                m_originalPixmap = pixmap;
                imagePreview->setPixmap(pixmap.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
        reply->deleteLater();
    });
}

void ContinuousDetectionPopupDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        dragPosition = event->globalPosition().toPoint() - this->pos();
    }
    QDialog::mousePressEvent(event);
}

void ContinuousDetectionPopupDialog::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isDragging) {
        move(event->globalPosition().toPoint() - dragPosition);
    }
    QDialog::mouseMoveEvent(event);
}

void ContinuousDetectionPopupDialog::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;
    QDialog::mouseReleaseEvent(event);
}
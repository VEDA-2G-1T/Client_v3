#include "fullscreencameradialog.h"

// 구현에 필요한 헤더 포함
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>

FullscreenCameraDialog::FullscreenCameraDialog(const QString &cameraName, const QString &rtspUrl, QWidget *parent)
    : QDialog(parent), m_cameraName(cameraName), m_rtspUrl(rtspUrl)
{
    setWindowTitle(tr("전체화면 - %1").arg(cameraName));
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(false);

    resize(1280, 960);

    setupUI();
    startStream();
}

FullscreenCameraDialog::~FullscreenCameraDialog()
{
    if (m_mediaPlayer) {
        m_mediaPlayer->stop();
        delete m_mediaPlayer;
    }
}

void FullscreenCameraDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_toolbar = new QWidget(this);
    m_toolbar->setFixedHeight(50);
    m_toolbar->setStyleSheet("background-color: #2b2b2b;");

    auto* toolbarLayout = new QHBoxLayout(m_toolbar);
    toolbarLayout->setContentsMargins(15, 10, 15, 10);
    toolbarLayout->setSpacing(15);

    auto* titleLabel = new QLabel(m_cameraName, m_toolbar);
    titleLabel->setStyleSheet(R"(
        color: #e0e0e0; font-size: 16px; font-weight: bold;
        font-family: 'Segoe UI', Arial, sans-serif;
        background-color: transparent; border: none;
    )");
    toolbarLayout->addWidget(titleLabel);

    toolbarLayout->addStretch();

    m_statusLabel = new QLabel(tr("연결 중..."), m_toolbar);
    m_statusLabel->setStyleSheet(R"(
        color: #ffaa00; font-size: 12px;
        font-family: 'Segoe UI', Arial, sans-serif;
        background-color: transparent; border: none;
    )");
    toolbarLayout->addWidget(m_statusLabel);

    m_closeButton = new QToolButton(m_toolbar);
    m_closeButton->setIcon(QIcon(":/resources/icons/close_small_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"));
    m_closeButton->setToolTip(tr("닫기"));
    m_closeButton->setFixedSize(32, 32);
    m_closeButton->setStyleSheet(R"(
        QToolButton { background-color: transparent; border: none; color: #b7b7b7; border-radius: 16px; }
        QToolButton:hover { background-color: #ff4444; border-radius: 16px; }
    )");

    connect(m_closeButton, &QToolButton::clicked, this, &FullscreenCameraDialog::onCloseButtonClicked);
    toolbarLayout->addWidget(m_closeButton);

    mainLayout->addWidget(m_toolbar);

    m_videoWidget = new QVideoWidget(this);
    m_videoWidget->setStyleSheet("background-color: black;");
    mainLayout->addWidget(m_videoWidget, 1);

    setLayout(mainLayout);
}

void FullscreenCameraDialog::startStream()
{
    m_mediaPlayer = new QMediaPlayer(this);
    m_mediaPlayer->setVideoOutput(m_videoWidget);

    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &FullscreenCameraDialog::onPlaybackStateChanged);
    connect(m_mediaPlayer, qOverload<QMediaPlayer::Error, const QString&>(&QMediaPlayer::errorOccurred), this, &FullscreenCameraDialog::onMediaError);

    m_mediaPlayer->setSource(QUrl(m_rtspUrl));
    m_mediaPlayer->play();
}

void FullscreenCameraDialog::onCloseButtonClicked()
{
    close();
}

void FullscreenCameraDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_toolbar->geometry().contains(event->pos())) {
        m_isDragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void FullscreenCameraDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_isDragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void FullscreenCameraDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isDragging = false;
}

void FullscreenCameraDialog::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        m_statusLabel->setText(tr("재생 중"));
        m_statusLabel->setStyleSheet("color: #00ff00; font-size: 12px; font-family: 'Segoe UI', Arial, sans-serif; background-color: transparent; border: none;");
        break;
    case QMediaPlayer::PausedState:
        m_statusLabel->setText(tr("일시정지"));
        m_statusLabel->setStyleSheet("color: #ffaa00; font-size: 12px; font-family: 'Segoe UI', Arial, sans-serif; background-color: transparent; border: none;");
        break;
    case QMediaPlayer::StoppedState:
        m_statusLabel->setText(tr("정지됨"));
        m_statusLabel->setStyleSheet("color: #ff4444; font-size: 12px; font-family: 'Segoe UI', Arial, sans-serif; background-color: transparent; border: none;");
        break;
    }
}

void FullscreenCameraDialog::onMediaError(QMediaPlayer::Error error, const QString &errorString)
{
    qDebug() << "Media error:" << error << errorString;
    m_statusLabel->setText(tr("연결 실패"));
    m_statusLabel->setStyleSheet("color: #ff4444; font-size: 12px; font-family: 'Segoe UI', Arial, sans-serif; background-color: transparent; border: none;");
}
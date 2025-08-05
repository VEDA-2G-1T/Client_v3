#include "eventcardpopupdialog.h"

// 구현에 필요한 헤더 포함
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QToolButton>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <opencv2/opencv.hpp>

// MSVC 호환성을 위한 using 선언
using std::max;
using std::min;
using std::vector;

EventCardPopupDialog::EventCardPopupDialog(const QString& cameraName,
                                           const QString& eventText,
                                           const QDateTime& timestamp,
                                           const QString& imageUrl,
                                           QWidget *parent)
    : QDialog(parent),
    m_cameraName(cameraName),
    m_eventText(eventText),
    m_timestamp(timestamp),
    m_imageUrl(imageUrl),
    m_networkManager(new QNetworkAccessManager(this)),
    isDragging(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(600, 500);

    setupUi();
    loadImage();
}

void EventCardPopupDialog::setupUi()
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

    auto *titleLabel = new QLabel(QString("이벤트 상세 - %1").arg(m_cameraName), titleBar);
    titleLabel->setStyleSheet("color: #e0e0e0; font-size: 16px; font-weight: bold;");

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

    auto *infoFrame = new QFrame(background);
    infoFrame->setStyleSheet("background-color: transparent; padding: 10px;");
    auto *infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setContentsMargins(15, 15, 15, 15);
    infoLayout->setSpacing(8);

    auto *eventLabel = new QLabel(m_eventText, infoFrame);
    eventLabel->setStyleSheet("color: #f4731f; font-size: 14px; font-weight: bold;");
    eventLabel->setWordWrap(true);

    auto *timeLabel = new QLabel(m_timestamp.toString("yyyy-MM-dd hh:mm:ss"), infoFrame);
    timeLabel->setStyleSheet("color: #b0b0b0; font-size: 12px;");

    eventLabel->setAlignment(Qt::AlignHCenter);
    timeLabel->setAlignment(Qt::AlignHCenter);

    infoLayout->addWidget(eventLabel);
    infoLayout->addWidget(timeLabel);

    imagePreview = new QLabel(background);
    imagePreview->setMinimumSize(400, 300);
    imagePreview->setAlignment(Qt::AlignCenter);
    imagePreview->setStyleSheet("background-color: #000000; border: 1px solid #555; border-radius: 4px;");

    auto *enhancementWidget = createImageEnhancementWidget();

    auto* separator1 = new QFrame(background);
    separator1->setFrameShape(QFrame::HLine);
    separator1->setStyleSheet("background-color: #444; border: none;");
    separator1->setFixedHeight(1);

    auto* separator2 = new QFrame(background);
    separator2->setFrameShape(QFrame::HLine);
    separator2->setStyleSheet("background-color: #444; border: none;");
    separator2->setFixedHeight(1);

    contentLayout->addWidget(titleBar);
    contentLayout->addWidget(infoFrame);
    contentLayout->addWidget(separator1);
    contentLayout->addWidget(imagePreview, 1);
    contentLayout->addWidget(separator2);
    contentLayout->addWidget(enhancementWidget);

    mainLayout->addWidget(background);

    connect(closeBtn, &QToolButton::clicked, this, &QDialog::accept);
}

void EventCardPopupDialog::loadImage()
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

void EventCardPopupDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        dragPosition = event->globalPosition().toPoint() - this->pos();
    }
    QDialog::mousePressEvent(event);
}

void EventCardPopupDialog::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isDragging) {
        move(event->globalPosition().toPoint() - dragPosition);
    }
    QDialog::mouseMoveEvent(event);
}

void EventCardPopupDialog::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;
    QDialog::mouseReleaseEvent(event);
}

void EventCardPopupDialog::onSharpnessChanged(int value)
{
    sharpnessLabel->setText(QString("샤프닝: %1").arg(value));
    applyImageEnhancement();
}

void EventCardPopupDialog::onContrastChanged(int value)
{
    contrastLabel->setText(QString("대비: %1").arg(value));
    applyImageEnhancement();
}

void EventCardPopupDialog::applyImageEnhancement()
{
    if (m_originalPixmap.isNull()) return;
    QPixmap result = m_originalPixmap;
    if (sharpnessSlider && sharpnessSlider->value() != 0) {
        result = enhanceSharpness(result, sharpnessSlider->value());
    }
    if (contrastSlider && contrastSlider->value() != 0) {
        result = enhanceCLAHE(result, contrastSlider->value());
    }
    imagePreview->setPixmap(result.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QWidget* EventCardPopupDialog::createImageEnhancementWidget()
{
    auto *widget = new QWidget(this);
    auto *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(15);

    auto *titleLabel = new QLabel("Image Editing", widget);
    titleLabel->setStyleSheet("font-weight: bold; color: #e0e0e0; font-size: 14px;");
    titleLabel->setAlignment(Qt::AlignHCenter);
    layout->addWidget(titleLabel);

    const QString sliderStyle = R"(
        QSlider::groove:horizontal { border: none; height: 4px; background: #555555; margin: 0px; border-radius: 2px; }
        QSlider::handle:horizontal { background: #F4731F; border: none; width: 10px; height: 20px; margin: -8px 0; border-radius: 2px; }
        QSlider::sub-page:horizontal { background: #F4731F; border-radius: 2px; }
    )";

    auto *sharpnessGroup = new QWidget(widget);
    auto *sharpnessLayout = new QHBoxLayout(sharpnessGroup);
    sharpnessLayout->setContentsMargins(0, 0, 0, 0);
    sharpnessLabel = new QLabel("샤프닝: 0", widget);
    sharpnessLabel->setStyleSheet("color: #d0d0d0; min-width: 80px;");
    sharpnessSlider = new QSlider(Qt::Horizontal, widget);
    sharpnessSlider->setRange(-100, 100);
    sharpnessSlider->setValue(0);
    sharpnessSlider->setStyleSheet(sliderStyle);
    connect(sharpnessSlider, &QSlider::valueChanged, this, &EventCardPopupDialog::onSharpnessChanged);
    sharpnessLayout->addWidget(sharpnessLabel);
    sharpnessLayout->addWidget(sharpnessSlider);
    layout->addWidget(sharpnessGroup);

    auto *contrastGroup = new QWidget(widget);
    auto *contrastLayout = new QHBoxLayout(contrastGroup);
    contrastLayout->setContentsMargins(0, 0, 0, 0);
    contrastLabel = new QLabel("대비: 0", widget);
    contrastLabel->setStyleSheet("color: #d0d0d0; min-width: 80px;");
    contrastSlider = new QSlider(Qt::Horizontal, widget);
    contrastSlider->setRange(-100, 100);
    contrastSlider->setValue(0);
    contrastSlider->setStyleSheet(sliderStyle);
    connect(contrastSlider, &QSlider::valueChanged, this, &EventCardPopupDialog::onContrastChanged);
    contrastLayout->addWidget(contrastLabel);
    contrastLayout->addWidget(contrastSlider);
    layout->addWidget(contrastGroup);

    return widget;
}

QPixmap EventCardPopupDialog::enhanceSharpness(const QPixmap &pixmap, int level)
{
    if (pixmap.isNull() || level == 0) return pixmap;
    QImage qimg = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(qimg.height(), qimg.width(), CV_8UC3, (uchar*)qimg.bits(), qimg.bytesPerLine());
    cv::Mat result;
    if (level > 0) {
        float alpha = 1.0f + (level / 50.0f);
        cv::Mat blurred;
        cv::GaussianBlur(mat, blurred, cv::Size(0,0), 3);
        cv::addWeighted(mat, alpha, blurred, -(alpha - 1.0f), 0, result);
    } else {
        int ksize = max(1, -level / 10 * 2 + 1);
        cv::GaussianBlur(mat, result, cv::Size(ksize, ksize), 0);
    }
    return QPixmap::fromImage(QImage((uchar*)result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy());
}

QPixmap EventCardPopupDialog::enhanceCLAHE(const QPixmap &pixmap, int level)
{
    if (pixmap.isNull() || level == 0) return pixmap;
    QImage qimg = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(qimg.height(), qimg.width(), CV_8UC3, (uchar*)qimg.bits(), qimg.bytesPerLine());
    cv::Mat result;
    if (level > 0) {
        cv::Mat labImg;
        cv::cvtColor(mat, labImg, cv::COLOR_RGB2Lab);
        vector<cv::Mat> labPlanes(3);
        cv::split(labImg, labPlanes);
        int clipLimit = min(2 + level / 20, 10);
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clipLimit, cv::Size(8, 8));
        clahe->apply(labPlanes[0], labPlanes[0]);
        cv::merge(labPlanes, labImg);
        cv::cvtColor(labImg, result, cv::COLOR_Lab2RGB);
    } else {
        int ksize = max(1, -level / 20 * 2 + 1);
        cv::GaussianBlur(mat, result, cv::Size(ksize, ksize), 0);
    }
    return QPixmap::fromImage(QImage((uchar*)result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy());
}
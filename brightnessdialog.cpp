#include "brightnessdialog.h"

// 구현에 필요한 헤더 포함
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QFrame>

BrightnessDialog::BrightnessDialog(const QString &cameraName, int initialValue, QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(420, 280);

    QFrame *background = new QFrame(this);
    background->setObjectName("backgroundFrame");

    titleBar = new QFrame(background);
    titleBar->setFixedHeight(35);
    titleBar->setStyleSheet("background-color: transparent; border: none;");

    auto *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(10, 0, 5, 0);

    QToolButton *closeButton = new QToolButton(titleBar);
    closeButton->setText("✕");
    closeButton->setStyleSheet("color: #AAAAAA; border: none; font-size: 16px;");
    connect(closeButton, &QToolButton::clicked, this, &QDialog::reject);

    titleLayout->addStretch();
    titleLayout->addWidget(closeButton);

    QLabel *cameraNameLabel = new QLabel(QString("<b>%1</b> 밝기 조절").arg(cameraName), background);
    cameraNameLabel->setAlignment(Qt::AlignCenter);
    cameraNameLabel->setStyleSheet("color: #E0E0E0; font-size: 16px; border: none;");

    brightnessSlider = new QSlider(Qt::Horizontal, background);
    brightnessSlider->setRange(-255, 255);
    brightnessSlider->setValue(initialValue);
    brightnessSlider->setTickPosition(QSlider::TicksBelow);
    brightnessSlider->setTickInterval(50);

    valueLabel = new QLabel(QString::number(initialValue), background);
    valueLabel->setMinimumWidth(35);
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet("color: #E0E0E0; border: none;");

    QHBoxLayout *sliderRowLayout = new QHBoxLayout();
    sliderRowLayout->addWidget(brightnessSlider, 1);
    sliderRowLayout->addWidget(valueLabel);

    QVBoxLayout *sliderGroupLayout = new QVBoxLayout();
    sliderGroupLayout->setContentsMargins(20, 0, 20, 0);
    sliderGroupLayout->addLayout(sliderRowLayout);
    sliderGroupLayout->addSpacing(5);

    QPushButton *confirmButton = new QPushButton("닫기", background);
    confirmButton->setFixedSize(140, 40);
    connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout *mainLayout = new QVBoxLayout(background);
    mainLayout->setContentsMargins(1, 1, 1, 20);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addStretch(2);
    mainLayout->addWidget(cameraNameLabel);
    mainLayout->addStretch(1);
    mainLayout->addLayout(sliderGroupLayout);
    mainLayout->addStretch(3);
    mainLayout->addWidget(confirmButton, 0, Qt::AlignHCenter);

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(background);

    const QString qss = R"(
       QFrame#backgroundFrame {
            background-color: #232323;
            border: 1px solid #777777;
       }
       QSlider { background-color: transparent; border: none; }
       QSlider::groove:horizontal { border: none; height: 2px; background: #444444; margin: 0; }
       QSlider::add-page:horizontal { background: #444444; border: none; }
       QSlider::sub-page:horizontal { background: #777777; border: none; }
       QSlider::handle:horizontal { background: #CCCCCC; border: none; width: 8px; height: 16px; margin: -7px 0; }
       QSlider::tick:below { color: #444444; }
       QPushButton { background-color: #232323; color: #ffffff; border: 1px solid #777777; font-size: 11px; }
       QPushButton:hover { border-color: #ffffff; }
       QPushButton:pressed { background-color: #3c3c3c; }
    )";
    this->setStyleSheet(qss);

    connect(brightnessSlider, &QSlider::valueChanged, this, &BrightnessDialog::brightnessChanged);
    connect(brightnessSlider, &QSlider::valueChanged, this, [this](int value){
        valueLabel->setText(QString::number(value));
    });
}

void BrightnessDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && titleBar->geometry().contains(event->pos())) {
        m_isDragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void BrightnessDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_isDragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void BrightnessDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isDragging = false;
}
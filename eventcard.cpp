#include "eventcard.h"
#include "eventcardpopupdialog.h" // EventCard 클릭 시 팝업을 띄우므로 포함

// 구현에 필요한 헤더 포함
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

EventCard::EventCard(const QString& cameraName,
                     const QString& eventText,
                     const QDateTime& timestamp,
                     const QString& imageUrl,
                     Mode mode,
                     QWidget *parent)
    : QFrame(parent),
    m_camera(cameraName),
    m_event(eventText),
    m_ts(timestamp),
    m_imageUrl(imageUrl),
    m_mode(mode)
{
    setFrameShape(QFrame::StyledPanel);
    setLineWidth(1);

    auto *mainLay = new QVBoxLayout(this);
    mainLay->setContentsMargins(8, 8, 8, 8);
    mainLay->setSpacing(10);

    // --- Image Area ---
    auto *imageArea = new QWidget(this);
    auto *imageLayout = new QGridLayout(imageArea);
    imageLayout->setContentsMargins(0, 0, 0, 0);

    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setFixedSize(70, 70 * 3 / 4);
    m_imageLabel->setStyleSheet("background-color: #000; color: #fff;");
    m_imageLabel->setText(tr("Loading..."));

    if (!imageUrl.isEmpty()) {
        auto *manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, [this, manager](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QPixmap pixmap;
                pixmap.loadFromData(reply->readAll());
                if (!pixmap.isNull()) {
                    m_imageLabel->setPixmap(pixmap.scaled(m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                } else {
                    m_imageLabel->setText(tr("No Image"));
                }
            } else {
                m_imageLabel->setText(tr("Load Failed"));
            }
            reply->deleteLater();
            manager->deleteLater();
        });
        manager->get(QNetworkRequest(QUrl(imageUrl)));
    } else {
        m_imageLabel->setText(tr("No Image"));
    }

    m_actionButton = new QToolButton(this);
    m_actionButton->setIconSize(QSize(16, 16));
    m_actionButton->setAutoRaise(true);
    m_actionButton->setStyleSheet("QToolButton { background-color: transparent; border: none;} QToolButton:checked { background-color: transparent; }");

    if (m_mode == Event) {
        auto *buttonContainer = new QWidget(this);
        auto *buttonLayout = new QHBoxLayout(buttonContainer);
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        buttonLayout->setSpacing(2);

        m_actionButton->setCheckable(true);
        m_actionButton->setIcon(QIcon(":/resources/icons/star_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"));
        connect(m_actionButton, &QToolButton::toggled, this, [this](bool checked){
            m_actionButton->setIcon(QIcon(checked ? ":/resources/icons/star_24dp_F4731F_FILL0_wght400_GRAD0_opsz24.svg" : ":/resources/icons/star_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"));
            emit starToggled(this, checked);
        });

        m_removeButton = new QToolButton(this);
        m_removeButton->setText("✕");
        m_removeButton->setToolTip(tr("Delete event"));
        m_removeButton->setFixedSize(16, 16);
        m_removeButton->setStyleSheet("QToolButton { background-color: transparent; border: none; color: #ff4444; font-weight: bold; font-size: 10px; } QToolButton:hover { background-color: #ff4444; color: white; border-radius: 8px; }");
        connect(m_removeButton, &QToolButton::clicked, this, [this](){ emit removeRequested(this); });

        buttonLayout->addWidget(m_actionButton);
        buttonLayout->addWidget(m_removeButton);
        buttonLayout->addStretch();
        imageLayout->addWidget(buttonContainer, 0, 0, Qt::AlignTop | Qt::AlignRight);
    } else { // Bookmark mode
        m_actionButton->setCheckable(true);
        m_actionButton->setChecked(true);
        m_actionButton->setIcon(QIcon(":/resources/icons/star_24dp_F4731F_FILL0_wght400_GRAD0_opsz24.svg"));
        m_actionButton->setToolTip(tr("Click to remove bookmark"));
        connect(m_actionButton, &QToolButton::clicked, this, [this](){ emit removeRequested(this); });
        imageLayout->addWidget(m_actionButton, 0, 0, Qt::AlignTop | Qt::AlignRight);
    }

    imageLayout->addWidget(m_imageLabel, 0, 0, Qt::AlignCenter);
    mainLay->addWidget(imageArea);

    auto *info = new QVBoxLayout;
    info->setSpacing(4);
    auto *c = new QLabel(m_camera, this);
    c->setAlignment(Qt::AlignHCenter);
    QFont fb = c->font(); fb.setBold(true); c->setFont(fb);
    auto *d = new QLabel(m_ts.toString("yyyy-MM-dd hh:mm:ss"), this);
    d->setAlignment(Qt::AlignHCenter);
    d->setStyleSheet("color:#888;");
    auto *e = new QLabel(m_event, this);
    e->setAlignment(Qt::AlignHCenter);
    info->addWidget(c);
    info->addWidget(d);
    info->addWidget(e);
    mainLay->addLayout(info);

    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
}

const QString& EventCard::cameraName() const { return m_camera; }
const QString& EventCard::eventText() const { return m_event; }
const QString& EventCard::imageUrl() const { return m_imageUrl; }
const QDateTime& EventCard::timestamp() const { return m_ts; }
EventCard::Mode EventCard::mode() const { return m_mode; }

void EventCard::setStarChecked(bool checked)
{
    bool b = m_actionButton->blockSignals(true);
    m_actionButton->setChecked(checked);
    m_actionButton->setIcon(QIcon(checked ? ":/resources/icons/star_24dp_F4731F_FILL0_wght400_GRAD0_opsz24.svg" : ":/resources/icons/star_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"));
    m_actionButton->blockSignals(b);
}

void EventCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QWidget *clickedWidget = childAt(event->pos());
        if (!clickedWidget || (clickedWidget != m_actionButton && clickedWidget != m_removeButton && !clickedWidget->inherits("QToolButton"))) {
            EventCardPopupDialog *dialog = new EventCardPopupDialog(m_camera, m_event, m_ts, m_imageUrl, this);
            dialog->exec();
            dialog->deleteLater();
        }
    }
    QFrame::mousePressEvent(event);
}

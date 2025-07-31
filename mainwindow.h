#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <QMainWindow>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileDialog>
#include <QStackedWidget>
#include <QToolButton>
#include <QTreeWidget>
#include <QTabBar>
#include <QStringList>
#include <QVector>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QIcon>
#include <QColor>
#include <QTimer>
#include <QScrollArea>
#include <QPushButton>

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVariantMap>
#include <QMediaPlayer>
#include <QVideoWidget>

#include <QMap>
#include <QtWebSockets/QWebSocket>
#include <QAbstractSocket>
#include <QDateTimeEdit>
#include <QTableWidget>
#include <QPainter>
#include <QComboBox>
#include <QNetworkAccessManager>

class QTreeWidgetItem;
class EventCard;
class CustomTabBar;
class NotificationListPanel;
class LoginPopup;
class CustomMessageBox;
class LoginWidget;
class CameraRegistrationDialog;
class QWebSocket;
class EventSearchDialog;
class ExitConfirmDialog;
class BrightnessDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddView();
    void onSwitchView(int idx);
    void onCloseView(int idx);

    void onLayoutTreeClicked(QTreeWidgetItem *item, int column);
    void onCameraSlotClicked(int slotIndex);
    void onSlotDropped(int srcIndex, int dstIndex);
    void onCameraRegistered(int slotIndex, const QString &deviceName, const QString &ip, const QString &port);
    void onCameraDeleteRequested(int slotIndex);
    void onCameraModeChanged(int slotIndex, const QString &mode);
    void onLayoutTreeContextMenu(const QPoint &point);

    void onCardStarToggled(EventCard* card, bool checked);
    void onBookmarkRemoveRequested(EventCard* card);
    void onEventRemoveRequested(EventCard* card);  // [2024-12-19] 이벤트 삭제 기능 추가

    void toggleNotificationPanel();
    void openHelpFile();
    void toggleLoginPopup();


    void handleLogout();
    void onLoginSuccess();

    void onWebSocketConnected();
    void onWebSocketDisconnected();
    void onWebSocketError(QAbstractSocket::SocketError error);
    void onWebSocketMessageReceived(const QString &message);

    void showEventSearchDialog();

    void onCloseButtonClicked();


    void onBrightnessControlClicked();
    void sendBrightnessRequest(const QVariantMap &cameraData, int value);


    void onGlobalCameraRegisterClicked();

    // 과거 로그 로딩 관련 메서드 추가
    void loadInitialLogs();
    void loadHistoricalEvents(const QString &cameraName, const QString &cameraIp, const QString &cameraPort);
    void processHistoricalEvent(const QString &cameraName, const QString &eventType,
                                const QJsonObject &eventData, const QString &cameraIp);
    void updateCameraConnectionStatus(const QString &ip, const QString &status, const QString &color);
    void startHealthCheck(const QString &ip); // 헬스체크 시작

    void onCameraValidationFailed(int slotIndex, const QString& error);

    void onHeadlessValidationSuccess(const QString& ip);
    void onHeadlessValidationFailure(const QString& ip, const QString& error);

private:
    void updateLayoutTree();
    QWidget* createCameraGridWidget();
    void setupWebSocket(const QString& ip, const QString& port);
    void addEvent(const QString &cam, const QString &txt, const QDateTime &ts, const QString &imageUrl);

    void unregisterCamera(int layoutIndex, int slotIndex);

    void startHeadlessValidation(int slotIndex, const QVariantMap& cameraData);
     QMap<QString, QMediaPlayer*> m_validationPlayers;
    
    CustomTabBar  *viewTabBar;
    QStackedWidget *viewStack;
    QStackedWidget *centerStack;
    QWidget    *emptyWidget;
    QToolButton  *addBtn;
    QToolButton  *bigAddBtn;
    QTreeWidget  *layoutTree;
    QVector<QVector<QVariantMap>> layoutSlotData;


    QVBoxLayout *bookmarkLay = nullptr;
    NotificationListPanel *m_notificationPanel = nullptr;
    LoginPopup *m_loginPopup = nullptr;

    QPoint m_dragPosition;
    QToolBar *topBar;
    QPushButton *maxB;
    bool m_isWindowDragging = false;
    QStackedWidget *m_mainStack;
    QMap<QString, QWebSocket*> m_webSocketMap;
    int m_activeSlotIndex = -1;

    QWidget *eventContainer = nullptr;
    QVBoxLayout *eventLay = nullptr;

    QMap<QString, QPair<int, QVariantMap>> m_pendingRegistrations;
    QMap<QString, int> m_cameraBrightnessLevels;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void changeEvent(QEvent* event) override;
    void closeEvent(QCloseEvent *event) override;




public:
    struct RawEv { QString cam, txt; QDateTime ts; QString imageUrl; };
    QList<RawEv> m_eventData;
    EventSearchDialog *m_eventSearchDialog = nullptr;

    QNetworkAccessManager *m_networkManager = nullptr;
    QMap<QString, QDateTime> m_lastEventTimes;  // 각 카메라별 마지막 이벤트 시간
    QMap<QString, bool> m_healthCheckResponded; // 헬스체크 응답 상태
    QMap<QString, QString> m_cameraConnectionStatus; // 카메라별 연결 상태 (ip -> status)
    QTimer *m_healthCheckTimer = nullptr; // 헬스체크 타이머
    QMap<QString, QTimer*> m_healthCheckTimers; // 카메라별 개별 헬스체크 타이머

    // [2024-12-19] 연속 감지 카운터 및 타이머
    QMap<QString, int> m_detectionCount;  // 카메라별 감지 횟수
    QMap<QString, QDateTime> m_firstDetectionTime;  // 카메라별 첫 감지 시간
    QMap<QString, QTimer*> m_detectionTimers;  // 카메라별 타이머

private slots:
    void onDetectionTimerTimeout(const QString& cameraName);  // 감지 타이머 타임아웃 처리
    void showContinuousDetectionPopup(const QString& cameraName, const QString& imageUrl);  // 연속 감지 팝업 표시
    void handleContinuousDetection(const QString& cameraName, const QString& imageUrl);  // 연속 감지 처리
    void resetDetectionCounter(const QString& cameraName);  // 감지 카운터 리셋
};




class AspectRatioWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal aspectRatio READ aspectRatio WRITE setAspectRatio)

public:
    explicit AspectRatioWidget(QWidget *parent = nullptr, qreal ratio = 4.0/3.0)
        : QWidget(parent), m_ratio(ratio)
    {
        // This policy is key. It tells the layout to expand horizontally
        // but to determine the height based on our custom heightForWidth function.
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }

    qreal aspectRatio() const { return m_ratio; }
    void setAspectRatio(qreal ratio) {
        if (m_ratio != ratio) {
            m_ratio = ratio;
            updateGeometry(); // Tell the layout to recalculate our size
        }
    }

    // This is the magic function that enforces the aspect ratio.
    int heightForWidth(int w) const override {
        if (m_ratio <= 0) {
            return QWidget::heightForWidth(w);
        }
        return static_cast<int>(w / m_ratio);
    }


private:
    qreal m_ratio;
};


class CameraSlot : public AspectRatioWidget {
    Q_OBJECT
public:
    explicit CameraSlot(int w, int h, int slotIndex, QWidget *parent = nullptr);
    ~CameraSlot();

    void setData(const QVariantMap &data);
    void setActive(bool on);
    void stopStream();

    int slotIndex() const { return m_slotIndex; }


signals:
    void clicked(int slotIndex);
    void dropOccurred(int srcIndex, int dstIndex);
    void cameraRegistered(int slotIndex, const QString &deviceName, const QString &ip, const QString &port);
    void deleteRequested(int slotIndex);
    void modeChanged(int slotIndex, const QString &mode);
    
    void validationFailed(int slotIndex, const QString& error);


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *evt) override;
    
private slots:
    void onRegisterCamera();

    void onDeleteCamera();
    void handleMediaError();
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
public slots:
    void onEditCamera();

private:

    int  m_slotIndex;

    
    QFrame *frame;
    QPoint m_dragStartPos;
    QVariantMap m_currentData;
    // 위젯 멤버들을 Qt Multimedia 용으로 변경
    QLabel          *imageLabel;      // 이미지 표시용
    QVideoWidget    *videoWidget;     // 비디오 스트리밍용
    QMediaPlayer    *mediaPlayer;     // 미디어 제어용
    QLabel          *infoLabel;       // "No Camera" 또는 에러 메시지 표시용


};

// ===================================================================
// [2024-12-19] EventCard 팝업 다이얼로그 클래스 추가
// EventCard 클릭 시 이미지와 이미지 향상 기능을 제공하는 팝업창
// ===================================================================
class EventCardPopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EventCardPopupDialog(const QString& cameraName,
                                 const QString& eventText,
                                 const QDateTime& timestamp,
                                 const QString& imageUrl,
                                 QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onSharpnessChanged(int value);
    void onContrastChanged(int value);
    void applyImageEnhancement();
    QWidget* createImageEnhancementWidget();
    QPixmap enhanceSharpness(const QPixmap &pixmap, int level);
    QPixmap enhanceCLAHE(const QPixmap &pixmap, int level);

private:
    void setupUi();
    void loadImage();

    // UI 컴포넌트
    QLabel *imagePreview;
    QSlider *sharpnessSlider;
    QSlider *contrastSlider;
    QLabel *sharpnessLabel;
    QLabel *contrastLabel;
    QPixmap m_originalPixmap;
    QNetworkAccessManager *m_networkManager;

    // 데이터
    QString m_cameraName;
    QString m_eventText;
    QDateTime m_timestamp;
    QString m_imageUrl;

    // 창 드래그
    QPoint dragPosition;
    bool isDragging;
};

// ===================================================================
// [2024-12-19] 연속 감지 팝업 다이얼로그 클래스 추가
// 20초 안에 4번 연속 감지 시 표시되는 팝업창
// ===================================================================
class ContinuousDetectionPopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContinuousDetectionPopupDialog(const QString& cameraName,
                                           const QString& imageUrl,
                                           QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void loadImage();

private:
    void setupUi();

    // UI 컴포넌트
    QLabel *imagePreview;
    QPixmap m_originalPixmap;
    QNetworkAccessManager *m_networkManager;

    // 데이터
    QString m_cameraName;
    QString m_imageUrl;

    // 창 드래그
    QPoint dragPosition;
    bool isDragging;
};

class EventCard : public QFrame {
    Q_OBJECT
public:
    enum Mode { Event, Bookmark };

    EventCard(const QString& cameraName,
              const QString& eventText,
              const QDateTime& timestamp,
              const QString& imageUrl,
              Mode mode,
              QWidget *parent = nullptr);

    const QString& cameraName() const { return m_camera; }
    const QString& eventText() const { return m_event; }
    const QString& imageUrl() const { return m_imageUrl; }
    const QDateTime& timestamp()const { return m_ts;  }
    void setStarChecked(bool checked);

    Mode mode() const { return m_mode; }

signals:
    void starToggled(EventCard* card, bool checked);
    void removeRequested(EventCard* card);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString  m_camera;
    QString  m_event;
    QDateTime m_ts;
    QToolButton* m_actionButton;
    QToolButton* m_removeButton;  // [2024-12-19] 북마크 삭제 버튼 추가
    QLabel* m_imageLabel;
    Mode    m_mode;
    QString m_imageUrl;
};
class CustomTabBar : public QTabBar {
    Q_OBJECT
public:
    explicit CustomTabBar(QWidget *parent = nullptr)
        : QTabBar(parent),
        m_padding(12),
        m_radius(0),
        m_fixedTabWidth(120),
        m_closeIcon(QStringLiteral(":/icons/icons/close_small_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"))
    {
        setExpanding(false);
        setMovable(true);
        setUsesScrollButtons(true);
    }

    void setFixedTabWidth(int w)  { m_fixedTabWidth = w; updateGeometry(); }

protected:
    // 선언만, 본문은 .cpp 로 옮깁니다
    QSize tabSizeHint(int index) const override;
    void paintEvent(QPaintEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;

    void mouseMoveEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

private:
    int  m_padding;
    int  m_radius;
    int  m_fixedTabWidth;
    // QColor m_bgNormal;
    // QColor m_bgSelected;
    QIcon m_closeIcon;
    QRect closeIconRect(int idx) const;
    QPoint m_dragStartPos;
};


// NotificationItemWidget: 알림 목록의 각 항목을 표시하는 위젯


class NotificationItemWidget : public QFrame
{
    Q_OBJECT 
public:
    explicit NotificationItemWidget(const QString &title, const QString &message, const QDateTime &timestamp, bool hasCloseBtn, QWidget *parent = nullptr)
        : QFrame(parent)
    {

        setStyleSheet("background-color: transparent; border: none;");

        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 10); // 항목 아래쪽에만 여백을 주어 간격 조절
        mainLayout->setSpacing(4);


        // 상단부 (제목 + 닫기 버튼)
        auto *topLayout = new QHBoxLayout();
        auto *titleLabel = new QLabel(title, this);
        titleLabel->setStyleSheet("color: #F4731F; font-weight: bold;");

        topLayout->addWidget(titleLabel);
        topLayout->addStretch();

        if (hasCloseBtn) {
            auto *closeButton = new QToolButton(this);
            closeButton->setText("✕");
            closeButton->setStyleSheet("color: #AAAAAA; border: none; font-size: 14px;");
            // 클릭 시 자신을 삭제하고, 부모 패널이 알 수 있도록 destroyed 시그널이 발생합니다.
            connect(closeButton, &QToolButton::clicked, this, &QWidget::deleteLater);
            topLayout->addWidget(closeButton);
        }

        // 메시지 및 타임스탬프
        auto *msgLabel = new QLabel(message, this);
        msgLabel->setWordWrap(true);
        msgLabel->setStyleSheet("color: #FFFFFF;"); // 글씨색은 다시 흰색 계열로 변경 (WISENET 원본 참고)

        auto *tsLabel = new QLabel(timestamp.toString("yyyy.MM.dd. hh:mm:ss"), this); // 날짜 형식 원복
        tsLabel->setStyleSheet("color: #AAAAAA;");

        mainLayout->addLayout(topLayout);
        mainLayout->addWidget(msgLabel);
        mainLayout->addWidget(tsLabel);
        mainLayout->addSpacing(10); // 타임스탬프와 구분선 사이 간격

        auto *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFixedHeight(1);
        separator->setStyleSheet("background-color: #444444; border: none;"); // 구분선 색상
        mainLayout->addWidget(separator);
    }
};

// NotificationListPanel: 여러 알림 항목을 담는 스크롤 가능한 패널
class NotificationListPanel : public QFrame
{
    Q_OBJECT

    QVBoxLayout *m_listLayout;

public:
    explicit NotificationListPanel(QWidget *parent = nullptr) : QFrame(parent)
    {
        setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_DeleteOnClose, false);

        // 포커스를 받을 수 있도록 설정 (외부 클릭 감지를 위함)
        setFocusPolicy(Qt::StrongFocus);

        setStyleSheet(R"(
      QFrame {
        background-color: #333333;
        border: 1px solid #555555;
        border-radius: 4px;
      }
    )");

        m_listLayout = new QVBoxLayout(this);
        m_listLayout->setContentsMargins(12, 8, 12, 8);
        m_listLayout->setSpacing(0);
        m_listLayout->addStretch();

        setFixedWidth(350);
    }

    ~NotificationListPanel()   {
        QList<NotificationItemWidget*> items = findChildren<NotificationItemWidget*>();
        for (NotificationItemWidget* item : items) {
            disconnect(item, &QWidget::destroyed, this, &NotificationListPanel::checkItemCount);
        }
    }
    void addNotification(const QString &title, const QString &message, const QDateTime &timestamp, bool hasCloseBtn)
    {
        auto *item = new NotificationItemWidget(title, message, timestamp, hasCloseBtn);
        connect(item, &QWidget::destroyed, this, &NotificationListPanel::checkItemCount);
        m_listLayout->insertWidget(m_listLayout->count() - 1, item);
    }

protected:
    // ▼▼▼ 추가된 부분: 위젯이 포커스를 잃으면 스스로 숨김 ▼▼▼
    void focusOutEvent(QFocusEvent *event) override
    {
        Q_UNUSED(event);
        hide();
    }

private slots:
    void checkItemCount()
    {
        // ▼▼▼ 수정된 부분: QTimer를 이용해 위젯 삭제가 완전히 처리된 후 항목 수를 체크 ▼▼▼
        QTimer::singleShot(0, this, [this]() {
            int itemCount = 0;
            for (int i = 0; i < m_listLayout->count(); ++i) {
                if (m_listLayout->itemAt(i)->widget()) {
                    itemCount++;
                }
            }
            if (itemCount == 0) {
                hide();
            }
        });
    }
};

class LoginPopup : public QFrame
{
    Q_OBJECT

public:
    explicit LoginPopup(QWidget *parent = nullptr) : QFrame(parent)
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

protected:
    void focusOutEvent(QFocusEvent *event) override
    {
        Q_UNUSED(event);
        hide();
    }
signals:
    void logoutRequested();

};



class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccessful(); // 로그인 성공 시 발생시킬 신호

private slots:
    void attemptLogin();

private:
    QLineEdit   *idLineEdit;
    QLineEdit   *passwordLineEdit;
};


class CustomMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit CustomMessageBox(const QString &text, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        setAttribute(Qt::WA_TranslucentBackground);
        setModal(true);

        // 전체 배경
        QFrame *background = new QFrame(this);
        background->setStyleSheet("background-color: #232323; border: 1px solid #909090;");

        // --- Title Bar (Draggable Area) ---
        m_titleBar = new QFrame(background);
        // m_titleBar->setFixedHeight(30); // Set a fixed height for the title bar
        m_titleBar->setStyleSheet("background-color: transparent; border: none;");

        // 메시지 레이블
        QLabel *msgLabel = new QLabel(text, background);
        msgLabel->setAlignment(Qt::AlignCenter);
        msgLabel->setStyleSheet("color: #E0E0E0; border: none; padding: 20px;");

        // 닫기 버튼
        QPushButton *closeButton = new QPushButton("닫기", background);
        closeButton->setFixedSize(80, 30);
        connect(closeButton, &QPushButton::clicked, this, &CustomMessageBox::accept);

        // X 버튼
        QToolButton *xButton = new QToolButton(m_titleBar);
        xButton->setText("✕");
        xButton->setStyleSheet("color: #AAAAAA; border: none; font-size: 14px;");
        connect(xButton, &QToolButton::clicked, this, &CustomMessageBox::accept);


        // Title Bar 레이아웃
        QHBoxLayout *topBarLayout = new QHBoxLayout(m_titleBar);
        topBarLayout->setContentsMargins(0, 0, 5, 0); // 오른쪽에 약간의 여백 추가
        topBarLayout->addStretch();                   // 왼쪽에 빈 공간을 추가하여
        topBarLayout->addWidget(xButton);             // 버튼을 오른쪽으로 밉니다.

        // 레이아웃
        QVBoxLayout *vbox = new QVBoxLayout(background);
        vbox->setContentsMargins(0, 0, 0, 0);
        vbox->setSpacing(0);
        vbox->addWidget(m_titleBar);
        vbox->addStretch();
        vbox->addWidget(msgLabel, 0, Qt::AlignCenter);
        vbox->addStretch();
        vbox->addWidget(closeButton, 0, Qt::AlignCenter);
        vbox->addStretch();

        // 메인 레이아웃
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0,0,0,0);
        mainLayout->addWidget(background);

        setFixedSize(360, 160);

        // 버튼 스타일
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
#ifdef DEBUG_UI_BORDER
        m_titleBar->setStyleSheet("border: 1px solid lime;"); // 타이틀 바 영역 (초록색)
        background->setStyleSheet(background->styleSheet() + " border: 1px solid red;"); // 전체 배경 영역 (빨간색)
#endif
    }
protected:
    // Event handlers to make the dialog draggable
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton && m_titleBar->geometry().contains(event->pos())) {
            m_isDragging = true;
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (event->buttons() & Qt::LeftButton && m_isDragging) {
            move(event->globalPosition().toPoint() - m_dragPosition);
            event->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        Q_UNUSED(event);
        m_isDragging = false;
    }

private:
    QPoint m_dragPosition;
    bool m_isDragging = false;
    QFrame* m_titleBar; // Draggable title bar

};

class CameraRegistrationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CameraRegistrationDialog(QWidget *parent = nullptr);

    QString deviceName() const { return nameEdit->text(); }
    QString ip() const { return ipEdit->text(); }
    QString port() const { return portEdit->text(); }
    void setInitialData(const QString &name, const QString &ip, const QString &port);

protected:
    // Event handlers to make the dialog draggable
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QLineEdit *nameEdit;
    QLineEdit *ipEdit;
    QLineEdit *portEdit;

    // For custom window dragging
    QFrame* titleBar;
    QPoint m_dragPosition;
    bool m_isDragging = false;

    bool validateInput();
private slots: // private slots 섹션 추가
    void onAcceptClicked(); // '확인' 버튼 클릭 시 호출될 슬롯
};


// ===================================================================
// [2024-12-19] 이미지 향상 기능 추가 - DragDropImageLabel 클래스
// 드래그 앤 드롭으로 이미지를 로드할 수 있는 커스텀 QLabel
// ===================================================================
class DragDropImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit DragDropImageLabel(QWidget *parent = nullptr);

signals:
    void imageDropped(const QPixmap &pixmap);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

class EventSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EventSearchDialog(const QList<MainWindow::RawEv>& eventData, QWidget *parent = nullptr);
    ~EventSearchDialog();
    void updateEventData(const QList<MainWindow::RawEv>& eventData);

protected:
    // 프레임 없는 창을 위한 이벤트 핸들러 오버라이드
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // [2024-12-19] 드래그 앤 드롭 이벤트 핸들러 추가
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onSearchClicked();
    void onResetClicked();
    void onDeviceComboChanged(const QString &device);
    void onEventTypeComboChanged(const QString &eventType);


private:
    void setupUi();
    void populateTable(const QList<MainWindow::RawEv>& eventData);
    void filterAndPopulateTable();
    QList<MainWindow::RawEv> filterEvents();
    void populateDeviceCombo();
    void populateEventTypeCombo();
    
    // [2024-12-19] 이미지 향상 기능 관련 함수들 추가
    void setupImageEnhancementControls();                    // 이미지 향상 컨트롤 초기화
    void onSharpnessChanged(int value);                     // 샤프닝 슬라이더 값 변경 처리
    void onContrastChanged(int value);                      // 대비 슬라이더 값 변경 처리
    void applyImageEnhancement();                           // 이미지 향상 효과 적용
    QWidget* createImageEnhancementWidget();                // 이미지 향상 UI 위젯 생성
    QPixmap enhanceSharpness(const QPixmap &pixmap, int level);  // 샤프닝 효과 적용
    QPixmap enhanceCLAHE(const QPixmap &pixmap, int level);      // 대비 효과 적용 (CLAHE)

public:
    void resetImagePreview();

private:
    // UI 위젯 포인터
    QTableWidget *resultsTable;
    QLabel *resultsCountLabel;
    QComboBox *deviceCombo;
    QComboBox *eventTypeCombo;
    // QDateTimeEdit *fromDateTime;
    // QDateTimeEdit *toDateTime;
    QDateEdit *fromDateEdit;
    QTimeEdit *fromTimeEdit;
    QDateEdit *toDateEdit;
    QTimeEdit *toTimeEdit;
    QPushButton *searchBtn;
    QPushButton *resetBtn;

    // 원본 데이터
    QList<MainWindow::RawEv> m_originalEventData;

    // 현재 검색 필터 상태 저장
    QString m_currentDeviceFilter = "All";
    QString m_currentEventTypeFilter = "All";
    QDateTime m_currentFromDate = QDateTime::currentDateTime().addDays(-2);
    QDateTime m_currentToDate = QDateTime::currentDateTime();

    // [2024-12-19] 이미지 향상 기능 관련 멤버 변수 추가
    QSlider *sharpnessSlider = nullptr;    // 샤프닝 조절 슬라이더
    QSlider *contrastSlider = nullptr;     // 대비 조절 슬라이더
    QLabel *sharpnessLabel = nullptr;      // 샤프닝 값 표시 라벨
    QLabel *contrastLabel = nullptr;       // 대비 값 표시 라벨
    QPixmap m_originalPixmap;              // 원본 이미지 저장 (비파괴적 편집용)
    QList<MainWindow::RawEv> m_filteredEventData;  // 필터링된 이벤트 데이터


    // 이미지 미리보기용
    DragDropImageLabel *imagePreview = nullptr;
    QNetworkAccessManager *m_networkManager = nullptr;

    // 창 드래그를 위한 멤버 변수
    QPoint dragPosition;
    bool isDragging = false;
};




class ExitConfirmDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExitConfirmDialog(QWidget *parent = nullptr)
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
        // [Req 2] Message Label: Font size 10px
        QLabel *messageLabel = new QLabel("프로그램을 종료하시겠습니까?", background);
        messageLabel->setAlignment(Qt::AlignCenter);
        messageLabel->setStyleSheet("color: #ffffff; font-size: 11px; border: none;");

        // [Req 3] Exit Button: Font size 10px
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
        mainLayout->addWidget(titleBar); // Add title bar at the top

        // [Req 2, 3] Adjust layout to move content down
        mainLayout->addStretch(5); // More stretch at the top to push content down
        mainLayout->addWidget(messageLabel);
        mainLayout->addSpacing(60); // Space between label and button
        mainLayout->addWidget(exitButton, 0, Qt::AlignHCenter);
        mainLayout->addStretch(1); // Less stretch at the bottom

        // --- Dialog Layout ---
        QVBoxLayout *dialogLayout = new QVBoxLayout(this);
        dialogLayout->setContentsMargins(0, 0, 0, 0);
        dialogLayout->addWidget(background);
    }

protected:
    // [Req 4] Add event handlers to make the dialog draggable via the title bar
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton && titleBar->geometry().contains(event->pos())) {
            m_isDragging = true;
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (event->buttons() & Qt::LeftButton && m_isDragging) {
            move(event->globalPosition().toPoint() - m_dragPosition);
            event->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        Q_UNUSED(event);
        m_isDragging = false;
    }

private:
    QPoint m_dragPosition;
    bool m_isDragging = false;
    QFrame* titleBar; // Member variable to access in mouse events
};



class BrightnessDialog : public QDialog
{
    Q_OBJECT

public:
     explicit BrightnessDialog(const QString &cameraName, int initialValue, QWidget *parent = nullptr);

signals:
    void brightnessChanged(int value);

protected:
    // Event handlers to make the dialog draggable
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QSlider *brightnessSlider;
    QLabel *valueLabel;

    // For custom window dragging
    QFrame* titleBar;
    QPoint m_dragPosition;
    bool m_isDragging = false;
};

#endif // MAINWINDOW_H


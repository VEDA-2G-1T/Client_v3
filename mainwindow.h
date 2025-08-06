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
#include <QProgressBar>
#include <QNetworkAccessManager>

#include "aspectratiowidget.h"
#include "dragdropimagelabel.h"
#include "custommessagebox.h"
#include "exitconfirmdialog.h"
#include "loginpopup.h"
#include "cameraregistrationdialog.h"
#include "brightnessdialog.h"
#include "notificationitemwidget.h"
#include "notificationlistpanel.h"
#include "loginwidget.h"
#include "customtabbar.h"
#include "eventcard.h" 
#include "continuousdetectionpopupdialog.h"
#include "fullscreencameradialog.h" 
/* refactoring */
class AspectRatioWidget;
class DragDropImageLabel;
class CustomMessageBox;
class ExitConfirmDialog;
class LoginPopup;
class CameraRegistrationDialog;
class BrightnessDialog;
class NotificationListPanel;
class LoginWidget;
class CustomTabBar;
class EventCard;
class FullscreenCameraDialog;
/* refactoring */
class SystemMonitorDialog;
class QTreeWidgetItem;
class QWebSocket;
class EventSearchDialog;



// [2024-08-01] CameraSystemInfo 구조체를 클래스 밖으로 이동
struct CameraSystemInfo {
    QString name;
    QLabel *nameLabel;
    QProgressBar *cpuBar;
    QProgressBar *memoryBar;
    QLabel *cpuLabel;
    QLabel *memoryLabel;
    QWidget *container;
    double cpuUsage = 0.0;
    double memoryUsage = 0.0;
    bool hasWebSocketData = false;
};



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

private:
    // 테마 관련 함수들
    bool isDarkMode() const;
    QString getThemeColor(const QString& darkColor, const QString& lightColor) const;
    void applyThemeColors();
    void updateAllStyleSheets();

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

    void onFullscreenButtonClicked(int slotIndex);

    void handleLogout();
    void onLoginSuccess();

    void onWebSocketConnected();
    void onWebSocketDisconnected();
    void onWebSocketError(QAbstractSocket::SocketError error);
    void onWebSocketMessageReceived(const QString &message);

    void showEventSearchDialog();
    void showSystemMonitorDialog();
    void createCameraSystemMonitor(const QString &cameraName, const QString &cameraIp);
    void removeCameraSystemMonitor(const QString &cameraIp);

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
    SystemMonitorDialog *m_systemMonitorDialog = nullptr;
    // 카메라별 시스템 모니터는 더 이상 사용하지 않음 (통합된 하나의 다이얼로그 사용)
    QMap<QString, SystemMonitorDialog*> m_cameraSystemMonitors; // [2024-08-01] 카메라별 시스템 모니터 맵 추가

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
    void resizeEvent(QResizeEvent *event) override;




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

    // 등록된 카메라 정보 저장 (메모리 확인 버튼용)
    QMap<QString, QString> m_registeredCameras; // IP -> 카메라명

private slots:
    void onDetectionTimerTimeout(const QString& cameraName);  // 감지 타이머 타임아웃 처리
    void showContinuousDetectionPopup(const QString& cameraName, const QString& imageUrl);  // 연속 감지 팝업 표시
    void handleContinuousDetection(const QString& cameraName, const QString& imageUrl);  // 연속 감지 처리
    void resetDetectionCounter(const QString& cameraName);  // 감지 카운터 리셋
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
    QMediaPlayer* getMediaPlayer() const { return mediaPlayer; }
    QVariantMap getCurrentData() const { return m_currentData; }



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



// ===================================================================
// SystemMonitorDialog 클래스
class SystemMonitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemMonitorDialog(const QString &cameraName = QString(), QWidget *parent = nullptr);
    ~SystemMonitorDialog();

    void setCameraName(const QString &cameraName);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
    void updateSystemInfo();
    void onCloseButtonClicked();
    void updateSystemInfoFromWebSocket(double cpuUsage, double memoryUsage);
    void addCameraInfo(const QString &cameraName, const QString &cameraIp);
    void removeCameraInfo(const QString &cameraIp);
    void updateCameraSystemInfo(const QString &cameraIp, double cpuUsage, double memoryUsage);
    void restartUpdateTimer();

public:
    QMap<QString, CameraSystemInfo> m_cameraInfos; // [2024-08-01] public으로 변경

private:
    void setupUI();
    void getSystemInfo(double& cpuUsage, double& memoryUsage);
    void updateDialogSize();

    QLabel *m_cpuLabel;
    QLabel *m_memoryLabel;
    QProgressBar *m_cpuBar;
    QProgressBar *m_memoryBar;
    QTimer *m_updateTimer;
    QFrame* m_titleBar;
    QPoint m_dragPosition;
    bool m_isDragging = false;

    // 카메라 정보
    QString m_cameraName;

    // 웹소켓으로 받은 시스템 정보
    double m_webSocketCpuUsage = -1.0;
    double m_webSocketMemoryUsage = -1.0;
    bool m_hasWebSocketData = false;
    QTimer *m_webSocketTimeoutTimer;

    // 카메라별 시스템 정보
    QVBoxLayout *m_mainLayout;
};

#endif // MAINWINDOW_H


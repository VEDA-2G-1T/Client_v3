// mainwindow.cpp
#include "mainwindow.h"

#include <QToolBar>
#include <QToolButton>
#include <QTabBar>
#include <QStackedWidget>
#include <QIcon>
#include <QSplitter>
#include <QTreeWidget>
#include <QListWidget>
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>
#include <QScrollArea>
#include <QFileInfo>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QScrollBar>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QStyleOptionTab>

#include <QProcess>
#include <QTemporaryDir>
#include <QDirIterator>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QScreen>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QStackedLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QComboBox>
#include <QHeaderView>
#include <QDebug>
#include <QStandardPaths>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QTimer>

// #define DEBUG_UI_BORDER
#ifdef DEBUG_UI_BORDER
static const bool showDebugFrame = true;
#else
static const bool showDebugFrame = false;
#endif

static void applyDebugBorder(QWidget *w, const QString &color) {
    if (showDebugFrame && w) {
        QString s = w->styleSheet();
        w->setStyleSheet(s + QString(" border:1px solid %1;").arg(color));
    }
}

// ─── MainWindow 생성자 ────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("QMainWindow { border: 1px solid #5c5c5c; }");
    resize(1280, 720);
    QIcon app_icon(":/icons/icons/app_icon.png");
    setWindowIcon(app_icon);

    // ─── [1] Top Bar ───────────────────────────────────────────────
    topBar = new QToolBar(this);
    topBar->setMovable(false);
    topBar->setIconSize(QSize(24,24));
    const int desiredHeight = 40;
    // topBar->setMinimumHeight(desiredHeight);
    topBar->setFixedHeight(desiredHeight);
    topBar->setStyleSheet("QToolBar { border: none; }");
    auto *tbCont = new QWidget(this);
    auto *tbLay = new QHBoxLayout(tbCont);
    tbLay->setContentsMargins(0,0,0,0);
    tbLay->setSpacing(4);

    // [1-1] Logo
    // 1. 로고와 레이아웃을 담을 컨테이너 위젯을 생성합니다.
    auto *logoContainer = new QWidget(tbCont);
    // 2. WISENET UI와 유사하게 컨테이너의 너비를 250px로 고정합니다.
    logoContainer->setFixedWidth(250);

    // 3. 컨테이너 위젯이 사용할 레이아웃을 생성합니다.
    auto *logoLayout = new QHBoxLayout(logoContainer);
    logoLayout->setContentsMargins(0, 0, 0, 0); // 내부 여백은 0으로 설정
    logoLayout->setAlignment(Qt::AlignCenter); // 내용물(로고)을 가운데 정렬

    // 4. 실제 로고 이미지를 표시할 QLabel을 생성합니다.
    auto *logoLabel = new QLabel(logoContainer);
    QPixmap logoPixmap(":/icons/icons/45cac18a-7e5b-44a9-8a11-faf5878934f3-removebg-preview_cut_2.png");
    logoLabel->setPixmap(logoPixmap.scaledToHeight(28, Qt::SmoothTransformation));

    // 5. 로고 QLabel을 컨테이너의 레이아웃에 추가합니다.
    logoLayout->addWidget(logoLabel);

    // 6. 최종적으로 완성된 logoContainer를 상단 툴바 레이아웃에 추가합니다.
    tbLay->addWidget(logoContainer);

    // [1-2] “+” Button
    auto *addBtn = new QToolButton(tbCont);
    addBtn->setText("+");
    addBtn->setFixedSize(28,28);
    addBtn->setAutoRaise(true);
    addBtn->setToolTip("Add view");
    tbLay->addWidget(addBtn);


    // [1‑3] View Tabs bar (CustomTabBar)
    viewTabBar = new CustomTabBar(tbCont);
    viewTabBar->setFixedHeight(desiredHeight);
    viewTabBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    viewTabBar->setFixedTabWidth(200);
    tbLay->addWidget(viewTabBar, /*stretch=*/1);


    // [1-4] Clock
    auto *clock = new QLabel(tbCont);
    clock->setMinimumWidth(80);
    clock->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    tbLay->addWidget(clock);

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [clock]() {
        QTime currentTime = QTime::currentTime();

        int hour = currentTime.hour();
        QString ampm = (hour < 12) ? "오전" : "오후";
        int hour12 = hour % 12;
        if (hour12 == 0) hour12 = 12; // 0시는 12시로

        QString timeText = QString("%1:%2:%3")
                               .arg(hour12, 2, 10, QLatin1Char('0'))
                               .arg(currentTime.minute(), 2, 10, QLatin1Char('0'))
                               .arg(currentTime.second(), 2, 10, QLatin1Char('0'));

        clock->setText(QString("%1 %2").arg(ampm, timeText));
    });
    timer->start(1000);

    // [1-5] Icon buttons 예시
    constexpr QSize kIconSize(20, 20);
    constexpr QSize kButtonSize(24, 24);

    const QVector<QString> iconPaths = {
        ":/icons/icons/memory_24dp_BCAD20_FILL0_wght400_GRAD0_opsz24.svg",
        ":/icons/icons/campaign_24dp_EA3323_FILL0_wght400_GRAD0_opsz24.svg",
        ":/icons/icons/account_circle_24dp_E3E3E3_FILL0_wght400_GRAD0_opsz24.svg",
        ":/icons/icons/help_24dp_E3E3E3_FILL0_wght400_GRAD0_opsz24.svg"
    };

    QToolButton *campaignButton = nullptr; // 임시 포인터
    QToolButton *accountButton = nullptr;
    QToolButton *helpButton = nullptr;

    for (const QString& path : iconPaths) {
        auto* btn = new QToolButton(tbCont);

        btn->setIcon(QIcon(path));
        btn->setIconSize(kIconSize);
        btn->setFixedSize(kButtonSize);
        btn->setAutoRaise(true);

        tbLay->addWidget(btn);

        if (path.contains("campaign")) {
            campaignButton = btn; // 버튼 찾기
        }
        if (path.contains("account_circle")) {
            accountButton = btn;
        }
        if (path.contains("help")) {
            helpButton = btn;
        }

    }

    // --- 알림 패널 생성 및 연결 ---
    m_notificationPanel = new NotificationListPanel(this);
    m_notificationPanel->hide(); // 처음엔 숨겨둠

    // 예시 데이터 추가
    m_notificationPanel->addNotification("[서비스] CPU 사용량 안정 (모든 프레임 재생)", "CPU 사용량이 안정되어 모든 프레임을 재생합니다.", QDateTime::currentDateTime().addSecs(-60*2), true);
    m_notificationPanel->addNotification("[서비스] CPU 사용량 증가 (I-프레임 재생)", "CPU 사용량이 증가하여 프레임 건너뛰기 현상이 발생할 수 있습니다. 재생 중인 채널 수를 줄이십시오.", QDateTime::currentDateTime().addSecs(-60), true);



    // 버튼과 토글 슬롯 연결
    if (campaignButton) {
        connect(campaignButton, &QToolButton::clicked, this, &MainWindow::toggleNotificationPanel);
    }

    // --- 로그인 팝업 생성 및 연결 ---
    m_loginPopup = new LoginPopup(this);
    m_loginPopup->hide();
    connect(m_loginPopup, &LoginPopup::logoutRequested, this, &MainWindow::handleLogout);
    if (accountButton) {
        connect(accountButton, &QToolButton::clicked, this, &MainWindow::toggleLoginPopup);
    }

    if (helpButton) {
        connect(helpButton, &QToolButton::clicked, this, &MainWindow::openHelpFile);
    }

    // [1-6] Separator
    tbLay->addSpacing(4);
    {
        auto *sep = new QFrame(tbCont);
        sep->setFrameShape(QFrame::VLine);
        sep->setFixedSize(1,16);
        sep->setStyleSheet("background-color:#888888;");
        tbLay->addWidget(sep);
    }
    tbLay->addSpacing(4);

    // [1-7] Window controls
    auto *minB = new QPushButton("―", tbCont);
    maxB = new QPushButton("☐", tbCont);
    auto *clsB = new QPushButton("✕", tbCont);
    for(auto *b:{minB, maxB, clsB}) {
        b->setFixedSize(24,24);
        b->setFlat(true);
        tbLay->addWidget(b);
    }
    // 최소화/최대화 버튼을 위한 스타일시트
    const QString minMaxStyle = R"(
        QPushButton {
            background-color: transparent;
            border: none;
            color: #E0E0E0;
            font-size: 14px;
        }
        QPushButton:hover { background-color: #555555; }
        QPushButton:pressed { background-color: #444444; }
    )";

    // 닫기 버튼을 위한 스타일시트 (마우스 올리면 빨간색으로 변경)
    const QString closeStyle = R"(
        QPushButton {
            background-color: transparent;
            border: none;
            color: #E0E0E0;
            font-size: 14px;
        }
        QPushButton:hover { background-color: #E81123; }
        QPushButton:pressed { background-color: #B60F1C; }
    )";

    // 각 버튼에 스타일시트 적용
    minB->setStyleSheet(minMaxStyle);
    maxB->setStyleSheet(minMaxStyle);
    clsB->setStyleSheet(closeStyle);
    connect(minB, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(maxB, &QPushButton::clicked, this, [=](){
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(clsB, &QPushButton::clicked, this, &MainWindow::onCloseButtonClicked);

    tbCont->setLayout(tbLay);
    topBar->addWidget(tbCont);
    addToolBar(Qt::TopToolBarArea, topBar);

    // ─── [2] Left Panel ────────────────────────────────────────────
    auto *leftP = new QWidget(this);
    // leftP->setMinimumWidth(240);
    auto *leftL = new QVBoxLayout(leftP);
    leftL->setContentsMargins(2,2,2,2);
    leftL->setSpacing(6);

    // [2-1] Mini icon bar
    auto *mini = new QWidget(leftP);
    auto *gridLayout = new QGridLayout(mini);
    gridLayout->setContentsMargins(0, 4, 0, 4);
    gridLayout->setHorizontalSpacing(8);   // ← 아이콘 좌우 간격 확보
    gridLayout->setVerticalSpacing(8);    // ← 행 간격도 적당히 확보
    gridLayout->setAlignment(Qt::AlignCenter);

    // 아이콘 및 툴팁 정의
    struct MiniIconInfo {
        QString iconPath;
        QString tooltip;
    };

    const QVector<MiniIconInfo> iconInfos = {
        {":/icons/icons/search_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg", "Search"},
        {":/icons/icons/settings_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg",  "Settings"},
        {":/icons/icons/refresh_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg",  "Refresh"},
        {":/icons/icons/notifications_active_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg",   "What0"},
        {":/icons/icons/volume_up_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg", "Speaker"},

        {":/icons/icons/fullscreen_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg",   "Fullscreen"},
        {":/icons/icons/airplay_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg", "what1"},
        {":/icons/icons/featured_video_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg",   "what2"},
        {":/icons/icons/branding_watermark_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg",  "what3"},
        {":/icons/icons/speed_camera_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg","카메라 등록"}
    };

    constexpr int kColumnCount = 5;
    QToolButton *registerCameraBtn = nullptr;
    for (int i = 0; i < iconInfos.size(); ++i) {
        const auto& info = iconInfos[i];

        auto *button = new QToolButton;
        button->setIcon(QIcon(info.iconPath));
        button->setToolTip(info.tooltip);
        button->setFixedSize(kButtonSize);
        button->setIconSize(kButtonSize);
        button->setAutoRaise(true);

        int row = i / kColumnCount;
        int col = i % kColumnCount;

        if (info.iconPath.contains("notifications_active")) {
            connect(button, &QToolButton::clicked, this, &MainWindow::onBrightnessControlClicked);
        }  else if (info.iconPath.contains("speed_camera")) {
            registerCameraBtn = button;
        }


        gridLayout->addWidget(button, row, col);
    }

    if (registerCameraBtn) {
        connect(registerCameraBtn,
                &QToolButton::clicked,
                this,
                &MainWindow::onGlobalCameraRegisterClicked);
    }

    mini->setLayout(gridLayout);
    mini->setStyleSheet("background-color:#2b2b2b;");
    leftL->addWidget(mini);


    // [2-2] Resource tree
    auto *resT = new QTreeWidget(leftP);
    resT->setHeaderHidden(true);
    resT->setMinimumWidth(250);
    auto *rr = new QTreeWidgetItem(QStringList("DESKTOP-UHT (admin)"));
    auto *rg = new QTreeWidgetItem(rr, QStringList("Group"));
    rg->addChild(new QTreeWidgetItem(QStringList("2:1 Panoramic")));
    rg->addChild(new QTreeWidgetItem(QStringList("3:4 Panoramic")));
    resT->addTopLevelItem(rr);
    resT->expandAll();
    resT->setStyleSheet(R"(
    QTreeWidget{background:#1e1e1e;color:#ccc;border:none;}
    QTreeWidget::item:selected{background:#444;color:#fff;}
  )");
    leftL->addWidget(resT , 1);

    // [2-3] Layout tree
    layoutTree = new QTreeWidget(leftP);
    layoutTree->setHeaderHidden(true);
    layoutTree->setSortingEnabled(false);
    leftL->addWidget(layoutTree,2);
    connect(layoutTree, &QTreeWidget::itemClicked,
            this, &MainWindow::onLayoutTreeClicked);

    layoutTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(layoutTree, &QTreeWidget::customContextMenuRequested,
            this, &MainWindow::onLayoutTreeContextMenu);

    leftP->setLayout(leftL);

    // ─── [3] Center & placeholder ─────────────────────────────────
    emptyWidget = new QWidget(this);
    {
        int sbw = QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent);
        emptyWidget->setMinimumSize(320 * 3 + sbw, 240 * 3 + sbw);

        // 1. 레이아웃의 여백을 10px로 설정합니다.
        auto *vl = new QVBoxLayout(emptyWidget);
        vl->setContentsMargins(10, 10, 10, 10);

        bigAddBtn = new QToolButton(emptyWidget);
        bigAddBtn->setText("+");

        // 2. 버튼이 레이아웃 공간을 가득 채우도록 크기 정책을 'Expanding'으로 설정합니다.
        bigAddBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 3. 버튼이 커짐에 따라 '+' 기호도 크게 보이도록 폰트 크기를 조정합니다.
        QFont font = bigAddBtn->font();
        font.setPointSize(48); // 필요에 따라 폰트 크기 조정
        bigAddBtn->setFont(font);

        bigAddBtn->setAutoRaise(true);
        bigAddBtn->setToolTip("Add view");
        // bigAddBtn->hide();
        vl->addWidget(bigAddBtn);
    }

    viewStack = new QStackedWidget(this);
    centerStack = new QStackedWidget(this);
    centerStack->addWidget(emptyWidget);
    centerStack->addWidget(viewStack);
    centerStack->setCurrentWidget(emptyWidget);

    connect(addBtn,  &QToolButton::clicked, this, &MainWindow::onAddView);
    connect(bigAddBtn, &QToolButton::clicked, this, &MainWindow::onAddView);
    connect(viewTabBar, &QTabBar::currentChanged, this, &MainWindow::onSwitchView);
    connect(viewTabBar, &QTabBar::tabCloseRequested,this, &MainWindow::onCloseView);

    // ─── [4] Right Event ──────────────────────────────────────────
    auto *evtP = new QWidget(this);
    evtP->setMinimumWidth(300);
    auto *evL = new QVBoxLayout(evtP);
    evL->setContentsMargins(0,0,0,0);
    evL->setSpacing(0);
    auto *evTab = new QTabBar(evtP);
    evTab->setFixedHeight(40);
    evTab->addTab(QIcon(":/icons/icons/siren_16dp_6D6D6D_FILL0_wght400_GRAD0_opsz20.svg"), "Event");
    evTab->addTab(QIcon(":/icons/icons/bookmark_16dp_6D6D6D_FILL0_wght400_GRAD0_opsz20.svg"), "Bookmark");
    evTab->setExpanding(true);
    evTab->setDrawBase(false);
    evTab->setStyleSheet(R"(
   QTabBar::tab{color: #F4731F; background:transparent;color:#888;padding:6px 12px;border:none;}
   QTabBar::tab:selected{color:#fff;border-bottom:2px solid #f4731f;}
  )");
    evL->addWidget(evTab);


    // — Event 페이지: 동적 EventCard 생성을 위해 레이아웃 설정
    m_eventData.clear();
    eventContainer = new QWidget(evtP);
    eventLay = new QVBoxLayout(eventContainer);
    eventLay->setContentsMargins(4,4,4,4);
    eventLay->setSpacing(12);
    eventLay->addStretch();

    auto *eventScroll = new QScrollArea(evtP);
    eventScroll->setWidgetResizable(true);
    eventScroll->setFrameShape(QFrame::NoFrame);
    eventScroll->setWidget(eventContainer);
    // — Bookmark 페이지
    auto *bookmarkContainer = new QWidget(evtP);
    bookmarkLay = new QVBoxLayout(bookmarkContainer);
    bookmarkLay->setContentsMargins(4,4,4,4);
    bookmarkLay->setSpacing(8);
    bookmarkLay->addStretch();
    auto *bookmarkScroll = new QScrollArea(evtP);
    bookmarkScroll->setWidgetResizable(true);
    bookmarkScroll->setFrameShape(QFrame::NoFrame);
    bookmarkScroll->setWidget(bookmarkContainer);
    // — 두 페이지 스택에 추가 및 탭 연결
    auto *evSt = new QStackedWidget(evtP);
    evSt->addWidget(eventScroll);
    evSt->addWidget(bookmarkScroll);
    connect(evTab, &QTabBar::currentChanged,
            evSt, &QStackedWidget::setCurrentIndex);

    // 1. Add the event/bookmark lists to the main layout.
    evL->addWidget(evSt, 1);

    // 2. Create the button.
    auto *eventSearchButton = new QPushButton(QIcon(":/icons/icons/search_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"), " Event Search", evtP);
    eventSearchButton->setCursor(Qt::PointingHandCursor);
    eventSearchButton->setMinimumHeight(40);
    eventSearchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    eventSearchButton->setStyleSheet(R"(
        QPushButton {
            color: #E0E0E0;
            background-color: #1e1e1e;
            border: 1px solid #909090;
            border-radius: 0px;
            font-size: 14px;
        }
        QPushButton:hover { background-color: #f4731f; border-color: #f58a4e; }
    )");

    // 3. Add the button to fill the width (NO ALIGNMENT FLAG).
    //    And connect its clicked signal to the new slot.
    evL->addWidget(eventSearchButton);
    connect(eventSearchButton, &QPushButton::clicked, this, &MainWindow::showEventSearchDialog);

    // ─── [5] Main Layout ──────────────────────────────────────────
    auto *splitC = new QSplitter(Qt::Horizontal, this);
    splitC->addWidget(centerStack);
    splitC->addWidget(evtP);
    splitC->setStretchFactor(0,1);
    splitC->setStretchFactor(1,0);

    auto *mainContentWidget = new QSplitter(Qt::Horizontal, this); // 기존 splitM의 역할
    mainContentWidget->addWidget(leftP);
    mainContentWidget->addWidget(splitC);
    mainContentWidget->setStretchFactor(0,0);
    mainContentWidget->setStretchFactor(1,1);
    // ─── 로그인 위젯 및 화면 전환 스택 생성 ────────────────────────
    // 로그인 관련 디버그 주석
    LoginWidget *loginWidget = new LoginWidget(this);

    m_mainStack = new QStackedWidget(this);
    m_mainStack->addWidget(loginWidget);      // 0번 페이지: 로그인 화면
    m_mainStack->addWidget(mainContentWidget); // 1번 페이지: 메인 컨텐츠 화면

    setCentralWidget(m_mainStack); // 메인 스택을 중앙 위젯으로 설정

    // 로그인 성공 신호를 onLoginSuccess 슬롯에 연결
    connect(loginWidget, &LoginWidget::loginSuccessful, this, &MainWindow::onLoginSuccess);


    // ─── Debug Borders ────────────────────────────────────────────
    QList<QPair<QWidget*, QString>> debugList = {
        {tbCont,   "#FF3030"},
        {leftP,   "yellow"},
        {mini,    "yellow"},
        {resT,    "yellow"},
        {layoutTree, "lime"},
        {centerStack, "#00BFFF"},
        {evtP,    "magenta"}
    };
    // 로그인 관련 디버그 주석
    if (loginWidget) {
        debugList.append({loginWidget, "cyan"});
        if (auto* formFrame = loginWidget->findChild<QFrame*>("formFrame")) {
            debugList.append({formFrame, "red"});
        }
    }

    for (const auto &item : debugList)
        applyDebugBorder(item.first, item.second);


    //디버깅 용
    // setCentralWidget(mainContentWidget);
    // 초기 탭 하나 생성
    layoutSlotData.clear();

    // 네트워크 매니저 초기화
    m_networkManager = new QNetworkAccessManager(this);

    //디버깅 용
    // onAddView();
}

MainWindow::~MainWindow()
{
    // WebSocket 연결들 정리
    for (auto it = m_webSocketMap.begin(); it != m_webSocketMap.end(); ++it) {
        QWebSocket *socket = it.value();
        if (socket) {
            socket->close();
            socket->deleteLater();
        }
    }
    m_webSocketMap.clear();

    // 헬스체크 타이머들 정리
    for (auto it = m_healthCheckTimers.begin(); it != m_healthCheckTimers.end(); ++it) {
        QTimer *timer = it.value();
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
    }
    m_healthCheckTimers.clear();

    // EventSearchDialog 정리
    if (m_eventSearchDialog) {
        m_eventSearchDialog->deleteLater();
        m_eventSearchDialog = nullptr;
    }

    // 네트워크 매니저 정리
    if (m_networkManager) {
        m_networkManager->deleteLater();
        m_networkManager = nullptr;
    }

    // 카메라 슬롯들 정리
    for (int i = 0; i < viewStack->count(); ++i) {
        QWidget *page = viewStack->widget(i);
        if (page) {
            for (auto *slot : page->findChildren<CameraSlot*>()) {
                slot->stopStream();
            }
        }
    }

    qDebug() << "MainWindow destructor completed successfully";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 기존의 try-catch 블록을 제거했습니다. 여기에 사용된 Qt 함수들은
    // 예외를 발생시키지 않으므로, 해당 블록은 불필요하며 다른 잠재적 문제를 가릴 수 있습니다.

    qDebug() << "MainWindow closeEvent called";

    // 모든 WebSocket 연결 종료
    for (auto it = m_webSocketMap.begin(); it != m_webSocketMap.end(); ++it) {
        QWebSocket *socket = it.value();
        if (socket && socket->state() == QAbstractSocket::ConnectedState) {
            socket->close();
        }
    }

    // 모든 타이머 정지
    for (auto it = m_healthCheckTimers.begin(); it != m_healthCheckTimers.end(); ++it) {
        QTimer *timer = it.value();
        if (timer) {
            timer->stop();
        }
    }

    // EventSearchDialog 닫기
    if (m_eventSearchDialog && m_eventSearchDialog->isVisible()) {
        m_eventSearchDialog->close();
    }

    // 기본 closeEvent 처리
    QMainWindow::closeEvent(event);

    qDebug() << "MainWindow closeEvent completed successfully";
}

// ─── Slots / View 관리 ──────────────────────────────────────────
void MainWindow::onAddView()
{
    if (viewTabBar->count() >= 5) {
        // QMessageBox::warning(this, tr("Limit reached"),
        //                      tr("최대 5개의 탭만 추가할 수 있습니다."));
        CustomMessageBox msg("최대 5개의 탭만 추가할 수 있습니다.", this);
        msg.exec();
        return;
    }
    int idx = viewTabBar->count() + 1;
    viewTabBar->addTab(tr("Layout %1").arg(idx));

    // 디버그용
    QVector<QVariantMap> newLayoutSlots(6); // 6 empty slots

    if (viewTabBar->count() == 1) {
        // 테스트 카메라 1 (RTSP, 웹소켓 미사용)
        QVariantMap cam1;
        cam1["type"] = "camera";
        cam1["name"] = "onvif";
        cam1["ip"]   = "192.168.0.35";
        cam1["port"] = "554";
        cam1["url"]  = QString("rtsp://%1:%2/0/onvif/profile2/media.smp").arg(cam1["ip"].toString(), cam1["port"].toString());
        cam1["mode"] = "raw"; // 'mode' 키 추가
        newLayoutSlots[5] = cam1;

        // // 테스트 카메라 2 (RTMPS, 웹소켓 사용)
        // QVariantMap cam2;
        // cam2["type"] = "camera";
        // cam2["name"] = "출입구";
        // cam2["ip"]   = "192.168.0.54";
        // cam2["port"] = "8555";
        // cam2["url"]  = QString("rtsps://%1:%2/processed").arg(cam2["ip"].toString(), cam2["port"].toString());
        // cam2["mode"] = "raw"; // 'mode' 키 추가
        // newLayoutSlots[0] = cam2;
        // // 웹소켓 설정을 수동으로 호출합니다.
        // setupWebSocket(cam2["ip"].toString(), cam2["port"].toString());

        // // 테스트 카메라 3 (RTMPS, 웹소켓 사용)
        // QVariantMap cam3;
        // cam3["type"] = "camera";
        // cam3["name"] = "작업장2";
        // cam3["ip"]   = "192.168.0.36";
        // cam3["port"] = "8555";
        // cam3["url"]  = QString("rtsps://%1:%2/processed").arg(cam3["ip"].toString(), cam3["port"].toString());
        // cam3["mode"] = "raw"; // 'mode' 키 추가
        // newLayoutSlots[1] = cam3;
        // // 웹소켓 설정을 수동으로 호출합니다.
        // setupWebSocket(cam3["ip"].toString(), cam3["port"].toString());

        // // 테스트 카메라 4 (RTMPS, 웹소켓 사용)
        // QVariantMap cam4;
        // cam4["type"] = "camera";
        // cam4["name"] = "모서리";
        // cam4["ip"]   = "192.168.0.14";
        // cam4["port"] = "8555";
        // cam4["url"]  = QString("rtsps://%1:%2/processed").arg(cam4["ip"].toString(), cam4["port"].toString());
        // cam4["mode"] = "raw"; // 'mode' 키 추가
        // newLayoutSlots[2] = cam4;
        // // 웹소켓 설정을 수동으로 호출합니다.
        // setupWebSocket(cam4["ip"].toString(), cam4["port"].toString());

        // // 테스트 카메라 4 (RTMPS, 웹소켓 사용)
        // QVariantMap cam5;
        // cam5["type"] = "camera";
        // cam5["name"] = "작업장3";
        // cam5["ip"]   = "192.168.0.12";
        // cam5["port"] = "8555";
        // cam5["url"]  = QString("rtsps://%1:%2/processed").arg(cam5["ip"].toString(), cam5["port"].toString());
        // cam5["mode"] = "raw"; // 'mode' 키 추가
        // newLayoutSlots[4] = cam5;
        // // 웹소켓 설정을 수동으로 호출합니다.
        // setupWebSocket(cam5["ip"].toString(), cam5["port"].toString());
        // // 테스트 카메라 4 (RTMPS, 웹소켓 사용)
        // QVariantMap cam6;
        // cam6["type"] = "camera";
        // cam6["name"] = "작업장5";
        // cam6["ip"]   = "192.168.0.18";
        // cam6["port"] = "8555";
        // cam6["url"]  = QString("rtsps://%1:%2/processed").arg(cam6["ip"].toString(), cam6["port"].toString());
        // cam6["mode"] = "raw"; // 'mode' 키 추가
        // newLayoutSlots[3] = cam6;
        // // 웹소켓 설정을 수동으로 호출합니다.
        // setupWebSocket(cam6["ip"].toString(), cam6["port"].toString());

    }


    layoutSlotData.append(newLayoutSlots);

    // 새 페이지(스크롤 포함) 생성
    QWidget *page = createCameraGridWidget();
    auto slots_ = page->findChildren<CameraSlot*>();
    for (auto *s : slots_) {
        connect(s, &CameraSlot::clicked,   this, &MainWindow::onCameraSlotClicked);
        connect(s, &CameraSlot::dropOccurred, this, &MainWindow::onSlotDropped);
        connect(s, &CameraSlot::cameraRegistered, this, &MainWindow::onCameraRegistered);
        connect(s, &CameraSlot::deleteRequested, this, &MainWindow::onCameraDeleteRequested);
        connect(s, &CameraSlot::modeChanged, this, &MainWindow::onCameraModeChanged);
        connect(s, &CameraSlot::validationFailed, this, &MainWindow::onCameraValidationFailed);

    }

    viewStack->addWidget(page);
    viewTabBar->setCurrentIndex(idx-1);
    viewTabBar->setVisible(true);
    centerStack->setCurrentWidget(viewStack);
    updateLayoutTree();
}

void MainWindow::onCloseButtonClicked()
{
    ExitConfirmDialog dialog(this);

    // 다이얼로그를 부모 윈도우 중앙에 위치시키기
    QPoint centerPos = geometry().center() - QPoint(dialog.width() / 2, dialog.height() / 2);
    dialog.move(centerPos);

    if (dialog.exec() == QDialog::Accepted) {
        // 사용자가 "프로그램 종료"를 선택했을 때

        // WebSocket 연결들 정리
        for (auto it = m_webSocketMap.begin(); it != m_webSocketMap.end(); ++it) {
            QWebSocket *socket = it.value();
            if (socket) {
                socket->close();
                socket->deleteLater();
            }
        }
        m_webSocketMap.clear();

        // 모든 미디어 플레이어 정지
        for (int i = 0; i < viewStack->count(); ++i) {
            QWidget *page = viewStack->widget(i);
            if (page) {
                for (auto *slot : page->findChildren<CameraSlot*>()) {
                    slot->stopStream();
                }
            }
        }

        // 애플리케이션 종료
        QApplication::quit();
    }
    // 사용자가 "취소"를 선택했거나 X를 눌렀을 때는 아무것도 하지 않음
}

void MainWindow::onSwitchView(int idx)
{
    if (auto *prev = qobject_cast<QWidget*>(viewStack->currentWidget())) {
        for (auto *s : prev->findChildren<CameraSlot*>())
            s->setActive(false);
    }
    if (idx >= 0 && idx < viewStack->count())
        viewStack->setCurrentIndex(idx);
}

void MainWindow::onCloseView(int idx)
{
    if (idx < 0 || idx >= viewStack->count()) return;
    if (idx >= layoutSlotData.size()) return;

    const auto& slotsData = layoutSlotData.at(idx);
    for (const QVariantMap& slotData : slotsData) {
        if (slotData.value("type").toString() == "camera") {
            QString ip = slotData.value("ip").toString();
            if (m_webSocketMap.contains(ip)) {
                qDebug() << "Closing WebSocket for IP:" << ip << "due to view close";
                QWebSocket *socket = m_webSocketMap.take(ip);
                socket->close();
                socket->deleteLater();
            }
        }
    }


    QWidget *w = viewStack->widget(idx);

    for (auto* slot : w->findChildren<CameraSlot*>()) {
        slot->stopStream();
    }

    viewStack->removeWidget(w);
    viewTabBar->removeTab(idx);
    delete w;
    layoutSlotData.removeAt(idx);

    if (viewTabBar->count() == 0) {
        viewTabBar->setVisible(false);
        centerStack->setCurrentWidget(emptyWidget);
    }
    updateLayoutTree();
}

void MainWindow::onLayoutTreeClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "--- onLayoutTreeClicked 슬롯 호출 테스트 ---";

    if (item) {
        qDebug() << "클릭된 아이템:" << item->text(0);
        if (item->parent()) {
            qDebug() << "부모 아이템:" << item->parent()->text(0);
        } else {
            qDebug() << "부모 아이템 없음 (최상위 아이템)";
        }
    }

    auto *root = layoutTree->topLevelItem(0);
    // 클릭된 아이템이나 부모가 없는 경우 즉시 리턴하여 안정성 확보
    if (!root || !item || !item->parent()) return;

    // --- 1. 모드 아이템 클릭 확인 ---
    // 모드 아이템은 고유한 데이터(UserRole + 1)를 가지고 있으므로, 이것으로 먼저 확인합니다.
    QVariant modeData = item->data(0, Qt::UserRole + 1);
    if (modeData.isValid()) {
        QTreeWidgetItem* camItem = item->parent();
        QTreeWidgetItem* layItem = camItem->parent();

        // 올바른 계층 구조인지 한번 더 확인 (모드->카메라->레이아웃)
        if (layItem && layItem->parent() == root) {
            QString mode = modeData.toString();
            int slotIdx = camItem->data(0, Qt::UserRole).toInt();
            int layIdx = root->indexOfChild(layItem);

            if (viewTabBar->currentIndex() != layIdx) {
                viewTabBar->setCurrentIndex(layIdx);
            }
            // 모드 변경을 처리하는 핵심 함수 호출
            onCameraModeChanged(slotIdx, mode);
            return; // 모드 변경 처리 후 함수 종료
        }
    }

    // --- 2. 카메라 또는 레이아웃 아이템 클릭 확인 ---
    // 모드 아이템이 아닌 경우, 카메라 또는 레이아웃 클릭을 처리합니다.
    if (item->parent() == root) {
        // 클릭된 아이템이 레이아웃인 경우
        int layIdx = root->indexOfChild(item);
        if (viewTabBar->currentIndex() != layIdx) {
            viewTabBar->setCurrentIndex(layIdx);
        }
    } else if (item->parent()->parent() == root) {
        // 클릭된 아이템이 카메라인 경우
        int layIdx = root->indexOfChild(item->parent());
        if (viewTabBar->currentIndex() != layIdx) {
            viewTabBar->setCurrentIndex(layIdx);
        }
        int slotIdx = item->data(0, Qt::UserRole).toInt();
        onCameraSlotClicked(slotIdx);

        // 안전 검사 추가
        if (layIdx >= layoutSlotData.size()) return;
        const auto& slots_ = layoutSlotData.at(layIdx);
        if (slotIdx >= slots_.size()) return;
        const QVariantMap& camData = slots_.at(slotIdx);
        QString ip = camData.value("ip").toString();
        QString camName = camData.value("name").toString();

        if (ip.isEmpty() || !m_webSocketMap.contains(ip)) {
            qWarning() << "[Health Check] WebSocket for" << camName << "(" << ip << ") not found.";
            return;
        }

        // 헬스체크 시작
        startHealthCheck(ip);

    }
}
// 2. onCameraModeChanged 함수가 올바르게 작성되었는지 다시 확인합니다.
void MainWindow::onCameraModeChanged(int slotIndex, const QString &mode)
{
    int lay = viewTabBar->currentIndex();
    if (lay < 0) return;
    if (lay >= layoutSlotData.size()) return;
    // 1. 데이터 모델의 모드 값을 변경
    const auto& slots_ = layoutSlotData[lay];
    if (slotIndex >= slots_.size()) return;
    layoutSlotData[lay][slotIndex]["mode"] = mode.toLower();

    // 2. 웹소켓으로 모드 변경 메시지를 전송
    QVariantMap camData = layoutSlotData[lay][slotIndex];
    QString ip = camData.value("ip").toString();
    if (!ip.isEmpty() && m_webSocketMap.contains(ip)) {
        QWebSocket *socket = m_webSocketMap[ip];

        if (socket->state() == QAbstractSocket::ConnectedState) {
            QJsonObject payload;
            payload["type"] = "set_mode";
            payload["mode"] = mode.toLower();

            QJsonDocument doc(payload);
            QString message = doc.toJson(QJsonDocument::Compact);
            socket->sendTextMessage(message);
            // Reference 파일처럼 전송하는 JSON 객체를 출력하도록 수정
            qDebug() << "[WebSocket] Sent mode change to" << ip << ":" << message;
        } else {
            qWarning() << "[WebSocket] Could not send mode change, socket not connected for IP:" << ip;
        }
    } else {
        qWarning() << "[WebSocket] No socket found for IP:" << ip;
    }
    // 3. 변경된 모드를 트리에 반영하기 위해 UI 새로고침
    updateLayoutTree();
}

void MainWindow::onLayoutTreeContextMenu(const QPoint &point)
{
    QTreeWidgetItem *item = layoutTree->itemAt(point);
    if (!item) return;

    // 클릭된 아이템이 '카메라'인지 확인 (부모가 있고, 부모의 부모가 최상위 루트 아이템인지)
    QTreeWidgetItem *parent = item->parent();
    if (!parent || !parent->parent() || parent->parent() != layoutTree->topLevelItem(0)) {
        return; // 카메라 아이템이 아님
    }

    // 컨텍스트 메뉴 생성
    QMenu contextMenu(this);
    QAction *editAction = contextMenu.addAction("Camera Edit");
    QAction *deleteAction = contextMenu.addAction("Camera Delete");

    QAction *selectedAction = contextMenu.exec(layoutTree->mapToGlobal(point));

    if (selectedAction == editAction) {
        // 현재 활성화된 뷰(탭) 찾기
        int layIdx = viewTabBar->currentIndex();
        if (layIdx < 0) return;

        QWidget *currentPage = viewStack->widget(layIdx);
        if (!currentPage) return;

        // 아이템에 저장된 슬롯 인덱스 가져오기
        int slotIdx = item->data(0, Qt::UserRole).toInt();

        // 해당 슬롯 인덱스를 가진 CameraSlot 위젯 찾기
        for (auto *slot : currentPage->findChildren<CameraSlot*>()) {
            if (slot && slot->slotIndex() == slotIdx) {
                // 찾은 슬롯의 onEditCamera() 함수 호출
                slot->onEditCamera();
                break;
            }
        }
    } else if (selectedAction == deleteAction) { // deleteAction 처리 로직 추가
        // 아이템에 저장된 슬롯 인덱스 가져오기
        int slotIdx = item->data(0, Qt::UserRole).toInt();

        // 카메라 삭제 함수 호출
        onCameraDeleteRequested(slotIdx);
    }

}

void MainWindow::onCameraSlotClicked(int slotIndex)
{
    // 활성 슬롯 인덱스 저장
    m_activeSlotIndex = slotIndex;

    int lay = viewTabBar->currentIndex();
    if (lay<0) return;
    auto *page = viewStack->widget(lay);
    auto slots_ = page->findChildren<CameraSlot*>();
    for (auto *s : slots_) s->setActive(false);
    for (auto *s : slots_) {
        if (s->slotIndex()==slotIndex) {
            s->setActive(true);
            break;
        }
    }
    // 트리 강조
    auto *root = layoutTree->topLevelItem(0);
    if (!root) return;
    auto *layItem = root->child(lay);
    if (!layItem) return;
    for (int i = 0; i < layItem->childCount(); ++i) {
        auto *camItem = layItem->child(i);
        if (camItem->data(0, Qt::UserRole).toInt() == slotIndex) {
            layoutTree->setCurrentItem(camItem);
            layoutTree->scrollToItem(camItem);
            break;
        }
    }
}


void MainWindow::onSlotDropped(int srcIndex,int dstIndex)
{
    int lay = viewTabBar->currentIndex();
    if (lay < 0) return;

    // 배열 범위 검사 추가
    if (lay >= layoutSlotData.size()) {
        qDebug() << "Layout index out of range:" << lay;
        return;
    }

    auto &slots_ = layoutSlotData[lay];

    if (srcIndex >= slots_.size() || dstIndex >= slots_.size()) {
        qDebug() << "Slot index out of range: srcIndex=" << srcIndex << "dstIndex=" << dstIndex << "for layout:" << lay;
        return;
    }

    // ▼▼▼ 핵심 수정 부분 ▼▼▼
    // 기존의 swapItemsAt 대신, 모든 경우의 수를 명시적으로 처리하여 안정성을 높입니다.

    QVariantMap srcData = slots_[srcIndex];
    QVariantMap dstData = slots_[dstIndex];

    // 데이터를 직접 교환합니다. 이 방식은 빈 슬롯과의 이동, 데이터 슬롯 간의 교환
    // 모든 경우를 가장 확실하게 처리합니다.
    slots_[srcIndex] = dstData;
    slots_[dstIndex] = srcData;

    // --- 데이터 처리 로직 종료 ---

    // 트리 뷰 UI 업데이트
    updateLayoutTree();

    // 화면 갱신 (위치가 변경된 두 슬롯만 새로고침하는 효율적인 방식은 그대로 유지)
    auto *page = viewStack->widget(lay);
    if (!page) return;

    CameraSlot *srcWidget = nullptr;
    CameraSlot *dstWidget = nullptr;

    auto allSlots = page->findChildren<CameraSlot*>();
    for (auto *slot_ : allSlots) {
        if (slot_->slotIndex() == srcIndex) {
            srcWidget = slot_;
        }
        if (slot_->slotIndex() == dstIndex) {
            dstWidget = slot_;
        }
    }

    // 찾은 위젯에 대해서만 setData를 호출하여 화면을 갱신합니다.
    if (srcWidget) {
        srcWidget->setData(slots_[srcIndex]);
    }
    if (dstWidget) {
        dstWidget->setData(slots_[dstIndex]);
    }
}
void MainWindow::onCameraRegistered(int slotIndex, const QString &deviceName, const QString &ip, const QString &port)
{
    int lay = viewTabBar->currentIndex();
    if (lay < 0 || lay >= layoutSlotData.size() || slotIndex >= layoutSlotData[lay].size()) {
        return; // Safety check
    }

    // 1. Construct the camera data map
    QVariantMap cameraData;
    cameraData["type"] = "camera";
    cameraData["name"] = deviceName;
    cameraData["ip"] = ip;
    cameraData["port"] = port;
    cameraData["mode"] = "raw";

    if (port == "554") cameraData["url"] = QString("rtsp://%1:%2/0/onvif/profile2/media.smp").arg(ip, port);
    else cameraData["url"] = QString("rtsps://%1:%2/processed").arg(ip, port);

    // 2. Start the headless validation process
    startHeadlessValidation(slotIndex, cameraData);
}


void MainWindow::startHeadlessValidation(int slotIndex, const QVariantMap& cameraData)
{
    QString ip = cameraData.value("ip").toString();
    if (ip.isEmpty() || m_validationPlayers.contains(ip)) {
        return; // Invalid data or validation already in progress
    }

    // --- Update UI to show "Validating" state ---
    // int lay = viewTabBar->currentIndex();
    // QWidget* page = viewStack->widget(lay);
    // if(page) {
    //     for(auto* slot : page->findChildren<CameraSlot*>()) {
    //         if(slot->slotIndex() == slotIndex) {
    //             // Temporarily use setData to show a message without starting a stream
    //             QVariantMap msgData;
    //             msgData["type"] = "message";
    //             msgData["text"] = tr("Validating connection...");
    //             slot->setData(msgData);
    //             break;
    //         }
    //     }
    // }

    // --- Create and configure the invisible player ---
    QMediaPlayer* validator = new QMediaPlayer(this);
    // DO NOT set a video output to keep it headless.

    // Store the player and the pending data
    m_validationPlayers[ip] = validator;
    m_pendingRegistrations[ip] = qMakePair(slotIndex, cameraData);

    // --- Connect to signals using lambdas ---
    connect(validator, &QMediaPlayer::playbackStateChanged, this, [this, ip](QMediaPlayer::PlaybackState state){
        if (state == QMediaPlayer::PlayingState) {
            onHeadlessValidationSuccess(ip);
        }
    });

    connect(validator, &QMediaPlayer::errorOccurred, this, [this, ip, validator](){
        onHeadlessValidationFailure(ip, validator->errorString());
    });

    // --- Start the connection attempt ---
    qDebug() << "[Validator] Starting headless validation for" << ip;
    validator->setSource(QUrl(cameraData.value("url").toString()));
    validator->play();
}

void MainWindow::onHeadlessValidationSuccess(const QString& ip)
{
    qDebug() << "[Validator] Headless validation SUCCEEDED for" << ip;

    // --- Clean up the validator ---
    if (m_validationPlayers.contains(ip)) {
        QMediaPlayer* validator = m_validationPlayers.take(ip);
        validator->stop();
        validator->deleteLater();
    }

    if (!m_pendingRegistrations.contains(ip)) return;
    
    QVariantMap data = m_pendingRegistrations[ip].second;
    QString port = data.value("port").toString();

    // If it's a simple RTSP stream, the process is done. Commit it.
    if (port == "554") {
        int lay = viewTabBar->currentIndex();
        int slotIndex = m_pendingRegistrations[ip].first;
        layoutSlotData[lay][slotIndex] = data; // FINAL COMMIT

        // Update the actual slot
        for(auto* slot : viewStack->widget(lay)->findChildren<CameraSlot*>()) {
            if(slot->slotIndex() == slotIndex) slot->setData(data);
        }

        updateLayoutTree();
        onCameraSlotClicked(slotIndex);
        m_pendingRegistrations.remove(ip); // Done with this registration
    } else {
        // For RTMPS, we now proceed to WebSocket validation.
        // The data remains in m_pendingRegistrations until the WebSocket connects.
        setupWebSocket(ip, port);
    }
}



void MainWindow::onHeadlessValidationFailure(const QString& ip, const QString& error)
{
    qDebug() << "[Validator] Headless validation FAILED for" << ip << ":" << error;

    // --- Clean up the validator ---
    if (m_validationPlayers.contains(ip)) {
        QMediaPlayer* validator = m_validationPlayers.take(ip);
        validator->stop();
        validator->deleteLater();
    }

    if (!m_pendingRegistrations.contains(ip)) return;

    int slotIndex = m_pendingRegistrations.take(ip).first;
    int lay = viewTabBar->currentIndex();

    // Reset the UI of the failed slot
    QWidget* page = viewStack->widget(lay);
    if(page) {
        for(auto* slot : page->findChildren<CameraSlot*>()) {
            if(slot->slotIndex() == slotIndex) {
                slot->setData({}); // Reset to empty
                break;
            }
        }
    }

    CustomMessageBox msg(tr("Camera connection failed:\n%1").arg(error), this);
    msg.exec();
}



void MainWindow::onCameraDeleteRequested(int slotIndex)
{
    int lay = viewTabBar->currentIndex();
    if (lay < 0) return;

    // 배열 범위 검사 추가
    if (lay >= layoutSlotData.size()) {
        qDebug() << "Layout index out of range:" << lay;
        return;
    }

    if (slotIndex >= layoutSlotData[lay].size()) {
        qDebug() << "Slot index out of range:" << slotIndex << "for layout:" << lay;
        return;
    }

    // Get the data before clearing it to find the WebSocket
    QVariantMap oldData = layoutSlotData[lay][slotIndex];

    // Clear the data model
    layoutSlotData[lay][slotIndex].clear();

    // Clean up WebSocket connection if it exists
    if (oldData.value("type").toString() == "camera") {
        QString ip = oldData.value("ip").toString();
        if (m_webSocketMap.contains(ip)) {
            qDebug() << "Deleting camera, closing WebSocket for IP:" << ip;
            QWebSocket *socket = m_webSocketMap.take(ip);
            socket->close();
            socket->deleteLater();
        }
    }

    // Update the UI
    auto* page = viewStack->widget(lay);
    if (page) {
        for (auto* slot : page->findChildren<CameraSlot*>()) {
            if (slot->slotIndex() == slotIndex) {
                slot->setData({}); // Set to empty data
                break;
            }
        }
    }

    updateLayoutTree();
}


void MainWindow::updateLayoutTree()
{
    layoutTree->clear();
    auto *root = new QTreeWidgetItem(QStringList{tr("Layouts")});
    layoutTree->addTopLevelItem(root);

    for (int i = 0; i < viewTabBar->count(); ++i) {
        auto *layItem = new QTreeWidgetItem(QStringList{viewTabBar->tabText(i)});
        root->addChild(layItem);

        // 안전 검사 추가
        if (i >= layoutSlotData.size()) continue;
        const auto &slots_ = layoutSlotData.at(i);

        for (int j = 0; j < slots_.size(); ++j) {
            const auto &data = slots_[j];
            if (data.isEmpty()) continue;

            QString type = data.value("type").toString();
            QString displayName;
            QTreeWidgetItem *item = nullptr;

            if (type == "camera") {
                displayName = data.value("name").toString();

                // 연결 상태 확인
                QString ip = data.value("ip").toString();
                QString statusText = "";
                QString statusColor = "gray";

                if (m_cameraConnectionStatus.contains(ip)) {
                    QString status = m_cameraConnectionStatus[ip];
                    if (status == "연결됨") {
                        statusText = " ●";
                        statusColor = "green";
                    } else if (status == "연결 중") {
                        statusText = " ●";
                        statusColor = "orange";
                    } else if (status == "연결 실패" || status == "연결 해제") {
                        statusText = " ●";
                        statusColor = "red";
                    }
                } else {
                    // 헬스체크를 하지 않은 상태는 회색으로 표시
                    statusText = " ●";
                    statusColor = "gray";
                }
                // 카메라 이름에 상태 표시 추가
                QString displayNameWithStatus = displayName + statusText;
                item = new QTreeWidgetItem(layItem, QStringList{displayNameWithStatus});
                // 상태 색상 설정
                item->setForeground(0, QColor(statusColor));

                // 포트 번호가 "554"가 아닐 때만 모드 목록을 추가합니다.
                if (data.value("port").toString() != "554") {
                    const QStringList modes = {"Raw", "Blur", "Detect", "Trespass", "Fall"};
                    QString currentMode = data.value("mode", "raw").toString();

                    for (const QString &mode : modes) {
                        auto *modeItem = new QTreeWidgetItem(item, QStringList{mode});
                        // 클릭 시 구분을 위해 모드 아이템에 데이터 저장
                        modeItem->setData(0, Qt::UserRole + 1, mode.toLower());

                        // 현재 활성화된 모드는 굵게 표시
                        if (mode.toLower() == currentMode) {
                            QFont font = modeItem->font(0);
                            font.setBold(true);
                            modeItem->setFont(0, font);
                        }
                    }
                }
            }

            if (item) {
                // Store slot index on the main item (camera)
                item->setData(0, Qt::UserRole, j);
            }
        }
    }
    root->setExpanded(true);
    layoutTree->expandAll();
}

QWidget* MainWindow::createCameraGridWidget()
{
    // 1) Grid content
    QWidget *content = new QWidget;
    auto *g = new QGridLayout(content);
    g->setContentsMargins(0,0,0,0);
    g->setSpacing(2);

    auto makeSlot = [&](int w,int h,int idx){
        CameraSlot *s = new CameraSlot(w,h,idx,content);

        // 배열 범위 검사 추가
        if (!layoutSlotData.isEmpty() && idx < layoutSlotData.last().size()) {
            s->setData(layoutSlotData.last().at(idx));
        } else {
            s->setData(QVariantMap()); // 빈 데이터로 설정
        }

        s->setAcceptDrops(true);
        return s;
    };

    g->addWidget(makeSlot(320,240,0),0,0);
    g->addWidget(makeSlot(320,240,1),0,1);
    g->addWidget(makeSlot(320,240,2),0,2);
    g->addWidget(makeSlot(320,240,3),1,0);
    g->addWidget(makeSlot(320,240,4),2,0);
    g->addWidget(makeSlot(640,480,5),1,1,2,2);

    g->setColumnStretch(0, 1);
    g->setColumnStretch(1, 1);
    g->setColumnStretch(2, 1);

    // 각 행(Row)이 동일한 비율로 늘어나도록 스트레치(stretch)를 설정합니다.
    g->setRowStretch(0, 1);
    g->setRowStretch(1, 1);
    g->setRowStretch(2, 1);


    content->setLayout(g);

    // 2) ScrollArea
    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scroll->setWidget(content);
    int sbw = scroll->verticalScrollBar()->sizeHint().width();
    scroll->setMinimumWidth(320*3 + sbw);
    scroll->setMinimumHeight(240*3 + sbw);
    return scroll;
}

// ─── CameraSlot 구현 ────────────────────────────────────────────
CameraSlot::CameraSlot(int w, int h, int slotIndex, QWidget *parent)
    : AspectRatioWidget(parent),
    m_slotIndex(slotIndex),
    frame(new QFrame(this))
{
    setMinimumSize(w,h);
    setAcceptDrops(true);

    auto *vl = new QVBoxLayout(this);
    vl->setContentsMargins(0,0,0,0);
    vl->setSpacing(0);
    vl->addWidget(frame);

    frame->setFrameShape(QFrame::Box);
    frame->setLineWidth(1);

    auto *stackLayout = new QStackedLayout(frame);
    stackLayout->setContentsMargins(0,0,0,0);
    stackLayout->setStackingMode(QStackedLayout::StackAll);

    // 1. 이미지 표시용
    imageLabel = new QLabel(frame);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);

    // 2. 비디오 표시용
    videoWidget = new QVideoWidget(frame);
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 3. 정보 표시용
    infoLabel = new QLabel(frame);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("background-color: rgba(0, 0, 0, 150); color: white; padding: 5px;");
    infoLabel->setWordWrap(true);
    infoLabel->hide();
    // 4. 미디어 플레이어
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setVideoOutput(videoWidget); 

    stackLayout->addWidget(videoWidget);
    stackLayout->addWidget(imageLabel);
    stackLayout->addWidget(infoLabel);

    // Connections
    connect(mediaPlayer, &QMediaPlayer::errorOccurred, this, &CameraSlot::handleMediaError);
    connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &CameraSlot::onPlaybackStateChanged);

}


CameraSlot::~CameraSlot()
{
    if (mediaPlayer && mediaPlayer->playbackState() != QMediaPlayer::StoppedState) {
        mediaPlayer->stop();
    }

    qDebug() << "CameraSlot destructor completed successfully for slot" << m_slotIndex;
}


void CameraSlot::stopStream()
{
    if (mediaPlayer && mediaPlayer->playbackState() != QMediaPlayer::StoppedState) {
        mediaPlayer->stop();
    }
    // 비디오 위젯을 숨기고 기본 이미지가 보이도록 확실히 처리합니다.
    videoWidget->hide();
    imageLabel->show();
}
// mainwindow.cpp

void CameraSlot::setData(const QVariantMap &data)
{
    m_currentData = data;

    // 기존 스트림 정지
    if (mediaPlayer && mediaPlayer->playbackState() != QMediaPlayer::StoppedState) {
        mediaPlayer->stop();
    }

    imageLabel->clear();
    infoLabel->hide();
    videoWidget->hide();
    imageLabel->show();

    if (data.isEmpty() || !data.contains("type")) {
        QPixmap raw(":/test/no_camera.png");
        // imageLabel->setPixmap(
        //     raw.scaled(
        //         imageLabel->size(),
        //         Qt::KeepAspectRatio,
        //         Qt::SmoothTransformation
        //     )
        // );
        imageLabel->setPixmap(raw);
        return;
    }
    
    if (data.value("type").toString() == "message") {
        infoLabel->setText(data.value("text").toString());
        infoLabel->show();
        videoWidget->hide();
        imageLabel->show();
        imageLabel->setPixmap(QPixmap(":/test/no_camera.png"));
        return;
    }

    if (data.value("type").toString() == "camera") {
        qDebug() << "[CameraSlot] 카메라 설정 시작 - Slot:" << m_slotIndex << "URL:" << data.value("url").toString();

        // --- 수정 부분: 연결 중 메시지 표시 로직 제거 ---
        // 아래 두 줄을 주석 처리하거나 삭제합니다.
        // infoLabel->setText(tr("Connecting to stream..."));
        // infoLabel->show();
        // -----------------------------------------

        QString urlString = data.value("url").toString();

        if (urlString.isEmpty()) {
            qWarning() << "[CameraSlot] 카메라 URL이 비어있음 - Slot:" << m_slotIndex;
            infoLabel->setText(tr("카메라 URL이 없습니다."));
            infoLabel->show();
            return;
        }

        QUrl url(urlString);
        if (!url.isValid()) {
            qWarning() << "[CameraSlot] 잘못된 카메라 URL - Slot:" << m_slotIndex << "URL:" << urlString;
            infoLabel->setText(tr("잘못된 카메라 URL입니다."));
            infoLabel->show();
            return;
        }

        try {
            mediaPlayer->setVideoOutput(videoWidget);
            mediaPlayer->setSource(url);
            mediaPlayer->play();
            // 로그 메시지를 더 명확하게 변경
            qDebug() << "[CameraSlot] 재생 연결 시도 - Slot:" << m_slotIndex;

        } catch (...) {
            qWarning() << "[CameraSlot] MediaPlayer 오류 - Slot:" << m_slotIndex << "URL:" << urlString;
            infoLabel->setText(tr("미디어 플레이어 오류가 발생했습니다."));
            infoLabel->show();
        }
    }
}

void CameraSlot::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    qDebug() << "[CameraSlot] Playback state changed to:" << state << "for slot" << m_slotIndex;

    if (state == QMediaPlayer::PlayingState) {
        // --- Success Case ---
        // The stream is successfully playing. Hide placeholders and show the video.
        infoLabel->hide();
        imageLabel->hide();
        videoWidget->show();
    } else if (state == QMediaPlayer::StoppedState) {
        // --- Stopped/Failed Case ---
        // The stream has stopped. This could be due to an error or a normal stop.
        // Ensure the video widget is hidden. The UI will show whatever infoLabel
        // or imageLabel was set by handleMediaError() or setData().
        videoWidget->hide();
        imageLabel->show();
    }
}





void CameraSlot::handleMediaError()
{
    QString errorMsg = mediaPlayer ? mediaPlayer->errorString() : "Unknown media error";
    qWarning() << "[CameraSlot] 미디어 에러 발생 - Slot:" << m_slotIndex << "Error:" << errorMsg;

    // ... (update UI with error message as before) ...
    infoLabel->setText(tr("스트리밍 오류:\n%1").arg(errorMsg));
    infoLabel->show();
    videoWidget->hide();
    imageLabel->show();
    imageLabel->setPixmap(QPixmap(":/test/no_camera.png"));

    // Emit the failure signal with details
    emit validationFailed(m_slotIndex, errorMsg);
}

void CameraSlot::onRegisterCamera()
{
    CameraRegistrationDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.deviceName();
        QString ip = dialog.ip();
        QString port = dialog.port();
        if (name.isEmpty() || ip.isEmpty() || port.isEmpty()) return;
        emit cameraRegistered(m_slotIndex, name, ip, port);
    }
}

void CameraSlot::onEditCamera()
{
    if (m_currentData.value("type").toString() != "camera") return;

    CameraRegistrationDialog dialog(this);
    // Pre-fill the dialog with existing data
    dialog.setInitialData(
        m_currentData.value("name").toString(),
        m_currentData.value("ip").toString(),
        m_currentData.value("port").toString()
        );

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.deviceName();
        QString ip = dialog.ip();
        QString port = dialog.port();
        if (name.isEmpty() || ip.isEmpty() || port.isEmpty()) return;

        // Emit the same signal as registration; the main window will handle the update
        emit cameraRegistered(m_slotIndex, name, ip, port);
    }
}


void CameraSlot::onDeleteCamera()
{
    emit deleteRequested(m_slotIndex);
}

void CameraSlot::setActive(bool on)
{
    // Always use a stylesheet to prevent geometry changes.
    // The inactive state now has a defined 1px border, matching the active state's width.
    // A neutral color (#5c5c5c) is used, consistent with other UI elements.
    frame->setStyleSheet(on ? "border: 1px solid #f4731f;" : "border: 1px solid #5c5c5c;");
}
void CameraSlot::mousePressEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::LeftButton) {
        m_dragStartPos = evt->pos();
        emit clicked(m_slotIndex);
    }
    QWidget::mousePressEvent(evt);
}

void CameraSlot::mouseMoveEvent(QMouseEvent *evt)
{
    if (!(evt->buttons() & Qt::LeftButton)) return;
    if ((evt->pos() - m_dragStartPos).manhattanLength()
        < QApplication::startDragDistance()) return;

    auto *mime = new QMimeData;
    mime->setData("application/x-slot-index", QByteArray::number(m_slotIndex));

    auto *drag = new QDrag(this);
    drag->setMimeData(mime);

    // Capture the widget's current appearance to create an accurate drag preview.
    // This avoids issues with scaling already-scaled images.
    QPixmap pm = this->grab();
    drag->setPixmap(pm);
    drag->setHotSpot(evt->pos());

    drag->exec(Qt::MoveAction);
}

void CameraSlot::dragEnterEvent(QDragEnterEvent *evt)
{
    if (evt->mimeData()->hasFormat("application/x-slot-index"))
        evt->acceptProposedAction();
}

void CameraSlot::dropEvent(QDropEvent *evt)
{
    QByteArray ba = evt->mimeData()->data("application/x-slot-index");
    int src = ba.toInt(), dst = m_slotIndex;
    if (src == dst) { evt->ignore(); return; }

    emit dropOccurred(src, dst);
    evt->acceptProposedAction();
}

void CameraSlot::contextMenuEvent(QContextMenuEvent *evt)
{
    QMenu menu(this);
    bool isCamera = (m_currentData.value("type").toString() == "camera");


    QAction *registerAct = menu.addAction(tr("Camera Register"));
    QAction *editAct = menu.addAction(tr("Camera Edit"));
    QAction *deleteAct = menu.addAction(tr("Camera Delete"));

    // Enable/disable actions based on whether a camera is registered
    registerAct->setEnabled(!isCamera);
    editAct->setEnabled(isCamera);
    deleteAct->setEnabled(isCamera);

    QAction *selectedAction = menu.exec(evt->globalPos());

    if (selectedAction == registerAct) {
        onRegisterCamera();
    } else if (selectedAction == editAct) {
        onEditCamera();
    } else if (selectedAction == deleteAct) {
        onDeleteCamera();
    }
}


/**
 * @brief Unregisters a camera from a specific slot due to a connection failure.
 * @param layoutIndex The index of the tab/view containing the camera.
 * @param slotIndex The index of the slot within the layout.
 */
void MainWindow::unregisterCamera(int layoutIndex, int slotIndex)
{
    // --- Safety Checks ---
    if (layoutIndex < 0 || layoutIndex >= layoutSlotData.size() ||
        slotIndex < 0 || slotIndex >= layoutSlotData[layoutIndex].size()) {
        qWarning() << "unregisterCamera called with invalid indices:" << layoutIndex << slotIndex;
        return;
    }

    QVariantMap camData = layoutSlotData[layoutIndex][slotIndex];
    if (camData.isEmpty()) {
        return; // Already unregistered, do nothing.
    }

    // --- 1. Notify User ---
    QString camName = camData.value("name", "Unknown").toString();
    CustomMessageBox msg(tr("Failed to connect to camera: %1.\nPlease check IP/Port and network status.").arg(camName), this);
    msg.exec();

    // --- 2. Clean up WebSocket and other resources ---
    QString ip = camData.value("ip").toString();
    if (!ip.isEmpty()) {
        if (m_webSocketMap.contains(ip)) {
            qDebug() << "Unregistering camera, closing WebSocket for IP:" << ip;
            QWebSocket *socket = m_webSocketMap.take(ip);
            socket->close();
            socket->deleteLater();
        }
        // Clean up other associated data
        m_cameraConnectionStatus.remove(ip);
        m_healthCheckResponded.remove(ip);
        if(m_healthCheckTimers.contains(ip)) {
            QTimer* timer = m_healthCheckTimers.take(ip);
            timer->stop();
            timer->deleteLater();
        }
    }

    // --- 3. Clear data from the central data model ---
    layoutSlotData[layoutIndex][slotIndex].clear();

    // --- 4. Update the specific CameraSlot UI to show it's empty ---
    QWidget *page = viewStack->widget(layoutIndex);
    if (page) {
        for (auto* slot : page->findChildren<CameraSlot*>()) {
            if (slot->slotIndex() == slotIndex) {
                slot->setData({}); // Reset to empty state
                break;
            }
        }
    }

    // --- 5. Update the layout tree to remove the camera entry ---
    updateLayoutTree();
}

void MainWindow::onCameraValidationFailed(int slotIndex, const QString& error)
{
    int lay = viewTabBar->currentIndex();
    QWidget* page = viewStack->widget(lay);
    if (!page) return;

    // Find the slot and reset it to its initial empty state.
    for (auto* slot : page->findChildren<CameraSlot*>()) {
        if (slot->slotIndex() == slotIndex) {
            slot->setData({}); // Reset to empty
            break;
        }
    }
    CustomMessageBox msg(tr("Camera connection failed:\n%1").arg(error), this);
    msg.exec();
}



CameraRegistrationDialog::CameraRegistrationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(530, 400);

    // --- Main Background ---
    QFrame *background = new QFrame(this);
    background->setObjectName("background");
    background->setStyleSheet(R"(
        #background {
            background-color: #232323;
            border: 1px solid #909090;
        }
    )");

    // --- Title Bar ---
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


    // --- Content Area (Inputs) ---
    // A container widget is created to apply a debug border to the layout area
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


    // --- Button Area ---
    QPushButton *okButton = new QPushButton("확인", background);
    okButton->setObjectName("okButton"); // 3. 스타일시트에서 특정 버튼을 타겟팅하기 위해 객체 이름을 설정합니다
    okButton->setFixedSize(200, 40);     // 3. ExitConfirmDialog의 exitButton과 동일한 크기로 설정합니다
    // connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(okButton, &QPushButton::clicked, this, &CameraRegistrationDialog::onAcceptClicked); // 수정된 연결


    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();


    // --- Main Layout ---
    QVBoxLayout *mainLayout = new QVBoxLayout(background);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addStretch(1);
    mainLayout->addWidget(contentContainer); // Use the container widget
    mainLayout->addStretch(2);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(25);

    // --- Dialog Layout ---
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(background);

    // --- Styling ---
    // 3. okButton의 스타일을 ExitConfirmDialog의 exitButton과 동일하게 만듭니다.
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

    // 1. 디버그용 테두리를 적용합니다.
    // mainwindow.cpp 상단에 #define DEBUG_UI_BORDER 가 있어야 활성화됩니다.
#ifdef DEBUG_UI_BORDER
    applyDebugBorder(titleBar, "red");
    applyDebugBorder(contentContainer, "lime");
    applyDebugBorder(okButton, "cyan");
#endif
}
/**
 * @brief '확인' 버튼 클릭 시 호출되어, 입력값 유효성 검사를 먼저 수행하는 슬롯.
 */
void CameraRegistrationDialog::onAcceptClicked()
{
    // validateInput() 함수를 호출하여 모든 입력값이 유효한지 확인합니다.
    if (validateInput()) {
        // 유효하다면 다이얼로그를 닫고 Accepted 상태를 반환합니다.
        QDialog::accept();
    }
    // 유효하지 않다면 validateInput() 내부에서 오류 메시지 박스를 띄우고
    // 다이얼로그는 닫히지 않습니다.
}

/**
 * @brief 장치명, IP, 포트 입력값의 유효성을 검사하는 함수.
 * @return 모든 입력이 유효하면 true, 아니면 false를 반환합니다.
 */
bool CameraRegistrationDialog::validateInput()
{
    // 1. 장치명 유효성 검사 (20자 이내)
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

    // 2. IP 주소 유효성 검사 (IPv4 형식)
    QRegularExpression ipRegex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    if (!ipRegex.match(ipEdit->text()).hasMatch()) {
        CustomMessageBox msg("IP 주소 형식이 올바르지 않습니다.", this);
        msg.exec();
        return false;
    }

    // 3. 포트 유효성 검사 (숫자만, 1-65535 범위)
    bool isPortNumber;
    int portValue = portEdit->text().toInt(&isPortNumber);
    if (!isPortNumber || portValue < 1 || portValue > 65535) {
        CustomMessageBox msg("포트는 1에서 65535 사이의 숫자만 입력 가능합니다.", this);
        msg.exec();
        return false;
    }

    // 모든 검사를 통과한 경우
    return true;
}

// REPLACE the old setInitialData method with this one
void CameraRegistrationDialog::setInitialData(const QString &name, const QString &ip, const QString &port)
{
    nameEdit->setText(name);
    ipEdit->setText(ip);
    portEdit->setText(port);
}

// ADD these new methods for window dragging
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


void MainWindow::setupWebSocket(const QString& ip, const QString& port)
{
    if (m_webSocketMap.contains(ip)) {
        qDebug() << "Cleaning up stale WebSocket for IP:" << ip;
        QWebSocket *oldSocket = m_webSocketMap.take(ip); // 'take' removes the item and returns its value
        if (oldSocket) {
            oldSocket->close();
            oldSocket->deleteLater(); // Safely delete the old socket object
        }
    }

    // 연결 중 상태 표시
    updateCameraConnectionStatus(ip, "연결 중", "orange");
    QWebSocket* socket = new QWebSocket();

    // ✅ SSL 에러를 무시하도록 시그널에 직접 연결 (더 안정적인 방법)
    connect(socket, &QWebSocket::sslErrors, this, [socket](const QList<QSslError> &errors) {
        qWarning() << "SSL Errors occurred, ignoring:" << errors;
        socket->ignoreSslErrors();
    });

    connect(socket, &QWebSocket::connected, this, &MainWindow::onWebSocketConnected);
    connect(socket, &QWebSocket::disconnected, this, &MainWindow::onWebSocketDisconnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &MainWindow::onWebSocketMessageReceived);

    // connect(socket, &QWebSocket::errorOccurred, this, [this, ip](QAbstractSocket::SocketError error){
    //     qWarning() << "WebSocket Error:" << error;
    //     updateCameraConnectionStatus(ip, "연결 실패", "red");
    //     onWebSocketError(error);
    // });
        connect(socket, &QWebSocket::errorOccurred, this, [this, ip](QAbstractSocket::SocketError error){
        qWarning() << "WebSocket Error:" << error << "for IP:" << ip;

        // The connection failed. Find the camera and unregister it completely.
        for (int i = 0; i < layoutSlotData.size(); ++i) {
            for (int j = 0; j < layoutSlotData[i].size(); ++j) {
                const QVariantMap& camData = layoutSlotData[i].at(j);
                if (camData.value("type").toString() == "camera" && camData.value("ip").toString() == ip) {
                    // Found it. Call the central cleanup function.
                    // We must do this from the error handler to catch initial connection failures.
                    unregisterCamera(i, j);
                    return; // Exit lambda; camera has been handled.
                }
            }
        }
    });


    QString wsUrl = QString("wss://%1:%2/ws").arg(ip, port == "8555" ? "8443" : port); // 포트가 8555이면 8443으로 변경 (서버 설정에 따라 조정)

    qDebug() << "Attempting to open WebSocket to:" << wsUrl;
    m_webSocketMap.insert(ip, socket);
    socket->open(QUrl(wsUrl));
}

void MainWindow::onWebSocketConnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (!socket) return;

    QString ip = m_webSocketMap.key(socket, "");
    if (ip.isEmpty()) return;

    qDebug() << "WebSocket connected:" << ip;
    updateCameraConnectionStatus(ip, "연결됨", "green");

    // Check if this connection corresponds to a pending registration.
    if (m_pendingRegistrations.contains(ip)) {
        // --- FINAL COMMIT ---
        // Both media stream and WebSocket are now connected.
        // We can finally register the camera.
        int lay = viewTabBar->currentIndex();
        int slotIndex = m_pendingRegistrations[ip].first;
        QVariantMap data = m_pendingRegistrations[ip].second;
        
        if (lay < layoutSlotData.size() && slotIndex < layoutSlotData[lay].size()) {
             layoutSlotData[lay][slotIndex] = data; // Save to data model
            QWidget* page = viewStack->widget(lay);
            if (page) {
                for(auto* slot : page->findChildren<CameraSlot*>()) {
                    if(slot && slot->slotIndex() == slotIndex) {
                        slot->setData(data); // This call starts the video player
                        break;
                    }
                }
            }

             updateLayoutTree();
             onCameraSlotClicked(slotIndex); // Set as active
             loadHistoricalEvents(data.value("name").toString(), ip, data.value("port").toString());
        }
        m_pendingRegistrations.remove(ip); // Clean up pending entry
    }
}

void MainWindow::onWebSocketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (!socket) return;
    qDebug() << "WebSocket disconnected:" << socket->peerAddress().toString();

    // Find the IP associated with this socket and remove it from the map
    QString ip = m_webSocketMap.key(socket, "");
    if (!ip.isEmpty()) {
        updateCameraConnectionStatus(ip, "연결 해제", "red");
        m_webSocketMap.remove(ip);
    }
    socket->deleteLater();
}

void MainWindow::onWebSocketError(QAbstractSocket::SocketError error)
{
    // You can add UI feedback here, e.g., show an error icon on the camera slot
    qWarning() << "A WebSocket error occurred:" << error;
}
/**
 * @brief 웹소켓 메시지 수신 시 호출되는 함수 (수정됨)
 */

void MainWindow::onWebSocketMessageReceived(const QString &message)
{
    qDebug() << "WebSocket message received:" << message;
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        qWarning() << "Received WebSocket message is not a JSON object.";
        return;
    }
    QJsonObject obj = doc.object();
    QString type = obj["type"].toString();
    QJsonObject data = obj.value("data").toObject();

    QString senderIp = m_webSocketMap.key(pSender, "");
    if (senderIp.isEmpty()) {
        qWarning() << "Cannot determine sender IP for received message.";
        return;
    }

    // 헬스체크 응답 처리 - 어떤 메시지든 수신되면 해당 IP의 헬스체크 응답으로 간주
    m_healthCheckResponded[senderIp] = true;

    // 헬스체크 타이머가 실행 중이면 응답으로 처리
    if (m_healthCheckTimers.contains(senderIp)) {
        updateCameraConnectionStatus(senderIp, "연결됨", "green");

        // 타이머 정리
        QTimer *timer = m_healthCheckTimers[senderIp];
        timer->stop();
        timer->deleteLater();
        m_healthCheckTimers.remove(senderIp);
    }

    QString camName;
    for (const auto& layout : std::as_const(layoutSlotData)) {
        for (const auto& slotData : layout) {
            if (slotData.value("type").toString() == "camera" && slotData.value("ip").toString() == senderIp) {
                camName = slotData.value("name").toString();
                break;
            }
        }
        if (!camName.isEmpty()) break;
    }
    if (camName.isEmpty()) {
        camName = senderIp;
    }

    QString eventText;
    QString timestampStr = data.value("timestamp").toString();
    QDateTime timestamp = QDateTime::fromString(timestampStr, "yyyy-MM-dd hh:mm:ss");
    if (!timestamp.isValid()) {
        timestamp = QDateTime::currentDateTime();
    }

    if (type == "new_detection" || type == "new_trespass" || type == "new_fall" || type == "anomaly_status") {
        QString imagePath = data["image_path"].toString();
        qDebug() << "[Event:" << type << "] Received image_path from server:" << imagePath;

        // 이미지 전체 URL 생성
        QString imageUrl;
        if (!imagePath.isEmpty()) {
            QString cleanPath = imagePath.startsWith("../") ? imagePath.mid(3) : imagePath;
            imageUrl = QString("http://%1/%2").arg(senderIp, cleanPath);
        }

        // [수정] 디버깅을 위한 qDebug() 출력 추가
        qDebug() << "[Event:" << type << "] Constructed imageUrl:" << imageUrl;

        if (type == "new_detection") {
            int person = data["person_count"].toInt();
            int helmet = data["helmet_count"].toInt();
            int vest = data["safety_vest_count"].toInt();
            if (person > 0 && (helmet < person || vest < person)) {
                eventText = "PPE Violation";
            }
        } else if (type == "new_trespass") {
            int count = data["count"].toInt();
            if (count > 0) {
                eventText = QString("Trespass Detected (%1)").arg(count);
            }
        } else if (type == "new_fall") {
            int count = data["count"].toInt();
            if (count > 0) {
                eventText = "Fall Detected";
            }
        } else if (type == "anomaly_status") {
            QString status = data["status"].toString();
            if (status == "detected") {
                eventText = "Abnormal Sound Detected";
            }
        }

        if (!eventText.isEmpty()) {
            addEvent(camName, eventText, timestamp, imageUrl);
        }
    } else if (type == "stm_status_update") {
        double temp = data["temperature"].toDouble();
        int light = data["light"].toInt();
        bool buzzer = data["buzzer_on"].toBool();
        bool led = data["led_on"].toBool();
        qDebug() << "[Event: stm_status_update] Temp:" << temp << "Light:" << light << "JSON:" << message;
    } else if (type == "new_blur") {
        int count = data["count"].toInt();
        qDebug() << "[Event: new_blur] Count:" << count << "JSON:" << message;
    } else if (type == "mode_change_ack"){
        QString status = obj["status"].toString();
        QString mode = obj["mode"].toString();
        qDebug() << "[Event: mode_change_ack] Status:" << status << "Mode:" << mode << "JSON:" << message;
        if (status == "error") {
            QString errorMsg = obj["message"].toString();
            CustomMessageBox msg(QString("Mode change failed: %1").arg(errorMsg), this);
            msg.exec();
        }
    } else if (type == "log"){
        QString event = data["event"].toString();
        qDebug() << "[Event: log] Event:" << event << "JSON:" << message;
    }
    else {
        qWarning() << "Received unknown WebSocket message type:" << type;
    }
}

// Replace the entire EventCard constructor with this
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
    mainLay->setContentsMargins(8,8,8,8);
    mainLay->setSpacing(10);

    // --- Image Area (4:3 Ratio, Centered with Overlay Button) ---
    {
        auto *imageArea = new QWidget(this);
        auto *imageLayout = new QGridLayout(imageArea);
        imageLayout->setContentsMargins(0,0,0,0);

        m_imageLabel = new QLabel(this); // 멤버 변수 사용
        m_imageLabel->setAlignment(Qt::AlignCenter);

        constexpr int imageDisplayWidth = 70;
        constexpr int imageDisplayHeight = imageDisplayWidth * 3 / 4;
        m_imageLabel->setFixedSize(imageDisplayWidth, imageDisplayHeight);
        m_imageLabel->setStyleSheet("background-color: #000; color: #fff;"); // 로딩 중 배경
        m_imageLabel->setText(tr("Loading..."));

        // 이미지 URL이 있으면 비동기로 다운로드
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
            // 이미지 URL이 없으면 기본 이미지 표시
            m_imageLabel->setText(tr("No Image"));
        }

        // Action Button (기존 로직과 동일)
        m_actionButton = new QToolButton(this);
        m_actionButton->setIconSize(QSize(16,16));
        m_actionButton->setAutoRaise(true);
        m_actionButton->setStyleSheet(R"(
            QToolButton { background-color: transparent; border: none;}
            QToolButton:checked { background-color: transparent; }
        )");

        if (m_mode == Event) {
            m_actionButton->setCheckable(true);
            m_actionButton->setIcon(QIcon(":/icons/icons/star_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"));
            connect(m_actionButton, &QToolButton::toggled, this, [this](bool checked){
                m_actionButton->setIcon(QIcon(
                    checked
                        ? ":/icons/icons/star_24dp_F4731F_FILL0_wght400_GRAD0_opsz24.svg"
                        : ":/icons/icons/star_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"
                    ));
                emit starToggled(this, checked);
            });
        } else { // Bookmark mode
            m_actionButton->setText("✕");
            m_actionButton->setToolTip(tr("Remove bookmark"));
            connect(m_actionButton, &QToolButton::clicked, this, [this](){
                emit removeRequested(this);
            });
        }

        imageLayout->addWidget(m_imageLabel, 0, 0, Qt::AlignCenter);
        imageLayout->addWidget(m_actionButton, 0, 0, Qt::AlignTop | Qt::AlignRight);

        mainLay->addWidget(imageArea);
    }

    {
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
    }
}

// Update the setStarChecked helper function to use m_actionButton
void EventCard::setStarChecked(bool checked)
{
    bool b = m_actionButton->blockSignals(true);
    m_actionButton->setChecked(checked);
    m_actionButton->setIcon(QIcon(
        checked
            ? ":/icons/icons/star_24dp_F4731F_FILL0_wght400_GRAD0_opsz24.svg"
            : ":/icons/icons/star_24dp_B7B7B7_FILL0_wght400_GRAD0_opsz24.svg"
        ));
    m_actionButton->blockSignals(b);
}

void MainWindow::onBookmarkRemoveRequested(EventCard* card)
{
    // Find the original EventCard in the event list
    for (EventCard* ec : this->findChildren<EventCard*>()) {
        // Make sure we are only checking original Event cards, not other bookmarks
        if (ec->mode() == EventCard::Event &&
            ec->cameraName() == card->cameraName() &&
            ec->timestamp() == card->timestamp())
        {
            // Un-check the star on the original event card.
            // This only updates the UI, it won't re-emit a signal.
            ec->setStarChecked(false);
            break; // Found it, no need to search further
        }
    }

    // ▼▼▼ ADD THIS MISSING LOGIC ▼▼▼
    // Now, explicitly remove the bookmark card that was clicked.
    bookmarkLay->removeWidget(card);
    card->deleteLater();
}

// 4. Update onCardStarToggled to use the new system
void MainWindow::onCardStarToggled(EventCard* card, bool checked)
{
    if (!bookmarkLay) return;

    if (checked) {
        // Create a NEW EventCard, but in Bookmark mode
        auto *bk = new EventCard(
            card->cameraName(),
            card->eventText(),
            card->timestamp(),
            card->imageUrl(),
            EventCard::Bookmark,
            this
            );
        // Connect its remove signal to our new slot
        connect(bk, &EventCard::removeRequested, this, &MainWindow::onBookmarkRemoveRequested);

        bookmarkLay->insertWidget(bookmarkLay->count()-1, bk);
    } else {
        // This logic runs when the user un-stars an event from the EVENT list.
        // It needs to find and remove the corresponding card from the BOOKMARK list.
        for (int i = 0; i < bookmarkLay->count(); ++i) {
            // Use qobject_cast for safer type conversion
            if (auto *bf = qobject_cast<EventCard*>(bookmarkLay->itemAt(i)->widget())) {
                if (bf->mode() == EventCard::Bookmark &&
                    bf->cameraName() == card->cameraName() &&
                    bf->timestamp() == card->timestamp())
                {
                    bookmarkLay->removeWidget(bf);
                    bf->deleteLater();
                    break;
                }
            }
        }
    }
}
// ─── CustomTabBar paintEvent ─────────────────────────────────────────
void CustomTabBar::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < count(); ++i) {
        QStyleOptionTab opt;
        initStyleOption(&opt, i);
        QRect r = tabRect(i);
        bool sel = (currentIndex() == i);

        // 1. 상태별 색상 결정
        QColor textC = sel ? Qt::white : QColor(200, 200, 200);

        // 2. 탭 텍스트 그리기
        p.setPen(textC);
        QRect textRect = r.adjusted(m_padding, 0, -m_padding - 16, 0);
        p.drawText(textRect, Qt::AlignCenter, opt.text);

        // 3. 닫기 '✕' 아이콘 그리기
        QFont closeFont = font();
        closeFont.setPointSize(10);
        p.setFont(closeFont);
        p.drawText(closeIconRect(i), Qt::AlignCenter, QStringLiteral("✕"));

        // 4. 선택된 탭에만 밑줄 그리기
        if (sel) {
            QColor underlineColor = QColor("#f4731f");
            int underlineHeight = 4;
            // y 좌표를 수정하여 경계선에 가려지지 않도록 함
            QRect underlineRect(r.left(), r.bottom() - underlineHeight, r.width(), underlineHeight);
            p.fillRect(underlineRect, underlineColor);
        }
    }
}

void CustomTabBar::mousePressEvent(QMouseEvent *evt) {
    // 드래그 시작 위치 저장
    if (evt->button() == Qt::LeftButton) {
        m_dragStartPos = evt->globalPosition().toPoint();
    }

    // 기존 탭 닫기 로직은 그대로 둡니다.
    int idx = tabAt(evt->pos());
    if (idx >= 0) {
        QRect icr = closeIconRect(idx);
        if (icr.contains(evt->pos())) {
            emit tabCloseRequested(idx);
            return;
        }
    }

    // QTabBar의 기본 클릭 이벤트를 호출하여 탭 선택이 되도록 합니다.
    QTabBar::mousePressEvent(evt);
}

void CustomTabBar::mouseMoveEvent(QMouseEvent *evt)
{
    // 드래그 시작 시점에 탭을 클릭하지 않았다면 창 이동
    if ((evt->buttons() & Qt::LeftButton)) {
        int startTabIndex = tabAt(mapFromGlobal(m_dragStartPos));
        if (startTabIndex == -1) {
            // 탭이 없는 빈 공간에서 드래그를 시작했다면 창 이동
            if (QWidget *w = window()) {
                QPoint newPos = evt->globalPosition().toPoint();
                w->move(w->pos() + (newPos - m_dragStartPos));
                m_dragStartPos = newPos;
            }
        } else {
            // 탭 위에서 드래그하면 QTabBar의 기본 기능(탭 순서 바꾸기)을 사용
            QTabBar::mouseMoveEvent(evt);
        }
    }
}

// 3. 새로운 mouseReleaseEvent 추가
void CustomTabBar::mouseReleaseEvent(QMouseEvent *evt)
{
    // 버튼에서 손을 떼면 기본 이벤트를 호출합니다.
    QTabBar::mouseReleaseEvent(evt);
}

QSize CustomTabBar::tabSizeHint(int index) const {
    QSize s = QTabBar::tabSizeHint(index);
    s.setWidth(m_fixedTabWidth);
    s.setHeight(height());
    return s;
}

QRect CustomTabBar::closeIconRect(int idx) const {
    QRect r = tabRect(idx).adjusted(1, 0, -1, 0);
    constexpr int iconSz = 14;
    int x = r.right() - m_padding - iconSz;
    int y = r.top() + (r.height() - iconSz) / 2;
    return QRect(x, y, iconSz, iconSz);
}

void MainWindow::toggleNotificationPanel()
{
    if (m_notificationPanel->isVisible()) {
        m_notificationPanel->hide();
    } else {
        QToolButton *button = qobject_cast<QToolButton*>(sender());
        if (!button) return;

        QPoint globalPos = button->mapToGlobal(QPoint(0, 0));
        int x = globalPos.x() - m_notificationPanel->width() + button->width();
        int y = globalPos.y() + button->height() + 5;

        m_notificationPanel->move(x, y);
        m_notificationPanel->show();

        // ▼▼▼ 추가된 부분: 패널을 보여준 후 키보드 포커스를 줌 ▼▼▼
        m_notificationPanel->setFocus();
    }
}


void MainWindow::toggleLoginPopup()
{
    if (m_loginPopup->isVisible()) {
        m_loginPopup->hide();
    } else {
        QToolButton *button = qobject_cast<QToolButton*>(sender());
        if (!button) return;

        QPoint globalPos = button->mapToGlobal(QPoint(0, 0));
        // 팝업이 버튼의 중앙 하단에 오도록 위치 조정
        int x = globalPos.x() - (m_loginPopup->width() / 2) + (button->width() / 2);
        int y = globalPos.y() + button->height() + 5;

        m_loginPopup->move(x, y);
        m_loginPopup->show();
        m_loginPopup->setFocus();
    }
}


// 재귀적으로 리소스 폴더를 디스크에 복사하는 최종 수정된 헬퍼 함수
bool copyResourceDirectory(const QString &from, const QString &to)
{
    QDir destDir(to);

    // QDirIterator는 'from' 경로의 모든 하위 폴더와 파일을 순회합니다.
    // 따라서 'content' 폴더와 그 안의 파일들도 모두 포함됩니다.
    QDirIterator it(from, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString srcPath = it.next();
        QFileInfo srcFileInfo(srcPath);

        // 리소스 시스템에서 '폴더'는 실제 항목이 아니므로, 파일이 아닌 경우는 건너뜁니다.
        // 이렇게 하면 실제 파일만 복사 대상으로 삼게 되어 더 안정적입니다.
        if (!srcFileInfo.isFile()) {
            continue;
        }

        // 'from' 경로를 제외한 상대 경로를 정확하게 구합니다.
        // 예: ':/Documents/Documents/content/script.js' -> 'content/script.js'
        QString relativePath = srcPath.mid(from.length() + 1);

        // 최종 목적지 파일 경로를 계산합니다.
        // 예: 'C:/Users/.../temp_dir/content/script.js'
        QString destPath = destDir.filePath(relativePath);

        // 목적지 파일의 상위 폴더 경로를 구합니다 (예: '.../temp_dir/content')
        QDir parentDir = QFileInfo(destPath).dir();

        // 상위 폴더가 존재하지 않으면 재귀적으로 생성합니다.
        // 이 로직이 'content' 폴더를 만들어주는 핵심 부분입니다.
        if (!parentDir.exists()) {
            if (!parentDir.mkpath(".")) {
                qWarning() << "Failed to create parent directory:" << parentDir.path();
                return false;
            }
        }

        // 기존에 파일이 있으면 덮어쓰기 위해 삭제합니다.
        if (QFile::exists(destPath)) {
            QFile::remove(destPath);
        }

        // 리소스 파일을 실제 파일 시스템으로 복사합니다.
        if (!QFile::copy(srcPath, destPath)) {
            qWarning() << "Failed to copy file:" << srcPath << "to" << destPath;
            return false;
        }
    }
    return true;
}

void MainWindow::openHelpFile()
{
    // 1. 임시 "디렉터리" 생성
    // QTemporaryDir는 함수가 끝나면 자신과 내용물을 자동으로 삭제합니다.
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        CustomMessageBox msg("임시 디렉터리를 생성할 수 없습니다.", this);
        return;
    }
    // 크롬이 파일을 읽기 전에 디렉터리가 삭제되는 것을 방지합니다.
    tempDir.setAutoRemove(false);

    // 2. 리소스 폴더 전체를 임시 디렉터리로 복사
    QString resourceSourcePath = ":/Documents/Documents";
    QString tempDirPath = tempDir.path();

    if (!copyResourceDirectory(resourceSourcePath, tempDirPath)) {
        // QMessageBox::warning(this, "오류", "도움말 파일을 임시 폴더로 복사하는 데 실패했습니다.");
        CustomMessageBox msg("도움말 파일을 임시 폴더로 복사하는 데 실패했습니다.", this);
        msg.exec();
        return;
    }

    // 3. 임시 디렉터리 안의 index.html 파일 경로 설정
    QString tempHtmlPath = QDir(tempDirPath).filePath("index.html");
    qDebug() << "Help files copied to:" << tempDirPath;
    // 4. 크롬 경로를 찾아 QProcess로 실행
    QString chromePath;
#if defined(Q_OS_WIN)
    chromePath = "C:/Program Files/Google/Chrome/Application/chrome.exe";
    if (!QFile::exists(chromePath)) {
        chromePath = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe";
    }
#elif defined(Q_OS_MAC)
    chromePath = "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome";
#else // Linux
    chromePath = "google-chrome";
#endif

    // 크롬이 없으면 기본 브라우저로 대신 실행
    if (!QFile::exists(chromePath)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(tempHtmlPath));
        return;
    }

    QProcess::startDetached(chromePath, QStringList() << tempHtmlPath);
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // topBar의 전역 좌표계에서의 영역을 구합니다
    QRect topBarGlobalRect = topBar->geometry();

    // 마우스 클릭 위치가 topBar 영역 내에 있는지 확인
    if (event->button() == Qt::LeftButton && topBarGlobalRect.contains(event->pos())) {
        m_dragPosition = event->globalPosition().toPoint() - this->pos();
        m_isWindowDragging = true;  // 드래그 상태 플래그 설정
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // Move the window ONLY if the left button is held down.
    if ((event->buttons() & Qt::LeftButton) && m_isWindowDragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

// mainwindow.cpp에 mouseReleaseEvent 추가
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isWindowDragging = false;  // 드래그 상태 해제
}

// mainwindow.cpp의 mouseDoubleClickEvent 수정
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    // topBar의 전역 좌표계에서의 영역을 구합니다
    QRect topBarGlobalRect = topBar->geometry();

    // 더블클릭 위치가 topBar 영역 내에 있는지 확인
    if (event->button() == Qt::LeftButton && topBarGlobalRect.contains(event->pos())) {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
        event->accept();
    }
}
void MainWindow::changeEvent(QEvent* event)
{
    QMainWindow::changeEvent(event);
    // Update the maximize button's icon when the window state changes
    if (event->type() == QEvent::WindowStateChange && maxB) {
        maxB->setText(isMaximized() ? "🗗" : "☐");
    }
}


LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    // --- Main Layout and Background ---
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // --- Login Form Container ---
    auto *formFrame = new QFrame(this);
    formFrame->setObjectName("formFrame");
    formFrame->setFixedWidth(600);
    formFrame->setStyleSheet("background-color: transparent;"); // Frame is only for layout

    auto *formLayout = new QVBoxLayout(formFrame);
    formLayout->setContentsMargins(120, 70, 120, 70);
    formLayout->setSpacing(0);

    // --- Title & Version ---
    // auto *titleLabel = new QLabel("SAFETYNET Viewer", formFrame);
    // titleLabel->setObjectName("titleLabel");

    // auto *versionLabel = new QLabel("1.05.00", formFrame);
    // versionLabel->setObjectName("versionLabel");
    // --- Title & Version ---
    // 타이틀을 위한 수평 레이아웃 생성
    auto *titleContainer = new QWidget(formFrame);
    auto *titleLayout = new QHBoxLayout(titleContainer);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(8); // 로고와 텍스트 사이 간격
    titleLayout->setAlignment(Qt::AlignCenter);

    // SAFETYNET 로고 이미지
    auto *logoLabel = new QLabel(titleContainer);
    QPixmap logoPixmap(":/icons/icons/45cac18a-7e5b-44a9-8a11-faf5878934f3-removebg-preview_cut_2.png");
    // 로고 크기를 타이틀 텍스트와 어울리도록 조정 (높이 32px)
    logoLabel->setPixmap(logoPixmap.scaledToHeight(32, Qt::SmoothTransformation));

    // Viewer 텍스트 레이블
    auto *viewerLabel = new QLabel("Viewer", titleContainer);
    viewerLabel->setObjectName("viewerLabel");
    viewerLabel->setStyleSheet(R"(
        #viewerLabel {
            color: #E0E0E0;
            font-size: 28px;
            font-weight: bold;
        }
    )");

    // 레이아웃에 추가
    titleLayout->addWidget(logoLabel);
    titleLayout->addWidget(viewerLabel);
    auto *versionLabel = new QLabel("1.01.00", formFrame);
    versionLabel->setObjectName("versionLabel");
    // --- ID Field ---
    idLineEdit = new QLineEdit(formFrame);
    idLineEdit->setPlaceholderText("ID");
    // --- Password Field ---
    passwordLineEdit = new QLineEdit(formFrame);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("비밀번호"); // 1. Placeholder 추가

    // --- Checkboxes ---
    auto *saveIdCheckBox = new QCheckBox("ID 저장", formFrame);
    auto *autoLoginCheckBox = new QCheckBox("자동 로그인", formFrame);
    saveIdCheckBox->setChecked(true);
    autoLoginCheckBox->setChecked(true);

    // --- Login Button ---
    auto *loginButton = new QPushButton("로그인", formFrame);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setFixedHeight(40);
    // --- Add Widgets to Layout ---
    // formLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
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

    // formLayout->addStretch(1);
    formLayout->addWidget(loginButton);

    mainLayout->addWidget(formFrame);

    // --- Stylesheet for the new design ---
    const QString styleSheet = R"(
    QWidget {
        background-color: #121212;
    }
    #formFrame {
        border: 1px solid #909090;
        border-radius: 4px;
        background-color: #121212;
    }
    #titleLabel {
        color: #E0E0E0;
        font-size: 28px;
        font-weight: bold;
    }
    #versionLabel {
        color: #888888;
        font-size: 14px;
    }
    QLineEdit {
        color: #E0E0E0;
        font-size: 12px;
        background-color: transparent;
        border: none;
        border-bottom: 1px solid #5c5c5c;
        padding: 6px 2px;
    }
    QLineEdit:focus {
        border-bottom: 1px solid #f4731f;
    }
    QCheckBox {
        color: #B0B0B0;
        font-size: 12px;
        spacing: 12px;
    }
    QCheckBox::indicator {
        width: 18px;
        height: 18px;
        border: 1px solid #909090;
        background-color: transparent;
    }
    QCheckBox::indicator:checked {
        image: url(:/icons/icons/check_16dp_E5E5E5_FILL0_wght400_GRAD0_opsz20.svg);
    }
    QPushButton {
        color: #AAAAAA;
        background-color: transparent;
        border: 1px solid #5c5c5c;
        font-size: 14px;
        font-weight: bold;
        padding: 8px 16px;
        text-align: center;
    }
    QPushButton:pressed {
        background-color: rgb(224, 0, 0);
    }
    QPushButton:hover {
        color: #FFFFFF !important;
        background-color: #909090 !important;
        border-color: #888888 !important;
    }
)";
    this->setStyleSheet(styleSheet);

    // --- Connect Signals ---
    idLineEdit->setFocus();
    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::attemptLogin);
}


void LoginWidget::attemptLogin()
{
    if (idLineEdit->text() == "admin" && passwordLineEdit->text() == "admin") {
        emit loginSuccessful(); // 성공 시 신호 발생
    } else {
        CustomMessageBox msg("ID 또는 비밀번호가 잘못되었습니다.", this);
        msg.exec();
        passwordLineEdit->clear();
        passwordLineEdit->setFocus();
    }
}


void MainWindow::onLoginSuccess()
{
    // 스택 위젯의 페이지를 메인 컨텐츠 화면으로 전환
    m_mainStack->setCurrentIndex(1);

    // bigAddBtn->show();

    // 로그인 성공 후 초기 탭 생성
    onAddView();
}
void MainWindow::handleLogout()
{
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath(), qApp->arguments());
}





void MainWindow::showEventSearchDialog()
{
    qDebug() << "[EventSearchDialog] 다이얼로그 열기 시작";
    qDebug() << "[EventSearchDialog] 현재 m_eventData 크기:" << m_eventData.size();

    // 과거 로그 로드 (EventSearchDialog가 처음 열릴 때만)
    if (!m_eventSearchDialog) {
        qDebug() << "[EventSearchDialog] 새로운 다이얼로그 생성 - 과거 로그 로드 시작";
        loadInitialLogs();

        // 과거 로그 로드 후 잠시 대기 (네트워크 요청 완료 대기)
        QTimer::singleShot(1000, this, [this]() {
            qDebug() << "[EventSearchDialog] 과거 로그 로드 후 m_eventData 크기:" << m_eventData.size();
        });
    }

    if (!m_eventSearchDialog) {
        qDebug() << "[EventSearchDialog] EventSearchDialog 생성";
        m_eventSearchDialog = new EventSearchDialog(m_eventData, this);
    } else {
        qDebug() << "[EventSearchDialog] 기존 다이얼로그 업데이트";
        // 기존 다이얼로그가 있다면 데이터를 업데이트
        m_eventSearchDialog->updateEventData(m_eventData);
        // 이미지 초기화
        m_eventSearchDialog->resetImagePreview();
    }

    m_eventSearchDialog->show();
    m_eventSearchDialog->raise();
    m_eventSearchDialog->activateWindow();

    qDebug() << "[EventSearchDialog] 다이얼로그 표시 완료";
}







EventSearchDialog::EventSearchDialog(const QList<MainWindow::RawEv>& eventData, QWidget *parent)
    : QDialog(parent)
{
    m_originalEventData = eventData;
    m_networkManager = new QNetworkAccessManager(this);
    setupUi();
    populateTable(eventData);

    // Connect custom window buttons
    connect(findChild<QToolButton*>("closeBtn"), &QToolButton::clicked, this, &EventSearchDialog::close);
}

EventSearchDialog::~EventSearchDialog()
{
    try {
        // 네트워크 매니저 정리
        if (m_networkManager) {
            m_networkManager->deleteLater();
            m_networkManager = nullptr;
        }

        qDebug() << "EventSearchDialog destructor completed successfully";
    } catch (...) {
        qWarning() << "Exception in EventSearchDialog destructor";
    }
}

void EventSearchDialog::populateTable(const QList<MainWindow::RawEv>& eventData)
{
    resultsTable->setRowCount(0); // Clear existing data

    // 실제 이벤트 데이터를 테이블에 추가
    for(const auto& event : eventData) {
        int newRow = resultsTable->rowCount();
        resultsTable->insertRow(newRow);

        // 날짜/시간 포맷팅 (yyyy-MM-dd hh:mm:ss)
        QString formattedDateTime = event.ts.toString("yyyy-MM-dd hh:mm:ss");
        resultsTable->setItem(newRow, 0, new QTableWidgetItem(formattedDateTime));
        resultsTable->setItem(newRow, 1, new QTableWidgetItem(event.cam));
        resultsTable->setItem(newRow, 2, new QTableWidgetItem(event.txt));
    }

    resultsCountLabel->setText(QString("%L1 Results").arg(resultsTable->rowCount()));
}


void EventSearchDialog::setupUi()
{
    setMinimumSize(1290, 700);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window); // Remove default title bar

    // Main vertical layout (TitleBar + Content)
    auto *mainVLayout = new QVBoxLayout(this);
    mainVLayout->setContentsMargins(1, 1, 1, 1); // Border thickness
    mainVLayout->setSpacing(0);

    // --- Custom Title Bar (Height Increased) ---
    auto *titleBar = new QFrame(this);
    titleBar->setFixedHeight(40);
    titleBar->setObjectName("titleBar");
    auto *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(10, 0, 5, 0);

    auto *closeBtn = new QToolButton(titleBar);
    closeBtn->setObjectName("closeBtn");
    closeBtn->setText("✕");
    closeBtn->setFixedSize(28, 28);

    titleLayout->addStretch(); // Title is removed, so just stretch and add close button
    titleLayout->addWidget(closeBtn);

    // --- Main Content Horizontal Layout ---
    auto *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(12);
    contentLayout->setContentsMargins(15, 15, 15, 15);

    // --- 1. Left Search Panel ---
    auto *leftPanel = new QFrame(this);
    leftPanel->setObjectName("leftPanel");
    auto *leftVBox = new QVBoxLayout(leftPanel);
    leftVBox->setContentsMargins(15, 15, 15, 15);
    leftVBox->setSpacing(15);

    // Helper lambda to create a separator
    auto addSeparator = [&](QVBoxLayout* layout){
        auto* sep = new QFrame(leftPanel);
        sep->setFrameShape(QFrame::HLine);
        sep->setFrameShadow(QFrame::Sunken);
        sep->setStyleSheet("background-color: #5c5c5c;");
        sep->setFixedHeight(1);
        layout->addWidget(sep);
    };

    // Add "Event search" title to the left panel
    auto *panelTitleLabel = new QLabel("Event search", leftPanel);
    panelTitleLabel->setObjectName("panelTitle");
    leftVBox->addWidget(panelTitleLabel);
    leftVBox->addSpacing(10);

    addSeparator(leftVBox);
    leftVBox->addSpacing(10);

    auto addTitle = [&](const QString& title){
        auto* label = new QLabel(title, leftPanel);
        label->setObjectName("sectionTitle");
        leftVBox->addWidget(label);
    };

    addTitle("Device");
    deviceCombo = new QComboBox(leftPanel);
    deviceCombo->addItem("All");
    deviceCombo->setObjectName("borderedCombo");
    leftVBox->addWidget(deviceCombo);

    leftVBox->addSpacing(15);
    addSeparator(leftVBox);
    leftVBox->addSpacing(15);

    // leftVBox->addWidget(dateTimeFrame);
    addTitle("Date & Time");
    auto* dateTimeFrame = new QFrame(leftPanel);
    // dateTimeFrame->setObjectName("borderedFrame");
    // 레이아웃을 QGridLayout으로 변경
    auto* gridLayout = new QGridLayout(dateTimeFrame);
    gridLayout->setContentsMargins(10, 10, 10, 10);
    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(8);

    // 1행: "From" / "To" 레이블
    gridLayout->addWidget(new QLabel("From", leftPanel), 0, 0);
    gridLayout->addWidget(new QLabel("To", leftPanel), 0, 1);

    // 2행: 날짜 선택 위젯 (QDateEdit)
    fromDateEdit = new QDateEdit(QDate::currentDate().addDays(-2), leftPanel);
    fromDateEdit->setDisplayFormat("MM/dd/yyyy"); // 형식 변경
    fromDateEdit->setAlignment(Qt::AlignCenter);
    fromDateEdit->setObjectName("dateEditStyle");       // 스타일용 이름 설정

    toDateEdit = new QDateEdit(QDate::currentDate(), leftPanel);
    toDateEdit->setDisplayFormat("MM/dd/yyyy");   // 형식 변경
    toDateEdit->setAlignment(Qt::AlignCenter);
    toDateEdit->setObjectName("dateEditStyle");         // 스타일용 이름 설정

    gridLayout->addWidget(fromDateEdit, 1, 0);
    gridLayout->addWidget(toDateEdit, 1, 1);

    // 3행: 시간 선택 위젯 (QTimeEdit)
    fromTimeEdit = new QTimeEdit(QTime(0, 0, 0), leftPanel);
    fromTimeEdit->setDisplayFormat("hh:mm:ss");   // 형식 변경
    fromTimeEdit->setAlignment(Qt::AlignCenter);
    fromTimeEdit->setObjectName("timeEditStyle");       // 스타일용 이름 설정
    toTimeEdit = new QTimeEdit(QTime(23, 59, 59), leftPanel);
    toTimeEdit->setDisplayFormat("hh:mm:ss");     // 형식 변경
    toTimeEdit->setAlignment(Qt::AlignCenter);
    toTimeEdit->setObjectName("timeEditStyle");         // 스타일용 이름 설정

    gridLayout->addWidget(fromTimeEdit, 2, 0);
    gridLayout->addWidget(toTimeEdit, 2, 1);

    leftVBox->addWidget(dateTimeFrame);

    leftVBox->addSpacing(15);
    addSeparator(leftVBox);
    leftVBox->addSpacing(15);

    addTitle("Event type");
    eventTypeCombo = new QComboBox(leftPanel);
    eventTypeCombo->addItem("All");
    eventTypeCombo->setObjectName("borderedCombo");
    leftVBox->addWidget(eventTypeCombo);
    leftVBox->addSpacing(20);

    leftVBox->addStretch();

    auto *buttonHBox = new QHBoxLayout();
    buttonHBox->setSpacing(10);
    searchBtn = new QPushButton("Search", leftPanel);
    searchBtn->setObjectName("searchBtn");
    resetBtn = new QPushButton("Reset", leftPanel);
    resetBtn->setObjectName("resetBtn");
    buttonHBox->addWidget(searchBtn, 2); // Stretch factor 2
    buttonHBox->addWidget(resetBtn, 1);  // Stretch factor 1
    leftVBox->addLayout(buttonHBox);

    // --- 2. Center Results Panel ---
    auto *centerPanel = new QWidget(this);
    auto *centerVBox = new QVBoxLayout(centerPanel);
    centerVBox->setContentsMargins(0, 0, 0, 0);
    centerVBox->setSpacing(6);

    resultsTable = new QTableWidget(this);
    resultsTable->setColumnCount(2);
    resultsTable->setHorizontalHeaderLabels({"Date & Time", "Device name"});
    resultsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    resultsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    resultsTable->setAlternatingRowColors(false);
    resultsTable->setShowGrid(false);
    resultsTable->verticalHeader()->hide();



    resultsCountLabel = new QLabel("0 Results", this);

    centerVBox->addWidget(resultsTable);
    centerVBox->addWidget(resultsCountLabel, 0, Qt::AlignLeft);

    // --- 3. Right Panel ---
    auto *rightPanel = new QWidget(this);
    auto *rightVBox = new QVBoxLayout(rightPanel);
    rightVBox->setContentsMargins(0,0,0,0);
    rightVBox->setSpacing(10);

    imagePreview = new QLabel(rightPanel);
    imagePreview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 기본 이미지 로드
    QPixmap p(":/test/no_camera.png");
    QPixmap scaledPixmap = p.scaledToWidth(460, Qt::SmoothTransformation);
    imagePreview->setPixmap(scaledPixmap);

    imagePreview->setAlignment(Qt::AlignCenter);
    imagePreview->setStyleSheet("background-color: #000000;");
    rightVBox->addWidget(imagePreview, 1);


    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1); // 스플리터 핸들 너비 조정
    splitter->setStyleSheet("QSplitter::handle { background-color: #5c5c5c; }");

    splitter->addWidget(leftPanel);
    splitter->addWidget(centerPanel);
    splitter->addWidget(rightPanel);

    // setSizes를 사용하여 초기 비율을 1:2:2에 가깝게 설정합니다.
    // 실제 크기는 창의 전체 너비에 따라 달라지므로, 임의의 큰 값으로 비율을 지정합니다.
    splitter->setSizes({250, 500, 500});

    // --- Main Layout Setup ---
    mainVLayout->addWidget(titleBar);
    mainVLayout->addWidget(splitter, 1); // 스플리터를 메인 레이아웃에 추가하고 stretch factor를 1로 설정

    const QString styleSheet = R"(
        EventSearchDialog {
            background-color: #2b2b2b; /* 배경색 통일 */
            border: 1px solid #5c5c5c;
            color: #d0d0d0;
        }
        #titleBar {
            background-color: #2b2b2b;
            border-bottom: 1px solid #5c5c5c;
        }
        #leftPanel, #centerPanel, #rightPanel {
            background-color: transparent;
            border: none;
        }
        QLabel {
            border: none;
            background: transparent;
            color: #d0d0d0;
        }
        #panelTitle {
            font-size: 22px;
            font-weight: bold;
            color: #e0e0e0;
        }
        #sectionTitle {
            font-weight: bold;
            color: #e0e0e0;
            margin-top: 8px;
            margin-bottom: 4px;
        }
        #borderedFrame {
             border: 1px solid #555;
             border-radius: 4px;
        }
        QComboBox {
            background-color: #2b2b2b;
            border: 1px solid #555;
            border-radius: 4px;
            padding: 5px;
            color: #d0d0d0;
        }
        #borderedCombo {
             border: 1px solid #555;
        }
        QComboBox::drop-down { border: none; }

        /* QDateEdit과 QTimeEdit의 공통 스타일 */
        QDateEdit, QTimeEdit {
            background-color: transparent; /* 배경을 투명하게 하여 패널 색이 보이게 함 */
            padding: 5px;
            color: #d0d0d0;
        }

        /* 날짜 위젯: 아래쪽 테두리만 설정 */
        #dateEditStyle {
            border: none;
            border-bottom: 1px solid #5c5c5c;
        }

        /* 시간 위젯: 전체 테두리 설정 */
        #timeEditStyle {
            border: 1px solid #5c5c5c;
            border-radius: 4px;
        }

        /* 화살표 버튼 숨기기 (디자인 통일성) */
        QDateEdit::up-button, QDateEdit::down-button,
        QTimeEdit::up-button, QTimeEdit::down-button {
            border: none;
            width: 0px;
        }

        QPushButton {
            background-color: #4f4f4f;
            color: #e0e0e0;
            border: 1px solid #666;
            padding: 12px 16px; /* Increased padding for bigger buttons */
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton:hover { background-color: #606060; }
        #searchBtn, #resetBtn {
            background-color: transparent;
            border: 1px solid #888;
        }
        #searchBtn {
            color: #f4731f;
            border-color: #f4731f;
        }
        #searchBtn:hover { background-color: #f4731f; color: white; }
        #resetBtn:hover { background-color: #888; color: white; }
        QHeaderView::section {
            background-color: #3c3c3c; /* 헤더 배경색 유지 */
            border: none;
            border-bottom: 1px solid #5c5c5c;
            padding: 8px;
            font-weight: bold;
            color: #d0d0d0;
        }
        QTableWidget {
            border: none;
            gridline-color: #3c3c3c;
            background-color: #2b2b2b;
        }
        QTableWidget::item { padding: 8px; border-bottom: 1px solid #3c3c3c;}
        QTableWidget::item:selected {
             background-color: #f4731f;
             color: white;
        }
        #closeBtn {
            background-color: transparent;
            color: #d0d0d0;
            border: none;
            font-size: 14px;
            font-weight: bold;
            font-family: "Segoe UI Symbol";
        }
        #closeBtn:hover { background-color: red; color: white; }
    )";
    this->setStyleSheet(styleSheet);

// --- Conditional Debug Borders ---
#ifdef DEBUG_UI_BORDER
    QList<QPair<QWidget*, QString>> debugList = {
        {titleBar, "red"},
        {leftPanel, "yellow"},
        {centerPanel, "lime"},
        {rightPanel, "cyan"},
        {resultsTable, "magenta"},
        {imagePreview, "orange"}
    };

    for (const auto &item : debugList) {
        // Use the global applyDebugBorder function from mainwindow.cpp
        applyDebugBorder(item.first, item.second);
    }
#endif // DEBUG_UI_BORDER

    // 이벤트 연결
    connect(searchBtn, &QPushButton::clicked, this, &EventSearchDialog::onSearchClicked);
    connect(resetBtn, &QPushButton::clicked, this, &EventSearchDialog::onResetClicked);
    connect(deviceCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &EventSearchDialog::onDeviceComboChanged);
    connect(eventTypeCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &EventSearchDialog::onEventTypeComboChanged);

    // 디바이스 콤보박스에 실제 카메라 목록 추가
    populateDeviceCombo();
    populateEventTypeCombo();

    // 초기 이미지 설정
    resetImagePreview();

    // 테이블 선택 시 이미지 업데이트 연결
    connect(resultsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        int currentRow = resultsTable->currentRow();
        if (currentRow >= 0 && currentRow < m_originalEventData.size()) {
            const auto& event = m_originalEventData[currentRow];
            if (!event.imageUrl.isEmpty()) {
                // 네트워크에서 이미지 로드
                QNetworkRequest request{QUrl(event.imageUrl)};
                QNetworkReply *reply = m_networkManager->get(request);
                reply->ignoreSslErrors();

                connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                    reply->deleteLater();
                    if (reply->error() == QNetworkReply::NoError) {
                        QPixmap pixmap;
                        if (pixmap.loadFromData(reply->readAll())) {
                            QPixmap scaledPixmap = pixmap.scaledToWidth(460, Qt::SmoothTransformation);
                            imagePreview->setPixmap(scaledPixmap);
                        } else {
                            // 이미지 로드 실패 시 기본 이미지 표시
                            QPixmap p(":/test/test_img_2.png");
                            QPixmap scaledPixmap = p.scaledToWidth(460, Qt::SmoothTransformation);
                            imagePreview->setPixmap(scaledPixmap);
                        }
                    } else {
                        // 네트워크 오류 시 기본 이미지 표시
                        QPixmap p(":/test/test_img_2.png");
                        QPixmap scaledPixmap = p.scaledToWidth(460, Qt::SmoothTransformation);
                        imagePreview->setPixmap(scaledPixmap);
                    }
                });
            } else {
                // 기본 이미지 표시
                QPixmap p(":/test/test_img_2.png");
                QPixmap scaledPixmap = p.scaledToWidth(460, Qt::SmoothTransformation);
                imagePreview->setPixmap(scaledPixmap);
            }
        }
    });
}

// Event handlers for the frameless window
void EventSearchDialog::mousePressEvent(QMouseEvent *event)
{
    // Check if the press is on the titleBar frame
    if (event->button() == Qt::LeftButton) {
        QWidget* child = childAt(event->pos());
        // Make sure child is not null before accessing objectName()
        if (child && child->objectName() == "titleBar") {
            isDragging = true;
            dragPosition = event->globalPosition().toPoint() - this->pos();
        }
    }
    QDialog::mousePressEvent(event);
}

void EventSearchDialog::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isDragging) {
        move(event->globalPosition().toPoint() - dragPosition);
    }
    QDialog::mouseMoveEvent(event);
}

void EventSearchDialog::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;
    QDialog::mouseReleaseEvent(event);
}


// ===================================================================
// BrightnessDialog 클래스 멤버 함수 구현
// ===================================================================
BrightnessDialog::BrightnessDialog(const QString &cameraName, int initialValue, QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(380, 240); 

    // --- Main Background ---
    QFrame *background = new QFrame(this);
    background->setStyleSheet("background-color: #232323; border: 1px solid #909090;");

    // --- Custom Draggable Title Bar ---
    titleBar = new QFrame(background);
    titleBar->setFixedHeight(35);
    titleBar->setStyleSheet("background-color: transparent; border: none;");

    auto *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(10, 0, 5, 0);
    

    auto *titleLabel = new QLabel("카메라 밝기 조절", titleBar);
    titleLabel->setStyleSheet("color: #E0E0E0; font-weight: bold;");

    QToolButton *closeButton = new QToolButton(titleBar);
    closeButton->setText("✕");
    closeButton->setStyleSheet("color: #AAAAAA; border: none; font-size: 16px;");
    connect(closeButton, &QToolButton::clicked, this, &QDialog::reject);

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton);

    // --- Content Widgets ---
    QLabel *cameraNameLabel = new QLabel(QString("<b>%1</b> 밝기 조절").arg(cameraName), background);
    cameraNameLabel->setAlignment(Qt::AlignCenter);
    cameraNameLabel->setStyleSheet("color: #E0E0E0; font-size: 18px; border: none;");

    QLabel *sliderTitleLabel = new QLabel("밝기 값 (-255~255):", background); // Text updated for new range
    sliderTitleLabel->setStyleSheet("color: #E0E0E0; border: none;");

    brightnessSlider = new QSlider(Qt::Horizontal, background);
    brightnessSlider->setRange(-255, 255); // REQUIREMENT 1: Set new range
    brightnessSlider->setValue(initialValue);  // REQUIREMENT 2: Set initial value

    valueLabel = new QLabel(QString::number(initialValue), background); // REQUIREMENT 2: Set initial label text
    valueLabel->setMinimumWidth(35); 
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet("color: #E0E0E0; border: none;");

    QHBoxLayout *sliderLayout = new QHBoxLayout();
    sliderLayout->setContentsMargins(20, 0, 20, 0);
    sliderLayout->addWidget(sliderTitleLabel);
    sliderLayout->addWidget(brightnessSlider, 1);
    sliderLayout->addWidget(valueLabel);

    QPushButton *confirmButton = new QPushButton("닫기", background);
    confirmButton->setFixedSize(140, 40);
    connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);

    // --- Main Layout ---
    QVBoxLayout *mainLayout = new QVBoxLayout(background);
    mainLayout->setContentsMargins(1, 1, 1, 20);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addStretch(2);
    mainLayout->addWidget(cameraNameLabel);
    mainLayout->addStretch(1);
    mainLayout->addLayout(sliderLayout);
    mainLayout->addStretch(3);
    mainLayout->addWidget(confirmButton, 0, Qt::AlignHCenter);

    // --- Dialog Layout ---
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(background);

    // --- Styling ---
    const QString qss = R"(
        QSlider::groove:horizontal {
            border: none;
            height: 4px;
            background: #555;
            margin: 0;
        }
        QSlider::handle:horizontal {
            background: #D0D0D0;
            border: 1px solid #D0D0D0;
            width: 16px;
            height: 16px;
            margin: -7px 0;
            border-radius: 8px;
        }
        QPushButton {
            background-color: #232323;
            color: #ffffff;
            border: 1px solid #909090;
            font-size: 11px;
        }
        QPushButton:hover { border-color: #ffffff; }
        QPushButton:pressed { background-color: #3c3c3c; }
    )";
    this->setStyleSheet(qss);

    // --- Connections ---
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

void MainWindow::onBrightnessControlClicked()
{
    if (m_activeSlotIndex == -1) {
        CustomMessageBox msg("먼저 밝기를 조절할 카메라 화면을 클릭해주세요.", this);
        msg.exec();
        return;
    }

    int layIdx = viewTabBar->currentIndex();
    if (layIdx < 0 || layIdx >= layoutSlotData.size() || m_activeSlotIndex >= layoutSlotData[layIdx].size()) {
        // Safety checks for out-of-bounds access
        return;
    }

    const QVariantMap& activeCamData = layoutSlotData.at(layIdx).at(m_activeSlotIndex);

    if (activeCamData.value("type").toString() != "camera") {
        CustomMessageBox msg("선택된 슬롯에 카메라가 등록되어 있지 않습니다.", this);
        msg.exec();
        return;
    }

    QString activeCamName = activeCamData.value("name").toString();
    QString activeCamIp = activeCamData.value("ip").toString();

    // REQUIREMENT 2: Get the last brightness value, defaulting to 0
    int lastBrightness = m_cameraBrightnessLevels.value(activeCamIp, 0);

    // Pass the retrieved value to the dialog's constructor
    BrightnessDialog *dialog = new BrightnessDialog(activeCamName, lastBrightness, this);

    connect(dialog, &BrightnessDialog::brightnessChanged, this, [this, activeCamData](int value) {
        sendBrightnessRequest(activeCamData, value);
    });

    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::sendBrightnessRequest(const QVariantMap &cameraData, int value)
{
    QString ip = cameraData.value("ip").toString();
    QString name = cameraData.value("name").toString();

    if (ip.isEmpty() || !m_webSocketMap.contains(ip)) {
        qWarning() << "[Brightness] WebSocket for" << name << "(" << ip << ") not found.";
        return;
    }

    QWebSocket *socket = m_webSocketMap[ip];
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "[Brightness] WebSocket for" << name << "(" << ip << ") is not connected.";
        return;
    }

    QJsonObject payload;
    payload["type"] = "set_brightness";
    payload["value"] = value;

    QJsonDocument doc(payload);
    socket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    qDebug() << "💡 [Brightness] Sent 'set_brightness' to" << name << "(" << ip << ") with value:" << value;
}

void MainWindow::onGlobalCameraRegisterClicked()
{
    int lay = viewTabBar->currentIndex();
    if (lay < 0) {
        // QMessageBox::warning(this, tr("No Layout"),
        //                      tr("먼저 레이아웃 탭을 추가하세요."));
        CustomMessageBox msg("먼저 레이아웃 탭을 추가하세요.", this);
        msg.exec();
        return;
    }

    // 배열 범위 검사 추가
    if (lay >= layoutSlotData.size()) {
        qDebug() << "Layout index out of range:" << lay;
        CustomMessageBox msg("레이아웃 인덱스가 범위를 벗어났습니다.", this);
        msg.exec();
        return;
    }

    // 1) find first empty slot
    const auto &slots_ = layoutSlotData[lay];
    int freeIdx = -1;
    for (int i = 0; i < slots_.size(); ++i) {
        if (slots_[i].isEmpty()) {
            freeIdx = i;
            break;
        }
    }
    if (freeIdx < 0) {
        // QMessageBox::information(this, tr("No Empty Slot"),
        //                          tr("빈 슬롯이 없습니다."));

        CustomMessageBox msg("빈 슬롯이 없습니다.", this);
        msg.exec();
        return;
    }

    // 2) show the registration dialog
    CameraRegistrationDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        const QString name = dlg.deviceName();
        const QString ip   = dlg.ip();
        const QString port = dlg.port();

        // 3) forward to your existing handler (it sets up the URL/WebSocket and starts streaming)
        onCameraRegistered(freeIdx, name, ip, port);
    }
}


/**
 * @brief 이벤트 목록에 새 EventCard를 추가하는 함수
 */
void MainWindow::addEvent(const QString &cam, const QString &txt, const QDateTime &ts, const QString &imageUrl)
{
    if (!eventContainer || !eventLay) return;

    // 실시간 이벤트를 m_eventData에 추가
    m_eventData.prepend({cam, txt, ts, imageUrl});

    // 실시간 이벤트만 메인 화면에 표시
    EventCard *card = new EventCard(cam, txt, ts, imageUrl, EventCard::Event, eventContainer);
    connect(card, &EventCard::starToggled, this, &MainWindow::onCardStarToggled);

    // 새 카드를 목록의 맨 위에 추가 (스트레치 아이템 위)
    eventLay->insertWidget(0, card);

    // EventSearchDialog가 열려있다면 데이터 업데이트 (실시간 + 과거 로그 모두)
    if (m_eventSearchDialog && m_eventSearchDialog->isVisible()) {
        m_eventSearchDialog->updateEventData(m_eventData);
    }
}



// ===================================================================
// EventSearchDialog 추가 메서드 구현
// ===================================================================

void EventSearchDialog::onSearchClicked()
{
    filterAndPopulateTable();
}

void EventSearchDialog::onResetClicked()
{
    // 모든 필터를 초기화
    deviceCombo->setCurrentText("All");
    eventTypeCombo->setCurrentText("All");
    // fromDateTime->setDateTime(QDateTime::currentDateTime().addDays(-2));
    // toDateTime->setDateTime(QDateTime::currentDateTime());

    fromDateEdit->setDate(QDate::currentDate().addDays(-2));
    fromTimeEdit->setTime(QTime(0, 0, 0));
    toDateEdit->setDate(QDate::currentDate());
    toTimeEdit->setTime(QTime(23, 59, 59));

    // 전체 데이터 표시
    populateTable(m_originalEventData);
}

void EventSearchDialog::onDeviceComboChanged(const QString &device)
{
    Q_UNUSED(device)
    // 실시간 필터링을 원한다면 여기에 filterAndPopulateTable() 호출
}

void EventSearchDialog::onEventTypeComboChanged(const QString &eventType)
{
    Q_UNUSED(eventType)
    // 실시간 필터링을 원한다면 여기에 filterAndPopulateTable() 호출
}

void EventSearchDialog::filterAndPopulateTable()
{
    QList<MainWindow::RawEv> filteredData = filterEvents();
    populateTable(filteredData);
}

QList<MainWindow::RawEv> EventSearchDialog::filterEvents()
{
    QList<MainWindow::RawEv> filteredData;

    QString selectedDevice = deviceCombo->currentText();
    QString selectedEventType = eventTypeCombo->currentText();
    // QDateTime fromDate = fromDateTime->dateTime();
    // QDateTime toDate = toDateTime->dateTime();
    QDateTime fromDate(fromDateEdit->date(), fromTimeEdit->time());
    QDateTime toDate(toDateEdit->date(), toTimeEdit->time());

    for (const auto& event : m_originalEventData) {
        bool deviceMatch = (selectedDevice == "All" || event.cam == selectedDevice);
        bool eventTypeMatch = (selectedEventType == "All" || event.txt.contains(selectedEventType, Qt::CaseInsensitive));
        bool dateMatch = (event.ts >= fromDate && event.ts <= toDate);

        if (deviceMatch && eventTypeMatch && dateMatch) {
            filteredData.append(event);
        }
    }

    return filteredData;
}

void EventSearchDialog::populateDeviceCombo()
{
    QString currentSelection = deviceCombo->currentText();
    deviceCombo->clear();
    deviceCombo->addItem("All");

    // 실제 카메라 목록을 가져와서 추가
    QSet<QString> uniqueDevices;
    for (const auto& event : m_originalEventData) {
        uniqueDevices.insert(event.cam);
    }

    QStringList deviceList = uniqueDevices.values();
    deviceList.sort();
    deviceCombo->addItems(deviceList);

    // 이전 선택 상태 복원 (해당 항목이 여전히 존재하는 경우)
    if (!currentSelection.isEmpty() && deviceCombo->findText(currentSelection) >= 0) {
        deviceCombo->setCurrentText(currentSelection);
    }
}

void EventSearchDialog::populateEventTypeCombo()
{
    QString currentSelection = eventTypeCombo->currentText();
    eventTypeCombo->clear();
    eventTypeCombo->addItem("All");

    // 실제 이벤트 타입 목록을 가져와서 추가
    QSet<QString> uniqueEventTypes;
    for (const auto& event : m_originalEventData) {
        uniqueEventTypes.insert(event.txt);
    }

    QStringList eventTypeList = uniqueEventTypes.values();
    eventTypeList.sort();
    eventTypeCombo->addItems(eventTypeList);

    // 이전 선택 상태 복원 (해당 항목이 여전히 존재하는 경우)
    if (!currentSelection.isEmpty() && eventTypeCombo->findText(currentSelection) >= 0) {
        eventTypeCombo->setCurrentText(currentSelection);
    }
}

void EventSearchDialog::updateEventData(const QList<MainWindow::RawEv>& eventData)
{
    m_originalEventData = eventData;
    populateDeviceCombo();
    populateEventTypeCombo();

    // 항상 전체 데이터를 표시하고, 사용자가 직접 검색 버튼을 눌러야 필터링됨
    populateTable(eventData);
}

// ===================================================================
// 과거 로그 로딩 관련 메서드들
// ===================================================================

void MainWindow::loadInitialLogs()
{
    qDebug() << "[과거 로그 로딩 시작]";

    // m_networkManager null 체크
    if (!m_networkManager) {
        qWarning() << "[과거 로그 로딩 실패] m_networkManager is null";
        return;
    }

    // layoutSlotData가 비어있는지 확인
    if (layoutSlotData.isEmpty()) {
        qDebug() << "[과거 로그 로딩] layoutSlotData가 비어있음 - 카메라가 등록되지 않음";
        return;
    }

    qDebug() << "[과거 로그 로딩] layoutSlotData 크기:" << layoutSlotData.size();

    // 실시간 이벤트는 유지하고 과거 로그만 추가
    // m_eventData.clear() 제거 - 실시간 이벤트가 사라지지 않도록 함

    int cameraCount = 0;
    // 등록된 카메라들에 대해 과거 로그 요청
    for (const auto& layout : layoutSlotData) {
        for (const auto& slotData : layout) {
            if (slotData.value("type").toString() == "camera") {
                QString cameraName = slotData.value("name").toString();
                QString cameraIp = slotData.value("ip").toString();
                QString cameraPort = slotData.value("port").toString();

                qDebug() << "[과거 로그 로딩] 카메라 발견:" << cameraName << cameraIp << cameraPort;

                if (!cameraIp.isEmpty() && cameraPort != "554") {  // RTMPS 카메라만
                    loadHistoricalEvents(cameraName, cameraIp, cameraPort);
                    cameraCount++;
                } else {
                    qDebug() << "[과거 로그 로딩] 카메라 제외 (IP 비어있거나 RTSP):" << cameraName;
                }
            }
        }
    }

    qDebug() << "[과거 로그 로딩 완료] 총" << cameraCount << "개 카메라에 대해 과거 로그 요청";
}

void MainWindow::loadHistoricalEvents(const QString &cameraName, const QString &cameraIp, const QString &cameraPort)
{
    qDebug() << "[과거 로그 요청]" << cameraName << cameraIp;

    // m_networkManager null 체크 추가
    if (!m_networkManager) {
        qWarning() << "[과거 로그 요청 실패] m_networkManager is null";
        return;
    }

    // 각 이벤트 타입별로 API 호출
    QStringList eventTypes = {"detections", "trespass", "fall"};

    for (const QString &eventType : eventTypes) {
        QString url = QString("https://%1:8443/api/%2").arg(cameraIp, eventType);
        QNetworkRequest request{QUrl(url)};
        QNetworkReply *reply = m_networkManager->get(request);
        reply->ignoreSslErrors();

        connect(reply, &QNetworkReply::finished, this, [=]() {
            reply->deleteLater();

            // this 포인터 유효성 검사
            if (!this) {
                qWarning() << "[과거 로그 요청 실패] this pointer is null";
                return;
            }

            if (reply->error() != QNetworkReply::NoError) {
                qWarning() << "[과거 로그 요청 실패]" << cameraName << eventType << reply->errorString();
                return;
            }

            try {
                QByteArray responseData = reply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(responseData);
                if (!doc.isObject()) {
                    qWarning() << "[과거 로그 JSON 파싱 실패]" << cameraName << eventType;
                    return;
                }

                QJsonArray events = doc[eventType].toArray();
                for (const QJsonValue &val : events) {
                    try {
                        QJsonObject eventData = val.toObject();
                        processHistoricalEvent(cameraName, eventType, eventData, cameraIp);
                    } catch (...) {
                        qWarning() << "Exception in JSON event processing for camera:" << cameraName << "eventType:" << eventType;
                    }
                }

                qDebug() << "[과거 로그 처리 완료]" << cameraName << eventType << events.size() << "건";
            } catch (...) {
                qWarning() << "[과거 로그 처리 실패] Exception in lambda for camera:" << cameraName << "eventType:" << eventType;
            }
        });
    }
}

void MainWindow::processHistoricalEvent(const QString &cameraName, const QString &eventType,
                                        const QJsonObject &eventData, const QString &cameraIp)
{
    try {
        QString timestamp;
        QString imagePath;
        QString eventText;

        try {
            timestamp = eventData["timestamp"].toString();
            imagePath = eventData["image_path"].toString();
        } catch (...) {
            qWarning() << "Exception in JSON field access for camera:" << cameraName;
            return;
        }

        // 이벤트 타입별 처리
        try {
            if (eventType == "detections") {
                int person = eventData["person_count"].toInt();
                int helmet = eventData["helmet_count"].toInt();
                int vest = eventData["safety_vest_count"].toInt();

                if (helmet < person && vest >= person) {
                    eventText = "⛑️ 헬멧 미착용 감지";
                } else if (vest < person && helmet >= person) {
                    eventText = "🦺 조끼 미착용 감지";
                } else {
                    eventText = "⛑️ 🦺 PPE 미착용 감지";
                }
            }
            else if (eventType == "trespass") {
                int count = eventData["count"].toInt();
                eventText = QString("🚷 무단 침입 감지 (%1명)").arg(count);
            }
            else if (eventType == "fall") {
                eventText = "🚨 낙상 감지";
            }
        } catch (...) {
            qWarning() << "Exception in event type processing for camera:" << cameraName << "eventType:" << eventType;
            return;
        }

        if (!eventText.isEmpty()) {
            QDateTime eventTime;
            try {
                eventTime = QDateTime::fromString(timestamp, "yyyy-MM-dd hh:mm:ss");
            } catch (...) {
                qWarning() << "Exception in QDateTime::fromString for timestamp:" << timestamp;
                return;
            }

            if (eventTime.isValid()) {
                // 이미지 URL 생성
                QString imageUrl;
                if (!imagePath.isEmpty()) {
                    try {
                        QString cleanPath = imagePath.startsWith("../") ? imagePath.mid(3) : imagePath;
                        imageUrl = QString("http://%1/%2").arg(cameraIp, cleanPath);
                    } catch (...) {
                        qWarning() << "Exception in imageUrl generation for camera:" << cameraName;
                    }
                }

                // 과거 로그는 m_eventData에만 추가하고 메인 화면에는 표시하지 않음
                try {
                    m_eventData.append({cameraName, eventText, eventTime, imageUrl});
                    qDebug() << "[과거 로그 추가] 카메라:" << cameraName << "이벤트:" << eventText << "시간:" << eventTime.toString("yyyy-MM-dd hh:mm:ss");
                } catch (...) {
                    qWarning() << "Exception in m_eventData.append for camera:" << cameraName;
                }

                // EventSearchDialog가 열려있다면 업데이트
                if (m_eventSearchDialog && m_eventSearchDialog->isVisible()) {
                    try {
                        m_eventSearchDialog->updateEventData(m_eventData);
                    } catch (...) {
                        qWarning() << "Exception in updateEventData for camera:" << cameraName;
                    }
                }
            }
        }
    } catch (...) {
        qWarning() << "Exception in processHistoricalEvent for camera:" << cameraName << "eventType:" << eventType;
    }
}

void EventSearchDialog::resetImagePreview()
{
    if (imagePreview) {
        // 기본 이미지로 초기화
        QPixmap p(":/test/no_camera.png");
        QPixmap scaledPixmap = p.scaledToWidth(460, Qt::SmoothTransformation);
        imagePreview->setPixmap(scaledPixmap);
    }
}



void MainWindow::updateCameraConnectionStatus(const QString &ip, const QString &status, const QString &color)
{
    // 카메라 연결 상태 저장
    m_cameraConnectionStatus[ip] = status;

    // 레이아웃 트리 업데이트
    updateLayoutTree();
}

void MainWindow::startHealthCheck(const QString &ip)
{
    // 기존 타이머가 있다면 제거
    if (m_healthCheckTimers.contains(ip)) {
        QTimer *oldTimer = m_healthCheckTimers[ip];
        oldTimer->stop();
        oldTimer->deleteLater();
        m_healthCheckTimers.remove(ip);
    }

    // 연결 중 상태로 변경 (주황색)
    updateCameraConnectionStatus(ip, "연결 중", "orange");

    // 헬스체크 요청 전송
    if (m_webSocketMap.contains(ip)) {
        QWebSocket *socket = m_webSocketMap[ip];
        if (socket->state() == QAbstractSocket::ConnectedState) {
            QJsonObject payload;
            payload["type"] = "request_stm_status";

            QJsonDocument doc(payload);
            socket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
            qDebug() << "✅ [Health Check] Sent 'request_stm_status' to" << ip;
        } else {
            qWarning() << "⚠️ [Health Check] WebSocket for" << ip << "is not connected.";
            updateCameraConnectionStatus(ip, "연결 실패", "red");
            return;
        }
    } else {
        qWarning() << "⚠️ [Health Check] WebSocket for" << ip << "not found.";
        updateCameraConnectionStatus(ip, "연결 실패", "red");
        return;
    }

    // 5초 타이머 생성
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, [this, ip, timer]() {
        // 5초 후에도 응답이 없으면 빨간색으로 변경
        if (!m_healthCheckResponded.value(ip, false)) {
            updateCameraConnectionStatus(ip, "연결 실패", "red");
        }
        // 타이머 정리
        m_healthCheckTimers.remove(ip);
        timer->deleteLater();
    });

    m_healthCheckTimers[ip] = timer;
    timer->start(5000); // 5초
}

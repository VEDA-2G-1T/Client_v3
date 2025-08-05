#ifndef EVENTCARDPOPUPDIALOG_H
#define EVENTCARDPOPUPDIALOG_H

#include <QDialog>
#include <QDateTime>

// 클래스 전방 선언
class QLabel;
class QSlider;
class QPixmap;
class QNetworkAccessManager;
class QMouseEvent;
class QPoint;

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

private:
    void setupUi();
    void loadImage();
    void applyImageEnhancement();
    QWidget* createImageEnhancementWidget();
    QPixmap enhanceSharpness(const QPixmap &pixmap, int level);
    QPixmap enhanceCLAHE(const QPixmap &pixmap, int level);

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

#endif // EVENTCARDPOPUPDIALOG_H
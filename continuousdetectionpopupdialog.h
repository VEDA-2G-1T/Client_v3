#ifndef CONTINUOUSDETECTIONPOPUPDIALOG_H
#define CONTINUOUSDETECTIONPOPUPDIALOG_H

#include <QDialog>

// 클래스 전방 선언
class QLabel;
class QPixmap;
class QNetworkAccessManager;
class QMouseEvent;
class QPoint;

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

#endif // CONTINUOUSDETECTIONPOPUPDIALOG_H
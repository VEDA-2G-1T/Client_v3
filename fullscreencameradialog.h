#ifndef FULLSCREENCAMERADIALOG_H
#define FULLSCREENCAMERADIALOG_H

#include <QDialog>
#include <QMediaPlayer> // QMediaPlayer::Error enum을 위해 포함

// 클래스 전방 선언
class QMediaPlayer;
class QVideoWidget;
class QToolButton;
class QLabel;
class QMouseEvent;
class QPoint;

class FullscreenCameraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FullscreenCameraDialog(const QString &cameraName, const QString &rtspUrl, QWidget *parent = nullptr);
    ~FullscreenCameraDialog();

private slots:
    void onCloseButtonClicked();
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaError(QMediaPlayer::Error error, const QString &errorString);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupUI();
    void startStream();

    QString m_cameraName;
    QString m_rtspUrl;
    QMediaPlayer *m_mediaPlayer;
    QVideoWidget *m_videoWidget;
    QToolButton *m_closeButton;
    QLabel *m_statusLabel;
    QWidget *m_toolbar;
    bool m_isDragging = false;
    QPoint m_dragPosition;
};

#endif // FULLSCREENCAMERADIALOG_H
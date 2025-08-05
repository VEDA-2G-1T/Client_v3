#ifndef EVENTCARD_H
#define EVENTCARD_H

#include <QFrame>
#include <QDateTime>

// 클래스 전방 선언
class QToolButton;
class QLabel;
class QMouseEvent;

class EventCard : public QFrame
{
    Q_OBJECT

public:
    enum Mode { Event, Bookmark };

    EventCard(const QString& cameraName,
              const QString& eventText,
              const QDateTime& timestamp,
              const QString& imageUrl,
              Mode mode,
              QWidget *parent = nullptr);

    const QString& cameraName() const;
    const QString& eventText() const;
    const QString& imageUrl() const;
    const QDateTime& timestamp() const;
    Mode mode() const;

    void setStarChecked(bool checked);

signals:
    void starToggled(EventCard* card, bool checked);
    void removeRequested(EventCard* card);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_camera;
    QString m_event;
    QDateTime m_ts;
    QString m_imageUrl;
    Mode m_mode;

    QToolButton* m_actionButton;
    QToolButton* m_removeButton;
    QLabel* m_imageLabel;
};

#endif // EVENTCARD_H
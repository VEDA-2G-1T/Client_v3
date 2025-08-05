#ifndef DRAGDROPIMAGELABEL_H
#define DRAGDROPIMAGELABEL_H

#include <QLabel>
#include <QPixmap>

class QDragEnterEvent;
class QDropEvent;

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

#endif // DRAGDROPIMAGELABEL_H
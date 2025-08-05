#include "dragdropimagelabel.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QList>

DragDropImageLabel::DragDropImageLabel(QWidget *parent)
    : QLabel(parent)
{
    setAcceptDrops(true);
}

void DragDropImageLabel::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasImage() ||
        event->mimeData()->hasUrls() ||
        event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void DragDropImageLabel::dropEvent(QDropEvent *event)
{
    QPixmap pixmap;

    if (event->mimeData()->hasImage()) {
        QImage image = qvariant_cast<QImage>(event->mimeData()->imageData());
        pixmap = QPixmap::fromImage(image);
    }
    else if (event->mimeData()->hasUrls()) {
        QList<QUrl> urls = event->mimeData()->urls();
        if (!urls.isEmpty()) {
            QString filePath = urls.first().toLocalFile();
            if (!filePath.isEmpty()) {
                pixmap.load(filePath);
            }
        }
    }
    else if (event->mimeData()->hasFormat("text/uri-list")) {
        QString uriList = QString::fromUtf8(event->mimeData()->data("text/uri-list"));
        QStringList uris = uriList.split("\n", Qt::SkipEmptyParts);
        if (!uris.isEmpty()) {
            QUrl url(uris.first());
            QString filePath = url.toLocalFile();
            if (!filePath.isEmpty()) {
                pixmap.load(filePath);
            }
        }
    }

    if (!pixmap.isNull()) {
        emit imageDropped(pixmap);
    }
}
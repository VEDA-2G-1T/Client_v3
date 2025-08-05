#ifndef CAMERAREGISTRATIONDIALOG_H
#define CAMERAREGISTRATIONDIALOG_H

#include <QDialog>

// 클래스 전방 선언 (Forward Declarations)
class QLineEdit;
class QFrame;
class QMouseEvent;
class QPoint;

class CameraRegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CameraRegistrationDialog(QWidget *parent = nullptr);

    QString deviceName() const;
    QString ip() const;
    QString port() const;
    void setInitialData(const QString &name, const QString &ip, const QString &port);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 멤버 변수
    QLineEdit *nameEdit;
    QLineEdit *ipEdit;
    QLineEdit *portEdit;
    QFrame* titleBar;
    QPoint m_dragPosition;
    bool m_isDragging = false;

    // private 함수
    bool validateInput();

private slots:
    void onAcceptClicked();
};

#endif // CAMERAREGISTRATIONDIALOG_H
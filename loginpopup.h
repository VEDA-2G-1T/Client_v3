#ifndef LOGINPOPUP_H
#define LOGINPOPUP_H

#include <QFrame>

// 클래스 전방 선언
class QFocusEvent;

class LoginPopup : public QFrame
{
    Q_OBJECT

public:
    explicit LoginPopup(QWidget *parent = nullptr);

signals:
    void logoutRequested();

protected:
    void focusOutEvent(QFocusEvent *event) override;
};

#endif // LOGINPOPUP_H
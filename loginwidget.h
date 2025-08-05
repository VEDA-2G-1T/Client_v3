#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

// 클래스 전방 선언
class QLineEdit;

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccessful(); // 로그인 성공 시 발생시킬 신호

private slots:
    void attemptLogin();

private:
    QLineEdit *idLineEdit;
    QLineEdit *passwordLineEdit;
};

#endif // LOGINWIDGET_H
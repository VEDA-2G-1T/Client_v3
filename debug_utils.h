#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <QWidget>
#include <QString>

// 프로그램 전체에서 공유될 '디버그 테두리 표시' 상태 변수
// extern 키워드는 변수가 다른 소스 파일에 정의되어 있음을 알립니다.
extern bool G_SHOW_DEBUG_BORDERS;

// 디버그 테두리를 적용하는 함수 선언
void applyDebugBorder(QWidget *w, const QString &color);

#endif // DEBUG_UTILS_H
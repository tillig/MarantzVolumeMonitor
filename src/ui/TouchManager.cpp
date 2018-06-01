#include "TouchManager.h"

void TouchManager::begin() {
    _ts.begin();
    _ts.setRotation(1);
}

bool TouchManager::isTouched() {
    return _ts.touched();
}

TS_Point TouchManager::getPoint() {
    return _ts.getPoint();
}

#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <Arduino.h>

namespace DeviceInfo {
const char* firmwareVersion();
const char* resetReasonName();
} // namespace DeviceInfo

#endif

#include "MarantzClient.h"

namespace {
const char* const ModeTags[] = {
    "selectSurround",
    "SurrMode",
    "SurroundMode",
    "selectNOS"
};

const char* const MasterVolumeNestedTags[] = {
    "dispvalue",
    "DispValue",
    "displayvalue",
    "DisplayValue",
    "value",
    "Value"
};

constexpr float MinimumReceiverVolumeDb = -80.0f;

bool isMinimumVolumeSentinel(const String& value) {
    if (value.length() == 0) {
        return false;
    }

    bool sawDash = false;
    for (uint16_t i = 0; i < value.length(); ++i) {
        char current = value.charAt(i);
        if (current == '-') {
            sawDash = true;
            continue;
        }
        if (current == ' ') {
            continue;
        }
        return false;
    }

    return sawDash;
}

bool isNonNegativeZeroValue(const String& value) {
    if (value.length() == 0) {
        return false;
    }

    bool sawZeroDigit = false;
    for (uint16_t i = 0; i < value.length(); ++i) {
        char current = value.charAt(i);
        if (current == '0') {
            sawZeroDigit = true;
            continue;
        }
        if (current == '.' || current == '+') {
            continue;
        }
        if (current == '-') {
            return false;
        }
        return false;
    }

    return sawZeroDigit;
}

float parseReceiverVolume(const String& rawValue, bool* hasValue) {
    String value = rawValue;
    value.trim();
    if (value.length() == 0) {
        if (hasValue != nullptr) {
            *hasValue = false;
        }
        return MinimumReceiverVolumeDb;
    }

    if (isMinimumVolumeSentinel(value)) {
        if (hasValue != nullptr) {
            *hasValue = true;
        }
        return MinimumReceiverVolumeDb;
    }

    float parsedVolume = value.toFloat();
    bool isNumeric = value.indexOf('.') >= 0 || value.indexOf('-') >= 0 ||
                     value.indexOf('+') >= 0 || parsedVolume != 0.0f ||
                     isNonNegativeZeroValue(value);
    if (!isNumeric) {
        if (hasValue != nullptr) {
            *hasValue = false;
        }
        return MinimumReceiverVolumeDb;
    }

    if (parsedVolume == 0.0f && isNonNegativeZeroValue(value)) {
        parsedVolume = MinimumReceiverVolumeDb;
    }

    if (hasValue != nullptr) {
        *hasValue = true;
    }
    return parsedVolume;
}
}

void MarantzClient::setReceiverIp(const String& ip) {
    _receiverIp = ip;
}

MarantzStatus MarantzClient::getStatus() {
    return fetchStatus(_receiverIp);
}

bool MarantzClient::verifyReceiver(const String& ip, MarantzStatus* verifiedStatus) {
    MarantzStatus status = fetchStatus(ip);
    if (verifiedStatus != nullptr) {
        *verifiedStatus = status;
    }
    return status.isValid;
}

MarantzStatus MarantzClient::fetchStatus(const String& ip) {
    MarantzStatus status;
    if (ip == "") return status;

    WiFiClient client;
    HTTPClient http;

    String url = "http://" + ip + "/goform/formMainZone_MainZoneXml.xml";

    if (http.begin(client, url)) {
        http.setTimeout(5000);
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();

            String volStr = extractPreferredStatusValue(payload, "MasterVolume",
                                                        MasterVolumeNestedTags,
                                                        sizeof(MasterVolumeNestedTags) /
                                                            sizeof(MasterVolumeNestedTags[0]));
            status.volume = parseReceiverVolume(volStr, &status.hasVolume);

            String powerStr = extractStatusValue(payload, "Power");
            status.powerKnown = powerStr.length() > 0;
            status.power = (powerStr == "ON");
            if (!status.powerKnown && status.hasVolume) {
                status.powerKnown = true;
                status.power = true;
            }

            status.input = extractStatusValue(payload, "InputFuncSelect");
            status.mode = extractFirstStatusValue(payload, ModeTags,
                                                  sizeof(ModeTags) / sizeof(ModeTags[0]));
            status.isValid = true;
        }
        http.end();
    }

    return status;
}

String MarantzClient::extractValue(const String& xml, const String& tag) {
    String startTag = "<" + tag + ">";
    String endTag = "</" + tag + ">";

    int startIdx = xml.indexOf(startTag);
    if (startIdx == -1) return "";

    startIdx += startTag.length();
    int endIdx = xml.indexOf(endTag, startIdx);
    if (endIdx == -1) return "";

    String value = xml.substring(startIdx, endIdx);

    // Simple XML entity decoding if needed
    value.replace("&amp;", "&");

    return value;
}

String MarantzClient::extractStatusValue(const String& xml, const String& tag) {
    const char* const defaultNestedTags[] = {"value", "Value"};
    return extractPreferredStatusValue(xml, tag, defaultNestedTags,
                                       sizeof(defaultNestedTags) /
                                           sizeof(defaultNestedTags[0]));
}

String MarantzClient::extractPreferredStatusValue(const String& xml, const String& tag,
                                                  const char* const nestedTags[],
                                                  size_t nestedTagCount) {
    String value = extractValue(xml, tag);
    value.trim();

    if (value.indexOf('<') >= 0) {
        for (size_t i = 0; i < nestedTagCount; ++i) {
            String nestedValue = extractValue(value, nestedTags[i]);
            if (nestedValue.length() > 0) {
                value = nestedValue;
                value.trim();
                break;
            }
        }
    }

    value.replace("&amp;", "&");
    value.trim();
    return value;
}

String MarantzClient::extractFirstStatusValue(const String& xml, const char* const tags[],
                                              size_t tagCount) {
    for (size_t i = 0; i < tagCount; ++i) {
        String value = extractStatusValue(xml, tags[i]);
        if (value.length() > 0) {
            return value;
        }
    }
    return "";
}

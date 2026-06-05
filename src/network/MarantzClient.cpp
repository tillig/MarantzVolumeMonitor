#include "MarantzClient.h"

namespace {
const char* const ModeTags[] = {
    "selectSurround",
    "SurrMode",
    "SurroundMode",
    "selectNOS"
};
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

            String volStr = extractStatusValue(payload, "MasterVolume");
            if (volStr != "") {
                status.volume = volStr.toFloat();
                status.hasVolume = true;
            }

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
    String value = extractValue(xml, tag);
    value.trim();

    if (value.indexOf('<') >= 0) {
        String nestedValue = extractValue(value, "value");
        if (nestedValue.length() == 0) {
            nestedValue = extractValue(value, "Value");
        }
        if (nestedValue.length() > 0) {
            value = nestedValue;
            value.trim();
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

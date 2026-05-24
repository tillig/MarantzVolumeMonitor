#include "MarantzClient.h"

void MarantzClient::setReceiverIp(const String& ip) {
    _receiverIp = ip;
}

MarantzStatus MarantzClient::getStatus() {
    MarantzStatus status;
    if (_receiverIp == "") return status;

    WiFiClient client;
    HTTPClient http;

    String url = "http://" + _receiverIp + "/goform/formMainZone_MainZoneXml.xml";
    
    if (http.begin(client, url)) {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            
            String powerStr = extractValue(payload, "Power");
            status.power = (powerStr == "ON");
            
            String volStr = extractValue(payload, "MasterVolume");
            if (volStr != "") {
                status.volume = volStr.toFloat();
            }
            
            status.input = extractValue(payload, "InputFuncSelect");
            status.mode = extractValue(payload, "selectNOS");
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

#include "MaterialStyle.h"

#include "IconRenderer.h"

namespace {
constexpr uint16_t COLOR_SUCCESS_BG = 0x03E8; // dark green
constexpr uint16_t COLOR_ERROR_BG = 0x6800;   // dark red
constexpr uint16_t COLOR_WARNING_BG = 0x6300; // dark amber
constexpr uint16_t COLOR_DISABLED_BG = DisplayManager::COLOR_BAR_BG;

uint16_t buttonFillFor(MaterialStyle::ComponentState state) {
    switch (state) {
        case MaterialStyle::ComponentState::Success:
        case MaterialStyle::ComponentState::Selected:
            return COLOR_SUCCESS_BG;
        case MaterialStyle::ComponentState::Error:
            return COLOR_ERROR_BG;
        case MaterialStyle::ComponentState::Warning:
            return COLOR_WARNING_BG;
        case MaterialStyle::ComponentState::Disabled:
        case MaterialStyle::ComponentState::Unavailable:
            return COLOR_DISABLED_BG;
        case MaterialStyle::ComponentState::Focused:
        case MaterialStyle::ComponentState::Pressed:
            return DisplayManager::COLOR_ACCENT;
        case MaterialStyle::ComponentState::Loading:
        case MaterialStyle::ComponentState::Inactive:
        case MaterialStyle::ComponentState::Normal:
        default:
            return DisplayManager::COLOR_PANEL;
    }
}

void drawProgressSegment(TFT_eSPI& tft, int x, int y, int w, int h, uint8_t progressFrame) {
    tft.fillRoundRect(x, y, w, h, h / 2, DisplayManager::COLOR_BAR_BG);
    constexpr int segmentWidth = 68;
    int travel = w + segmentWidth;
    int segmentX = x - segmentWidth + ((progressFrame % 12) * travel) / 11;
    int clippedX = segmentX < x ? x : segmentX;
    int clippedRight = (segmentX + segmentWidth) > (x + w) ? (x + w) : (segmentX + segmentWidth);
    if (clippedRight > clippedX) {
        tft.fillRoundRect(clippedX, y, clippedRight - clippedX, h, h / 2, DisplayManager::COLOR_ACCENT);
    }
}

void drawWifiSignal(TFT_eSPI& tft, int centerX, int centerY, int level) {
    int x = centerX - 12;
    int baseY = centerY + 9;
    uint16_t active = level >= 3 ? DisplayManager::COLOR_ICON_ACTIVE
                      : level == 2 ? DisplayManager::COLOR_WARNING
                                   : DisplayManager::COLOR_ERROR;
    for (int i = 0; i < 3; ++i) {
        int h = 8 + (i * 5);
        uint16_t color = i < level ? active : DisplayManager::COLOR_BAR_BG;
        tft.fillRoundRect(x + (i * 8), baseY - h, 6, h, 3, color);
    }
}
}

namespace MaterialStyle {

uint8_t fontFor(TextRole role) {
    switch (role) {
        case TextRole::HomePrimaryValue:
            return 8;
        case TextRole::ScreenTitle:
        case TextRole::SectionLabel:
        case TextRole::StatusMessage:
            return 4;
        case TextRole::CompactMetadata:
            return 1;
        case TextRole::Body:
        case TextRole::ButtonLabel:
        case TextRole::ListPrimary:
        case TextRole::ListSecondary:
        default:
            return 2;
    }
}

uint16_t textColorFor(TextRole role, ComponentState state) {
    if (state == ComponentState::Disabled || state == ComponentState::Unavailable ||
        state == ComponentState::Inactive) {
        return DisplayManager::COLOR_TEXT_DIMMED;
    }
    if (state == ComponentState::Error) {
        return DisplayManager::COLOR_ERROR;
    }
    if (state == ComponentState::Warning) {
        return DisplayManager::COLOR_WARNING;
    }
    if (state == ComponentState::Success || state == ComponentState::Selected) {
        return DisplayManager::COLOR_ICON_ACTIVE;
    }

    switch (role) {
        case TextRole::ListSecondary:
        case TextRole::CompactMetadata:
            return DisplayManager::COLOR_TEXT_DIMMED;
        case TextRole::Body:
        case TextRole::SectionLabel:
            return DisplayManager::COLOR_TEXT_SECONDARY;
        case TextRole::HomePrimaryValue:
        case TextRole::ScreenTitle:
        case TextRole::ButtonLabel:
        case TextRole::ListPrimary:
        case TextRole::StatusMessage:
        default:
            return DisplayManager::COLOR_TEXT_PRIMARY;
    }
}

uint16_t surfaceColorFor(ComponentState state) {
    return buttonFillFor(state);
}

uint16_t outlineColorFor(ComponentState state) {
    switch (state) {
        case ComponentState::Focused:
        case ComponentState::Selected:
            return DisplayManager::COLOR_ACCENT;
        case ComponentState::Error:
            return DisplayManager::COLOR_ERROR;
        case ComponentState::Warning:
            return DisplayManager::COLOR_WARNING;
        case ComponentState::Success:
            return DisplayManager::COLOR_ICON_ACTIVE;
        case ComponentState::Disabled:
        case ComponentState::Unavailable:
            return DisplayManager::COLOR_TEXT_DIMMED;
        default:
            return DisplayManager::COLOR_BAR_BG;
    }
}

uint16_t stateColorFor(ComponentState state) {
    switch (state) {
        case ComponentState::Success:
        case ComponentState::Selected:
            return DisplayManager::COLOR_ICON_ACTIVE;
        case ComponentState::Warning:
            return DisplayManager::COLOR_WARNING;
        case ComponentState::Error:
            return DisplayManager::COLOR_ERROR;
        case ComponentState::Disabled:
        case ComponentState::Inactive:
        case ComponentState::Unavailable:
            return DisplayManager::COLOR_TEXT_DIMMED;
        case ComponentState::Focused:
        case ComponentState::Pressed:
        case ComponentState::Loading:
            return DisplayManager::COLOR_ACCENT;
        case ComponentState::Normal:
        default:
            return DisplayManager::COLOR_TEXT_PRIMARY;
    }
}

void drawText(TFT_eSPI& tft, const String& text, int x, int y, TextRole role,
              uint8_t datum, ComponentState state) {
    tft.setTextDatum(datum);
    tft.setTextColor(textColorFor(role, state), DisplayManager::COLOR_BACKGROUND);
    tft.drawString(text, x, y, fontFor(role));
}

String truncateToWidth(TFT_eSPI& tft, const String& text, int maxWidth, uint8_t font) {
    if (text.length() == 0 || tft.textWidth(text, font) <= maxWidth) {
        return text;
    }

    const String ellipsis = "...";
    int ellipsisWidth = tft.textWidth(ellipsis, font);
    if (ellipsisWidth >= maxWidth) {
        return ellipsis;
    }

    String out = text;
    while (out.length() > 0 && tft.textWidth(out, font) + ellipsisWidth > maxWidth) {
        out.remove(out.length() - 1);
    }
    return out + ellipsis;
}

void drawStandardButton(TFT_eSPI& tft, int x, int y, int w, int h,
                        const Icons::IconBitmap& icon, const char* label,
                        ComponentState state) {
    uint16_t fill = buttonFillFor(state);
    uint16_t outline = outlineColorFor(state);
    uint16_t foreground = (state == ComponentState::Disabled || state == ComponentState::Unavailable)
                              ? DisplayManager::COLOR_TEXT_DIMMED
                              : DisplayManager::COLOR_TEXT_PRIMARY;
    int radius = h / 2 < ButtonRadius ? h / 2 : ButtonRadius;

    tft.fillRoundRect(x, y, w, h, radius, fill);
    tft.drawRoundRect(x, y, w, h, radius, outline);

    String text = truncateToWidth(tft, String(label), w - icon.width - IconLabelGap - 18,
                                  fontFor(TextRole::ButtonLabel));
    int iconWidth = icon.width > StandardButtonIconMaxSize ? StandardButtonIconMaxSize : icon.width;
    int groupWidth = iconWidth + IconLabelGap + tft.textWidth(text, fontFor(TextRole::ButtonLabel));
    int horizontalInset = (w - groupWidth) / 2;
    if (horizontalInset < 8) {
        horizontalInset = 8;
    }
    int startX = x + horizontalInset;
    int centerY = y + h / 2;

    IconRenderer::drawFittedCentered(tft, icon, startX + iconWidth / 2, centerY,
                                     StandardButtonIconMaxSize, StandardButtonIconMaxSize,
                                     foreground);
    tft.setTextDatum(ML_DATUM);
    tft.setTextColor(foreground, fill);
    tft.drawString(text, startX + iconWidth + IconLabelGap, centerY, fontFor(TextRole::ButtonLabel));
}

void drawTextButton(TFT_eSPI& tft, int x, int y, int w, int h,
                    const char* label, ComponentState state) {
    uint16_t fill = buttonFillFor(state);
    uint16_t foreground = (state == ComponentState::Disabled || state == ComponentState::Unavailable)
                              ? DisplayManager::COLOR_TEXT_DIMMED
                              : DisplayManager::COLOR_TEXT_PRIMARY;
    String text = truncateToWidth(tft, String(label), w - 16, fontFor(TextRole::ButtonLabel));
    int radius = h / 2 < ButtonRadius ? h / 2 : ButtonRadius;
    tft.fillRoundRect(x, y, w, h, radius, fill);
    tft.drawRoundRect(x, y, w, h, radius, outlineColorFor(state));
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(foreground, fill);
    tft.drawString(text, x + w / 2, y + h / 2, fontFor(TextRole::ButtonLabel));
}

void drawKeyboardIconButton(TFT_eSPI& tft, int x, int y, int w, int h,
                            const Icons::IconBitmap& icon, ComponentState state,
                            int iconYOffset) {
    uint16_t fill = buttonFillFor(state);
    uint16_t foreground = (state == ComponentState::Selected || state == ComponentState::Focused)
                              ? DisplayManager::COLOR_TEXT_DARK
                              : DisplayManager::COLOR_TEXT_PRIMARY;
    if (state == ComponentState::Error || state == ComponentState::Success) {
        foreground = DisplayManager::COLOR_TEXT_PRIMARY;
    }
    tft.fillRoundRect(x, y, w, h, KeyboardButtonRadius, fill);
    tft.drawRoundRect(x, y, w, h, KeyboardButtonRadius, outlineColorFor(state));
    IconRenderer::drawCentered(tft, icon, x + w / 2, y + h / 2 + iconYOffset, foreground);
}

void drawKeyboardTextButton(TFT_eSPI& tft, int x, int y, int w, int h,
                            const String& label, ComponentState state) {
    uint16_t fill = buttonFillFor(state);
    uint16_t foreground = (state == ComponentState::Selected || state == ComponentState::Focused)
                              ? DisplayManager::COLOR_TEXT_DARK
                              : DisplayManager::COLOR_TEXT_PRIMARY;
    String text = truncateToWidth(tft, label, w - 10, fontFor(TextRole::ButtonLabel));
    tft.fillRoundRect(x, y, w, h, KeyboardButtonRadius, fill);
    tft.drawRoundRect(x, y, w, h, KeyboardButtonRadius, outlineColorFor(state));
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(foreground, fill);
    tft.drawString(text, x + w / 2, y + h / 2, fontFor(TextRole::ButtonLabel));
}

void drawListRow(TFT_eSPI& tft, const ListRowSpec& spec) {
    uint16_t fill = spec.state == ComponentState::Selected ? DisplayManager::COLOR_BAR_BG
                                                           : DisplayManager::COLOR_PANEL;
    tft.fillRoundRect(spec.x, spec.y, spec.w, spec.h, ListRowRadius, fill);
    tft.drawRoundRect(spec.x, spec.y, spec.w, spec.h, ListRowRadius, outlineColorFor(spec.state));

    int textX = spec.x + 16;
    if (spec.wifiSignalLevel > 0) {
        drawWifiSignal(tft, spec.x + 24, spec.y + spec.h / 2, spec.wifiSignalLevel);
        textX = spec.x + 48;
    } else if (spec.leadingIcon != nullptr) {
        IconRenderer::drawFittedCentered(tft, *spec.leadingIcon, spec.x + 24, spec.y + spec.h / 2,
                                         ListRowIconMaxSize, ListRowIconMaxSize,
                                         DisplayManager::COLOR_TEXT_SECONDARY);
        textX = spec.x + 48;
    }

    int trailingWidth = spec.trailing.length() > 0 ? 44 : 12;
    int textWidth = spec.w - (textX - spec.x) - trailingWidth - 12;
    String primary = truncateToWidth(tft, spec.primary, textWidth, fontFor(TextRole::ListPrimary));
    String secondary = truncateToWidth(tft, spec.secondary, textWidth, fontFor(TextRole::ListSecondary));

    tft.setTextDatum(ML_DATUM);
    tft.setTextColor(textColorFor(TextRole::ListPrimary, spec.state), fill);
    int primaryY = spec.secondary.length() > 0 ? spec.y + 16 : spec.y + spec.h / 2;
    tft.drawString(primary, textX, primaryY, fontFor(TextRole::ListPrimary));

    if (spec.secondary.length() > 0) {
        tft.setTextColor(textColorFor(TextRole::ListSecondary, spec.state), fill);
        tft.drawString(secondary, textX, spec.y + 34, fontFor(TextRole::ListSecondary));
    }

    if (spec.trailing.length() > 0) {
        tft.setTextDatum(MR_DATUM);
        tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED, fill);
        tft.drawString(spec.trailing, spec.x + spec.w - 16, spec.y + spec.h / 2,
                       fontFor(TextRole::CompactMetadata));
    }
}

void drawChoiceRow(TFT_eSPI& tft, const ChoiceRowSpec& spec) {
    uint16_t fill = spec.selected ? DisplayManager::COLOR_BAR_BG
                                  : DisplayManager::COLOR_PANEL;
    ComponentState outlineState = spec.selected ? ComponentState::Selected : spec.state;
    tft.fillRoundRect(spec.x, spec.y, spec.w, spec.h, ListRowRadius, fill);
    tft.drawRoundRect(spec.x, spec.y, spec.w, spec.h, ListRowRadius,
                      outlineColorFor(outlineState));

    int indicatorCenterX = spec.x + 24;
    int indicatorCenterY = spec.y + (spec.h / 2);
    uint16_t indicatorColor = spec.selected ? DisplayManager::COLOR_ACCENT
                                            : DisplayManager::COLOR_TEXT_DIMMED;
    tft.drawCircle(indicatorCenterX, indicatorCenterY, 10, indicatorColor);
    tft.drawCircle(indicatorCenterX, indicatorCenterY, 9, indicatorColor);
    if (spec.selected) {
        tft.fillCircle(indicatorCenterX, indicatorCenterY, 5, indicatorColor);
    }

    int textX = spec.x + 48;
    int trailingInset = 16;
    int textWidth = spec.w - (textX - spec.x) - trailingInset;
    String primary = truncateToWidth(tft, spec.primary, textWidth,
                                     fontFor(TextRole::ListPrimary));
    String secondary = truncateToWidth(tft, spec.secondary, textWidth,
                                       fontFor(TextRole::ListSecondary));

    tft.setTextDatum(ML_DATUM);
    tft.setTextColor(textColorFor(TextRole::ListPrimary, ComponentState::Normal), fill);
    int primaryY = spec.secondary.length() > 0 ? spec.y + 16 : spec.y + (spec.h / 2);
    tft.drawString(primary, textX, primaryY, fontFor(TextRole::ListPrimary));

    if (spec.secondary.length() > 0) {
        tft.setTextColor(textColorFor(TextRole::ListSecondary, spec.state), fill);
        tft.drawString(secondary, textX, spec.y + 34, fontFor(TextRole::ListSecondary));
    }
}

void drawPageHeader(TFT_eSPI& tft, const Icons::IconBitmap& icon,
                    const String& title, const String& subtitle) {
    IconRenderer::drawFittedCentered(tft, icon, PageHeaderIconCenterX, PageHeaderIconCenterY,
                                     HeaderIconMaxSize, HeaderIconMaxSize,
                                     DisplayManager::COLOR_TEXT_SECONDARY);
    drawText(tft, title, PageHeaderTitleX, SetupHeaderTitleY, TextRole::ScreenTitle, TL_DATUM);
    drawText(tft, subtitle, PageHeaderTitleX, SetupHeaderSubtitleY, TextRole::Body, TL_DATUM);
}

void drawSetupHeader(TFT_eSPI& tft, const String& title, const String& subtitle) {
    drawText(tft, title, ScreenWidth / 2, SetupHeaderTitleY, TextRole::ScreenTitle, TC_DATUM);
    drawText(tft, subtitle, ScreenWidth / 2, SetupHeaderSubtitleY, TextRole::Body, TC_DATUM);
}

void drawSearchingState(TFT_eSPI& tft, const String& title, const String& message,
                        const Icons::IconBitmap& icon, uint8_t progressFrame) {
    drawSetupHeader(tft, title, message);
    IconRenderer::drawCentered(tft, icon, ScreenWidth / 2, 104, DisplayManager::COLOR_TEXT_SECONDARY);
    drawProgressBar(tft, SearchingProgressX, SearchingProgressY, SearchingProgressW, progressFrame);
}

void drawStatusBlock(TFT_eSPI& tft, StatusKind kind, const String& title,
                     const String& message, const Icons::IconBitmap& icon,
                     uint8_t progressFrame) {
    ComponentState state = ComponentState::Normal;
    switch (kind) {
        case StatusKind::Loading:
            state = ComponentState::Loading;
            break;
        case StatusKind::Success:
            state = ComponentState::Success;
            break;
        case StatusKind::Warning:
            state = ComponentState::Warning;
            break;
        case StatusKind::Error:
            state = ComponentState::Error;
            break;
        case StatusKind::Unavailable:
            state = ComponentState::Unavailable;
            break;
        case StatusKind::Empty:
        default:
            state = ComponentState::Inactive;
            break;
    }

    IconRenderer::drawCentered(tft, icon, ScreenWidth / 2, 112, stateColorFor(state));
    drawText(tft, title, ScreenWidth / 2, 154, TextRole::StatusMessage, TC_DATUM, state);
    if (message.length() > 0) {
        drawText(tft, message, ScreenWidth / 2, 194, TextRole::Body, TC_DATUM, state);
    }
    if (kind == StatusKind::Loading) {
        drawProgressBar(tft, StatusBlockProgressX, StatusBlockProgressY,
                        StatusBlockProgressW, progressFrame);
    }
}

void drawInfoCard(TFT_eSPI& tft, int x, int y, int w, int h,
                  const Icons::IconBitmap& icon, const String& title) {
    tft.fillRoundRect(x, y, w, h, ListRowRadius, DisplayManager::COLOR_PANEL);
    tft.drawRoundRect(x, y, w, h, ListRowRadius, DisplayManager::COLOR_BAR_BG);
    IconRenderer::drawCentered(tft, icon, x + 18, y + 16, DisplayManager::COLOR_TEXT_SECONDARY);
    drawText(tft, title, x + 36, y + 6, TextRole::SectionLabel, TL_DATUM);
}

void drawStatusRow(TFT_eSPI& tft, const StatusRowSpec& spec) {
    if (spec.wifiSignalLevel > 0) {
        drawText(tft, spec.label, spec.x + 12, spec.y + 2, TextRole::CompactMetadata, TL_DATUM);
        int metadataWidth = spec.metadata.length() > 0
                                ? tft.textWidth(spec.metadata, fontFor(TextRole::Body))
                                : 0;
        int metadataRight = spec.x + spec.w - 12;
        if (spec.metadata.length() > 0) {
            tft.setTextDatum(MR_DATUM);
            tft.setTextColor(textColorFor(TextRole::Body, spec.state), DisplayManager::COLOR_PANEL);
            tft.drawString(spec.metadata, metadataRight, spec.y + spec.h / 2 + 2,
                           fontFor(TextRole::Body));
        }
        int wifiCenterX = metadataRight - metadataWidth - 18;
        drawWifiSignal(tft, wifiCenterX, spec.y + spec.h / 2 + 1, spec.wifiSignalLevel);
    } else {
        drawText(tft, spec.label, spec.x + 12, spec.y + 2, TextRole::CompactMetadata, TL_DATUM);
        int labelWidth = tft.textWidth(spec.label, fontFor(TextRole::CompactMetadata));
        int valueMaxWidth = spec.w - labelWidth - 36;
        String value = truncateToWidth(tft, spec.value, valueMaxWidth, fontFor(TextRole::Body));
        tft.setTextDatum(MR_DATUM);
        tft.setTextColor(textColorFor(TextRole::Body, spec.state), DisplayManager::COLOR_PANEL);
        tft.drawString(value, spec.x + spec.w - 12, spec.y + spec.h / 2 + 2,
                       fontFor(TextRole::Body));
    }

    tft.drawFastHLine(spec.x + 12, spec.y + spec.h - 1, spec.w - 24, DisplayManager::COLOR_BAR_BG);
}

void drawPagination(TFT_eSPI& tft, int currentPage, int totalPages,
                    bool showPrev, bool showNext, int y) {
    if (totalPages <= 1) {
        return;
    }

    String pageInfo = "Page " + String(currentPage + 1) + " of " + String(totalPages);
    drawText(tft, pageInfo, ScreenWidth / 2, y + 16, TextRole::CompactMetadata, MC_DATUM);

    if (showPrev) {
        drawTextButton(tft, 20, y, 96, 32, "PREV");
    }
    if (showNext) {
        drawTextButton(tft, 364, y, 96, 32, "NEXT");
    }
}

void clearProgressBar(TFT_eSPI& tft, int x, int y, int w) {
    tft.fillRect(x - 2, y - 2, w + 4, 12, DisplayManager::COLOR_BACKGROUND);
}

void drawInputField(TFT_eSPI& tft, int x, int y, int w, int h, const String& value,
                    TextRole role) {
    tft.fillRoundRect(x, y, w, h, InputRadius, DisplayManager::COLOR_PANEL);
    tft.drawRoundRect(x, y, w, h, InputRadius, DisplayManager::COLOR_ACCENT);
    String text = truncateToWidth(tft, value, w - 20, fontFor(role));
    tft.setTextDatum(ML_DATUM);
    tft.setTextColor(textColorFor(role), DisplayManager::COLOR_PANEL);
    tft.drawString(text, x + 10, y + h / 2, fontFor(role));
}

void drawProgressBar(TFT_eSPI& tft, int x, int y, int w, uint8_t progressFrame) {
    drawProgressSegment(tft, x, y, w, 8, progressFrame);
}

} // namespace MaterialStyle

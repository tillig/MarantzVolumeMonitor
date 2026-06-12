#ifndef MATERIAL_STYLE_H
#define MATERIAL_STYLE_H

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "DisplayManager.h"
#include "assets/IconBitmaps.h"

namespace MaterialStyle {

constexpr int ScreenMargin = 20;
constexpr int ScreenWidth = 480;
constexpr int ScreenHeight = 320;
constexpr int ButtonHeight = 40;
constexpr int ButtonRadius = 20;
constexpr int BottomActionY = 270;
constexpr int InputRadius = 12;
constexpr int SearchingProgressX = 150;
constexpr int SearchingProgressY = 186;
constexpr int SearchingProgressW = 180;
constexpr int StatusBlockProgressX = 150;
constexpr int StatusBlockProgressY = 210;
constexpr int StatusBlockProgressW = 180;
constexpr int SetupHeaderTitleY = 14;
constexpr int SetupHeaderSubtitleY = 46;
constexpr int PageHeaderIconCenterX = 32;
constexpr int PageHeaderIconCenterY = 28;
constexpr int PageHeaderTitleX = 56;
constexpr int HeaderIconMaxSize = 24;
constexpr int ListRowIconMaxSize = 20;
constexpr int StandardButtonIconMaxSize = 20;
constexpr int SetupListTopY = 72;
constexpr int KeyboardButtonRadius = 6;
constexpr int ListRowHeight = 46;
constexpr int ListRowRadius = 8;
constexpr int IconLabelGap = 8;
constexpr int RowGap = 9;
constexpr int ButtonGap = 12;
constexpr int KeyboardKeyGap = 6;
constexpr int KeyboardKeyHeight = 40;
constexpr int MinTouchTarget = 40;
constexpr uint32_t ProgressThresholdMs = 500;
constexpr uint32_t ProgressFrameMs = 180;

enum class TextRole {
    HomePrimaryValue,
    ScreenTitle,
    SectionLabel,
    Body,
    ButtonLabel,
    ListPrimary,
    ListSecondary,
    StatusMessage,
    CompactMetadata
};

enum class ButtonVariant {
    StandardIconText,
    KeyboardIconOnly,
    DocumentedException
};

enum class ComponentState {
    Normal,
    Pressed,
    Selected,
    Focused,
    Disabled,
    Loading,
    Success,
    Warning,
    Error,
    Inactive,
    Unavailable
};

enum class StatusKind {
    Loading,
    Success,
    Warning,
    Error,
    Empty,
    Unavailable
};

struct ButtonSpec {
    int x;
    int y;
    int w;
    int h;
    const Icons::IconBitmap* icon;
    const char* label;
    ComponentState state;
};

struct ListRowSpec {
    int x;
    int y;
    int w;
    int h;
    const Icons::IconBitmap* leadingIcon;
    int wifiSignalLevel;
    String primary;
    String secondary;
    String trailing;
    ComponentState state;
};

struct ChoiceRowSpec {
    int x;
    int y;
    int w;
    int h;
    String primary;
    String secondary;
    bool selected;
    ComponentState state;
};

struct StatusRowSpec {
    int x;
    int y;
    int w;
    int h;
    String label;
    String value;
    ComponentState state;
    int wifiSignalLevel;
    String metadata;
};

uint8_t fontFor(TextRole role);
uint16_t textColorFor(TextRole role, ComponentState state = ComponentState::Normal);
uint16_t surfaceColorFor(ComponentState state = ComponentState::Normal);
uint16_t outlineColorFor(ComponentState state = ComponentState::Normal);
uint16_t stateColorFor(ComponentState state);

void drawText(TFT_eSPI& tft, const String& text, int x, int y, TextRole role,
              uint8_t datum = MC_DATUM,
              ComponentState state = ComponentState::Normal);
String truncateToWidth(TFT_eSPI& tft, const String& text, int maxWidth, uint8_t font);

void drawStandardButton(TFT_eSPI& tft, int x, int y, int w, int h,
                        const Icons::IconBitmap& icon, const char* label,
                        ComponentState state = ComponentState::Normal);
void drawTextButton(TFT_eSPI& tft, int x, int y, int w, int h,
                    const char* label,
                    ComponentState state = ComponentState::Normal);
void drawKeyboardIconButton(TFT_eSPI& tft, int x, int y, int w, int h,
                            const Icons::IconBitmap& icon,
                            ComponentState state = ComponentState::Normal,
                            int iconYOffset = 0);
void drawKeyboardTextButton(TFT_eSPI& tft, int x, int y, int w, int h,
                            const String& label,
                            ComponentState state = ComponentState::Normal);
void drawListRow(TFT_eSPI& tft, const ListRowSpec& spec);
void drawChoiceRow(TFT_eSPI& tft, const ChoiceRowSpec& spec);
void drawPageHeader(TFT_eSPI& tft, const Icons::IconBitmap& icon,
                    const String& title, const String& subtitle);
void drawSetupHeader(TFT_eSPI& tft, const String& title, const String& subtitle);
void drawSearchingState(TFT_eSPI& tft, const String& title, const String& message,
                        const Icons::IconBitmap& icon, uint8_t progressFrame);
void drawStatusBlock(TFT_eSPI& tft, StatusKind kind, const String& title,
                     const String& message, const Icons::IconBitmap& icon,
                     uint8_t progressFrame = 0);
void drawInfoCard(TFT_eSPI& tft, int x, int y, int w, int h,
                  const Icons::IconBitmap& icon, const String& title);
void drawStatusRow(TFT_eSPI& tft, const StatusRowSpec& spec);
void drawPagination(TFT_eSPI& tft, int currentPage, int totalPages,
                    bool showPrev, bool showNext, int y = 230);
void clearProgressBar(TFT_eSPI& tft, int x, int y, int w);
void drawInputField(TFT_eSPI& tft, int x, int y, int w, int h, const String& value,
                    TextRole role = TextRole::Body);
void drawProgressBar(TFT_eSPI& tft, int x, int y, int w, uint8_t progressFrame);

} // namespace MaterialStyle

#endif

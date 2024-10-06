#pragma once
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <TGUI/TGUI.hpp>
#include <yoga/Yoga.h>

class DomNode {
public:
  virtual void ApplyLayout(float parentLeft, float parentTop) = 0;
  virtual void AddToGui(tgui::Gui &gui) = 0;
  virtual ~DomNode() {}
  YGNodeRef m_yogaNode;

  // Shortened name 'c' for creating children
  template <typename... T>
  static std::vector<std::unique_ptr<DomNode>> c(T &&...nodes) {
    std::vector<std::unique_ptr<DomNode>> children;
    (children.emplace_back(std::forward<T>(nodes)), ...);
    return children;
  }
};

enum YGStyleProperty {
  FLEX,
  FLEX_ROW,
  FLEX_COL,
  GAP_1,
  GAP_2,
  GAP_3,
  GAP_4,
  GAP_5,
  GAP_6,
  GAP_7,
  GAP_8,
  GAP_9,
  GAP_10,
  ITEMS_STRETCH,
  ITEMS_START,
  ITEMS_CENTER,
  ITEMS_END,
  ITEMS_BASELINE,
  JUSTIFY_START,
  JUSTIFY_CENTER,
  JUSTIFY_END,
  JUSTIFY_BETWEEN,
  JUSTIFY_AROUND,
  JUSTIFY_EVENLY,
  GROW,
  W_FULL,
  W_0,
  W_0_5,
  W_1,
  W_1_5,
  W_2,
  W_2_5,
  W_3,
  W_3_5,
  W_4,
  W_5,
  W_6,
  W_7,
  W_8,
  W_9,
  W_10,
  W_11,
  W_12,
  W_14,
  W_16,
  W_20,
  W_24,
  W_28,
  W_32,
  W_36,
  W_40,
  W_44,
  W_48,
  W_52,
  W_56,
  W_60,
  W_64,
  W_72,
  W_80,
  W_96,
  W_AUTO,
  H_FULL,
  H_0,
  H_1,
  H_0_5,
  H_1_5,
  H_2,
  H_2_5,
  H_3,
  H_3_5,
  H_4,
  H_5,
  H_6,
  H_7,
  H_8,
  H_9,
  H_10,
  H_11,
  H_12,
  H_14,
  H_16,
  H_20,
  H_24,
  H_28,
  H_32,
  H_36,
  H_40,
  H_44,
  H_48,
  H_52,
  H_56,
  H_60,
  H_64,
  H_72,
  H_80,
  H_96,
  H_AUTO,
  TEXT_XS,
  TEXT_SM,
  TEXT_BASE,
  TEXT_LG,
  TEXT_XL,
  TEXT_2XL,
  TEXT_3XL,
  TEXT_4XL,
  TEXT_5XL,
  TEXT_6XL,
  TEXT_7XL,
  TEXT_8XL,
  TEXT_9XL,
  BG_BLACK,
  BG_BLUE_500,
  BG_BLUE_400,
  BG_WHITE,
  BG_SLATE_950,
  TEXT_WHITE,
  TEXT_BLACK,
  P_1,
  P_2,
  P_3,
  P_4,
  P_5,
  P_6,
  P_8,
  P_10,
  P_12,
  P_16,
  P_20
};

inline std::vector<YGStyleProperty> SX() {
  return {}; // Empty vector for no styles
}

template <typename... Styles>
inline std::vector<YGStyleProperty> SX(Styles... styles) {
  return {styles...}; // Vector with styles passed
}

inline void ApplyStyles(const std::vector<YGStyleProperty> &styles,
                        tgui::WidgetRenderer *widget) {
  // clang-format off
  static const std::unordered_map<YGStyleProperty,std::function<void(tgui::WidgetRenderer *)>> styleMap = {
    {TEXT_XS,     [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * .75f); }},
    {TEXT_SM,     [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * .875f); }},
    {TEXT_BASE,   [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 1.0f); }},
    {TEXT_LG,     [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 1.125f); }},
    {TEXT_XL,     [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 1.25f); }},
    {TEXT_2XL,    [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 1.25f); }},
    {TEXT_3XL,    [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 1.5f); }},
    {TEXT_4XL,    [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 1.875f); }},
    {TEXT_5XL,    [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 2.25f); }},
    {TEXT_6XL,    [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 3.0f); }},
    {TEXT_7XL,    [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 3.75f); }},
    {TEXT_8XL,    [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 4.5f); }},
    {TEXT_9XL,    [](tgui::WidgetRenderer* renderer) { renderer->setTextSize(16.0f * 6.0f); }},
  };
  // clang-format on
  for (const auto &style : styles) {
    auto it = styleMap.find(style);
    if (it != styleMap.end()) {
      it->second(widget);
    }
  }
}

inline void ApplyStyles(const std::vector<YGStyleProperty> &styles,
                        YGNodeRef node) {
  static const std::unordered_map<YGStyleProperty,
                                  std::function<void(YGNodeRef)>>
      // clang-format off
      styleMap = {
        {FLEX, [](YGNodeRef node) { YGNodeStyleSetFlex(node, 1.0f); }},
        {FLEX_ROW, [](YGNodeRef node) { YGNodeStyleSetFlexDirection(node, YGFlexDirectionRow); }},
        {FLEX_COL, [](YGNodeRef node) { YGNodeStyleSetFlexDirection(node, YGFlexDirectionColumn); }},
        {GAP_1, [](YGNodeRef node) { YGNodeStyleSetGap(node, YGGutterAll, 4.0f); }},
        {GAP_2, [](YGNodeRef node) { YGNodeStyleSetGap(node, YGGutterAll, 8.0f); }},
        {GAP_3, [](YGNodeRef node) { YGNodeStyleSetGap(node, YGGutterAll, 12.0f); }},
        {GAP_4, [](YGNodeRef node) { YGNodeStyleSetGap(node, YGGutterAll, 16.0f); }},
        {GAP_5, [](YGNodeRef node) { YGNodeStyleSetGap(node, YGGutterAll, 20.0f); }},
        {GAP_6, [](YGNodeRef node) { YGNodeStyleSetGap(node, YGGutterAll, 24.0f); }},
        {GAP_8, [](YGNodeRef node) { YGNodeStyleSetGap(node, YGGutterAll, 32.0f); }},
        {GAP_10, [](YGNodeRef node) { YGNodeStyleSetGap(node, YGGutterAll, 40.0f); }},
        {GROW, [](YGNodeRef node) { YGNodeStyleSetFlexGrow(node, 1.0f); }},
        {ITEMS_STRETCH, [](YGNodeRef node) { YGNodeStyleSetAlignItems(node, YGAlignStretch); }},
        {ITEMS_START, [](YGNodeRef node) { YGNodeStyleSetAlignItems(node, YGAlignFlexStart); }},
        {ITEMS_CENTER, [](YGNodeRef node) { YGNodeStyleSetAlignItems(node, YGAlignCenter); }},
        {ITEMS_END, [](YGNodeRef node) { YGNodeStyleSetAlignItems(node, YGAlignFlexEnd); }},
        {ITEMS_BASELINE, [](YGNodeRef node) { YGNodeStyleSetAlignItems(node, YGAlignBaseline); }},
        {JUSTIFY_START, [](YGNodeRef node) { YGNodeStyleSetJustifyContent(node, YGJustifyFlexStart); }},
        {JUSTIFY_CENTER, [](YGNodeRef node) { YGNodeStyleSetJustifyContent(node, YGJustifyCenter); }},
        {JUSTIFY_END, [](YGNodeRef node) { YGNodeStyleSetJustifyContent(node, YGJustifyFlexEnd); }},
        {JUSTIFY_BETWEEN, [](YGNodeRef node) { YGNodeStyleSetJustifyContent(node, YGJustifySpaceAround); }},
        {JUSTIFY_AROUND, [](YGNodeRef node) { YGNodeStyleSetJustifyContent(node, YGJustifySpaceBetween); }},
        {JUSTIFY_EVENLY, [](YGNodeRef node) { YGNodeStyleSetHeightPercent(node, 100.0f); }},
        {H_FULL, [](YGNodeRef node) { YGNodeStyleSetHeightPercent(node, 100.0f); }},
        {H_0, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 0.0f); }},
        {H_0_5, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 2.0f); }},
        {H_1, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 4.0f); }},
        {H_1_5, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 6.0f); }},
        {H_2, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 8.0f); }},
        {H_2_5, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 10.0f); }},
        {H_3, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 12.0f); }},
        {H_3_5, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 14.0f); }},
        {H_4, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 16.0f); }},
        {H_5, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 20.0f); }},
        {H_6, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 24.0f); }},
        {H_7, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 28.0f); }},
        {H_8, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 32.0f); }},
        {H_9, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 36.0f); }},
        {H_10, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 40.0f); }},
        {H_11, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 44.0f); }},
        {H_12, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 48.0f); }},
        {H_14, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 56.0f); }},
        {H_16, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 64.0f); }},
        {H_20, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 80.0f); }},
        {H_24, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 96.0f); }},
        {H_28, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 112.0f); }},
        {H_32, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 128.0f); }},
        {H_36, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 144.0f); }},
        {H_40, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 160.0f); }},
        {H_44, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 176.0f); }},
        {H_48, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 192.0f); }},
        {H_52, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 208.0f); }},
        {H_56, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 224.0f); }},
        {H_60, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 240.0f); }},
        {H_64, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 256.0f); }},
        {H_72, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 288.0f); }},
        {H_80, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 320.0f); }},
        {H_96, [](YGNodeRef node) { YGNodeStyleSetHeight(node, 384.0f); }},
        {H_AUTO, [](YGNodeRef node) { YGNodeStyleSetHeightAuto(node); }},
        {W_0, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 0.0f); }},
        {W_0_5, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 2.0f); }},
        {W_1, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 4.0f); }},
        {W_1_5, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 6.0f); }},
        {W_2, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 8.0f); }},
        {W_2_5, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 10.0f); }},
        {W_3, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 12.0f); }},
        {W_3_5, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 14.0f); }},
        {W_4, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 16.0f); }},
        {W_5, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 20.0f); }},
        {W_6, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 24.0f); }},
        {W_7, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 28.0f); }},
        {W_8, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 32.0f); }},
        {W_9, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 36.0f); }},
        {W_10, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 40.0f); }},
        {W_11, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 44.0f); }},
        {W_12, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 48.0f); }},
        {W_14, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 56.0f); }},
        {W_16, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 64.0f); }},
        {W_20, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 80.0f); }},
        {W_24, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 96.0f); }},
        {W_28, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 112.0f); }},
        {W_32, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 128.0f); }},
        {W_36, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 144.0f); }},
        {W_40, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 160.0f); }},
        {W_44, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 176.0f); }},
        {W_48, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 192.0f); }},
        {W_52, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 208.0f); }},
        {W_56, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 224.0f); }},
        {W_60, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 240.0f); }},
        {W_64, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 256.0f); }},
        {W_72, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 288.0f); }},
        {W_80, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 320.0f); }},
        {W_96, [](YGNodeRef node) { YGNodeStyleSetWidth(node, 384.0f); }},
        {W_AUTO, [](YGNodeRef node) { YGNodeStyleSetWidthAuto(node); }},
        {W_FULL, [](YGNodeRef node) { YGNodeStyleSetWidthPercent(node, 100.0f); }},
        {P_1, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 4.0f); }},
        {P_2, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 8.0f); }},
        {P_3, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 12.0f); }},
        {P_4, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 16.0f); }},
        {P_5, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 20.0f); }},
        {P_6, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 24.0f); }},
        {P_8, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 32.0f); }},
        {P_10, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 40.0f); }},
        {P_12, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 48.0f); }},
        {P_16, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 64.0f); }},
        {P_20, [](YGNodeRef node) { YGNodeStyleSetPadding(node, YGEdgeAll, 80.0f); }}
  };
  // clang-format on
  for (const auto &style : styles) {
    auto it = styleMap.find(style);
    if (it != styleMap.end()) {
      it->second(node);
    }
  }
}

struct PositionStruct {
  float left;
  float top;
  float width;
  float height;
  PositionStruct(float l, float t, float w, float h)
      : left(l), top(t), width(w), height(h) {}
};
// Placement debugging
void LogNode(const PositionStruct &pos) {
  std::cout << "Left: " << pos.left << std::endl;
  std::cout << "Top: " << pos.top << std::endl;
  std::cout << "Width: " << pos.width << std::endl;
  std::cout << "Height: " << pos.height << std::endl;
  std::cout << std::endl;
}
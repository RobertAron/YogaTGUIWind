#pragma once
#include <yoga/Yoga.h>
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <TGUI/TGUI.hpp>

class DomNode
{
public:
  virtual void ApplyLayout(tgui::Gui &gui) = 0;
  virtual ~DomNode() {}
  YGNodeRef m_yogaNode;

  // Shortened name 'c' for creating children
  template <typename... T>
  static std::vector<std::unique_ptr<DomNode>> c(T &&...nodes)
  {
    std::vector<std::unique_ptr<DomNode>> children;
    (children.emplace_back(std::forward<T>(nodes)), ...);
    return children;
  }
};

enum YGStyleProperty
{
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
  W_FULL,
  H_FULL,
  H_1,
  GROW
};

inline std::vector<YGStyleProperty> SX()
{
  return {}; // Empty vector for no styles
}

template <typename... Styles>
inline std::vector<YGStyleProperty> SX(Styles... styles)
{
  return {styles...}; // Vector with styles passed
}

inline void ApplyStyles(const std::vector<YGStyleProperty> &styles, YGNodeRef node)
{
  static const std::unordered_map<YGStyleProperty, std::function<void(YGNodeRef)>> styleMap = {
      {FLEX, [](YGNodeRef node)
       { YGNodeStyleSetFlex(node, 1.0f); }},
      {FLEX_ROW, [](YGNodeRef node)
       { YGNodeStyleSetFlexDirection(node, YGFlexDirectionRow); }},
      {FLEX_COL, [](YGNodeRef node)
       { YGNodeStyleSetFlexDirection(node, YGFlexDirectionColumn); }},
      {GAP_1, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 4.0f); }},
      {GAP_2, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 8.0f); }},
      {GAP_3, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 12.0f); }},
      {GAP_4, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 16.0f); }},
      {GAP_5, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 20.0f); }},
      {GAP_6, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 24.0f); }},
      {GAP_8, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 32.0f); }},
      {GAP_10, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 40.0f); }},
      {ITEMS_STRETCH, [](YGNodeRef node)
       { YGNodeStyleSetAlignItems(node, YGAlignStretch); }},
      {ITEMS_START, [](YGNodeRef node)
       { YGNodeStyleSetAlignItems(node, YGAlignFlexStart); }},
      {ITEMS_CENTER, [](YGNodeRef node)
       { YGNodeStyleSetAlignItems(node, YGAlignCenter); }},
      {ITEMS_END, [](YGNodeRef node)
       { YGNodeStyleSetAlignItems(node, YGAlignFlexEnd); }},
      {ITEMS_BASELINE, [](YGNodeRef node)
       { YGNodeStyleSetAlignItems(node, YGAlignBaseline); }},
      {JUSTIFY_START, [](YGNodeRef node)
       { YGNodeStyleSetJustifyContent(node, YGJustifyFlexStart); }},
      {JUSTIFY_CENTER, [](YGNodeRef node)
       { YGNodeStyleSetJustifyContent(node, YGJustifyCenter); }},
      {JUSTIFY_END, [](YGNodeRef node)
       { YGNodeStyleSetJustifyContent(node, YGJustifyFlexEnd); }},
      {JUSTIFY_BETWEEN, [](YGNodeRef node)
       { YGNodeStyleSetJustifyContent(node, YGJustifySpaceAround); }},
      {JUSTIFY_AROUND, [](YGNodeRef node)
       { YGNodeStyleSetJustifyContent(node, YGJustifySpaceBetween); }},
      {JUSTIFY_EVENLY, [](YGNodeRef node)
       { YGNodeStyleSetHeightPercent(node, 100.0f); }},
      {H_FULL, [](YGNodeRef node)
       { YGNodeStyleSetHeightPercent(node, 100.0f); }},
      {H_1, [](YGNodeRef node)
       { YGNodeStyleSetHeight(node, 10.0f); }},
      {GROW, [](YGNodeRef node)
       { YGNodeStyleSetFlexGrow(node, 1.0f); }}};

  for (const auto &style : styles)
  {
    auto it = styleMap.find(style);
    if (it != styleMap.end())
    {
      it->second(node);
    }
  }
}
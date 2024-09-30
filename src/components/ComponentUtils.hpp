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

  // Static method to create a vector of shared_ptr<DomNode>
  template <typename... T>
  static std::vector<std::shared_ptr<DomNode>> c(T &&...nodes)
  {
    return {std::forward<T>(nodes)...};
  }
};

enum YGStyleProperty
{
  FLEX,
  FLEX_ROW,
  FLEX_COL,
  GAP_2,
  ITEMS_STRETCH,
  ITEMS_START,
  ITEMS_CENTER,
  ITEMS_END,
  ITEMS_BASELINE,
  W_FULL,
  H_FULL,
  H_1,
  GROW
};

inline std::vector<YGStyleProperty> SX() {
    return {};  // Empty vector for no styles
}

template<typename... Styles>
inline std::vector<YGStyleProperty> SX(Styles... styles) {
    return {styles...};  // Vector with styles passed
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
      {GAP_2, [](YGNodeRef node)
       { YGNodeStyleSetGap(node, YGGutterAll, 2.0f); }},
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
      {W_FULL, [](YGNodeRef node)
       { YGNodeStyleSetWidthPercent(node, 100.0f); }},
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
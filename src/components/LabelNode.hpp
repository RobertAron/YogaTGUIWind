#pragma once
#include "ComponentUtils.hpp"

class LabelNode : public DomNode {
public:
  LabelNode(const std::vector<YGStyleProperty> &styles,
            const std::string &text) {
    m_yogaNode = YGNodeNew();
    m_label = tgui::Label::create(text);
    m_label->setIgnoreMouseEvents(true);
    auto renderer = m_label->getRenderer();
    ApplyStyles(styles, m_yogaNode);
    ApplyStyles(styles, renderer);
    YGNodeSetContext(m_yogaNode, m_label.get());
    YGNodeSetMeasureFunc(m_yogaNode, &MeasureWidget);
    // clang-format off
    static const std::unordered_map<YGStyleProperty,std::function<void(tgui::LabelRenderer *)>> styleMap = {
      {TEXT_WHITE,    [](tgui::LabelRenderer * renderer) { renderer->setTextColor(tgui::Color::White); }},
      {TEXT_BLACK,    [](tgui::LabelRenderer * renderer) { renderer->setTextColor(tgui::Color::Black); }},
    };
    // clang-format on
    for (const auto &style : styles) {
      auto it = styleMap.find(style);
      if (it != styleMap.end()) {
        it->second(renderer);
      }
    }
  }

  void AddToGui(tgui::Gui &gui) override { gui.add(m_label); }

  void ApplyLayout(float parentLeft, float parentTop) override {
    float left = YGNodeLayoutGetLeft(m_yogaNode) + parentLeft;
    float top = YGNodeLayoutGetTop(m_yogaNode) + parentTop;
    float width = YGNodeLayoutGetWidth(m_yogaNode);
    float height = YGNodeLayoutGetHeight(m_yogaNode);
    std::cout << "Label" << std::endl;
    LogNode(PositionStruct(left, top, width, height));

    m_label->setPosition(left, top);
    m_label->setSize(width, height);
  }
  static std::unique_ptr<LabelNode>
  make(const std::vector<YGStyleProperty> &styles, const std::string &text) {
    return std::make_unique<LabelNode>(styles, text);
  }

private:
  tgui::Label::Ptr m_label;
  static YGSize MeasureWidget(YGNodeConstRef node, float width,
                              YGMeasureMode widthMode, float height,
                              YGMeasureMode heightMode) {
    auto label = static_cast<tgui::Label *>(
        YGNodeGetContext(node)); // Retrieve the label from context
    label->setAutoSize(true);
    std::cout << label->getText() << std::endl;
    auto labelAutoSize = label->getSize();
    // for max width
    if (widthMode == YGMeasureModeExactly) {
      label->setMaximumTextWidth(width);
    } else if (widthMode == YGMeasureModeAtMost) {
      if (labelAutoSize.x > width) {
        label->setMaximumTextWidth(width);
      }
    }
    // height will always be minimum needed from there.
    // If the actual height gets set lower, it will scroll.
    auto size = label->getSize();
    return YGSize{.width = size.x, .height = size.y};
  }
};
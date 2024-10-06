#pragma once
#include "ComponentUtils.hpp"

class ContainerNode : public DomNode {
public:
  ContainerNode(const std::vector<YGStyleProperty> &styles,
                std::vector<std::unique_ptr<DomNode>> children)
      : children(std::move(children)) {
    m_yogaNode = YGNodeNew();
    m_panel = tgui::Panel::create();
    auto renderer = m_panel->getRenderer();
    ApplyStyles(styles, m_yogaNode);
    ApplyStyles(styles, renderer);
    // clang-format off
    static const std::unordered_map<YGStyleProperty,std::function<void(tgui::PanelRenderer *)>> styleMap = {
      {BG_BLACK,    [](tgui::PanelRenderer * renderer) { renderer->setBackgroundColor(tgui::Color::Black); }},
      {BG_BLUE_500, [](tgui::PanelRenderer * renderer) { renderer->setBackgroundColor(tgui::Color(59,130,236)); }},
      {BG_BLUE_400, [](tgui::PanelRenderer * renderer) { renderer->setBackgroundColor(tgui::Color(96,165,250)); }},
      {BG_SLATE_950,[](tgui::PanelRenderer * renderer) { renderer->setBackgroundColor(tgui::Color(2,6,23)); }},
      {BG_WHITE,    [](tgui::PanelRenderer * renderer) { renderer->setBackgroundColor(tgui::Color::White); }},
    };
    // clang-format on
    renderer->setBackgroundColor(tgui::Color::Transparent);
    for (const auto &style : styles) {
      auto it = styleMap.find(style);
      if (it != styleMap.end()) {
        it->second(renderer);
      }
    }
    for (size_t i = 0; i < this->children.size(); i++) {
      YGNodeInsertChild(m_yogaNode, this->children[i]->m_yogaNode, i);
    }
  }

  void AddToGui(tgui::Gui &gui) override {
    gui.add(m_panel);
    for (auto &child : children) {
      child->AddToGui(gui);
    }
  }

  void ApplyLayout(float parentLeft, float parentTop) override {
    float left = YGNodeLayoutGetLeft(m_yogaNode) + parentLeft;
    float top = YGNodeLayoutGetTop(m_yogaNode) + parentTop;
    float width = YGNodeLayoutGetWidth(m_yogaNode);
    float height = YGNodeLayoutGetHeight(m_yogaNode);
    std::cout << "Container" << std::endl;
    LogNode(PositionStruct(left, top, width, height));


    m_panel->setPosition(left, top);
    m_panel->setSize(width, height);
    for (auto &child : children) {
      child->ApplyLayout(left, top);
    }
  }

  template <typename... T>
  static std::unique_ptr<ContainerNode>
  make(const std::vector<YGStyleProperty> &styles, T &&...children) {
    std::vector<std::unique_ptr<DomNode>> childNodes;
    (childNodes.emplace_back(std::forward<T>(children)), ...);

    return std::make_unique<ContainerNode>(styles, std::move(childNodes));
  }

private:
  tgui::Panel::Ptr m_panel;
  std::vector<std::unique_ptr<DomNode>> children;
};
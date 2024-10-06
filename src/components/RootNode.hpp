#pragma once
#include "ComponentUtils.hpp"

class RootNode : public DomNode {
public:
  RootNode(std::vector<std::unique_ptr<DomNode>> children)
      : children(std::move(children)) {
    m_yogaNode = YGNodeNew();
    YGNodeStyleSetFlexDirection(m_yogaNode, YGFlexDirectionColumn);
    for (size_t i = 0; i < this->children.size(); i++) {
      YGNodeInsertChild(m_yogaNode, this->children[i]->m_yogaNode, i);
    }
  }

  void Update(sf::Vector2u windowSize) {
    YGNodeStyleSetWidth(m_yogaNode, windowSize.x);
    YGNodeStyleSetHeight(m_yogaNode, windowSize.y);
    YGNodeCalculateLayout(m_yogaNode, windowSize.x, windowSize.y,
                          YGDirectionLTR);
    ApplyLayout(0, 0);
  }

  void AddToGui(tgui::Gui &gui) override {
    for (auto &child : children) {
      child->AddToGui(gui);
    }
  }

  void ApplyLayout(float parentLeft, float parentTop) override {
    for (auto &child : children) {
      child->ApplyLayout(0, 0);
    }
  }
  template <typename... T>
  static std::unique_ptr<RootNode> make(T &&...children) {
    std::vector<std::unique_ptr<DomNode>> childNodes;
    // Expand variadic arguments into the childNodes vector
    (childNodes.emplace_back(std::forward<T>(children)), ...);
    return std::make_unique<RootNode>(std::move(childNodes));
  }

private:
  std::vector<std::unique_ptr<DomNode>> children;
};

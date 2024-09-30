#pragma once
#include "ComponentUtils.hpp"

class ButtonNode : public DomNode
{
public:
  ButtonNode(const std::vector<YGStyleProperty> &styles,
             std::vector<std::unique_ptr<DomNode>> children)
      : children(std::move(children))
  {
    m_yogaNode = YGNodeNew();

    ApplyStyles(styles, m_yogaNode);
    for (size_t i = 0; i < this->children.size(); i++)
    {
      YGNodeInsertChild(m_yogaNode, this->children[i]->m_yogaNode, i);
    }
  }

  void ApplyLayout(tgui::Gui &gui) override
  {
    float left = YGNodeLayoutGetLeft(m_yogaNode);
    float top = YGNodeLayoutGetTop(m_yogaNode);
    float width = YGNodeLayoutGetWidth(m_yogaNode);
    float height = YGNodeLayoutGetHeight(m_yogaNode);

    auto button = tgui::Button::create();
    button->setPosition(left, top);
    button->setSize(width, height);
    button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    gui.add(button);
    for (auto &child : children)
    {
      child->ApplyLayout(gui);
    }
  }

  template <typename... T>
  static std::unique_ptr<ButtonNode>
  make(const std::vector<YGStyleProperty> &styles, T &&...children)
  {
    std::vector<std::unique_ptr<DomNode>> childNodes;
    // Expand variadic arguments into the childNodes vector
    (childNodes.emplace_back(std::forward<T>(children)), ...);
    return std::make_unique<ButtonNode>(styles, std::move(childNodes));
  }

private:
  std::vector<std::unique_ptr<DomNode>> children;
};
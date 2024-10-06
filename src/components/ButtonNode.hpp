// #pragma once
// #include "ComponentUtils.hpp"

// class ButtonNode : public DomNode {
// public:
//   ButtonNode(const std::vector<YGStyleProperty> &styles,
//              std::vector<std::unique_ptr<DomNode>> children)
//       : children(std::move(children)) {
//     m_yogaNode = YGNodeNew();
//     m_button = tgui::Button::create();
//     ApplyStyles(styles, m_yogaNode);
//     ApplyStyles(styles, m_button->getRenderer());
//     for (size_t i = 0; i < this->children.size(); i++) {
//       YGNodeInsertChild(m_yogaNode, this->children[i]->m_yogaNode, i);
//     }
//   }

//   void AddToGui(tgui::Gui &gui) override {
//     gui.add(m_button);
//     for (auto &child : children) {
//       child->AddToGui(gui);
//     }
//   }

//   void ApplyLayout(float parentLeft, float parentTop) override {
//     float left = YGNodeLayoutGetLeft(m_yogaNode) + parentLeft;
//     float top = YGNodeLayoutGetTop(m_yogaNode) + parentTop;
//     float width = YGNodeLayoutGetWidth(m_yogaNode);
//     float height = YGNodeLayoutGetHeight(m_yogaNode);

//     m_button->setPosition(left, top);
//     m_button->setSize(width, height);
//     for (auto &child : children) {
//       child->ApplyLayout(left,top);
//     }
//   }

//   template <typename... T>
//   static std::unique_ptr<ButtonNode>
//   make(const std::vector<YGStyleProperty> &styles, T &&...children) {
//     std::vector<std::unique_ptr<DomNode>> childNodes;
//     // Expand variadic arguments into the childNodes vector
//     (childNodes.emplace_back(std::forward<T>(children)), ...);
//     return std::make_unique<ButtonNode>(styles, std::move(childNodes));
//   }

// private:
//   tgui::Button::Ptr m_button;
//   std::vector<std::unique_ptr<DomNode>> children;
// };
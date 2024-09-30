#pragma once
#include "ComponentUtils.hpp"

class ButtonNode : public DomNode {
public:
    ButtonNode(const std::vector<YGStyleProperty>& styles) {
        m_yogaNode = YGNodeNew();
        ApplyStyles(styles, m_yogaNode);
    }

    void ApplyLayout(tgui::Gui &gui) override {
        float left = YGNodeLayoutGetLeft(m_yogaNode);
        float top = YGNodeLayoutGetTop(m_yogaNode);
        float width = YGNodeLayoutGetWidth(m_yogaNode);
        float height = YGNodeLayoutGetHeight(m_yogaNode);

        auto button = tgui::Button::create("Button 1");
        button->setPosition(left, top);
        button->setSize(width, height);
        gui.add(button);
    }

    static std::shared_ptr<ButtonNode> make(const std::vector<YGStyleProperty>& styles) {
        return std::make_shared<ButtonNode>(styles);
    }
};
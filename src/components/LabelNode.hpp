#pragma once
#include "ComponentUtils.hpp"

class LabelNode : public DomNode
{
public:
    LabelNode(const std::vector<YGStyleProperty> &styles, const std::string &text)
    {
        m_yogaNode = YGNodeNew();
        ApplyStyles(styles, m_yogaNode);
    }

    void ApplyLayout(tgui::Gui &gui) override
    {
        float left = YGNodeLayoutGetLeft(m_yogaNode);
        float top = YGNodeLayoutGetTop(m_yogaNode);
        float width = YGNodeLayoutGetWidth(m_yogaNode);
        float height = YGNodeLayoutGetHeight(m_yogaNode);

        auto button = tgui::Label::create("Button 1");
        button->getRenderer()->setTextColor(tgui::Color::White);
        button->getRenderer()->setBackgroundColor(tgui::Color::Blue);
        button->setPosition(left, top);
        button->setSize(width, height);
        gui.add(button);
    }
    static std::shared_ptr<LabelNode> make(const std::vector<YGStyleProperty> &styles, const std::string &text)
    {
        return std::make_shared<LabelNode>(styles, text);
    }
};
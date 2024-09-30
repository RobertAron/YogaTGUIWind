#pragma once
#include "ComponentUtils.hpp"

class LabelNode : public DomNode
{
public:
    LabelNode(const std::vector<YGStyleProperty> &styles, const std::string &text)
    {
        m_yogaNode = YGNodeNew();
        m_label = tgui::Label::create(text);
        ApplyStyles(styles, m_yogaNode);
        YGNodeSetContext(m_yogaNode, m_label.get());
        YGNodeSetMeasureFunc(m_yogaNode, &MeasureWidget);
        YGNodeMarkDirty(m_yogaNode);
    }

    void ApplyLayout(tgui::Gui &gui) override
    {
        float left = YGNodeLayoutGetLeft(m_yogaNode);
        float top = YGNodeLayoutGetTop(m_yogaNode);
        float width = YGNodeLayoutGetWidth(m_yogaNode);
        float height = YGNodeLayoutGetHeight(m_yogaNode);

        m_label->getRenderer()->setTextColor(tgui::Color::White);
        m_label->getRenderer()->setBackgroundColor(tgui::Color::Blue);
        m_label->setPosition(left, top);
        m_label->setSize(width, height);
        gui.add(m_label);
    }
    static std::shared_ptr<LabelNode> make(const std::vector<YGStyleProperty> &styles, const std::string &text)
    {
        return std::make_shared<LabelNode>(styles, text);
    }

private:
    tgui::Label::Ptr m_label;
    static YGSize MeasureWidget(
        YGNodeConstRef node,
        float width,
        YGMeasureMode widthMode,
        float height,
        YGMeasureMode heightMode)
    {
        auto label = static_cast<tgui::Label *>(YGNodeGetContext(node)); // Retrieve the label from context
        label->setAutoSize(true);
        // for max width
        if (widthMode == YGMeasureModeExactly)
            label->setMaximumTextWidth(width);
        else if (widthMode == YGMeasureModeAtMost)
            label->setMaximumTextWidth(width);
        // height will always be minimum needed from there.
        // If the actual height gets set lower, it will scroll.
        auto size = label->getSize();

        return YGSize{
            .width = size.x,
            .height = size.y};
    }
};
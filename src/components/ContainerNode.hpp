#pragma once
#include "ComponentUtils.hpp"

class ContainerNode : public DomNode
{
public:
    ContainerNode(const std::vector<YGStyleProperty> &styles, std::vector<std::unique_ptr<DomNode>> children)
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
        for (auto &child : children)
        {
            child->ApplyLayout(gui);
        }
    }

    template <typename... T>
    static std::unique_ptr<ContainerNode> make(const std::vector<YGStyleProperty>& styles, T&&... children)
    {
        std::vector<std::unique_ptr<DomNode>> childNodes;
        (childNodes.emplace_back(std::forward<T>(children)), ...);

        return std::make_unique<ContainerNode>(styles, std::move(childNodes));
    }

private:
    std::vector<std::unique_ptr<DomNode>> children;
};
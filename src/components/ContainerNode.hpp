#pragma once
#include "ComponentUtils.hpp"

class ContainerNode : public DomNode {
public:
    ContainerNode(const std::vector<YGStyleProperty>& styles, std::vector<std::shared_ptr<DomNode>> children)
        : children(std::move(children)) {
        m_yogaNode = YGNodeNew();
        ApplyStyles(styles, m_yogaNode);
        for (size_t i = 0; i < this->children.size(); i++) {
            YGNodeInsertChild(m_yogaNode, this->children[i]->m_yogaNode, i);
        }
    }

    void ApplyLayout(tgui::Gui &gui) override {
        for (auto &child : children) {
            child->ApplyLayout(gui);
        }
    }

    static std::shared_ptr<ContainerNode> make(const std::vector<YGStyleProperty>& styles, std::vector<std::shared_ptr<DomNode>> children) {
        return std::make_shared<ContainerNode>(styles, std::move(children));
    }

private:
    std::vector<std::shared_ptr<DomNode>> children;
};
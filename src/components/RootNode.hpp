#pragma once
#include "ComponentUtils.hpp"

class RootNode : public DomNode {
public:
    RootNode(std::vector<std::shared_ptr<DomNode>> children) : children(std::move(children)) {
        m_yogaNode = YGNodeNew();
        YGNodeStyleSetFlexDirection(m_yogaNode, YGFlexDirectionColumn);
        for (size_t i = 0; i < this->children.size(); i++) {
            YGNodeInsertChild(m_yogaNode, this->children[i]->m_yogaNode, i);
        }
    }

    void Update(sf::Vector2u windowSize, tgui::Gui &gui) {
        YGNodeStyleSetWidth(m_yogaNode, windowSize.x);
        YGNodeStyleSetHeight(m_yogaNode, windowSize.y);
        YGNodeCalculateLayout(m_yogaNode, windowSize.x, windowSize.y, YGDirectionLTR);
        ApplyLayout(gui);
    }

    void ApplyLayout(tgui::Gui &gui) override {
        for (auto &child : children) {
            child->ApplyLayout(gui);
        }
    }

    static std::shared_ptr<RootNode> make(std::vector<std::shared_ptr<DomNode>> children) {
        return std::make_shared<RootNode>(std::move(children));
    }

private:
    std::vector<std::shared_ptr<DomNode>> children;
};

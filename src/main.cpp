#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <yoga/Yoga.h>
#include <stdio.h>
#include <memory>
#include <vector>

#define GL_SILENCE_DEPRECATION // Suppress OpenGL deprecation warnings on macOS

class DomNode
{
public:
  virtual std::vector<tgui::Widget::Ptr> ApplyLayout() = 0;
  virtual ~DomNode() {}
  YGNodeRef m_yogaNode;
};

class RootNode : public DomNode
{
public:
  RootNode(sf::Vector2u windowSize, std::vector<std::shared_ptr<DomNode>> children)
      : children(std::move(children))
  {
    m_yogaNode = YGNodeNew();
    YGNodeStyleSetWidth(m_yogaNode, windowSize.x);
    YGNodeStyleSetHeight(m_yogaNode, windowSize.y);
    YGNodeStyleSetFlexDirection(m_yogaNode, YGFlexDirectionColumn);
    for (size_t i = 0; i < this->children.size(); i++)
    {
      YGNodeInsertChild(m_yogaNode, this->children[i]->m_yogaNode, i);
    }
  }

  std::vector<tgui::Widget::Ptr> ApplyLayout() override
  {
    std::vector<tgui::Widget::Ptr> results;
    for (auto &child : children)
    {
      auto widgets = child->ApplyLayout();
      results.insert(results.end(), widgets.begin(), widgets.end());
    }
    return results;
  }

private:
  std::vector<std::shared_ptr<DomNode>> children;
};

class ButtonNode : public DomNode
{
public:
  ButtonNode()
  {
    m_yogaNode = YGNodeNew();
    YGNodeStyleSetWidthPercent(m_yogaNode, 100);
    YGNodeStyleSetHeightPercent(m_yogaNode, 30);
    YGNodeStyleSetMargin(m_yogaNode, YGEdgeAll, 10);
  }

  std::vector<tgui::Widget::Ptr> ApplyLayout() override
  {
    auto position = tgui::Layout2d{YGNodeLayoutGetLeft(m_yogaNode), YGNodeLayoutGetTop(m_yogaNode)};
    auto size = tgui::Layout2d{YGNodeLayoutGetWidth(m_yogaNode), YGNodeLayoutGetHeight(m_yogaNode)};
    auto button = tgui::Button::create("Button 1");
    button->setPosition(position);
    button->setSize(size);
    return std::vector<tgui::Widget::Ptr>{button};
  }
};

int main()
{
  sf::ContextSettings settings;
  settings.attributeFlags = sf::ContextSettings::Attribute::Core;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  sf::Window window(sf::VideoMode(800, 600), "TGUI + Yoga + OpenGL Example", sf::Style::Default, settings);
  tgui::Gui gui{window};

  auto root = std::make_shared<RootNode>(window.getSize(), std::vector<std::shared_ptr<DomNode>>{
                                                               std::make_shared<ButtonNode>(),
                                                               std::make_shared<ButtonNode>(),
                                                               std::make_shared<ButtonNode>()});

  auto windowSize = window.getSize();
  YGNodeCalculateLayout(root->m_yogaNode, windowSize.x, windowSize.y, YGDirectionLTR);
  auto widgets = root->ApplyLayout();
  for (auto &widget : widgets)
  {
    gui.add(widget);
  }

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      gui.handleEvent(event);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gui.draw();
    window.display();
  }
  return 0;
}

#include <stdio.h>
#include <yoga/Yoga.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <vector>

#define GL_SILENCE_DEPRECATION // Suppress OpenGL deprecation warnings on macOS

// enum YGStyleProperty
// {
//   FLEX,
//   FLEX_COL,
//   GAP_2,
//   // align - items
//   ITEMS_STRETCH,
//   ITEMS_START,
//   ITEMS_CENTER,
//   ITEMS_END,
// };

class DomNode
{
public:
  virtual void ApplyLayout(tgui::Gui &gui) = 0;
  virtual ~DomNode() {}
  YGNodeRef m_yogaNode;

  // Static method to create a shared_ptr for DomNode or its derived classes
  template <typename T, typename... Args>
  static std::shared_ptr<T> make(Args &&...args)
  {
    static_assert(std::is_base_of<DomNode, T>::value, "T must derive from DomNode");
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  // Static method to create a vector of shared_ptr<DomNode>
  template <typename... T>
  static std::vector<std::shared_ptr<DomNode>> c(T &&...nodes)
  {
    return {std::forward<T>(nodes)...};
  }
};

class RootNode : public DomNode
{
public:
  RootNode(
      std::vector<std::shared_ptr<DomNode>> children)
      : children(std::move(children))
  {
    m_yogaNode = YGNodeNew();

    YGNodeStyleSetFlexDirection(m_yogaNode, YGFlexDirectionColumn);
    for (size_t i = 0; i < this->children.size(); i++)
    {
      YGNodeInsertChild(m_yogaNode, this->children[i]->m_yogaNode, i);
    }
  }

  void Update(sf::Vector2u windowSize, tgui::Gui &gui)
  {
    YGNodeStyleSetWidth(m_yogaNode, windowSize.x);
    YGNodeStyleSetHeight(m_yogaNode, windowSize.y);
    YGNodeCalculateLayout(
        m_yogaNode, windowSize.x, windowSize.y, YGDirectionLTR);
    ApplyLayout(gui);
  }

  void ApplyLayout(tgui::Gui &gui) override
  {
    for (auto &child : children)
    {
      child->ApplyLayout(gui);
    }
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

  void ApplyLayout(tgui::Gui &gui) override
  {
    auto position = tgui::Layout2d{YGNodeLayoutGetLeft(m_yogaNode),
                                   YGNodeLayoutGetTop(m_yogaNode)};
    auto size = tgui::Layout2d{YGNodeLayoutGetWidth(m_yogaNode),
                               YGNodeLayoutGetHeight(m_yogaNode)};
    auto button = tgui::Button::create("Button 1");
    button->setPosition(position);
    button->setSize(size);
    gui.add(button);
  }
};

int main()
{
  sf::ContextSettings settings;
  settings.attributeFlags = sf::ContextSettings::Attribute::Core;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  sf::Window window(sf::VideoMode(800, 600),
                    "TGUI + Yoga + OpenGL Example",
                    sf::Style::Default,
                    settings);
  tgui::Gui gui{window};

  auto root = DomNode::make<RootNode>(
      DomNode::c(
          DomNode::make<ButtonNode>(),
          DomNode::make<ButtonNode>(),
          DomNode::make<ButtonNode>()));
  auto windowSize = window.getSize();
  root->Update(windowSize, gui);

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

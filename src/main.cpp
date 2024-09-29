#include <stdio.h>
#include <yoga/Yoga.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <vector>

#define GL_SILENCE_DEPRECATION // Suppress OpenGL deprecation warnings on macOS

enum YGStyleProperty
{
  FLEX,
  FLEX_ROW,
  FLEX_COL,
  GAP_2,
  // align - items
  ITEMS_STRETCH,
  ITEMS_START,
  ITEMS_CENTER,
  ITEMS_END,
  ITEMS_BASELINE,
  // size
  W_FULL,
  H_FULL,
  H_1,
  H_2,
  H_3,
  H_4,
  GROW
};

class DomNode
{
public:
  virtual void ApplyLayout(tgui::Gui &gui) = 0;
  virtual ~DomNode() {}
  YGNodeRef m_yogaNode;

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

  static std::shared_ptr<RootNode> make(std::vector<std::shared_ptr<DomNode>> children)
  {
    return std::make_shared<RootNode>(std::move(children));
  }

private:
  std::vector<std::shared_ptr<DomNode>> children;
};

void ApplyStyles(const std::vector<YGStyleProperty> &styles, YGNodeRef node)
{
  for (const auto &style : styles)
  {
    switch (style)
    {
    case FLEX:
      YGNodeStyleSetFlex(node, 1.0f);
      break;
    case FLEX_ROW:
      YGNodeStyleSetFlexDirection(node, YGFlexDirectionRow);
      break;
    case FLEX_COL:
      YGNodeStyleSetFlexDirection(node, YGFlexDirectionColumn);
      break;
    case GAP_2:
      YGNodeStyleSetGap(node, YGGutterAll, 2.0f);
      break;
    case ITEMS_STRETCH:
      YGNodeStyleSetAlignItems(node, YGAlignStretch);
      break;
    case ITEMS_START:
      YGNodeStyleSetAlignItems(node, YGAlignFlexStart);
      break;
    case ITEMS_CENTER:
      YGNodeStyleSetAlignItems(node, YGAlignCenter);
      break;
    case ITEMS_END:
      YGNodeStyleSetAlignItems(node, YGAlignFlexEnd);
      break;
    case ITEMS_BASELINE:
      YGNodeStyleSetAlignItems(node, YGAlignBaseline);
      break;
    case W_FULL:
      YGNodeStyleSetWidthPercent(node, 100.0f);
      break;
    case H_FULL:
      YGNodeStyleSetHeightPercent(node, 100.0f);
      break;
    case H_1:
      YGNodeStyleSetHeight(node, 10.0f);
      break;
    case GROW:
      YGNodeStyleSetFlexGrow(node, 1.0f);
      break;
    default:
      break;
    }
  }
}

class ContainerNode : public DomNode
{
public:
  ContainerNode(const std::vector<YGStyleProperty> &styles, std::vector<std::shared_ptr<DomNode>> children)
      : children(std::move(children))
  {
    std::cout << "making container" << std::endl;
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
    std::cout << "applying container layout" << std::endl;
    std::cout << "Container Layout - Left: " << left << ", Top: " << top
              << ", Width: " << width << ", Height: " << height << std::endl;
    for (auto &child : children)
    {
      child->ApplyLayout(gui);
    }
  }

  static std::shared_ptr<ContainerNode> make(const std::vector<YGStyleProperty> &styles, std::vector<std::shared_ptr<DomNode>> children)
  {
    return std::make_shared<ContainerNode>(styles, std::move(children));
  }

private:
  std::vector<std::shared_ptr<DomNode>> children;
};

class ButtonNode : public DomNode
{
public:
  ButtonNode(const std::vector<YGStyleProperty> &styles)
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

    std::cout << "applying button layout" << std::endl;
    std::cout << "button Layout - Left: " << left << ", Top: " << top
              << ", Width: " << width << ", Height: " << height << std::endl;
    auto button = tgui::Button::create("Button 1");
    button->setPosition(left, top);
    button->setSize(width, height);
    gui.add(button);
  }

  static std::shared_ptr<ButtonNode> make(const std::vector<YGStyleProperty> &styles)
  {
    return std::make_shared<ButtonNode>(styles);
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

  auto root = RootNode::make(
      DomNode::c(
          ContainerNode::make(
              std::vector<YGStyleProperty>{H_FULL, W_FULL, FLEX, FLEX_COL, GAP_2},
              DomNode::c(
                  ButtonNode::make(std::vector<YGStyleProperty>{H_1}),
                  ButtonNode::make(std::vector<YGStyleProperty>{H_1}),
                  ButtonNode::make(std::vector<YGStyleProperty>{H_1})))));
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

#include <stdio.h>
#include <yoga/Yoga.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <vector>

// #include "./components/ButtonNode.hpp"
#include "./components/ComponentUtils.hpp"
#include "./components/ContainerNode.hpp"
#include "./components/LabelNode.hpp"
#include "./components/RootNode.hpp"

#define GL_SILENCE_DEPRECATION // Suppress OpenGL deprecation warnings on macOS

int main() {
  sf::ContextSettings settings;
  settings.attributeFlags = sf::ContextSettings::Attribute::Core;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  sf::Window window(sf::VideoMode(800, 600), "TGUI + Yoga + OpenGL Example",
                    sf::Style::Default, settings);
  tgui::Gui gui{window};

  // clang-format off
  auto root = RootNode::make(
    ContainerNode::make(SX(H_FULL, W_FULL, FLEX, FLEX_ROW),
      ContainerNode::make(SX(BG_BLACK, W_96, FLEX_COL, JUSTIFY_END, P_4, GAP_4),
        ContainerNode::make(SX(FLEX_COL),
          LabelNode::make(SX(TEXT_4XL, TEXT_WHITE), "Some title here!"),
          LabelNode::make(SX(TEXT_BASE, TEXT_WHITE), "Some subtitle information goes here.")
        ),
        LabelNode::make(SX(TEXT_BASE, TEXT_WHITE), "Main information goes here! The information here is longer after grabbing you with the hook. This content actually informs you about stuff."),
        LabelNode::make(SX(TEXT_BASE, TEXT_WHITE), "A little bit more information here."),
        ContainerNode::make(SX(FLEX_ROW,GAP_4),
          ContainerNode::make(SX(W_8,H_8,BG_BLUE_400)),
          ContainerNode::make(SX(W_8,H_8,BG_BLUE_400)),
          ContainerNode::make(SX(W_8,H_8,BG_BLUE_400)),
          ContainerNode::make(SX(W_8,H_8,BG_BLUE_400))
        )
      ),
      ContainerNode::make(SX(BG_SLATE_950, FLEX, FLEX_COL, ITEMS_CENTER, JUSTIFY_CENTER, GROW),
        ContainerNode::make(SX(FLEX_COL, ITEMS_CENTER, GAP_1),
          ContainerNode::make(SX(W_20,H_20,BG_BLUE_400)),
          LabelNode::make(SX(TEXT_BASE, TEXT_WHITE), "Start!")
        )
      )
    )
  );
  // clang-format on

  auto windowSize = window.getSize();
  root->AddToGui(gui);
  root->Update(windowSize);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      gui.handleEvent(event);
    }
    auto newWindowSize = window.getSize();
    if (newWindowSize.x != windowSize.x || newWindowSize.y != windowSize.y) {
      windowSize = newWindowSize;
      root->Update(windowSize);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gui.draw();
    window.display();
    sf::sleep(sf::milliseconds(10));
  }
  return 0;
}

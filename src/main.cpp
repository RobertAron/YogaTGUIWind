#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <TGUI/TGUI.hpp>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// Include your component headers
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

  // Deactivate the window's context in the main thread
  window.setActive(false);

  // Shared variables
  std::atomic<bool> running{true};

  // Thread-safe event queue
  std::mutex eventMutex;
  std::condition_variable eventCV;
  std::queue<sf::Event> eventQueue;

  // Start the render thread
  std::thread renderThread([&]() {
    // Activate the window's context in this thread
    window.setActive(true);

    // Create TGUI GUI in the render thread
    tgui::Gui gui{window};

    // Build your UI components
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

    // Add the root component to the GUI
    root->AddToGui(gui);

    // Initial window size
    sf::Vector2u previousWindowSize = window.getSize();
    root->Update(previousWindowSize);

    while (running) {
      // Process events from the main thread
      std::queue<sf::Event> localEventQueue;
      {
        std::unique_lock<std::mutex> lock(eventMutex);
        eventCV.wait_for(lock, std::chrono::milliseconds(10),
                         [&]() { return !eventQueue.empty() || !running; });

        if (!running)
          break;

        // Move events to a local queue to minimize locking time
        std::swap(localEventQueue, eventQueue);
      }

      // Handle events
      while (!localEventQueue.empty()) {
        sf::Event event = localEventQueue.front();
        localEventQueue.pop();

        if (event.type == sf::Event::Closed) {
          running = false;
          window.close();
        }

        // Handle GUI events
        gui.handleEvent(event);
      }

      // Check for window size changes independently of events
      sf::Vector2u currentWindowSize = window.getSize();
      if (currentWindowSize != previousWindowSize) {
        previousWindowSize = currentWindowSize;
        root->Update(currentWindowSize);
      }

      // Render the GUI
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gui.draw();
      window.display();
    }
  });

  // Event polling in the main thread
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      // Push event into the thread-safe queue
      {
        std::lock_guard<std::mutex> lock(eventMutex);
        eventQueue.push(event);
      }
      eventCV.notify_one();

      if (event.type == sf::Event::Closed) {
        // Signal the render thread to stop
        running = false;
        window.close();
        eventCV.notify_one();
      }
    }

    // Sleep briefly to prevent high CPU usage
    sf::sleep(sf::milliseconds(10));
  }

  // Wait for the render thread to finish
  running = false;
  eventCV.notify_all();
  renderThread.join();

  return 0;
}

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <yoga/Yoga.h>

// This function sets up Yoga layout and returns the calculated positions and sizes
std::vector<std::pair<tgui::Layout2d, tgui::Layout2d>> setupYogaLayout(sf::Vector2u windowSize)
{
    // Create the root Yoga node
    YGNodeRef root = YGNodeNew();
    YGNodeStyleSetWidth(root, windowSize.x);
    YGNodeStyleSetHeight(root, windowSize.y);
    YGNodeStyleSetFlexDirection(root, YGFlexDirectionColumn);
    
    // Create child nodes for a column layout (3 elements stacked vertically)
    YGNodeRef buttonNode1 = YGNodeNew();
    YGNodeStyleSetWidthPercent(buttonNode1, 100);
    YGNodeStyleSetHeightPercent(buttonNode1, 30);
    YGNodeStyleSetMargin(buttonNode1, YGEdgeAll, 10);
    
    YGNodeRef buttonNode2 = YGNodeNew();
    YGNodeStyleSetWidthPercent(buttonNode2, 100);
    YGNodeStyleSetHeightPercent(buttonNode2, 30);
    YGNodeStyleSetMargin(buttonNode2, YGEdgeAll, 10);
    
    YGNodeRef buttonNode3 = YGNodeNew();
    YGNodeStyleSetWidthPercent(buttonNode3, 100);
    YGNodeStyleSetHeightPercent(buttonNode3, 30);
    YGNodeStyleSetMargin(buttonNode3, YGEdgeAll, 10);
    
    // Add the buttons to the root node
    YGNodeInsertChild(root, buttonNode1, 0);
    YGNodeInsertChild(root, buttonNode2, 1);
    YGNodeInsertChild(root, buttonNode3, 2);
    
    // Calculate the layout
    YGNodeCalculateLayout(root, windowSize.x, windowSize.y, YGDirectionLTR);
    
    // Retrieve calculated positions and sizes
    std::vector<std::pair<tgui::Layout2d, tgui::Layout2d>> layouts;
    layouts.emplace_back(
        tgui::Layout2d{ YGNodeLayoutGetLeft(buttonNode1), YGNodeLayoutGetTop(buttonNode1) },
        tgui::Layout2d{ YGNodeLayoutGetWidth(buttonNode1), YGNodeLayoutGetHeight(buttonNode1) }
    );
    layouts.emplace_back(
        tgui::Layout2d{ YGNodeLayoutGetLeft(buttonNode2), YGNodeLayoutGetTop(buttonNode2) },
        tgui::Layout2d{ YGNodeLayoutGetWidth(buttonNode2), YGNodeLayoutGetHeight(buttonNode2) }
    );
    layouts.emplace_back(
        tgui::Layout2d{ YGNodeLayoutGetLeft(buttonNode3), YGNodeLayoutGetTop(buttonNode3) },
        tgui::Layout2d{ YGNodeLayoutGetWidth(buttonNode3), YGNodeLayoutGetHeight(buttonNode3) }
    );
    
    // Clean up Yoga nodes
    YGNodeFreeRecursive(root);
    
    return layouts;
}

int main()
{
    // Set OpenGL 3.3 context
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Attribute::Core;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    
    // Create SFML window
    sf::Window window(sf::VideoMode(800, 600), "TGUI + Yoga + OpenGL Example", sf::Style::Default, settings);
    
    // Create TGUI GUI context (backend for OpenGL3)
    tgui::Gui gui{window};

    // Get layout from Yoga
    auto layouts = setupYogaLayout(window.getSize());

    // Create buttons using TGUI, and place them according to Yoga layout
    auto button1 = tgui::Button::create("Button 1");
    button1->setPosition(layouts[0].first);
    button1->setSize(layouts[0].second);

    auto button2 = tgui::Button::create("Button 2");
    button2->setPosition(layouts[1].first);
    button2->setSize(layouts[1].second);

    auto button3 = tgui::Button::create("Button 3");
    button3->setPosition(layouts[2].first);
    button3->setSize(layouts[2].second);

    // Add buttons to the GUI
    gui.add(button1);
    gui.add(button2);
    gui.add(button3);

    // Run the main loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvent(event);
        }

        // Clear the window with a black background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw TGUI components
        gui.draw();
        
        // Display the rendered frame
        window.display();
    }

    return 0;
}

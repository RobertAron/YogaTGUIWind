#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-OpenGL3.hpp>
#include <yoga/Yoga.h>
#include <stdio.h>

YGSize measureWidget(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
{
    // Get the context (our tgui::Text object)
    auto text = static_cast<tgui::Label *>(YGNodeGetContext(node));
    if (text == nullptr)
    {
        std::cerr << "Context is null!" << std::endl;
        return YGSize{0, 0};
    }

    // Set a maximum width if necessary
    if (widthMode != YGMeasureModeUndefined)
    {
        // text->setMaximumTextWidth(width);
    }

    // Get the size of the text
    tgui::Vector2f textSize = text->getSize();

    // Return the measured size as a YGSize struct
    return YGSize{
        .width = textSize.x,
        .height = textSize.y};
}

static inline YGNodeRef MyNodeMaker(tgui::Label::Ptr text)
{
    auto node = YGNodeNew();

    // Store the label pointer as the context
    YGNodeSetContext(node, text.get());

    // Set the custom measure function for Yoga
    YGNodeSetMeasureFunc(node, &measureWidget);

    return node;
}

// This function sets up Yoga layout and returns the calculated positions and sizes
std::vector<std::pair<tgui::Layout2d, tgui::Layout2d>> setupYogaLayout(sf::Vector2u windowSize, tgui::Label::Ptr label)
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

    auto hmm = MyNodeMaker(label);
    // Calculate the layout
    YGNodeCalculateLayout(root, windowSize.x, windowSize.y, YGDirectionLTR);

    // Retrieve calculated positions and sizes
    std::vector<std::pair<tgui::Layout2d, tgui::Layout2d>> layouts;
    layouts.emplace_back(
        tgui::Layout2d{YGNodeLayoutGetLeft(buttonNode1), YGNodeLayoutGetTop(buttonNode1)},
        tgui::Layout2d{YGNodeLayoutGetWidth(buttonNode1), YGNodeLayoutGetHeight(buttonNode1)});
    layouts.emplace_back(
        tgui::Layout2d{YGNodeLayoutGetLeft(buttonNode2), YGNodeLayoutGetTop(buttonNode2)},
        tgui::Layout2d{YGNodeLayoutGetWidth(buttonNode2), YGNodeLayoutGetHeight(buttonNode2)});
    layouts.emplace_back(
        tgui::Layout2d{YGNodeLayoutGetLeft(buttonNode3), YGNodeLayoutGetTop(buttonNode3)},
        tgui::Layout2d{YGNodeLayoutGetWidth(buttonNode3), YGNodeLayoutGetHeight(buttonNode3)});

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

    // Create a label widget
    auto label = tgui::Label::create("Hello world!");
    label->getRenderer()->setTextColor(tgui::Color::White);
    label->getRenderer()->setTextSize(24);

    // Get layout from Yoga, passing the label to MyNodeMaker
    auto myNode = MyNodeMaker(label); // Pass the label here
    // YGNodeInsertChild(root, myNode, 2);

    // Add buttons and label to the GUI based on Yoga layout
    auto layouts = setupYogaLayout(window.getSize(), label);

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

    // Add the label to the GUI
    label->setPosition(layouts[2].first); // Set the position based on Yoga layout
    label->setSize(layouts[2].second);    // Set the size based on Yoga layout
    gui.add(label);                       // Add the label to the GUI

    // Main loop
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

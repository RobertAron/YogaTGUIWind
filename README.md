# Yoga + TGUI + SFML Project

Tailwind like styling in C++ using Yoga/TGUI/SFML!


### Core UI Config & Result
```cpp
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
```

![What the Resulting UI looks like](output.webp)



How to Clone

```bash
git clone --recursive <repository-url>
```

If cloned without --recursive, run:

```bash
git submodule update --init
```

### wsl deps

```
sudo apt install libsfml-dev
sudo apt install libfreetype6-dev
sudo apt install clang-format
```

### mac deps

```
brew install sfml
brew install freetype
brew install clang-format
brew install ccache
brew install ninja
```

How to Build

```bash
./dev.sh
```



# <img src="https://raw.githubusercontent.com/expressobits/inventory-system/addon/icon.png" alt= “icon” width="32" height="32"> Welcome to Expresso Inventory System 👋
![Version](https://img.shields.io/badge/version-2.12.0-blue.svg?cacheSeconds=2592000)
[![Documentation](https://img.shields.io/badge/documentation-yes-green.svg)](https://expressobits.com/inventory-system/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](MIT)

![Demo](https://raw.githubusercontent.com/wiki/ExpressoBits/inventory-system/demo.gif)

New here? Read our new [Documentation](https://expressobits.com/inventory-system/)

## Branchs Map

🪹[main](https://github.com/expressobits/inventory-system/tree/main): Contains C++ Plugin

🪹[addon](https://github.com/expressobits/inventory-system/tree/addon): Compiled version of the libs (with demos) that are in the godot asset lib store.

🪹[csharp](https://github.com/expressobits/inventory-system/tree/csharp): Contains Conversion version to csharp by [Glitshy](https://github.com/Glitshy).


## Features

✔️ Inventory with Item Stacks.

✔️ Separate UI inventory logic.

✔️ First Person demo with chests.

<!-- ✔️ Fully commented core and UI code. -->

✔️ Item Definition as Extendable Resource.

✔️ Demonstrations of dropped items and item pickup.

✔️ Multiplayer compatible (Including example).

✔️ Loot System (https://github.com/expressobits/inventory-system/issues/152).
 
✔️ Hotbar.

✔️ Craft System.

✔️ Item Editor.

✔️ Categories. (https://github.com/expressobits/inventory-system/issues/5)

✔️ Specialized slot category inventory. (https://github.com/expressobits/inventory-system/issues/40)

✔️ Autocrafts. (https://github.com/expressobits/inventory-system/issues/34)

✔️ Unconventional inventory. https://github.com/expressobits/inventory-system/issues/32

✔️ Inventory with grids like RE4. (https://github.com/expressobits/inventory-system/issues/35)

✔️ Equipament Inventory

## In Progress

## Planneds

📅 Inventory UI dynamic #83

## Install
See in [Docs](https://expressobits.com/inventory-system/getting_started/installation.html)

## Struture
### [Gloot](https://github.com/peter-kish/gloot)
Grid Inventory and Grid UIs based on plugin [Gloot](https://github.com/peter-kish/gloot)
Special thanks to [Peter Kiš](https://github.com/peter-kish) and its contributors
The grid system and its UI were based on this repository.

## CMake support

### git submodule setup in main repository
`git submodule init`  
`git submodule update`

### cmake configure in main repository
`cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -B cmake-build-debug .`

Define variables GODOTCPP_PRECISION=single|double and/or BITS=32|64 for further build variants if needed.

### cmake build/install in specified build folder cmake-build-debug
`cmake --build . --target install --config Debug`

This installs the built library in the bin folder of the repository.

## Authors

👤 **Rafael Correa**
* Twitter: [@ScriptsEngineer](https://twitter.com/ScriptsEngineer)
* Github: [@scriptsengineer](https://github.com/scriptsengineer)

## 🤝 Contributing

Contributions, issues and feature requests are welcome!

Feel free to check [issues page](https://github.com/ExpressoBits/inventory-system/issues).

To suggest or discuss some project structure, feel free here [discussions page](https://github.com/expressobits/inventory-system/discussions)


## Show your support

Give a ⭐️ if this project helped you!

## Assets

All assets (GUI, Models and textures) from https://kenney.nl/assets


## 📝 License

This project is [MIT](MIT) licensed.

.. _craft_station_tutorial_1_resources:

###########################################
CraftStation Tutorial 1: Resources
###########################################

This tutorial will guide you through creating a CraftStationType and the related items and recipes. We'll create a "Furnace" station type and recipes that can only be crafted at furnaces.

Prerequisites
=============

Before starting this tutorial, make sure you have:

- An inventory system project set up
- An :ref:`InventoryDatabase<class_InventoryDatabase>` resource created

Creating the CraftStationType
=============================

Step 1: Open the Database Editor
---------------------------------

1. In your project, open "Inventory" top menus
2. Select "Database" to open the database editor

Step 2: Create the Furnace Station Type
----------------------------------------

1. **Navigate to CraftStationTypes**
   
   In the database editor, click on the "CraftStationTypes" section.

   .. image:: ./../manual/database/images/craft_station_type_editor.png

2. **Add New Station Type**
   
   Click the "+" button to create a new craft station type.

   .. image:: ./../manual/database/images/add_craft_station_type.png

3. **Configure Station Type Properties**
   
   Set the following properties:
   
   - **ID**: Enter "furnace" (this will be used in code)
   - **Name**: Enter "Furnace" (this will be displayed to users)
   - **Icon**: Optionally select a texture that represents a furnace

   .. image:: ./../manual/database/images/craft_station_type_editing.png

4. **Save the Database**
   
   Save your InventoryDatabase resource (Ctrl+S).

Creating Station-Specific Items
===============================

Step 3: Create Smelting Items
------------------------------

For our furnace example, let's create items for smelting:

1. **Iron Ore** (ingredient)
   
   - In the "Item Definitions" section, add a new item
   - Set ID: "iron_ore", Name: "Iron Ore"
   - Set Max Stack Size: 64

2. **Iron Ingot** (product)
   
   - Add another item definition
   - Set ID: "iron_ingot", Name: "Iron Ingot"
   - Set Max Stack Size: 64

3. **Coal** (fuel - required item)
   
   - Add another item definition  
   - Set ID: "coal", Name: "Coal"
   - Set Max Stack Size: 64

4. **Stick** (product)

   - Add another item definition
   - Set ID: "stick", Name: "Stick"
   - Set Max Stack Size: 64

5. **Wood** (crafting ingredient)
   
   - Add another item definition
   - Set ID: "wood", Name: "Wood"
   - Set Max Stack Size: 64

.. image:: ./images/create_and_use_craft_station_types_add_items.png

Step 4: Create Furnace-Specific Recipe
---------------------------------------

1. **Add New Recipe**
   
   In the "Recipes" section, click "+" to create a new recipe.

   .. image:: ./../manual/database/images/recipes_editor_add.png


2. **Configure Recipe Properties**
   
   - **Time to Craft**: Set to 5.0 seconds (longer for smelting)

   - **Station**: Select the "Furnace" station type we just created


3. **Add Recipe Components**
   
   **Ingredients** (consumed during crafting):
   - Iron Ore: amount 2
   
   **Required Items** (needed but not consumed):
   - Coal: amount 1
   
   **Products** (created when crafting completes):
   - Iron Ingot: amount 1

.. image:: ./images/create_and_use_craft_station_types_recipe_iron_ingot.png

4. **Save the Database**

Creating a General Recipe
=========================

Step 5: Create a Universal Recipe
----------------------------------

To demonstrate the filtering, let's also create a recipe that works on any station:

1. **Add Another Recipe**
   
   Create a new recipe for crafting sticks from wood.

2. **Configure Universal Recipe**
   
   - **Time to Craft**: 1.0 seconds
   - **Station**: Leave this EMPTY (no station type)
   - **Ingredients**: Wood: amount 1
   - **Products**: Stick: amount 2

.. image:: ./images/create_and_use_craft_station_types_recipe_stick.png

.. note::
   When a recipe has no station type, it can only be crafted at stations that also have no station type assigned.

Next Steps
==========

Now that you have created the resource foundations (CraftStationType, items, and recipes), you can continue to the next tutorial to learn how to set up the actual craft station nodes and implement the crafting logic.

.. seealso::
   
   - :ref:`craft_station_tutorial_2_nodes` - Learn how to set up craft station nodes and scripting
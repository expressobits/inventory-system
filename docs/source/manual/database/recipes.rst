.. _recipes:

###################
Recipes
###################

Recipes define what items can be created through crafting and what materials are required. In the inventory system, recipes are resources that contain all the information needed for crafting items at :ref:`CraftStation<class_CraftStation>` nodes.

Recipes Overview
================

A recipe consists of several components:

- **Ingredients**: Items that are consumed during crafting
- **Required Items**: Items needed for crafting but not consumed (like tools)
- **Products**: Items produced when crafting is completed
- **Station Type**: The specific :ref:`CraftStationType<class_CraftStationType>` required for this recipe
- **Craft Time**: How long the crafting process takes

Creating Recipes
================

Recipes are created and managed through the :ref:`InventoryDatabase<class_InventoryDatabase>` editor. Here's how to create a new recipe:

1. **Open the Database Editor**
   
   Open your inventory database resource in the Inspector. The database editor will appear at the bottom of the editor.

2. **Navigate to Recipes**
   
   In the database editor, locate the "Recipes" section.

   .. image:: ./images/recipes_editor_open.png

3. **Add New Recipe**
   
   Click the "+" button to create a new recipe. A new recipe entry will appear in the list.

   .. image:: ./images/recipes_editor_add.png

4. **Configure Recipe Properties**
   
   Set the following properties for your recipe:
   
   - **Station**: Select the :ref:`CraftStationType<class_CraftStationType>` required for this recipe (optional)
   - **Time to Craft**: Set the duration for crafting in seconds
   - **Ingredients**: Add the items that will be consumed during crafting
   - **Required Items**: Add items needed but not consumed (like tools)
   - **Products**: Add the items that will be produced

   .. image:: ./images/recipes_editor_edit.png

Recipe Properties
=================

Ingredients
-----------

Ingredients are items that get consumed during the crafting process. These items are removed from the input inventories of the craft station.

**When ingredients are removed:**

- If :ref:`only_remove_ingredients_after_craft<class_CraftStation_property_only_remove_ingredients_after_craft>` is ``false``: Ingredients are removed when crafting starts
- If :ref:`only_remove_ingredients_after_craft<class_CraftStation_property_only_remove_ingredients_after_craft>` is ``true``: Ingredients are removed when crafting completes

.. note::
   Each ingredient is an :ref:`ItemStack<class_ItemStack>` that specifies both the item and the quantity needed.

Required Items
--------------

Required items are items that must be present in the input inventories for crafting to begin, but they are not consumed during the process. This is useful for tools or permanent equipment.

**Example use cases:**

- Crafting tools (hammer, anvil, etc.)
- Fuel sources that aren't consumed per recipe
- Permanent equipment or machinery

Products
--------

Products are the items created when crafting is successfully completed. These items are added to the output inventories of the craft station.

.. note::
   Products are added to output inventories only when crafting completes successfully.

Station Type
------------

The station type determines which :ref:`CraftStation<class_CraftStation>` nodes can execute this recipe. Only craft stations with a matching :ref:`type<class_CraftStation_property_type>` will include this recipe in their :ref:`valid_recipes<class_CraftStation_property_valid_recipes>` list.

.. note::
   If no station type is set, the recipe will be available to craft stations that also have no type set.

Craft Time
----------

The time (in seconds) required to complete the crafting process. During this time, the recipe will be in the craft station's :ref:`craftings<class_CraftStation_property_craftings>` list.

Recipe Validation
=================

Before a recipe can be crafted, the system validates:

1. **Station Compatibility**: The craft station must have the same type as the recipe (or both must have no type)
2. **Ingredient Availability**: All required ingredients must be available in input inventories
3. **Required Item Availability**: All required items must be present in input inventories
4. **Output Space**: There must be space in output inventories for the products

.. tip::
   Use the :ref:`can_craft()<class_CraftStation_method_can_craft>` method to check if a recipe can be executed before starting the crafting process.

Best Practices
==============

Recipe Organization
-------------------

- Group related recipes by station type
- Use descriptive names for easy identification
- Consider crafting time balance for gameplay

Station Type Assignment
-----------------------

- Assign specific station types to organize crafting (e.g., "Furnace", "Workbench", "Alchemy Table")
- Leave station type empty for universal recipes that can be crafted anywhere
- Use consistent naming conventions for station types

Balancing
---------

- Consider ingredient rarity and availability
- Balance crafting time with recipe complexity
- Ensure product value matches ingredient cost

.. seealso::
   
   - :ref:`craft_station_type` - Learn about CraftStationType resources
   - :ref:`craft_stations` - Learn about using CraftStation nodes

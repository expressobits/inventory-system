.. _craft_station_type:

###################
CraftStationType
###################

CraftStationType resources define the different types of crafting stations available in your inventory system. They are used to categorize and organize :ref:`Recipe<class_Recipe>` and :ref:`CraftStation<class_CraftStation>` nodes, ensuring that only compatible recipes can be crafted at specific stations.

CraftStationType Overview
=========================

A CraftStationType serves as a filter that connects recipes to their appropriate crafting stations. For example:

- A "Furnace" type could be used for smelting recipes
- A "Workbench" type could be used for general crafting recipes  
- An "Alchemy Table" type could be used for potion-making recipes

This system allows you to create diverse crafting mechanics where different types of stations handle different categories of recipes.

.. note::
   If a recipe has no station type assigned, it can only be crafted at craft stations that also have no station type assigned.

Creating CraftStationTypes
===========================

CraftStationType resources are created and managed through the :ref:`InventoryDatabase<class_InventoryDatabase>` editor:

1. **Open the Database Editor**
   
   Open your inventory database resource in the Inspector. The database editor will appear at the bottom of the editor.

2. **Navigate to CraftStationTypes**
   
   In the database editor, locate the "CraftStationTypes" section.

3. **Add New CraftStationType**
   
   Click the "+" button to create a new craft station type. A new entry will appear in the list.

4. **Configure Properties**
   
   Set the following properties for your craft station type:
   
   - **ID**: A unique identifier for the station type
   - **Name**: A human-readable name for the station type
   - **Icon**: An optional icon representing the station type

CraftStationType Properties
===========================

ID
--

The ID is a unique string identifier for the craft station type. This should be:

- Unique across all craft station types in your database
- Descriptive and consistent (e.g., "furnace", "workbench", "alchemy_table")
- Follow naming conventions for easy reference

**Example IDs:**
- ``"furnace"``
- ``"workbench"`` 
- ``"alchemy_table"``
- ``"cooking_pot"``

Name
----

The name is a human-readable label for the craft station type. This is typically used in user interfaces and should be:

- Clear and descriptive
- Localized if supporting multiple languages
- Consistent with your game's terminology

**Example Names:**
- "Furnace"
- "Crafting Workbench"
- "Alchemy Table"
- "Cooking Pot"

Icon
----

An optional :ref:`Texture2D<class_Texture2D>` that represents the station type visually. This icon can be used in:

- User interfaces showing available crafting stations
- Recipe displays indicating where they can be crafted
- Crafting menus and tutorials

Using CraftStationTypes
========================

Assigning to Recipes
--------------------

When creating recipes, you can assign a CraftStationType to specify which type of station can craft that recipe:

1. Open the recipe in the database editor
2. Set the **Station** property to your desired CraftStationType
3. Save the database

Only :ref:`CraftStation<class_CraftStation>` nodes with a matching type will include this recipe in their available recipes.

Assigning to CraftStations
---------------------------

CraftStation nodes use the CraftStationType to filter which recipes they can process:

1. Select your CraftStation node in the scene
2. In the Inspector, set the **Type** property to your desired CraftStationType
3. The station will automatically load matching recipes when the scene starts

.. code-block:: gdscript

   # Example: Setting station type in code
   @onready var craft_station = $CraftStation
   
   func _ready():
       # Load a specific station type from the database
       var station_type = database.get_craft_station_type_by_id("furnace")
       craft_station.type = station_type

Recipe Filtering
================

The CraftStationType system provides automatic recipe filtering:

**Matching Logic:**
- Recipes with a station type can only be crafted at stations with the same type
- Recipes with no station type can only be crafted at stations with no type
- Stations automatically populate their :ref:`valid_recipes<class_CraftStation_property_valid_recipes>` based on this filtering

**Example Scenario:**

.. code-block:: text

   Database contains:
   - Recipe A: Station Type = "Furnace"
   - Recipe B: Station Type = "Workbench"  
   - Recipe C: Station Type = None
   
   Craft Stations:
   - Station 1: Type = "Furnace" → Can craft Recipe A only
   - Station 2: Type = "Workbench" → Can craft Recipe B only
   - Station 3: Type = None → Can craft Recipe C only

Best Practices
==============

Organization
------------

- Create logical station types that match your game's crafting themes
- Use consistent naming conventions across IDs and names
- Group related recipes under appropriate station types

Station Type Design
-------------------

- Consider the complexity and specialization of different crafting processes
- Balance the number of station types (too many can be overwhelming, too few may be limiting)
- Think about progression - basic stations for early game, advanced stations for late game

Integration with Gameplay
--------------------------

- Use station types to gate progression (advanced recipes require advanced stations)
- Consider resource requirements for different station types
- Plan how players will obtain or build different types of crafting stations

.. tip::
   Start with a few basic station types and expand as your game's crafting system grows more complex.

Examples
========

Basic Setup
-----------

Here's an example of a simple crafting system with three station types:

.. code-block:: text

   CraftStationTypes:
   1. ID: "workbench", Name: "Crafting Workbench"
      - Basic tool and item crafting
   
   2. ID: "furnace", Name: "Furnace"  
      - Smelting and cooking recipes
   
   3. ID: "alchemy", Name: "Alchemy Table"
      - Potion and enchantment recipes

Advanced Setup
--------------

For more complex games, you might have specialized station types:

.. code-block:: text

   CraftStationTypes:
   1. ID: "basic_workbench", Name: "Basic Workbench"
   2. ID: "advanced_workbench", Name: "Advanced Workbench"
   3. ID: "stone_furnace", Name: "Stone Furnace"
   4. ID: "blast_furnace", Name: "Blast Furnace"
   5. ID: "alchemy_table", Name: "Alchemy Table"
   6. ID: "enchanting_table", Name: "Enchanting Table"

.. seealso::
   
   - :ref:`recipes` - Learn about creating and configuring recipes
   - :ref:`craft_stations` - Learn about using CraftStation nodes
   - :ref:`create_and_use_craft_station_type` - Step-by-step tutorial for creating station types
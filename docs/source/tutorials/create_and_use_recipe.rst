.. _create_and_use_recipe:

###########################################
Create and Use Recipe
###########################################

This tutorial will walk you through creating a recipe in the inventory database and using it in a scene with a craft station. We'll create a simple recipe that turns wood into planks.

Prerequisites
=============

Before starting this tutorial, make sure you have:

- An inventory system project set up
- An :ref:`InventoryDatabase<class_InventoryDatabase>` resource created
- Basic understanding of the Godot editor

Creating the Recipe
===================

Step 1: Open the Database Editor
---------------------------------

1. In your project, open "Inventory" top menus
2. Select "Database" to open the database editor

Step 2: Create Item Definitions
--------------------------------

Before creating a recipe, we need the items involved. If you don't already have them, create:

1. **Wood** (ingredient)
   
   - In the database editor, go to the "Item Definitions" section
   - Click the "+" button to add a new item
   - Set the ID to "wood" and name to "Wood"
   - Set other properties as desired (icon, description, etc.)

2. **Planks** (product)
   
   - Add another item definition
   - Set the ID to "planks" and name to "Planks"
   - Configure the item properties

Step 3: Create the Recipe
--------------------------

1. **Navigate to Recipes**
   
   In the database editor, click on the "Recipes" section.

   .. image:: ./../manual/database/images/recipes_editor_open.png

2. **Add New Recipe**
   
   Click the "+" button to create a new recipe.

   .. image:: ./../manual/database/images/recipes_editor_add.png

3. **Configure Recipe Properties**
   
   Set the following properties for your wood-to-planks recipe:
   
   - **Time to Craft**: Set to 2.0 seconds for quick testing
   - **Station**: Leave empty for now (we'll create station types later)

   .. image:: ./images/recipe_editor_edit.png

4. **Add Ingredients**
   
   In the Ingredients section:
   
   - Click "+" to add an ingredient
   - Select "Wood" from the item dropdown
   - Set the amount to 1

5. **Add Products**
   
   In the Products section:
   
   - Click "+" to add a product
   - Select "Planks" from the item dropdown  
   - Set the amount to 4

6. **Save the Database**
   
   Save your InventoryDatabase resource (Ctrl+S).

Setting Up the Scene
====================

Step 4: Create a Test Scene
----------------------------

1. **Create New Scene**
   
   Create a new scene with a Node as root (name it "RecipeTest").

2. **Add Required Nodes**
   
   Add the following nodes as children of your root:
   
   - :ref:`CraftStation<class_CraftStation>` (name: "CraftStation")
   - :ref:`Inventory<class_Inventory>` (name: "InputInventory")  
   - :ref:`Inventory<class_Inventory>` (name: "OutputInventory")

3. **Configure the Inventories**
   
   For both inventory nodes:
   
   - Set the **Database** property to your InventoryDatabase resource
   - Set the **Size** to a reasonable value (e.g., 10)

4. **Configure the CraftStation**
   
   Select the CraftStation node and set:
   
   - **Database**: Your InventoryDatabase resource
   - **Input Inventories**: Add the path to InputInventory (click + and select the node)
   - **Output Inventories**: Add the path to OutputInventory
   - **Can Processing Craftings**: Enable this
   - **Can Finish Craftings**: Enable this

Step 5: Add a Control Script
-----------------------------

1. **Attach Script**
   
   Attach a script to your root node:

.. code-block:: gdscript

   extends Node
   
   @onready var craft_station = $CraftStation
   @onready var input_inventory = $InputInventory  
   @onready var output_inventory = $OutputInventory
   
   func _ready():
       # Add some wood to the input inventory for testing
       var wood_stack = ItemStack.new()
       wood_stack.item = input_inventory.database.get_item_definition_by_id("wood")
       wood_stack.amount = 5
       input_inventory.add_item_stack(wood_stack)
       
       # Connect craft station signals for feedback
       craft_station.on_request_craft.connect(_on_craft_requested)
       craft_station.on_crafted.connect(_on_craft_completed)
       
       # Print available recipes
       print("Available recipes: ", craft_station.valid_recipes.size())
       for i in craft_station.valid_recipes.size():
           var recipe_index = craft_station.valid_recipes[i]
           var recipe = craft_station.database.recipes[recipe_index]
           print("Recipe ", i, ": ", recipe.products[0].item.name)
   
   func _input(event):
       # Press Space to start crafting
       if event.is_action_pressed("ui_accept"):
           if craft_station.valid_recipes.size() > 0:
               print("Starting craft...")
               craft_station.craft(0)
   
   func _on_craft_requested(recipe_index: int):
       print("Craft requested for recipe: ", recipe_index)
   
   func _on_craft_completed(recipe_index: int):
       print("Craft completed! Check output inventory.")
       
       # Print output inventory contents
       for i in output_inventory.slots.size():
           var slot = output_inventory.slots[i]
           if slot.item_stack:
               var stack = slot.item_stack
               print("Output slot ", i, ": ", stack.amount, "x ", stack.item.name)

2. **Save and Run**
   
   Save the scene and run it (F6).

Testing the Recipe
==================

Step 6: Test the Crafting Process
----------------------------------

1. **Run the Scene**
   
   When you run the scene, you should see console output showing:
   - Available recipes (should be 1)
   - The recipe name (planks)

2. **Start Crafting**
   
   Press the Space bar to start crafting. You should see:
   - "Starting craft..." message
   - "Craft requested..." message
   - After 2 seconds: "Craft completed!" message
   - Output showing 4x Planks in the output inventory

3. **Verify the Process**
   
   The crafting process should:
   - Remove 1 wood from the input inventory
   - Wait for 2 seconds (the craft time)
   - Add 4 planks to the output inventory

Troubleshooting
===============

Recipe Not Available
--------------------

If no recipes are available, check:

- The database is properly assigned to the craft station
- The recipe ingredients exist in the input inventory
- The item IDs match between recipe and item definitions

Crafting Doesn't Start
----------------------

If crafting doesn't start when pressing Space:

- Verify the craft station has input and output inventories assigned
- Check that ingredients are available in the input inventory
- Ensure ``can_processing_craftings`` is enabled

Craft Doesn't Complete
----------------------

If the craft starts but doesn't finish:

- Check that ``can_finish_craftings`` is enabled
- Verify there's space in the output inventory
- Make sure the craft station is processing (tick method is set correctly)

Adding Visual Feedback
======================

Step 7: Create a Simple UI
---------------------------

To see the crafting process visually:

1. **Add UI Nodes**
   
   Add these nodes to your scene:
   
   - CanvasLayer
   - VBoxContainer (child of CanvasLayer)
   - Label (child of VBoxContainer, name: "StatusLabel")
   - Button (child of VBoxContainer, name: "CraftButton")

2. **Update the Script**

.. code-block:: gdscript

   extends Node
   
   @onready var craft_station = $CraftStation
   @onready var input_inventory = $InputInventory  
   @onready var output_inventory = $OutputInventory
   @onready var status_label = $CanvasLayer/VBoxContainer/StatusLabel
   @onready var craft_button = $CanvasLayer/VBoxContainer/CraftButton
   
   func _ready():
       # ... (previous code)
       
       # Connect button
       craft_button.pressed.connect(_on_craft_button_pressed)
       craft_button.text = "Craft Planks"
       
       # Update status
       _update_ui()
   
   func _update_ui():
       var wood_count = input_inventory.get_item_count("wood")
       var plank_count = output_inventory.get_item_count("planks")
       
       status_label.text = "Wood: %d | Planks: %d" % [wood_count, plank_count]
       
       # Enable button only if we can craft
       craft_button.disabled = not (craft_station.valid_recipes.size() > 0 and 
                                   craft_station.can_craft(craft_station.database.recipes[craft_station.valid_recipes[0]]))
   
   func _on_craft_button_pressed():
       if craft_station.valid_recipes.size() > 0:
           craft_station.craft(0)
   
   func _on_craft_completed(recipe_index: int):
       print("Craft completed!")
       _update_ui()  # Update UI after crafting

Next Steps
==========

Now that you have a working recipe, you can:

- Create more complex recipes with multiple ingredients
- Add required items (tools that aren't consumed)
- Create station types to organize recipes
- Build a full crafting UI system

.. seealso::
   
   - :ref:`create_and_use_craft_station_type` - Learn about station types
   - :ref:`craft_station_ui_tutorial` - Build a complete crafting system with UI
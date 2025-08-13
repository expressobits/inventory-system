.. _craft_station_ui_tutorial:

###########################################
CraftStation UI Tutorial
###########################################

This tutorial will guide you through setting up a complete basic crafting scene from scratch. You'll create a simple crafting area with input/output inventories, a craft station, and basic UI to demonstrate the full crafting workflow.

Prerequisites
=============

Before starting this tutorial, make sure you have:

- An inventory system project set up
- An :ref:`InventoryDatabase<class_InventoryDatabase>` resource with some items and recipes
- Basic understanding of Godot scene creation and scripting

Planning the Scene
==================

Our basic crafting scene will include:

- A craft station node for processing recipes
- Input inventory for raw materials
- Output inventory for finished products
- Simple UI to show inventory contents and crafting status
- Controls for starting crafts and monitoring progress

Scene Setup
===========

Step 1: Create the Main Scene
------------------------------

1. **Create New Scene**
   
   Create a new scene and add a Node as the root (name it "CraftingScene").

2. **Add Core Nodes**
   
   Add these nodes as children of the root:
   
   - :ref:`CraftStation<class_CraftStation>` (name: "CraftStation")
   - :ref:`Inventory<class_Inventory>` (name: "MaterialsInventory")
   - :ref:`Inventory<class_Inventory>` (name: "ProductsInventory")

3. **Add UI Structure**
   
   Add a UI layer for the interface:
   
   - CanvasLayer (name: "UI")
   - MarginContainer (child of UI)
   - VBoxContainer (child of MarginContainer)

Step 2: Configure the Inventories
----------------------------------

**Materials Inventory:**

1. Select the MaterialsInventory node
2. In the Inspector, set:
   - **Database**: Your InventoryDatabase resource
   - **Size**: 20 (enough space for various materials)

**Products Inventory:**

1. Select the ProductsInventory node  
2. In the Inspector, set:
   - **Database**: Your InventoryDatabase resource
   - **Size**: 10 (for finished products)

Step 3: Configure the Craft Station
------------------------------------

1. **Select the CraftStation node**

2. **Set Basic Properties**
   
   In the Inspector:
   - **Database**: Your InventoryDatabase resource
   - **Can Processing Craftings**: Enable
   - **Can Finish Craftings**: Enable
   - **Processing Mode**: Sequential (for this basic example)

3. **Connect Inventories**
   
   - **Input Inventories**: Click "+", then select MaterialsInventory
   - **Output Inventories**: Click "+", then select ProductsInventory

4. **Optional: Set Station Type**
   
   If you have created station types, you can assign one here to filter recipes.

Creating the UI
===============

Step 4: Build the Interface Layout
-----------------------------------

Under the VBoxContainer, add these UI elements:

1. **Title Label**
   
   - Add a Label (name: "TitleLabel")
   - Set text to "Basic Crafting Station"
   - Adjust font size if desired

2. **Status Section**
   
   - Add an HBoxContainer (name: "StatusContainer")
   - Add two Labels as children:
     - "MaterialsLabel" - to show materials count
     - "ProductsLabel" - to show products count

3. **Recipe List**
   
   - Add a Label (name: "RecipesLabel") with text "Available Recipes:"
   - Add a VBoxContainer (name: "RecipesList") for recipe buttons

4. **Controls Section**
   
   - Add an HBoxContainer (name: "ControlsContainer")
   - Add these buttons:
     - "AddMaterialsButton" - to add test materials
     - "CraftAllButton" - to craft all possible recipes

5. **Progress Section**
   
   - Add a Label (name: "ProgressLabel") to show crafting progress
   - Add a ProgressBar (name: "CraftProgressBar")

Your UI structure should look like:

.. code-block:: text

   UI (CanvasLayer)
   └── MarginContainer
       └── VBoxContainer
           ├── TitleLabel
           ├── StatusContainer (HBoxContainer)
           │   ├── MaterialsLabel
           │   └── ProductsLabel
           ├── RecipesLabel
           ├── RecipesList (VBoxContainer)
           ├── ControlsContainer (HBoxContainer)
           │   ├── AddMaterialsButton
           │   └── CraftAllButton
           ├── ProgressLabel
           └── CraftProgressBar

Scripting the Scene
===================

Step 5: Add the Main Script
----------------------------

Attach this script to the root CraftingScene node:

.. code-block:: gdscript

   extends Node
   
   # Node references
   @onready var craft_station = $CraftStation
   @onready var materials_inventory = $MaterialsInventory
   @onready var products_inventory = $ProductsInventory
   
   # UI references
   @onready var materials_label = $UI/MarginContainer/VBoxContainer/StatusContainer/MaterialsLabel
   @onready var products_label = $UI/MarginContainer/VBoxContainer/StatusContainer/ProductsLabel
   @onready var recipes_list = $UI/MarginContainer/VBoxContainer/RecipesList
   @onready var add_materials_button = $UI/MarginContainer/VBoxContainer/ControlsContainer/AddMaterialsButton
   @onready var craft_all_button = $UI/MarginContainer/VBoxContainer/ControlsContainer/CraftAllButton
   @onready var progress_label = $UI/MarginContainer/VBoxContainer/ProgressLabel
   @onready var craft_progress_bar = $UI/MarginContainer/VBoxContainer/CraftProgressBar
   
   var recipe_buttons = []
   
   func _ready():
       # Set up initial state
       _setup_ui()
       _connect_signals()
       _add_initial_materials()
       _update_ui()
   
   func _setup_ui():
       # Configure buttons
       add_materials_button.text = "Add Test Materials"
       craft_all_button.text = "Craft All Possible"
       
       # Set up progress bar
       craft_progress_bar.value = 0
       craft_progress_bar.visible = false
       
       # Create recipe buttons
       _create_recipe_buttons()
   
   func _connect_signals():
       # Connect craft station signals
       craft_station.on_request_craft.connect(_on_craft_requested)
       craft_station.crafting_added.connect(_on_crafting_added)
       craft_station.on_crafted.connect(_on_craft_completed)
       
       # Connect inventory signals for UI updates
       materials_inventory.item_changed.connect(_update_ui)
       products_inventory.item_changed.connect(_update_ui)
       
       # Connect button signals
       add_materials_button.pressed.connect(_add_test_materials)
       craft_all_button.pressed.connect(_craft_all_possible)
   
   func _create_recipe_buttons():
       # Clear existing buttons
       for button in recipe_buttons:
           button.queue_free()
       recipe_buttons.clear()
       
       # Create button for each available recipe
       for i in craft_station.valid_recipes.size():
           var recipe_index = craft_station.valid_recipes[i]
           var recipe = craft_station.database.recipes[recipe_index]
           
           var button = Button.new()
           var product_name = recipe.products[0].item.name if recipe.products.size() > 0 else "Unknown"
           button.text = "Craft " + product_name
           
           # Connect button to craft function
           button.pressed.connect(_craft_recipe.bind(i))
           
           recipes_list.add_child(button)
           recipe_buttons.append(button)
   
   func _add_initial_materials():
       # Add some basic materials for testing
       # This assumes you have "wood" in your database
       var wood_item = materials_inventory.database.get_item_definition_by_id("wood")
       if wood_item:
           var wood_stack = ItemStack.new()
           wood_stack.item = wood_item
           wood_stack.amount = 10
           materials_inventory.add_item_stack(wood_stack)
   
   func _add_test_materials():
       # Add various materials for testing recipes
       var materials_to_add = ["wood", "iron_ore", "coal", "stone"]
       
       for material_id in materials_to_add:
           var item = materials_inventory.database.get_item_definition_by_id(material_id)
           if item:
               var stack = ItemStack.new()
               stack.item = item
               stack.amount = 5
               materials_inventory.add_item_stack(stack)
   
   func _craft_recipe(recipe_list_index: int):
       if recipe_list_index < craft_station.valid_recipes.size():
           craft_station.craft(recipe_list_index)
   
   func _craft_all_possible():
       # Try to craft each available recipe once
       for i in craft_station.valid_recipes.size():
           var recipe_index = craft_station.valid_recipes[i]
           var recipe = craft_station.database.recipes[recipe_index]
           
           if craft_station.can_craft(recipe):
               craft_station.craft(i)
               break  # Only start one craft in sequential mode
   
   func _update_ui():
       # Update inventory counts
       var materials_count = _count_inventory_items(materials_inventory)
       var products_count = _count_inventory_items(products_inventory)
       
       materials_label.text = "Materials: " + str(materials_count) + " items"
       products_label.text = "Products: " + str(products_count) + " items"
       
       # Update recipe button states
       for i in recipe_buttons.size():
           var button = recipe_buttons[i]
           if i < craft_station.valid_recipes.size():
               var recipe_index = craft_station.valid_recipes[i]
               var recipe = craft_station.database.recipes[recipe_index]
               var can_craft = craft_station.can_craft(recipe)
               
               button.disabled = not can_craft
               if can_craft:
                   button.modulate = Color.WHITE
               else:
                   button.modulate = Color.GRAY
       
       # Update craft all button
       craft_all_button.disabled = not _has_craftable_recipes()
       
       # Update progress if crafting
       _update_progress()
   
   func _count_inventory_items(inventory: Inventory) -> int:
       var count = 0
       for slot in inventory.slots:
           if slot.item_stack:
               count += slot.item_stack.amount
       return count
   
   func _has_craftable_recipes() -> bool:
       for recipe_index in craft_station.valid_recipes:
           var recipe = craft_station.database.recipes[recipe_index]
           if craft_station.can_craft(recipe):
               return true
       return false
   
   func _update_progress():
       if craft_station.craftings.size() > 0:
           # Show progress for first active craft
           var crafting = craft_station.craftings[0]
           var recipe_index = crafting.get_recipe_index()
           var recipe = craft_station.database.recipes[recipe_index]
           
           var progress = crafting.get_time() / recipe.time_to_craft
           progress = clamp(progress, 0.0, 1.0)
           
           craft_progress_bar.value = progress * 100
           craft_progress_bar.visible = true
           
           var product_name = recipe.products[0].item.name if recipe.products.size() > 0 else "Unknown"
           progress_label.text = "Crafting " + product_name + "... " + str(int(progress * 100)) + "%"
       else:
           # No active crafts
           craft_progress_bar.visible = false
           progress_label.text = "No active crafts"
   
   # Signal handlers
   func _on_craft_requested(recipe_index: int):
       var recipe = craft_station.database.recipes[recipe_index]
       var product_name = recipe.products[0].item.name if recipe.products.size() > 0 else "Unknown"
       print("Craft requested: ", product_name)
   
   func _on_crafting_added(crafting_index: int):
       print("Crafting started: ", crafting_index)
       _update_ui()
   
   func _on_craft_completed(recipe_index: int):
       var recipe = craft_station.database.recipes[recipe_index]
       var product_name = recipe.products[0].item.name if recipe.products.size() > 0 else "Unknown"
       print("Craft completed: ", product_name)
       _update_ui()
   
   # Update progress every frame while crafting
   func _process(_delta):
       if craft_station.craftings.size() > 0:
           _update_progress()

Testing the Scene
=================

Step 6: Run and Test
--------------------

1. **Save and Run**
   
   Save your scene and run it (F6).

2. **Test Basic Functionality**
   
   You should see:
   - A title and status showing material/product counts
   - Recipe buttons (enabled/disabled based on availability)
   - Controls to add materials and craft items

3. **Try Crafting**
   
   - Click "Add Test Materials" to add more ingredients
   - Click individual recipe buttons to craft specific items
   - Click "Craft All Possible" to automatically craft available recipes
   - Watch the progress bar fill as items are being crafted

4. **Observe the Process**
   
   - Materials should decrease when crafting starts
   - Products should appear when crafting completes
   - Progress bar should show crafting progress
   - Buttons should enable/disable based on material availability

Troubleshooting
===============

No Recipes Available
--------------------

If no recipe buttons appear:

- Check that your database has recipes defined
- Verify the craft station's database is set correctly
- Ensure recipes match the station type (if any)

Crafting Doesn't Work
---------------------

If crafts don't start or complete:

- Verify input/output inventories are connected to the craft station
- Check that materials are available in the materials inventory
- Ensure the craft station has processing enabled

UI Not Updating
---------------

If the interface doesn't update properly:

- Check that inventory signals are connected
- Verify the _update_ui() function is being called
- Make sure node references are correct

Enhancements
============

Now that you have a basic crafting scene, you can enhance it by:

**Visual Improvements:**
- Add inventory display UIs to show items visually
- Include item icons and descriptions
- Add sound effects for crafting

**Gameplay Features:**
- Add drag-and-drop item management
- Implement crafting queues
- Add crafting failure mechanics
- Include fuel or power requirements

**UI Enhancements:**
- Show ingredient requirements for each recipe
- Add crafting time estimates
- Include item tooltips and descriptions

.. seealso::
   
   - :ref:`create_and_use_recipe` - Learn about recipe creation
   - :ref:`create_and_use_craft_station_type` - Understand station types

**Image Description Needed**: Screenshot showing the complete basic crafting scene running with materials/products counts, recipe buttons, progress bar, and control buttons laid out clearly.

**Image Description Needed**: Screenshot during crafting showing the progress bar partially filled and the status "Crafting Planks... 67%" with some recipe buttons disabled due to lack of materials.
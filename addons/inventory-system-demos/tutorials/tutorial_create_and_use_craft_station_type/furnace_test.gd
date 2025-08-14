extends Node

# Existing nodes from Tutorial 2
@onready var craft_station = $FurnaceStation
@onready var input_inventory = $InputInventory
@onready var output_inventory = $OutputInventory

# New UI nodes
@onready var input_label = $UI/MarginContainer/VBoxContainer/StatusContainer/InputLabel
@onready var output_label = $UI/MarginContainer/VBoxContainer/StatusContainer/OutputLabel
@onready var recipes_list = $UI/MarginContainer/VBoxContainer/RecipesList
@onready var add_materials_button = $UI/MarginContainer/VBoxContainer/ControlsContainer/AddMaterialsButton
@onready var smelt_all_button = $UI/MarginContainer/VBoxContainer/ControlsContainer/SmeltAllButton
@onready var progress_label = $UI/MarginContainer/VBoxContainer/ProgressLabel
@onready var progress_bar = $UI/MarginContainer/VBoxContainer/ProgressBar

var recipe_buttons = []

func _ready():
	# Original setup from Tutorial 2
	_setup_initial_materials()
	_setup_station()

	# New UI setup
	_setup_ui()
	_connect_ui_signals()
	_update_ui()

func _setup_initial_materials():
	# Add iron ore and coal (from Tutorial 2)
	input_inventory.add("iron_ore", 10)
	input_inventory.add("coal", 5)

func _setup_station():
	# Connect station signals
	craft_station.on_crafted.connect(_on_craft_completed)
	craft_station.crafting_added.connect(_on_crafting_started)

	# Print station info (from Tutorial 2)
	print("=== FURNACE STATION UI ===")
	print("Station Type: ", craft_station.type.name if craft_station.type else "None")
	print("Available recipes: ", craft_station.valid_recipes.size())

func _setup_ui():
	# Configure progress bar
	progress_bar.value = 0
	progress_bar.visible = false

	# Create recipe buttons based on valid furnace recipes
	_create_recipe_buttons()

func _connect_ui_signals():
	# Connect button signals
	add_materials_button.pressed.connect(_add_more_materials)
	smelt_all_button.pressed.connect(_smelt_all_possible)

	# Connect inventory signals for UI updates
	input_inventory.contents_changed.connect(_update_ui)
	output_inventory.contents_changed.connect(_update_ui)

func _create_recipe_buttons():
	# Clear existing buttons
	for button in recipe_buttons:
		button.queue_free()
	recipe_buttons.clear()

	# Create button for each furnace recipe
	for i in craft_station.valid_recipes.size():
		var recipe_index = craft_station.valid_recipes[i]
		var recipe = craft_station.database.recipes[recipe_index]

		var button = Button.new()
		var product_name = recipe.products[0].item_id if recipe.products.size() > 0 else "Unknown"
		button.text = "Smelt " + product_name

		button.pressed.connect(_craft_recipe.bind(i))
		recipes_list.add_child(button)
		recipe_buttons.append(button)

func _add_more_materials():
	# Add more smelting materials
	input_inventory.add("iron_ore", 5)
	input_inventory.add("coal", 3)
	print("Added more iron ore and coal")

func _smelt_all_possible():
	# Try to start smelting if possible
	if craft_station.valid_recipes.size() > 0:
		print("Starting furnace smelting...")
		craft_station.craft(0)

func _craft_recipe(recipe_list_index: int):
	if recipe_list_index < craft_station.valid_recipes.size():
		print("Starting specific recipe: ", recipe_list_index)
		craft_station.craft(recipe_list_index)

func _update_ui():
	# Update inventory status
	var input_count = _count_inventory_items(input_inventory)
	var output_count = _count_inventory_items(output_inventory)

	input_label.text = "Input Materials: " + str(input_count) + " items"
	output_label.text = "Output Products: " + str(output_count) + " items"

	# Update recipe button availability
	for i in recipe_buttons.size():
		var button = recipe_buttons[i]
		if i < craft_station.valid_recipes.size():
			var recipe_index = craft_station.valid_recipes[i]
			var recipe = craft_station.database.recipes[recipe_index]
			var can_craft = craft_station.can_craft(recipe)

			button.disabled = not can_craft
			button.modulate = Color.WHITE if can_craft else Color.GRAY

	# Update smelt all button
	smelt_all_button.disabled = not _has_craftable_recipes()

	# Update progress
	_update_progress()

func _count_inventory_items(inventory: Inventory) -> int:
	var count = 0
	for stack in inventory.stacks:
		if stack:
			count += stack.amount
	return count

func _has_craftable_recipes() -> bool:
	for recipe_index in craft_station.valid_recipes:
		var recipe = craft_station.database.recipes[recipe_index]
		if craft_station.can_craft(recipe):
			return true
	return false

func _update_progress():
	if craft_station.craftings.size() > 0:
		var crafting = craft_station.craftings[0]
		var recipe_index = crafting.get_recipe_index()
		var recipe = craft_station.database.recipes[recipe_index]

		var progress = crafting.get_time() / recipe.time_to_craft
		progress = clamp(progress, 0.0, 1.0)

		progress_bar.value = progress * 100
		progress_bar.visible = true

		var product_name = recipe.products[0].item_id if recipe.products.size() > 0 else "Unknown"
		progress_label.text = "Smelting " + product_name + "... " + str(int(progress * 100)) + "%"
	else:
		progress_bar.visible = false
		progress_label.text = "Furnace ready"

# Enhanced signal handlers
func _on_crafting_started(crafting_index: int):
	print("Smelting started: ", crafting_index)
	_update_ui()

func _on_craft_completed(recipe_index: int):
	print("Smelting completed!")

	# Show output (enhanced from Tutorial 2)
	for i in output_inventory.stacks.size():
		var stack = output_inventory.stacks[i]
		if stack:
			print("Produced: ", stack.amount, "x ", stack.item_id)

	_update_ui()

# Continue updating progress during smelting
func _process(_delta):
	if craft_station.craftings.size() > 0:
		_update_progress()

# Keep the space key functionality from Tutorial 2
func _input(event):
	if event.is_action_pressed("ui_accept"):
		_smelt_all_possible()

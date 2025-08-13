extends Node

@onready var craft_station = $WorkbenchStation
@onready var input_inventory = $InputInventory
@onready var output_inventory = $OutputInventory

func _ready():
	# Add test items
	input_inventory.add("wood", 30)

	# Connect signals
	craft_station.on_crafted.connect(_on_craft_completed)

	# Print station info
	print("=== WORKBENCH STATION ===")
	print("Station Type: ", craft_station.type.name if craft_station.type else "None")
	print("Available recipes: ", craft_station.valid_recipes.size())

	# List available recipes
	for i in craft_station.valid_recipes.size():
		var recipe_index = craft_station.valid_recipes[i]
		var recipe = craft_station.database.recipes[recipe_index]
		var product_name = recipe.products[0].item_id if recipe.products.size() > 0 else "Unknown"
		print("Recipe ", i, ": ", product_name)
		
		var can_craft = craft_station.can_craft(recipe)
		print("  Can craft: ", can_craft)

func _input(event):
	if event.is_action_pressed("ui_accept"):
		if craft_station.valid_recipes.size() > 0:
			print("Starting crafting...")
			craft_station.craft(1)
		else:
			print("No recipes available!")

func _on_craft_completed(recipe_index: int):
	print("Crafting completed!")

	# Show output
	for i in output_inventory.stacks.size():
		var stack = output_inventory.stacks[i]
		if stack:
			print("Produced: ", stack.amount, "x ", stack.item_id)

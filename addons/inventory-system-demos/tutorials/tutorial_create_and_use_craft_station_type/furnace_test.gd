extends Node

@onready var craft_station = $FurnaceStation
@onready var input_inventory = $InputInventory
@onready var output_inventory = $OutputInventory

func _ready():
	# Add iron ore
	input_inventory.add("iron_ore", 10)

	# Add coal (fuel)
	input_inventory.add("coal", 5)

	# Connect signals
	craft_station.on_crafted.connect(_on_craft_completed)

	# Print station info
	print("=== FURNACE STATION ===")
	print("Station Type: ", craft_station.type.name if craft_station.type else "None")
	print("Available recipes: ", craft_station.valid_recipes.size())

	# List available recipes
	for i in craft_station.valid_recipes.size():
		var recipe_index = craft_station.valid_recipes[i]
		var recipe = craft_station.database.recipes[recipe_index]
		var product_name = recipe.products[0].item_id if recipe.products.size() > 0 else "Unknown"
		print("Recipe ", i, ": ", product_name)

		# Check if we can craft it
		var can_craft = craft_station.can_craft(recipe)
		print("  Can craft: ", can_craft)
	
	

func _input(event):
	if event.is_action_pressed("ui_accept"):
		if craft_station.valid_recipes.size() > 0:
			print("Starting furnace smelting...")
			craft_station.craft(0)
		else:
			print("No recipes available!")
	

func _on_craft_completed(recipe_index: int):
	print("Smelting completed!")

	# Show output
	for i in output_inventory.stacks.size():
		var stack = output_inventory.stacks[i]
		if stack:
			print("Produced: ", stack.amount, "x ", stack.item_id)

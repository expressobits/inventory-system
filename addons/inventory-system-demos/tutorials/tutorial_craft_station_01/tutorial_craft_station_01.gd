extends Node3D

@onready var craft_station: CraftStation = $CraftStation
@onready var inventory: Inventory = $CraftStation/Inventory


func _process(delta: float) -> void:
	if Input.is_action_just_pressed("add_item_a"):
		inventory.add("raw_meat")
		print_inventory()
		
	if Input.is_action_just_pressed("interact"):
		print_inventory()
		
	if Input.is_action_just_pressed("item_place"):
		print_recipes()


func print_recipes():
	print("Recipes:")
	var count = 0
	for recipe_index in craft_station.valid_recipes:
		var recipe = craft_station.database.recipes[recipe_index]
		print(recipe.products[0].item_id)
		count += 1


func print_inventory():
	print("Inventory Items:")
	for item in inventory.stacks:
		if item.item_id != "":
			print(item.item_id," x ", item.amount)
		else:
			print("Empty")

@icon("res://addons/inventory-system/icons/craft-station.svg")
extends NodeInventorySystemBase
class_name CraftStation

signal on_crafted(recipe_index : Recipe)
signal on_request_craft(recipe_index : Recipe)
signal on_add_crafting_at(crafting_index : int)
signal on_remove_crafting_at(crafting_index : int)
signal on_updated_crafting(crafting_index : int)

class Crafting:
	var recipe_index : int
	var time : float
	
	func is_finished() -> bool:
		return time <= 0
		
	func process(delta : float):
		time -= delta

@export var input_inventory : Inventory
@export var output_inventory : Inventory
@export var limit_number_crafts := -1
@export var can_processing_craftings := true
@export var type : CraftStationType

var craftings : Array[Crafting]


func is_crafting() -> bool:
	return not craftings.is_empty()


func crafting_count() -> int:
	return craftings.size()
	
	
func _process(delta):
	if not can_processing_craftings:
		return
	if not is_crafting():
		return
	for i in range(craftings.size() - 1, -1, -1):
		var c = craftings[i]
		# TODO set start time in crafting only (Problem with load game ?)
		c.time -= delta
		if c.time <= 0:
			finish_crafting(c.recipe_index)

	
func finish_crafting(crafting_index : int):
	var crafting = craftings[crafting_index]
	var recipe = database.recipes[crafting.recipe_index]
	# TODO add function for slot in inventory
	for product in recipe.products:
		output_inventory.add(product.item, product.amount)
	emit_signal("on_crafted", crafting.recipe_index)
	remove_crafting(crafting_index)
	
	
func remove_crafting(crafting_index : int):
	if crafting_index >= craftings.size():
		return
	craftings.remove_at(crafting_index)
	emit_signal("on_remove_crafting_at", crafting_index)
	

## Check if it is possible to create this recipe
## It is checked if the crafts limit has been exceeded and then it is checked if the recipe items contain in the inventory
func can_craft(recipe : Recipe) -> bool:
	if limit_number_crafts >= 0 and limit_number_crafts <= craftings.size():
		return false
	return contains_required_items(recipe)


func contains_required_items(recipe : Recipe) -> bool:
	for slot in recipe.required_items:
		if not input_inventory.contains(slot.item, slot.amount):
			return false
	return true


func craft(recipe_index : int):
	var recipe := database.recipes[recipe_index]
	if not can_craft(recipe):
		return
	if not _use_items(recipe):
		return
	var crafting = Crafting.new()
	crafting.recipe_index = recipe_index
	crafting.time = recipe.time_to_craft
	craftings.append(crafting)
	emit_signal("on_add_crafting_at", craftings.size() - 1)
	emit_signal("on_request_craft", recipe_index)


func cancel_craft(crafting_index : int):
	if crafting_index < 0 or crafting_index >= craftings.size():
		return
	var crafting = craftings[crafting_index]
	if crafting.is_finished():
		return
	var recipe = database.recipes[crafting.recipe_index]
	for slot in recipe.required_items:
		input_inventory.add(slot.item, slot.amount)
	remove_crafting(crafting_index)
	

func _use_items(recipe : Recipe) -> bool:
	for slot in recipe.required_items:
		if input_inventory.remove(slot.item, slot.amount) > 0:
			return false
	return true

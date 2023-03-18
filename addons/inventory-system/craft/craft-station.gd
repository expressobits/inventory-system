@tool
@icon("res://addons/inventory-system/icons/craft-station.svg")
extends NodeInventorySystemBase
class_name CraftStation

signal on_crafted(recipe_index : Recipe)
signal on_request_craft(recipe_index : Recipe)
signal crafting_added(crafting_index : int)
signal crafting_removed(crafting_index : int)
signal updated_crafting(crafting_index : int)

## Emitted when craft station is opened.
## Called inside the [b]open()[/b] function when the craft station is closed.
signal opened

## Emitted when craft station is closed.
## Called inside the [b]close()[/b] function when the craft station is closed.
signal closed

class Crafting:
	var recipe_index : int
	var time : float
	
	func is_finished() -> bool:
		return time <= 0
		
	func process(delta : float):
		time -= delta
		
	func to_data() -> Array:
		return [recipe_index,time]
		
	func from_data(data : Array):
		recipe_index = data[0]
		time = data[1]
		

@export var input_inventory : Inventory
@export var output_inventory : Inventory
@export var limit_number_crafts := -1
@export var can_processing_craftings := true
@export var type : CraftStationType

var craftings : Array[Crafting]
var is_open : bool
var valid_recipes : Array[int]


func _ready():
	for i in database.recipes.size():
		var recipe = database.recipes[i]
		if recipe.station == type:
			valid_recipes.append(i)


func is_crafting() -> bool:
	return not craftings.is_empty()


func crafting_count() -> int:
	return craftings.size()


## Check if it is possible to create this recipe
## It is checked if the crafts limit has been exceeded and then it is checked if the recipe items contain in the inventory
func can_craft(recipe : Recipe) -> bool:
	if recipe.station != type:
		return false
	if limit_number_crafts >= 0 and limit_number_crafts <= craftings.size():
		return false
	return contains_ingredients(recipe)


func contains_ingredients(recipe : Recipe) -> bool:
	for slot in recipe.ingredients:
		if not input_inventory.contains(slot.item, slot.amount):
			return false
	return true


func craft(recipe_index : int):
	var recipe := database.recipes[recipe_index]
	if recipe.station != type:
		return false
	if not can_craft(recipe):
		return
	if not _use_items(recipe):
		return
	_add_crafting(recipe_index, recipe)


func cancel_craft(crafting_index : int):
	if crafting_index < 0 or crafting_index >= craftings.size():
		return
	var crafting = craftings[crafting_index]
	if crafting.is_finished():
		return
	var recipe = database.recipes[crafting.recipe_index]
	for ingredient in recipe.ingredients:
		input_inventory.add(ingredient.item, ingredient.amount)
	_remove_crafting(crafting_index)
	
	
## Opens the craft station and returns true if done successfully.
## Emits the [b]opened[/b] signal if the was previously closed.
func open() -> bool:
	if !is_open:
		is_open = true
		emit_signal("opened")
		return true
	return false
	

## Closes the craft station and returns true if done successfully.
## Emits the [b]closed[/b] signal if was previously open.
func close() -> bool:
	if is_open:
		is_open = false
		emit_signal("closed")
		return true
	return false


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
			_finish_crafting(i)


func _finish_crafting(crafting_index : int):
	var crafting = craftings[crafting_index]
	var recipe = database.recipes[crafting.recipe_index]
	# TODO add function for slot in inventory
	output_inventory.add(recipe.product.item, recipe.product.amount)
	for subproduct in recipe.subproducts:
		output_inventory.add(subproduct.item, subproduct.amount)
	emit_signal("on_crafted", crafting.recipe_index)
	_remove_crafting(crafting_index)


func _use_items(recipe : Recipe) -> bool:
	if recipe.station != type:
		return false
	for ingredient in recipe.ingredients:
		if input_inventory.remove(ingredient.item, ingredient.amount) > 0:
			return false
	return true


func _add_crafting(recipe_index : int, recipe : Recipe):
	var crafting = Crafting.new()
	crafting.recipe_index = recipe_index
	crafting.time = recipe.time_to_craft
	craftings.append(crafting)
	emit_signal("crafting_added", craftings.size() - 1)
	emit_signal("on_request_craft", recipe_index)
	

func _remove_crafting(crafting_index : int):
	if crafting_index >= craftings.size():
		return
	emit_signal("crafting_removed", crafting_index)
	craftings.remove_at(crafting_index)

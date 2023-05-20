@tool
@icon("res://addons/inventory-system/icons/craft-station.svg")
extends NodeInventorySystemBase
class_name CraftStation

## This script create Craftings and stores an array.
##
## Crafting contains time to finish and recipe index from database.


## Emitted when the [Recipe] is crafted
## Called after the [Recipe]'s crafting time is up
signal on_crafted(recipe_index : Recipe)

## Emitted  when a new craft is required
## Called as soon as the craft function is called
signal on_request_craft(recipe_index : Recipe)

## Emitted when a new craft is added to the station
## Called when craft is called and the following requirements are met:
## - Have the recipe [InventoryItem]s
## - Be the [Recipe] [CraftStationType]'s station
## - Items successfully removed from input [Inventory]
signal crafting_added(crafting_index : int)

## Emitted when a craft is removed from the station
## Called when a craft has finished or when this craft has been cancelled.
signal crafting_removed(crafting_index : int)

## Emitted when craft station is opened.
## Called inside the [b]open()[/b] function when the craft station is closed.
signal opened

## Emitted when craft station is closed.
## Called inside the [b]close()[/b] function when the craft station is closed.
signal closed
		

## [Inventory] used to obtain crafting recipe ingredients
@export var input_inventory : Inventory

## [Inventory] used to store the product and by-products of crafts
@export var output_inventory : Inventory

## Maximum number of crafts you can have in this station
## Set -1 to have no limit
@export var limit_number_crafts := -1

## Enable time processing of crafts by _process()
@export var can_processing_craftings := true

## The type of this craftstation, this type defines which recipes can be created in that station
@export var type : CraftStationType

## Start crafting automatically if you have an item available.
@export var auto_craft : bool:
	set(new_value):
		if auto_craft != new_value and input_inventory != null:
			if new_value:
				input_inventory.item_added.connect(_on_input_inventory_changed.bind())
			else:
				input_inventory.item_added.disconnect(_on_input_inventory_changed.bind())
		auto_craft = new_value
		_check_for_auto_crafts()


## Current active craftings in this station
var craftings : Array[Crafting]

## It stores information if this station is open or not.
var is_open : bool

## Stores possible recipes for this station.
## This value is defined in the _ready() function using the comparison of the CraftStationType variable with the [InventoryDatabase] recipes
var valid_recipes : Array[int]


func _ready():
	for i in database.recipes.size():
		var recipe = database.recipes[i]
		if recipe.station == type:
			valid_recipes.append(i)


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


## Returns true if there are craftings being created by this station.
func is_crafting() -> bool:
	return not craftings.is_empty()


## Returns the current amount of craftings for this station.
func crafting_count() -> int:
	return craftings.size()


## Check if it is possible to create this recipe.
## It is checked if the crafts limit has been exceeded and then it is checked 
## if the recipe items contain in the inventory.
func can_craft(recipe : Recipe) -> bool:
	if recipe.station != type:
		return false
	if limit_number_crafts >= 0 and limit_number_crafts <= craftings.size():
		return false
	return contains_ingredients(recipe)


## Returns true if the input [Inventory] of this station contains the 
## ingredients of the [Recipe] sent by parameter.
func contains_ingredients(recipe : Recipe) -> bool:
	for slot in recipe.ingredients:
		if not input_inventory.contains(slot.item, slot.amount):
			return false
	return true


## Starts a new craft by [Recipe] sent by parameter.
## Note: This new craft will only be created if it has ingredients in the input 
## [Inventory] and it is possible to add a new craft at the station.
func craft(recipe_index : int):
	var recipe := database.recipes[recipe_index]
	emit_signal("on_request_craft", recipe_index)
	if not can_craft(recipe):
		return
	if not _use_items(recipe):
		return
	_add_crafting(recipe_index, recipe)


## This function removes a craft from the crafting list, 
## returning the ingredients to the input inventory.
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


func _finish_crafting(crafting_index : int):
	var crafting = craftings[crafting_index]
	var recipe = database.recipes[crafting.recipe_index]
	# TODO add function for slot in inventory
	output_inventory.add(recipe.product.item, recipe.product.amount)
	for subproduct in recipe.byproducts:
		output_inventory.add(subproduct.item, subproduct.amount)
	emit_signal("on_crafted", crafting.recipe_index)
	_remove_crafting(crafting_index)
	_check_for_auto_crafts()


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
	

func _remove_crafting(crafting_index : int):
	if crafting_index >= craftings.size():
		return
	emit_signal("crafting_removed", crafting_index)
	craftings.remove_at(crafting_index)


func _on_input_inventory_changed(item : InventoryItem, amount : int):
	_check_for_auto_crafts()


func _check_for_auto_crafts():
	if not auto_craft:
		return
	for i in valid_recipes:
		var recipe = database.recipes[i]
		if not can_craft(recipe):
			continue
		craft(i)


## Class that contain crafting information being processed.
class Crafting:
	
	## [Recipe] index of [Inventory Database] recipe list linked to this station.
	var recipe_index : int
	
	## Current time of the craft being processed, when it reaches zero the craft is completed
	var time : float
	
	## Returns true if the craft was completed
	func is_finished() -> bool:
		return time <= 0
	
	## Function that processes the craft
	func process(delta : float):
		time -= delta
	
	## Converts this craft to a [Dictionary], used for multiplayer synchronization
	func to_data() -> Array:
		return [recipe_index,time]
	
	## Sets crafting values based on a [Dictionary] received by parameter, Used in multiplayer synchronization
	func from_data(data : Array):
		recipe_index = data[0]
		time = data[1]

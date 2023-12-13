@tool
@icon("res://addons/inventory-system/icons/craft_station.svg")
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
		

## [Array[Inventory]] used to obtain crafting recipe ingredients
@export var input_inventories : Array[Inventory]

## [Array[Inventory]] used to store the products of crafts
@export var output_inventories : Array[Inventory]

## Maximum number of crafts you can have in this station
## Set -1 to have no limit
@export var limit_number_crafts := -1

## Enable time processing of crafts by _process()
@export var can_processing_craftings := true

## The type of this craftstation, this type defines which recipes can be created in that station
@export var type : CraftStationType

## Only remove ingredients from the inventory when the craft is finished
## Note: This only works when the limit_number_crafts is 1
@export var only_remove_ingredients_after_craft := false

## Start crafting automatically if you have an item available.
@export var auto_craft : bool:
	set(new_value):
		auto_craft = new_value
		_check_for_auto_crafts()

## Craft processing mode:
## - Sequential: only one craft is processed
## - Parallel: all crafts process together
@export var processing_mode : ProcessingMode = ProcessingMode.PARALLEL


## Current active craftings in this station
var craftings : Array[Crafting]

## It stores information if this station is open or not.
var is_open : bool

## Stores possible recipes for this station.
## This value is defined in the _ready() function using the comparison of the CraftStationType variable with the [InventoryDatabase] recipes
var valid_recipes : Array[int]


enum ProcessingMode { PARALLEL, SEQUENTIAL }


func _ready():
	for i in database.recipes.size():
		var recipe = database.recipes[i]
		if recipe.station == type:
			valid_recipes.append(i)
	for input_inventory in input_inventories:
		input_inventory.item_added.connect(_on_input_inventory_item_added.bind())
		input_inventory.item_removed.connect(_on_input_inventory_item_removed.bind())


func _process(delta : float):
	if not can_processing_craftings:
		return
	if not is_crafting():
		return
	_process_crafts(delta)
	
	for i in range(craftings.size() - 1, -1, -1):
		var c = craftings[i]
		if c.is_finished():
			_finish_crafting(i)
			return


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
		var amount_total : int = 0
		for input_inventory in input_inventories:
			amount_total += input_inventory.get_amount_of(slot.item)
		if amount_total < slot.amount:
			return false
	for slot in recipe.required_items:
		var amount_total : int = 0
		for input_inventory in input_inventories:
			amount_total += input_inventory.get_amount_of(slot.item)
		if amount_total < slot.amount:
			return false
	return true


## Starts a new craft by [Recipe] sent by parameter.
## Note: This new craft will only be created if it has ingredients in the input 
## [Inventory] and it is possible to add a new craft at the station.
func craft(recipe_index : int):
	var recipe := database.recipes[recipe_index]
	on_request_craft.emit(recipe_index)
	if not can_craft(recipe):
		return
	if not only_remove_ingredients_after_craft:
		if not _use_items(recipe):
			return
	_add_crafting(recipe_index, recipe)


## This function removes a craft from the crafting list, 
## returning the ingredients to the input inventory.
func cancel_craft(crafting_index : int):
	if crafting_index < 0 or crafting_index >= craftings.size():
		return
	var crafting = craftings[crafting_index]
	var recipe = database.recipes[crafting.recipe_index]
	if not only_remove_ingredients_after_craft:
		for ingredient in recipe.ingredients:
			input_inventories[0].add(ingredient.item, ingredient.amount)
	_remove_crafting(crafting_index)
	
	
## Opens the craft station and returns true if done successfully.
## Emits the [b]opened[/b] signal if the was previously closed.
func open() -> bool:
	if !is_open:
		is_open = true
		opened.emit()
		return true
	return false
	

## Closes the craft station and returns true if done successfully.
## Emits the [b]closed[/b] signal if was previously open.
func close() -> bool:
	if is_open:
		is_open = false
		closed.emit()
		return true
	return false


func _process_crafts(delta : float):
	for i in craftings.size():
		if processing_mode == ProcessingMode.SEQUENTIAL and i > 0:
			return
		var c = craftings[i]
		# TODO set start time in crafting only (Problem with load game ?)
		if not c.is_finished():
			c.time -= delta


func _finish_crafting(crafting_index : int):
	var crafting = craftings[crafting_index]
	var recipe = database.recipes[crafting.recipe_index]
	
	_remove_crafting(crafting_index)
	if only_remove_ingredients_after_craft:
		if not contains_ingredients(recipe):
			cancel_craft(crafting_index)
			return
		_use_items(recipe)
	# TODO add function for slot in inventory
	for product in recipe.products:
		var amount_to_add = product.amount
		for output_inventory in output_inventories:
			amount_to_add = output_inventory.add(product.item, product.amount)
	on_crafted.emit(crafting.recipe_index)
	_check_for_auto_crafts()


func _use_items(recipe : Recipe) -> bool:
	if recipe.station != type:
		return false
	for ingredient in recipe.ingredients:
		var amount_to_remove = ingredient.amount
		for input_inventory in input_inventories:
			amount_to_remove = input_inventory.remove(ingredient.item, amount_to_remove)
		if amount_to_remove > 0:
			return false
	return true


func _add_crafting(recipe_index : int, recipe : Recipe):
	var crafting = Crafting.new()
	crafting.recipe_index = recipe_index
	crafting.time = recipe.time_to_craft
	craftings.append(crafting)
	crafting_added.emit(craftings.size() - 1)
	

func _remove_crafting(crafting_index : int):
	if crafting_index >= craftings.size():
		return
	crafting_removed.emit(crafting_index)
	craftings.remove_at(crafting_index)


func _on_input_inventory_item_added(_item : SlotItem, _amount : int):
	if auto_craft:
		_check_for_auto_crafts()


func _on_input_inventory_item_removed(_item : SlotItem, _amount : int):
	if only_remove_ingredients_after_craft:
		var i = 0
		while i < craftings.size():
			var craft = craftings[i]
			var recipe = database.recipes[craft.recipe_index]
			if not contains_ingredients(recipe):
				cancel_craft(i)
				continue
			i = i + 1


func _check_for_auto_crafts():
	if not auto_craft:
		return
	for i in valid_recipes:
		var recipe = database.recipes[i]
		if not can_craft(recipe):
			continue
		craft(i)


func _get_configuration_warnings() -> PackedStringArray:
	var string_array = super._get_configuration_warnings()
	if only_remove_ingredients_after_craft and limit_number_crafts != 1:
		string_array.append("'only_remove_ingredients_after_craft' requires limit_number_crafts to be 1!")
	return string_array


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

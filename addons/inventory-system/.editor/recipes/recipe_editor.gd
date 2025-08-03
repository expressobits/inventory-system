@tool
class_name RecipeEditor
extends Control


signal changed_products(recipe : Recipe)
signal changed

@onready var time_to_craft_spin_box : SpinBox = $MarginContainer/MarginContainer/VBoxContainer/TimeToCraft/TimeToCraftSpinBox
@onready var craft_station_type_option_button = $MarginContainer/MarginContainer/VBoxContainer/CraftStationType/CraftStationTypeOptionButton
@onready var ingredients_v_box_container = %IngredientsVBoxContainer
@onready var products_v_box_container = %ProductsVBoxContainer
@onready var required_items_v_box_container = %RequiredItemsVBoxContainer
@onready var new_required_item_button = $MarginContainer/MarginContainer/VBoxContainer/ExtraRequiredItems/RequiredItemsVBoxContainer/NewRequiredItemButton


var editor_plugin : EditorPlugin
var recipe : Recipe
var database : InventoryDatabase
var ids_list : Array[int]
var stations_list : Array[CraftStationType]
var ingredients : Array[IngredientEditor]
var products : Array[IngredientEditor]
var required_items : Array[IngredientEditor]

var connected : bool

func _ready():
	apply_theme()


func apply_theme() -> void:
	if not is_instance_valid(editor_plugin):
		return
	
	#Dialogs
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	apply_theme()


func connect_signals():
	time_to_craft_spin_box.value_changed.connect(_on_time_to_craft_spin_box_value_changed)
	craft_station_type_option_button.item_selected.connect(_on_craft_station_type_option_button_item_selected)
	new_required_item_button.pressed.connect(_on_new_required_item_button_pressed)
	connected = true


func disconnect_signals():
	if not connected:
		return
	#TODO Change list
#	product_selector.slot_changed.disconnect(_on_product_slot_spin_box_slot_changed)
	time_to_craft_spin_box.value_changed.disconnect(_on_time_to_craft_spin_box_value_changed)
	craft_station_type_option_button.item_selected.disconnect(_on_craft_station_type_option_button_item_selected)
	new_required_item_button.pressed.disconnect(_on_new_required_item_button_pressed)
	connected = false


func setup_station():
	craft_station_type_option_button.clear()
	stations_list.clear()
	var popup : PopupMenu = craft_station_type_option_button.get_popup()
	for station in database.stations_type:
		craft_station_type_option_button.add_icon_item(station.icon, station.name)
		stations_list.append(station)
	for i in database.stations_type.size():
		popup.set_item_icon_max_width(i, 24)
	stations_list.append(null)
	craft_station_type_option_button.add_icon_item(null, "No station")
	
	for j in stations_list.size():
		var station = null
		if j < database.stations_type.size() and j >= 0:
			station = database.stations_type[j]
		if station == recipe.station:
			craft_station_type_option_button.select(j)
			

func setup_ingredients(recipe : Recipe, database : InventoryDatabase):
	for ingredient_editor in ingredients:
		ingredient_editor.queue_free()
	ingredients.clear()
	for index in recipe.ingredients.size():
		var ingredient = recipe.ingredients[index]
		var ingredient_node = IngredientEditor.new()
		ingredients_v_box_container.add_child(ingredient_node)
		var ingredient_editor : IngredientEditor = ingredient_node as IngredientEditor
		ingredient_editor.setup(ingredient, database, "Remove Ingredient")
		ingredient_editor.changed_slot.connect(_on_changed_slot_in_ingredient)
		ingredient_editor.request_remove.connect(_request_remove_ingredient.bind(index))
		ingredients.append(ingredient_editor)


func setup_required_items(recipe : Recipe, database : InventoryDatabase):
	for ingredient_editor in required_items:
		ingredient_editor.queue_free()
	required_items.clear()
	for index in recipe.required_items.size():
		var ingredient = recipe.required_items[index]
		var ingredient_node = IngredientEditor.new()
		required_items_v_box_container.add_child(ingredient_node)
		var ingredient_editor : IngredientEditor = ingredient_node as IngredientEditor
		ingredient_editor.setup(ingredient, database, "Remove Ingredient")
		ingredient_editor.changed_slot.connect(_on_changed_slot_required_item)
		ingredient_editor.request_remove.connect(_request_remove_required_item.bind(index))
		required_items.append(ingredient_editor)


func setup_products(recipe : Recipe, database : InventoryDatabase):
	for product_editor in products:
		product_editor.queue_free()
	products.clear()
	for index in recipe.products.size():
		var product = recipe.products[index]
		var ingredient_node = IngredientEditor.new()
		products_v_box_container.add_child(ingredient_node)
		var ingredient_editor : IngredientEditor = ingredient_node as IngredientEditor
		ingredient_editor.setup(product, database, "Remove Product")
		ingredient_editor.changed_slot.connect(_on_changed_slot_in_ingredient)
		ingredient_editor.request_remove.connect(_request_remove_product.bind(index))
		products.append(ingredient_editor)


func load_recipe(recipe : Recipe, database : InventoryDatabase):
	disconnect_signals()
	self.recipe = recipe
	self.database = database
	time_to_craft_spin_box.value = recipe.time_to_craft
	setup_station()
	setup_ingredients(recipe, database)
	setup_products(recipe, database)
	setup_required_items(recipe, database)
	connect_signals()


func reload():
	load_recipe(recipe, database)


func _on_time_to_craft_spin_box_value_changed(value):
	if recipe.time_to_craft != value:
		recipe.time_to_craft = value
		changed.emit()


func _on_craft_station_type_option_button_item_selected(index : int):
	if recipe.station != stations_list[index]:
		recipe.station = stations_list[index]
		changed.emit()


func _request_remove_ingredient(index):
	recipe.ingredients.remove_at(index)
	setup_ingredients(recipe, database)
	changed.emit()


func _request_remove_required_item(index):
	recipe.required_items.remove_at(index)
	setup_required_items(recipe, database)
	changed.emit()


func _request_remove_product(index):
	recipe.products.remove_at(index)
	setup_products(recipe, database)
	changed.emit()


func _on_new_ingredient_button_pressed():
	var item_stack = ItemStack.new()
	item_stack.amount = 1
	if not database.items.is_empty():
		item_stack.item_id = database.items[0].id
	recipe.ingredients.append(item_stack)
	setup_ingredients(recipe, database)
	changed.emit()


func _on_new_required_item_button_pressed():
	var item_stack = ItemStack.new()
	item_stack.amount = 1
	if not database.items.is_empty():
		item_stack.item_id = database.items[0].id
	recipe.required_items.append(item_stack)
	setup_required_items(recipe, database)
	changed.emit()


func _on_new_product_button_pressed():
	var item_stack = ItemStack.new()
	item_stack.amount = 1
	if not database.items.is_empty():
		item_stack.item_id = database.items[0].id
	recipe.products.append(item_stack)
	setup_products(recipe, database)
	changed.emit()
	changed_products.emit(recipe)


func _on_changed_slot_in_ingredient():
	changed.emit()


func _on_changed_slot_required_item():
	changed.emit()


func _on_recipe_resource_file_dialog_file_selected(path):
	var file = load(path)
	if file is Recipe:
		var recipe : Recipe = file
		load_recipe(recipe, database)
		changed.emit()
		changed_products.emit(recipe)
	else:
		print("Error on open scene!")

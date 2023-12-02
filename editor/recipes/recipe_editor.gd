@tool
extends Control
class_name RecipeEditor


signal changed_products(recipe : Recipe)
signal changed

@onready var time_to_craft_spin_box : SpinBox = $MarginContainer/MarginContainer/VBoxContainer/TimeToCraft/TimeToCraftSpinBox
@onready var craft_station_type_option_button = $MarginContainer/MarginContainer/VBoxContainer/CraftStationType/CraftStationTypeOptionButton
@onready var item_resource_text_edit : LineEdit = %RecipeResourceLineEdit
@onready var item_resource_edit_button : Button = %RecipeResourceEditButton
@export var ingredient_scene : PackedScene = preload("res://addons/inventory-system/editor/recipes/ingredient_editor.tscn")
@onready var ingredients_v_box_container = %IngredientsVBoxContainer
@onready var products_v_box_container = %ProductsVBoxContainer
@onready var required_items_v_box_container = %RequiredItemsVBoxContainer
@onready var item_resource_file_dialog : FileDialog = $ItemResourceFileDialog
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
	if not is_instance_valid(editor_plugin) or not is_instance_valid(item_resource_edit_button):
		return
	
	item_resource_edit_button.icon = get_theme_icon("Edit", "EditorIcons")
	item_resource_edit_button.tooltip_text = "Open Resource Inventory Item"
	
	#Dialogs
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	item_resource_file_dialog.min_size = Vector2(600, 500) * scale


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin
	apply_theme()


func connect_signals():
	time_to_craft_spin_box.value_changed.connect(_on_time_to_craft_spin_box_value_changed.bind())
	craft_station_type_option_button.item_selected.connect(_on_craft_station_type_option_button_item_selected.bind())
	item_resource_edit_button.pressed.connect(_on_item_resource_edit_button_pressed.bind())
	item_resource_file_dialog.file_selected.connect(_on_recipe_resource_file_dialog_file_selected.bind())
	new_required_item_button.pressed.connect(_on_new_required_item_button_pressed.bind())
	connected = true


func disconnect_signals():
	if not connected:
		return
	#TODO Change list
#	product_selector.slot_changed.disconnect(_on_product_slot_spin_box_slot_changed.bind())
	time_to_craft_spin_box.value_changed.disconnect(_on_time_to_craft_spin_box_value_changed.bind())
	craft_station_type_option_button.item_selected.disconnect(_on_craft_station_type_option_button_item_selected.bind())
	item_resource_edit_button.pressed.disconnect(_on_item_resource_edit_button_pressed.bind())
	item_resource_file_dialog.file_selected.disconnect(_on_recipe_resource_file_dialog_file_selected.bind())
	new_required_item_button.pressed.disconnect(_on_new_required_item_button_pressed.bind())
	connected = false


func setup_station():
	craft_station_type_option_button.clear()
	stations_list.clear()
	for station in database.stations_type:
		craft_station_type_option_button.add_icon_item(station.icon, station.resource_path)
		stations_list.append(station)
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
		var ingredient_node = ingredient_scene.instantiate()
		ingredients_v_box_container.add_child(ingredient_node)
		var ingredient_editor : IngredientEditor = ingredient_node as IngredientEditor
		ingredient_editor.setup(ingredient, database, "Remove Ingredient")
		ingredient_editor.changed_slot.connect(_on_changed_slot_in_ingredient.bind())
		ingredient_editor.request_remove.connect(_request_remove_ingredient.bind(index))
		ingredients.append(ingredient_editor)


func setup_required_items(recipe : Recipe, database : InventoryDatabase):
	for ingredient_editor in required_items:
		ingredient_editor.queue_free()
	required_items.clear()
	for index in recipe.required_items.size():
		var ingredient = recipe.required_items[index]
		var ingredient_node = ingredient_scene.instantiate()
		required_items_v_box_container.add_child(ingredient_node)
		var ingredient_editor : IngredientEditor = ingredient_node as IngredientEditor
		ingredient_editor.setup(ingredient, database, "Remove Ingredient")
		ingredient_editor.changed_slot.connect(_on_changed_slot_required_item.bind())
		ingredient_editor.request_remove.connect(_request_remove_required_item.bind(index))
		required_items.append(ingredient_editor)


func setup_products(recipe : Recipe, database : InventoryDatabase):
	for product_editor in products:
		product_editor.queue_free()
	products.clear()
	for index in recipe.products.size():
		var product = recipe.products[index]
		var ingredient_node = ingredient_scene.instantiate()
		products_v_box_container.add_child(ingredient_node)
		var ingredient_editor : IngredientEditor = ingredient_node as IngredientEditor
		ingredient_editor.setup(product, database, "Remove Product")
		ingredient_editor.changed_slot.connect(_on_changed_slot_in_ingredient.bind())
		ingredient_editor.request_remove.connect(_request_remove_product.bind(index))
		products.append(ingredient_editor)


func load_recipe(recipe : Recipe, database : InventoryDatabase):
	disconnect_signals()
	self.recipe = recipe
	self.database = database
	time_to_craft_spin_box.value = recipe.time_to_craft
	item_resource_text_edit.text = recipe.resource_path
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
	var slot = Slot.new()
	slot.amount = 1
	slot.item = SlotItem.new()
	slot.item.definition = database.get_item(0)
	recipe.ingredients.append(slot)
	setup_ingredients(recipe, database)
	changed.emit()


func _on_new_required_item_button_pressed():
	var slot = Slot.new()
	slot.amount = 1
	slot.item = SlotItem.new()
	slot.item.definition = database.get_item(0)
	var required_items : Array[Slot] = []
	required_items.append_array(recipe.required_items)
	required_items.append(slot)
	recipe.required_items = required_items
	setup_required_items(recipe, database)
	changed.emit()


func _on_new_product_button_pressed():
	var slot = Slot.new()
	slot.amount = 1
	slot.item = SlotItem.new()
	slot.item.definition = database.get_item(0)
	recipe.products.append(slot)
	setup_products(recipe, database)
	changed.emit()
	changed_products.emit(recipe)


func _on_changed_slot_in_ingredient():
	changed.emit()


func _on_changed_slot_required_item():
	changed.emit()


func _on_item_resource_edit_button_pressed():
	item_resource_file_dialog.popup_centered()


func _on_recipe_resource_file_dialog_file_selected(path):
	var file = load(path)
	if file is Recipe:
		var recipe : Recipe = file
		load_recipe(recipe, database)
		changed.emit()
		changed_products.emit(recipe)
	else:
		print("Error on open scene!")

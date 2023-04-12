@tool
extends Control
class_name RecipeEditor


signal changed_product


@onready var product_id_spin_box : SpinBox = $MarginContainer/MarginContainer/VBoxContainer/Product/ProductIDSpinBox
@onready var product_amount_spin_box : SpinBox = $MarginContainer/MarginContainer/VBoxContainer/Product/ProductAmountSpinBox
@onready var option_button : OptionButton = $MarginContainer/MarginContainer/VBoxContainer/Product/OptionButton
@onready var time_to_craft_spin_box : SpinBox = $MarginContainer/MarginContainer/VBoxContainer/TimeToCraft/TimeToCraftSpinBox
@onready var craft_station_type_option_button = $MarginContainer/MarginContainer/VBoxContainer/CraftStationType/CraftStationTypeOptionButton

@export var ingredient_scene : PackedScene = preload("res://addons/inventory-system/editor/recipes/ingrendient_editor.tscn")
@onready var ingredients_v_box_container = %IngredientsVBoxContainer
@onready var byproducts_v_box_container = %ByproductsVBoxContainer


var recipe : Recipe
var database : InventoryDatabase
var ids_list : Array[int]
var stations_list : Array[CraftStationType]
var ingredients : Array[IngredientEditor]
var byproducts : Array[IngredientEditor]

var connected : bool

func connect_signals():
	product_id_spin_box.value_changed.connect(_on_product_id_spin_box_value_changed.bind())
	product_amount_spin_box.value_changed.connect(_on_product_amount_spin_box_value_changed.bind())
	option_button.item_selected.connect(_on_product_item_option_button_selected.bind())
	time_to_craft_spin_box.value_changed.connect(_on_time_to_craft_spin_box_value_changed.bind())
	craft_station_type_option_button.item_selected.connect(_on_craft_station_type_option_button_item_selected.bind())
	connected = true


func disconnect_signals():
	if not connected:
		return
	product_id_spin_box.value_changed.disconnect(_on_product_id_spin_box_value_changed.bind())
	product_amount_spin_box.value_changed.disconnect(_on_product_amount_spin_box_value_changed.bind())
	option_button.item_selected.disconnect(_on_product_item_option_button_selected.bind())
	time_to_craft_spin_box.value_changed.disconnect(_on_time_to_craft_spin_box_value_changed.bind())
	craft_station_type_option_button.item_selected.disconnect(_on_craft_station_type_option_button_item_selected.bind())
	connected = false


func setup_product():
	option_button.clear()
	ids_list.clear()
	for i in database.items.size():
		var item_database = database.items[i]
		option_button.add_icon_item(item_database.item.icon ,item_database.item.name)
		ids_list.append(item_database.id)
		if item_database.item == recipe.product.item:
			option_button.select(i)


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
		ingredient_editor.request_remove.connect(_request_remove_ingredient.bind(index))
		ingredients.append(ingredient_editor)


func setup_byproducts(recipe : Recipe, database : InventoryDatabase):
	for byproduct_editor in byproducts:
		byproduct_editor.queue_free()
	byproducts.clear()
	for index in recipe.byproducts.size():
		var byproduct = recipe.byproducts[index]
		var ingredient_node = ingredient_scene.instantiate()
		byproducts_v_box_container.add_child(ingredient_node)
		var ingredient_editor : IngredientEditor = ingredient_node as IngredientEditor
		ingredient_editor.setup(byproduct, database, "Remove Byproduct")
		ingredient_editor.request_remove.connect(_request_remove_byproduct.bind(index))
		byproducts.append(ingredient_editor)


func load_recipe(recipe : Recipe, database : InventoryDatabase):
	disconnect_signals()
	self.recipe = recipe
	self.database = database
	var item_id = database.get_id_from_item(recipe.product.item)
	product_id_spin_box.value = item_id
	product_amount_spin_box.value = recipe.product.amount
	time_to_craft_spin_box.value = recipe.time_to_craft
	setup_product()	
	setup_station()
	setup_ingredients(recipe, database)
	setup_byproducts(recipe, database)
	connect_signals()


func _on_product_id_spin_box_value_changed(value):
	var id : int = product_id_spin_box.value
	var item = database.get_item(id)
	recipe.product.item = item
	var index = ids_list.find(id)
	if index != -1:
		if option_button.selected != index:
			option_button.select(index)
	emit_signal("changed_product")


func _on_product_amount_spin_box_value_changed(value):
	recipe.product.amount = value
	

func _on_product_item_option_button_selected(index : int):
	var id : int = ids_list[index]
	if product_id_spin_box.value != id:
		product_id_spin_box.value = id


func _on_time_to_craft_spin_box_value_changed(value):
	recipe.time_to_craft = value


func _on_craft_station_type_option_button_item_selected(index : int):
	recipe.station = stations_list[index]


func _request_remove_ingredient(index):
	recipe.ingredients.remove_at(index)
	setup_ingredients(recipe, database)


func _request_remove_byproduct(index):
	recipe.byproducts.remove_at(index)
	setup_byproducts(recipe, database)


func _on_new_ingredient_button_pressed():
	var slot = Slot.new()
	slot.amount = 1
	slot.item = database.get_item(0)
	recipe.ingredients.append(slot)
	setup_ingredients(recipe, database)


func _on_new_byproduct_button_pressed():
	var slot = Slot.new()
	slot.amount = 1
	slot.item = database.get_item(0)
	recipe.byproducts.append(slot)
	setup_byproducts(recipe, database)

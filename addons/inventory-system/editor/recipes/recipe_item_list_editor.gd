@tool
extends Control
class_name RecipeItemListEditor

signal selected

@onready var time_label : Label = $Panel/MarginContainer/VBoxContainer/MoreInfos/TimeLabel
@onready var craftstation_icon : TextureRect = $Panel/MarginContainer/VBoxContainer/MoreInfos/CraftstationIcon
@onready var ingredients_list = $Panel/MarginContainer/VBoxContainer/Ingredients
@onready var byproducts_list = $Panel/MarginContainer/VBoxContainer/Byproducts
@onready var panel : Panel = $Panel

var recipe : Recipe
var database : InventoryDatabase
@export var ingredient_item_scene = preload("res://addons/inventory-system/editor/recipes/ingredient_item_in_recipe_item.tscn")
var ingredients : Array
var byproducts : Array
var style_box : StyleBoxFlat


func _ready():
	style_box = StyleBoxFlat.new()
	panel.add_theme_stylebox_override("panel", style_box)
	style_box.set_corner_radius_all(4)
	unselect()
	if recipe.station != null:
		craftstation_icon.texture = recipe.station.icon
	else:
		craftstation_icon.texture = null
	time_label.text = str(recipe.time_to_craft)+" secs"
	
	for i in recipe.ingredients:
		var i_editor = ingredient_item_scene.instantiate()
		i_editor.setup(i)
		ingredients_list.add_child(i_editor)
		
	for i in recipe.byproducts:
		var i_editor = ingredient_item_scene.instantiate()
		i_editor.setup(i)
		byproducts_list.add_child(i_editor)
		


func load_recipe(recipe : Recipe, database : InventoryDatabase):
	self.recipe = recipe
	self.database = database


func unselect():
	style_box.bg_color = get_theme_color("dark_color_1","Editor")


func select():
	style_box.bg_color = get_theme_color("box_selection_fill_color","Editor")
	emit_signal("selected")


func _on_panel_gui_input(event):
	if event is InputEventMouseButton:
		var mouse_event = event as InputEventMouseButton
		if mouse_event.button_index == 1:
			select()

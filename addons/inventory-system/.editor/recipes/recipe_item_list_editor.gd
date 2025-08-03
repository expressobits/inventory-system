@tool
class_name RecipeItemListEditor
extends Control

signal selected
signal request_remove(recipe : Recipe)


@onready var time_label : Label = %TimeLabel
@onready var craftstation_icon : TextureRect = %CraftstationIcon
@onready var ingredients_list = $Panel/MarginContainer/VBoxContainer/Ingredients/IngredientsList
@onready var products_list = $Panel/MarginContainer/VBoxContainer/Products/ProductsList
@onready var panel : Panel = $Panel
@onready var delete_button : Button = %DeleteButton
@onready var time_icon = %TimeIcon
@onready var required_item_list = %RequiredItemList

var recipe : Recipe
var database : InventoryDatabase
var style_box : StyleBoxFlat


func _ready():
	style_box = StyleBoxFlat.new()
	panel.add_theme_stylebox_override("panel", style_box)
	time_icon.texture = get_theme_icon("Timer", "EditorIcons")
	delete_button.icon = get_theme_icon("Remove", "EditorIcons")
	delete_button.tooltip_text = "Delete"
	delete_button.pressed.connect(_on_open_menu_id_pressed)
	style_box.set_corner_radius_all(4)
	unselect()
	if recipe != null:
		update_recipe()


func update_recipe():
	if recipe.station != null:
		craftstation_icon.texture = recipe.station.icon
	else:
		craftstation_icon.texture = null
	time_label.text = str(recipe.time_to_craft)+" secs"
	
	for i in ingredients_list.get_children():
		i.queue_free()
	for i in recipe.ingredients:
		var i_editor = IngredientItemInRecipeItem.new()
		i_editor.setup(database, i)
		ingredients_list.add_child(i_editor)
		
	for i in products_list.get_children():
		i.queue_free()	
	for i in recipe.products:
		var i_editor = IngredientItemInRecipeItem.new()
		i_editor.setup(database, i)
		products_list.add_child(i_editor)
		
	for i in required_item_list.get_children():
		i.queue_free()
	for i in recipe.required_items:
		var i_editor = IngredientItemInRecipeItem.new()
		i_editor.setup(database, i)
		required_item_list.add_child(i_editor)


func load_recipe(recipe : Recipe, database : InventoryDatabase):
	self.recipe = recipe
	self.database = database


func unselect():
	style_box.bg_color = get_theme_color("dark_color_1","Editor")


func select():
	style_box.bg_color = get_theme_color("box_selection_fill_color","Editor")
	selected.emit()


func _on_panel_gui_input(event):
	if event is InputEventMouseButton:
		var mouse_event = event as InputEventMouseButton
		if mouse_event.button_index == 1:
			select()


func _on_open_menu_id_pressed():
	request_remove.emit(recipe)

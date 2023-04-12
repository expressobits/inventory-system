@tool
extends Control
class_name RecipeItem

signal selected

@onready var time_label : Label = $Panel/MarginContainer/VBoxContainer/MoreInfos/TimeLabel
@onready var craftstation_icon : TextureRect = $Panel/MarginContainer/VBoxContainer/MoreInfos/CraftstationIcon
var recipe : Recipe
var database : InventoryDatabase


func _ready():
	if recipe.station != null:
		craftstation_icon.texture = recipe.station.icon
	else:
		craftstation_icon.texture = null
	time_label.text = str(recipe.time_to_craft)


func load_recipe(recipe : Recipe, database : InventoryDatabase):
	self.recipe = recipe
	self.database = database


func select():
	emit_signal("selected")


func _on_panel_gui_input(event):
	if event is InputEventMouseButton:
		var mouse_event = event as InputEventMouseButton
		if mouse_event.button_index == 1:
			select()

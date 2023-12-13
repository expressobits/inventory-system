@tool
extends VBoxContainer
class_name CustomPropertiesItemEditor

var item : InventoryItem
var database : InventoryDatabase
var properties_obj : Array

@export var property_scene : PackedScene = preload("res://addons/inventory-system/editor/recipes/ingredient_editor.tscn")
@onready var v_box_container = $ScrollContainer/VBoxContainer
@onready var add_button = $Panel/HBoxContainer/AddButton
@onready var line_edit = $Panel/HBoxContainer/NewKeyLineEdit
@onready var new_type_option_button = $Panel/HBoxContainer/NewTypeOptionButton


func _ready():
	add_button.disabled = line_edit.text.is_empty()
	build_type_options()


func load_item(database : InventoryDatabase, item : InventoryItem):
	self.database = database
	self.item = item
	loading_properties()


func make_dictionary_unique(input_dictionary) -> Dictionary:
	var unique_dictionary := {}
	for key in input_dictionary.keys():
		unique_dictionary[key] = input_dictionary[key]
	return unique_dictionary


func loading_properties():
	for p in properties_obj:
		p.queue_free()
	properties_obj.clear()
	if item == null:
		return
	var new_item_properties : Dictionary = make_dictionary_unique(item.properties)
	for key in new_item_properties.keys():
		if key is String:
			var property_obj = property_scene.instantiate()
			var property : BasePropertyItemEditor = property_obj as BasePropertyItemEditor
			property.setup(item, key, item.properties[key])
			property.removed.connect(_property_removed.bind())
			properties_obj.append(property_obj)
			v_box_container.add_child(property_obj)
	item.properties = new_item_properties


func build_type_options():
	new_type_option_button.clear()
	new_type_option_button.add_icon_item(get_theme_icon("bool", "EditorIcons"),"Boolean", TYPE_BOOL)
	new_type_option_button.add_icon_item(get_theme_icon("int", "EditorIcons"),"Integer", TYPE_INT)
	new_type_option_button.add_icon_item(get_theme_icon("float", "EditorIcons"),"Float", TYPE_FLOAT)
	new_type_option_button.add_icon_item(get_theme_icon("String", "EditorIcons"),"String", TYPE_STRING)
	new_type_option_button.add_icon_item(get_theme_icon("Color", "EditorIcons"),"Color", TYPE_COLOR)


func _property_removed():
	loading_properties()


func _on_add_button_pressed():
	var new_item_properties : Dictionary = make_dictionary_unique(item.properties)
	match new_type_option_button.get_selected_id():
		TYPE_BOOL:
			new_item_properties[line_edit.text] = false
		TYPE_INT:
			new_item_properties[line_edit.text] = 0
		TYPE_FLOAT:
			new_item_properties[line_edit.text] = 0.0
		TYPE_STRING:
			new_item_properties[line_edit.text] = ""
		TYPE_COLOR:
			new_item_properties[line_edit.text] = Color()
	item.properties = new_item_properties
	loading_properties()


func _on_new_key_line_edit_text_changed(new_text):
	add_button.disabled = line_edit.text.is_empty()
	

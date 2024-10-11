@tool
class_name CustomPropertiesCategoryEditor
extends VBoxContainer

var category : ItemCategory
var properties_obj : Array

@onready var v_box_container = $ScrollContainer/VBoxContainer
@onready var add_button = $Panel/HBoxContainer/AddButton
@onready var line_edit = $Panel/HBoxContainer/NewKeyLineEdit
@onready var new_type_option_button = $Panel/HBoxContainer/NewTypeOptionButton


func _ready():
	add_button.disabled = line_edit.text.is_empty()
	build_type_options()


func load_item(category : ItemCategory):
	self.category = category
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
	if category == null:
		return
	var new_item_properties : Dictionary = make_dictionary_unique(category.item_properties)
	for key in new_item_properties.keys():
		if key is String:
			var property : ItemDefinitionPropertyEditor = ItemDefinitionPropertyEditor.new()
			property.setup(key, category.item_dynamic_properties.find(key) != -1, category.item_properties[key])
			property.removed.connect(_property_removed.bind(category))
			property.changed.connect(_property_changed.bind(category))
			properties_obj.append(property)
			v_box_container.add_child(property)
	category.item_properties = new_item_properties


func build_type_options():
	new_type_option_button.clear()
	new_type_option_button.add_icon_item(get_theme_icon("bool", "EditorIcons"),"Boolean", TYPE_BOOL)
	new_type_option_button.add_icon_item(get_theme_icon("int", "EditorIcons"),"Integer", TYPE_INT)
	new_type_option_button.add_icon_item(get_theme_icon("float", "EditorIcons"),"Float", TYPE_FLOAT)
	new_type_option_button.add_icon_item(get_theme_icon("String", "EditorIcons"),"String", TYPE_STRING)
	new_type_option_button.add_icon_item(get_theme_icon("Color", "EditorIcons"),"Color", TYPE_COLOR)


func _property_removed(key, category : ItemCategory):
	category.item_properties.erase(key)
	loading_properties()


func _property_changed(key : String, is_dynamic : bool, value, category : ItemCategory):
	category.item_properties[key] = value
	var index = category.item_dynamic_properties.find(key)
	var contains = index != -1
	if contains != is_dynamic:
		if is_dynamic:
			category.item_dynamic_properties.append(key)
		else:
			category.item_dynamic_properties.remove_at(index)


func _on_add_button_pressed():
	var new_item_properties : Dictionary = make_dictionary_unique(category.item_properties)
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
	category.item_properties = new_item_properties
	loading_properties()


func _on_new_key_line_edit_text_changed(new_text):
	add_button.disabled = line_edit.text.is_empty()
	

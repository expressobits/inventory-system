@tool
extends Control

#var _option_database = $VBoxContainer/MarginContainer/HBoxContainer/OptionButton

# The Inventory System plugin
var editor_plugin: EditorPlugin

var _default_database = preload("res://addons/inventory-system/demos/base/database.tres")

@onready var items_editor : ItemsEditor = get_node("MarginContainer/VBoxContainer/Content/TabContainer/Items")

# Dialogs
@onready var new_dialog: FileDialog = $NewDialog
@onready var save_dialog: FileDialog = $SaveDialog
@onready var open_dialog: FileDialog = $OpenDialog

# Toolbar
@onready var new_button: Button = %NewButton
@onready var open_button: Button = %OpenButton
@onready var save_all_button: Button = %SaveAllButton
@onready var title_label : Label = %TitleLabel


func _ready():
	apply_theme()


func _process(delta):
	pass


func load_database(database : InventoryDatabase):
	if database != null:
		items_editor.load_items_from_database(database)
		$MarginContainer/VBoxContainer/Content.visible = true
	else:
		$MarginContainer/VBoxContainer/Content.visible = false
	

func new_file(path: String, content: String = "") -> void:
#	if open_buffers.has(path):
#		remove_file_from_open_buffers(path)
	var database : InventoryDatabase = InventoryDatabase.new()
	
	ResourceSaver.save(database, path)
	
	editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	load_database(database)


func open_file(path: String) -> void:
	var database : InventoryDatabase = load(path)
	load_database(database)
	
	title_label.text = path
	
#	DialogueSettings.add_recent_file(path)
#	build_open_menu()
	
#	files_list.files = open_buffers.keys()
#	files_list.select_file(path)
	
#	self.current_file_path = path

# Apply theme colors and icons to the UI
func apply_theme() -> void:
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_button):
		return
	new_button.icon = get_theme_icon("New", "EditorIcons")
	new_button.tooltip_text = "Start a new database"
	
	open_button.icon = get_theme_icon("Load", "EditorIcons")
	open_button.tooltip_text = "Open a database"
	
	save_all_button.icon = get_theme_icon("Save", "EditorIcons")
	save_all_button.tooltip_text = "Save database"
	
	# Dialog sizes
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_dialog.min_size = Vector2(600, 500) * scale
	save_dialog.min_size = Vector2(600, 500) * scale
	open_dialog.min_size = Vector2(600, 500) * scale
#	export_dialog.min_size = Vector2(600, 500) * scale
#	export_dialog.min_size = Vector2(600, 500) * scale
#	settings_dialog.min_size = Vector2(600, 600) * scale


func _on_theme_changed():
	apply_theme()


func _on_new_button_pressed():
	new_dialog.popup_centered()


func _on_new_dialog_file_selected(path):
	new_file(path)


func _on_open_button_pressed():
	open_dialog.popup_centered()


func _on_open_dialog_file_selected(path):
	open_file(path)

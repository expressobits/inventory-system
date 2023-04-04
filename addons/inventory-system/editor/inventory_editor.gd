@tool
extends Control

#var _option_database = $VBoxContainer/MarginContainer/HBoxContainer/OptionButton

const InventoryConstants = preload("res://addons/inventory-system/editor/constants.gd")
const InventorySettings = preload("res://addons/inventory-system/editor/inventory_settings.gd")

const OPEN_OPEN = 100
const OPEN_CLEAR = 101

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
@onready var open_button: MenuButton = %OpenButton
@onready var save_all_button: Button = %SaveAllButton
@onready var title_label : Label = %TitleLabel


func _ready():
	items_editor.set_editor_plugin(editor_plugin)
	apply_theme()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin


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
	
	InventorySettings.add_recent_file(path)
	build_open_menu()
	
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


# Refresh the open menu with the latest files
func build_open_menu() -> void:
	var menu = open_button.get_popup()
	menu.clear()
	menu.add_icon_item(get_theme_icon("Load", "EditorIcons"), "Open...", OPEN_OPEN)
	menu.add_separator()
	
	var recent_files = InventorySettings.get_recent_files()
	if recent_files.size() == 0:
		menu.add_item("Open recent files")
		menu.set_item_disabled(2, true)
	else:
		for path in recent_files:
			menu.add_icon_item(get_theme_icon("File", "EditorIcons"), path)
			
	menu.add_separator()
	menu.add_item("Clear recent files", OPEN_CLEAR)
	if menu.id_pressed.is_connected(_on_open_menu_id_pressed):
		menu.id_pressed.disconnect(_on_open_menu_id_pressed)
	menu.id_pressed.connect(_on_open_menu_id_pressed)


### Signals

func _on_open_menu_id_pressed(id: int) -> void:
	match id:
		OPEN_OPEN:
			open_dialog.popup_centered()
		OPEN_CLEAR:
			InventorySettings.clear_recent_files()
			build_open_menu()
		_:
			var menu = open_button.get_popup()
			var item = menu.get_item_text(menu.get_item_index(id))
			open_file(item)


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


func _on_open_button_about_to_popup():
	build_open_menu()

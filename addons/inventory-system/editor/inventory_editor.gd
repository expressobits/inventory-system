@tool
extends Control

#var _option_database = $VBoxContainer/MarginContainer/HBoxContainer/OptionButton

const InventoryConstants = preload("res://addons/inventory-system/editor/constants.gd")
const InventorySettings = preload("res://addons/inventory-system/editor/inventory_settings.gd")

const OPEN_OPEN = 100
const OPEN_CLEAR = 101

const NEW_ITEM_NEW_RESOURCE = 100
const NEW_ITEM_FROM_RESOURCE = 101

# The Inventory System plugin
var editor_plugin: EditorPlugin

@onready var items_editor : ItemsEditor = get_node("MarginContainer/VBoxContainer/Content/TabContainer/Items")
@onready var recipes_editor : RecipesEditor = %Recipes
@onready var craft_stations_editor : CraftStationTypesEditor = $"MarginContainer/VBoxContainer/Content/TabContainer/Craft Stations"
@onready var categories_editor : CategoriesEditor = $"MarginContainer/VBoxContainer/Content/TabContainer/Categories"


# Dialogs
@onready var new_dialog: FileDialog = $NewDialog
@onready var save_dialog: FileDialog = $SaveDialog
@onready var open_dialog: FileDialog = $OpenDialog

# Toolbar
@onready var new_button: Button = %NewButton
@onready var open_button: MenuButton = %OpenButton
@onready var save_all_button: Button = %SaveAllButton
@onready var title_label : Label = %TitleLabel
@onready var new_item_button : MenuButton = %NewItemButton
@onready var new_recipe_button : MenuButton= %NewRecipeButton
@onready var new_craft_station_type_button : MenuButton = %NewCraftStationTypeButton
@onready var new_item_categories_button : MenuButton = %NewItemCategoryButton


func _ready():
	items_editor.set_editor_plugin(editor_plugin)
	recipes_editor.set_editor_plugin(editor_plugin)
	craft_stations_editor.set_editor_plugin(editor_plugin)
	categories_editor.set_editor_plugin(editor_plugin)
	apply_theme()
	load_database(null)
	build_new_item_menu()
	build_new_recipe_menu()
	build_new_craft_station_menu()
	build_new_item_category_menu()


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin


func load_database(database : InventoryDatabase):
	if database != null:
		items_editor.load_from_database(database)
		recipes_editor.load_from_database(database)
		craft_stations_editor.load_from_database(database)
		categories_editor.load_from_database(database)
		$MarginContainer/VBoxContainer/Content.visible = true
		new_item_button.disabled = false
		new_recipe_button.disabled = false
		new_craft_station_type_button.disabled = false
		new_item_categories_button.disabled = false
	else:
		$MarginContainer/VBoxContainer/Content.visible = false
		new_item_button.disabled = true
		new_recipe_button.disabled = true
		new_craft_station_type_button.disabled = true
		new_item_categories_button.disabled = true


func new_file(path: String, content: String = "") -> void:
#	if open_buffers.has(path):
#		remove_file_from_open_buffers(path)
	var database : InventoryDatabase = InventoryDatabase.new()
	
	ResourceSaver.save(database, path)
	
	editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	load_database(database)


func open_file(path: String) -> void:
	var res : Resource = load(path)
	if not res is InventoryDatabase:
		push_warning("Resource " + path + " is not an InventoryDatabase!")
		return
	var database : InventoryDatabase = res as InventoryDatabase
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
	new_button.tooltip_text = "Start a New Database"
	
	open_button.icon = get_theme_icon("Load", "EditorIcons")
	open_button.tooltip_text = "Open a Database"
	
	save_all_button.icon = get_theme_icon("Save", "EditorIcons")
	save_all_button.tooltip_text = "Save Database"
	
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


func build_new_item_menu() -> void:
	var menu = new_item_button.get_popup()
	menu.clear()
	menu.add_item("New Item With New Resource", NEW_ITEM_NEW_RESOURCE)
	menu.add_item("New Item With Existing Resource", NEW_ITEM_FROM_RESOURCE)
	if menu.id_pressed.is_connected(_on_new_item_menu_id_pressed):
		menu.id_pressed.disconnect(_on_new_item_menu_id_pressed)
	menu.id_pressed.connect(_on_new_item_menu_id_pressed)


func build_new_recipe_menu() -> void:
	var menu = new_recipe_button.get_popup()
	menu.clear()
	menu.add_item("New Recipe With New Resource", NEW_ITEM_NEW_RESOURCE)
	menu.add_item("New Recipe With Existing Resource", NEW_ITEM_FROM_RESOURCE)
	if menu.id_pressed.is_connected(_on_new_recipe_menu_id_pressed):
		menu.id_pressed.disconnect(_on_new_recipe_menu_id_pressed)
	menu.id_pressed.connect(_on_new_recipe_menu_id_pressed)


func build_new_craft_station_menu() -> void:
	var menu = new_craft_station_type_button.get_popup()
	menu.clear()
	menu.add_item("New Craft Station Type With New Resource", NEW_ITEM_NEW_RESOURCE)
	menu.add_item("New Craft Station Type With Existing Resource", NEW_ITEM_FROM_RESOURCE)
	if menu.id_pressed.is_connected(_on_new_craft_station_menu_id_pressed):
		menu.id_pressed.disconnect(_on_new_craft_station_menu_id_pressed)
	menu.id_pressed.connect(_on_new_craft_station_menu_id_pressed)


func build_new_item_category_menu() -> void:
	var menu = new_item_categories_button.get_popup()
	menu.clear()
	menu.add_item("New Item Category With New Resource", NEW_ITEM_NEW_RESOURCE)
	menu.add_item("New Item Category With Existing Resource", NEW_ITEM_FROM_RESOURCE)
	if menu.id_pressed.is_connected(_on_new_item_category_menu_id_pressed):
		menu.id_pressed.disconnect(_on_new_item_category_menu_id_pressed)
	menu.id_pressed.connect(_on_new_item_category_menu_id_pressed)
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


func _on_new_item_menu_id_pressed(id: int) -> void:
	match id:
		NEW_ITEM_NEW_RESOURCE:
			items_editor.new_data_pressed()
		NEW_ITEM_FROM_RESOURCE:
			items_editor.new_data_from_resource_pressed()


func _on_new_recipe_menu_id_pressed(id: int) -> void:
	match id:
		NEW_ITEM_NEW_RESOURCE:
			recipes_editor.new_data_pressed()
		NEW_ITEM_FROM_RESOURCE:
			recipes_editor.new_data_from_resource_pressed()


func _on_new_craft_station_menu_id_pressed(id: int) -> void:
	match id:
		NEW_ITEM_NEW_RESOURCE:
			craft_stations_editor.new_data_pressed()
		NEW_ITEM_FROM_RESOURCE:
			craft_stations_editor.new_data_from_resource_pressed()


func _on_new_item_category_menu_id_pressed(id: int) -> void:
	match id:
		NEW_ITEM_NEW_RESOURCE:
			categories_editor.new_data_pressed()
		NEW_ITEM_FROM_RESOURCE:
			categories_editor.new_data_from_resource_pressed()


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

	
func _on_new_recipe_button_pressed():
	recipes_editor.new_recipe_pressed()

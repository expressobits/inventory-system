@tool
extends Control

const InventoryConstants = preload("res://addons/inventory-system/editor/constants.gd")
const InventorySettings = preload("res://addons/inventory-system/editor/inventory_settings.gd")

const OPEN_OPEN = 100
const OPEN_CLEAR = 101

const DATABASE_NEW = 100
const DATABASE_OPEN = 200
const DATABASE_OPEN_RECENT = 201
const DATABASE_SAVE = 300

const NEW_ITEM_NEW_RESOURCE = 100
const NEW_ITEM_FROM_RESOURCE = 101

# The Inventory System plugin
var editor_plugin : InventorySystemEditorPlugin

@onready var items_editor : ItemsEditor = get_node("MarginContainer/VBoxContainer/Content/TabContainer/Items")
@onready var recipes_editor : RecipesEditor = %Recipes
@onready var craft_stations_editor : CraftStationTypesEditor = $"MarginContainer/VBoxContainer/Content/TabContainer/Craft Stations"
@onready var categories_editor : CategoriesEditor = $"MarginContainer/VBoxContainer/Content/TabContainer/Categories"


var database : InventoryDatabase
var database_path : String

# Dialogs
@onready var new_dialog: FileDialog = $NewDialog
@onready var save_dialog: FileDialog = $SaveDialog
@onready var open_dialog: FileDialog = $OpenDialog


# Toolbar
@onready var database_button : Button = %DatabaseButton
@onready var new_button: Button = %NewButton
@onready var open_button: MenuButton = %OpenButton
@onready var save_button: Button = %SaveButton
@onready var title_label : Label = %TitleLabel
@onready var new_item_button : Button = %NewItemButton
@onready var new_recipe_button : Button= %NewRecipeButton
@onready var new_craft_station_type_button : Button = %NewCraftStationTypeButton
@onready var new_item_categories_button : Button = %NewItemCategoryButton


func _ready():
	items_editor.set_editor_plugin(editor_plugin)
	recipes_editor.set_editor_plugin(editor_plugin)
	craft_stations_editor.set_editor_plugin(editor_plugin)
	categories_editor.set_editor_plugin(editor_plugin)
	items_editor.removed.connect(remove_item_definition)
	recipes_editor.removed.connect(remove_recipe)
	craft_stations_editor.removed.connect(remove_craft_station)
	categories_editor.removed.connect(remove_item_category)
	apply_theme()
	load_database(null)
	new_dialog.file_selected.connect(_on_new_dialog_file_selected)
	open_dialog.file_selected.connect(_on_open_dialog_file_selected)
	database_button.pressed.connect(_on_database_button_pressed)
	new_button.pressed.connect(_on_new_button_pressed)
	open_button.pressed.connect(_on_open_button_about_to_popup)
	new_item_button.pressed.connect(_on_new_item_menu_id_pressed)
	new_recipe_button.pressed.connect(_on_new_recipe_menu_id_pressed)
	new_craft_station_type_button.pressed.connect(_on_new_craft_station_menu_id_pressed)
	new_item_categories_button.pressed.connect(_on_new_item_category_menu_id_pressed)
	save_button.pressed.connect(save_file)


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
	var database : InventoryDatabase = InventoryDatabase.new()
	self.database = database
	self.database_path = path
	save_file()
	
	editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	load_database(database)


func open_file(path: String) -> void:
	var res : Resource = load(path)
	if not res is InventoryDatabase:
		push_warning("Resource " + path + " is not an InventoryDatabase!")
		return
	var data : InventoryDatabase = res as InventoryDatabase
	var database = data.duplicate()
	
	editor_plugin.import_from_inv_file(database, path)
	
	load_database(database)
	self.database = database
	self.database_path = path
	
	title_label.text = path
	
	InventorySettings.add_recent_file(path)
	build_open_menu()
	

func save_file() -> void:
	var json = database.export_to_invdata()
	
	var path = database_path
	var file = FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		print("ERROR TO SAVE")
		return FileAccess.get_open_error()
	file.store_string(json)
	print(json)
	file.close()
	

# Apply theme colors and icons to the UI
func apply_theme() -> void:
	if not is_instance_valid(editor_plugin) or not is_instance_valid(new_button):
		return
	new_button.icon = get_theme_icon("New", "EditorIcons")
	new_button.tooltip_text = "Start a New Database"
	
	open_button.icon = get_theme_icon("Load", "EditorIcons")
	open_button.tooltip_text = "Open a Database"
	
	save_button.icon = get_theme_icon("Save", "EditorIcons")
	save_button.tooltip_text = "Save Database"
	
	# Dialog sizes
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_dialog.min_size = Vector2(600, 500) * scale
	save_dialog.min_size = Vector2(600, 500) * scale
	open_dialog.min_size = Vector2(600, 500) * scale


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


func build_database_menu() -> void:
	var menu = database_button.get_popup()
	menu.clear()
	menu.add_icon_item(get_theme_icon("New", "EditorIcons"), "New Database", DATABASE_NEW)
	menu.add_icon_item(get_theme_icon("Open", "EditorIcons"), "Open Database...", DATABASE_OPEN)
	menu.add_icon_item(get_theme_icon("Open Recent", "EditorIcons"), "Open Recent", DATABASE_OPEN_RECENT)
	menu.add_separator()
	
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


func _on_new_item_menu_id_pressed() -> void:
	var new_item_definition = ItemDefinition.new()
	new_item_definition.name = "New Item Definition"
	database.add_new_item(new_item_definition)
	save_file()
	load_database(database)


func _on_new_recipe_menu_id_pressed() -> void:
	database.add_recipe()
	save_file()
	load_database(database)


func _on_new_craft_station_menu_id_pressed() -> void:
	var new_craft_station_type = CraftStationType.new()
	new_craft_station_type.name = "New Craft Station Type"
	database.stations_type.append(new_craft_station_type)
	save_file()
	load_database(database)


func _on_new_item_category_menu_id_pressed() -> void:
	var new_item_category = ItemCategory.new()
	new_item_category.name = "New Item Category"
	database.item_categories.append(new_item_category)
	save_file()
	load_database(database)


func remove_item_definition(item : ItemDefinition):
	if item == null:
		return
	database.remove_item(item)
	save_file()


func remove_item_category(category : ItemCategory):
	database.remove_category(category)
	categories_editor.load_item_categories()
	categories_editor.data_changed.emit()
	save_file()


func remove_craft_station(craft_station_type : CraftStationType):
	var index = database.stations_type.find(craft_station_type)
	if index == -1:
		return
	database.stations_type.remove_at(index)
	craft_stations_editor.load_craft_station_types()
	craft_stations_editor.data_changed.emit()
	save_file()


func remove_recipe(recipe : Recipe):
	if recipe == null:
		return
	var index = database.recipes.find(recipe)
	database.recipes.remove_at(index)
	recipes_editor.load_recipes()
	save_file()


func _on_theme_changed():
	apply_theme()


func _on_database_button_pressed():
	build_database_menu()

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

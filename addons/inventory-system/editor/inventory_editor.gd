@tool
extends Control

const InventorySettings = preload("res://addons/inventory-system/editor/inventory_settings.gd")

const OPEN_OPEN = 100
const OPEN_CLEAR = 101

const DATABASE_NEW = 100
const DATABASE_OPEN = 200
const DATABASE_OPEN_RECENT = 201
const DATABASE_OPEN_RECENT_CLEAR = 202
const DATABASE_SAVE = 300

const DATABASE_IMPORT = 400
const DATABASE_IMPORT_JSON = 401

const DATABASE_EXPORT = 500
const DATABASE_EXPORT_JSON = 501

const NEW_ITEM_NEW_RESOURCE = 100
const NEW_ITEM_FROM_RESOURCE = 101

# The Inventory System plugin
var editor_plugin : InventorySystemEditorPlugin

@onready var tab_container : TabContainer = $MarginContainer/VBoxContainer/Content/TabContainer

@onready var item_definitions_editor : ItemDefinitionsEditor = get_node("MarginContainer/VBoxContainer/Content/TabContainer/Items")
@onready var recipes_editor : RecipesEditor = %RecipesEditor
@onready var craft_station_types_editor : CraftStationTypesEditor = $"MarginContainer/VBoxContainer/Content/TabContainer/CraftStationTypesEditor"
@onready var categories_editor : CategoriesEditor = $"MarginContainer/VBoxContainer/Content/TabContainer/CategoriesEditor"


var database : InventoryDatabase
var database_path : String

# Dialogs
@onready var new_dialog: FileDialog = $NewDialog
@onready var save_dialog: FileDialog = $SaveDialog
@onready var open_dialog: FileDialog = $OpenDialog
@onready var open_inv_dialog: FileDialog = $OpenInvDialog
@onready var save_inv_dialog: FileDialog = $SaveJsonDialog


# Toolbar
@onready var database_button : MenuButton = %DatabaseButton
@onready var title_label : Label = %TitleLabel
@onready var new_item_button : Button = %NewItemButton
@onready var new_recipe_button : Button= %NewRecipeButton
@onready var new_craft_station_type_button : Button = %NewCraftStationTypeButton
@onready var new_item_categories_button : Button = %NewItemCategoryButton


func _ready():
	item_definitions_editor.set_editor_plugin(editor_plugin)
	recipes_editor.set_editor_plugin(editor_plugin)
	craft_station_types_editor.set_editor_plugin(editor_plugin)
	categories_editor.set_editor_plugin(editor_plugin)
	item_definitions_editor.removed.connect(remove_item_definition)
	item_definitions_editor.duplicated.connect(duplicate_item_definition)
	recipes_editor.removed.connect(remove_recipe)
	craft_station_types_editor.removed.connect(remove_craft_station_type)
	craft_station_types_editor.duplicated.connect(duplicate_craft_station_type)
	categories_editor.removed.connect(remove_item_category)
	categories_editor.duplicated.connect(duplicate_item_category)
	apply_theme()
	load_database(null)
	new_dialog.file_selected.connect(_on_new_dialog_file_selected)
	save_inv_dialog.file_selected.connect(_on_save_inv_dialog_file_selected)
	open_dialog.file_selected.connect(_on_open_dialog_file_selected)
	open_inv_dialog.file_selected.connect(_on_open_inv_dialog_file_selected)
	database_button.pressed.connect(_on_database_button_pressed)
	new_item_button.pressed.connect(_on_new_item_menu_id_pressed)
	new_recipe_button.pressed.connect(_on_new_recipe_menu_id_pressed)
	new_craft_station_type_button.pressed.connect(_on_new_craft_station_menu_id_pressed)
	new_item_categories_button.pressed.connect(_on_new_item_category_menu_id_pressed)


func set_editor_plugin(editor_plugin : EditorPlugin):
	self.editor_plugin = editor_plugin


func load_database(database : InventoryDatabase):
	var menu : Popup = database_button.get_popup()
	if database != null:
		item_definitions_editor.load_from_database(database)
		recipes_editor.load_from_database(database)
		craft_station_types_editor.load_from_database(database)
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
	load_database(database)


func import_inv_file(path: String) -> void:
	if database == null:
		return
	database.clear_current_data()
	database.import_json_file(path)
	
	load_database(database)
	self.database = database
	
	build_database_menu()


func open_file(path: String) -> void:
	var res : Resource = load(path)
	if not res is InventoryDatabase:
		push_warning("Resource " + path + " is not an InventoryDatabase!")
		return
	var database : InventoryDatabase = res as InventoryDatabase
	
	load_database(database)
	self.database = database
	self.database_path = path
	
	title_label.text = path
	
	InventorySettings.add_recent_file(path)
	build_database_menu()


func save_file() -> void:
	ResourceSaver.save(database, database_path)
	editor_plugin.get_editor_interface().get_resource_filesystem().scan()
	

# Apply theme colors and icons to the UI
func apply_theme() -> void:
	if not is_instance_valid(editor_plugin) or not is_instance_valid(database_button):
		return
	database_button.tooltip_text = "Database Menu"
	
	# Dialog sizes
	var scale: float = editor_plugin.get_editor_interface().get_editor_scale()
	new_dialog.min_size = Vector2(600, 500) * scale
	save_dialog.min_size = Vector2(600, 500) * scale
	open_dialog.min_size = Vector2(600, 500) * scale
	open_inv_dialog.min_size = Vector2(600, 500) * scale
	save_inv_dialog.min_size = Vector2(600, 500) * scale


func build_database_menu() -> void:
	var menu : Popup = database_button.get_popup()
	menu.clear()
	menu.add_icon_item(get_theme_icon("New", "EditorIcons"), "New Database", DATABASE_NEW)
	menu.add_icon_item(get_theme_icon("Load", "EditorIcons"), "Open Database...", DATABASE_OPEN)
	
	var open_menu : PopupMenu = PopupMenu.new()
	var recent_files = InventorySettings.get_recent_files()
	if recent_files.size() == 0:
		open_menu.add_item("Open recent files")
		open_menu.set_item_disabled(0, true)
	else:
		for path in recent_files:
			open_menu.add_icon_item(get_theme_icon("File", "EditorIcons"), path)
			
	open_menu.add_separator()
	open_menu.add_item("Clear recent files", OPEN_CLEAR)
	if open_menu.id_pressed.is_connected(_on_open_menu_id_pressed):
		open_menu.id_pressed.disconnect(_on_open_menu_id_pressed)
	open_menu.id_pressed.connect(_on_open_menu_id_pressed)
	
	menu.add_submenu_node_item("Open Recent", open_menu)
	
	menu.set_item_icon(2, get_theme_icon("Load", "EditorIcons"))
	menu.add_separator()
	menu.add_icon_item(get_theme_icon("Save", "EditorIcons"), "Save Database", DATABASE_SAVE)
	
	menu.add_separator()
	
	var import_menu : PopupMenu = PopupMenu.new()
	import_menu.add_item("Json File", DATABASE_IMPORT_JSON)
	if import_menu.id_pressed.is_connected(_on_import_menu_id_pressed):
		import_menu.id_pressed.disconnect(_on_import_menu_id_pressed)
	import_menu.id_pressed.connect(_on_import_menu_id_pressed)
	
	menu.add_submenu_node_item("Import With...", import_menu)
	#menu.set_item_icon(6, get_theme_icon("Load", "EditorIcons"))
	
	var export_menu : PopupMenu = PopupMenu.new()
	export_menu.add_item("Json File", DATABASE_EXPORT_JSON)
	if export_menu.id_pressed.is_connected(_on_export_menu_id_pressed):
		export_menu.id_pressed.disconnect(_on_export_menu_id_pressed)
	export_menu.id_pressed.connect(_on_export_menu_id_pressed)
	
	menu.add_submenu_node_item("Export As...", export_menu)
	#menu.set_item_icon(7, get_theme_icon("Load", "EditorIcons"))
	
	if menu.id_pressed.is_connected(_on_database_menu_id_pressed):
		menu.id_pressed.disconnect(_on_database_menu_id_pressed)
	menu.id_pressed.connect(_on_database_menu_id_pressed)
	
	menu.set_item_disabled(4, database == null)
	menu.set_item_disabled(6, database == null)
	menu.set_item_disabled(7, database == null)


func _on_database_menu_id_pressed(id: int) -> void:
	match id:
		DATABASE_NEW:
			new_dialog.popup_centered()
		DATABASE_SAVE:
			save_file()
		DATABASE_OPEN:
			open_dialog.popup_centered()


func _on_open_menu_id_pressed(id: int) -> void:
	match id:
		OPEN_CLEAR:
			InventorySettings.clear_recent_files()
			build_database_menu()
		_:
			var menu = database_button.get_popup()
			var open_menu : PopupMenu = menu.get_item_submenu_node(2)
			var item = open_menu.get_item_text(open_menu.get_item_index(id))
			open_file(item)


func _on_import_menu_id_pressed(id: int) -> void:
	match id:
		DATABASE_IMPORT_JSON:
			open_inv_dialog.popup_centered()


func _on_export_menu_id_pressed(id: int) -> void:
	match id:
		DATABASE_EXPORT_JSON:
			save_inv_dialog.popup_centered()


func _on_new_item_menu_id_pressed() -> void:
	if database.items.any(func(item) -> bool: return item.id == ""):
		push_warning("Item definition with empty id can exist only once.")
		return
	var new_item_definition = ItemDefinition.new()
	new_item_definition.name = "New Item definition"
	database.add_new_item(new_item_definition)
	save_file()
	load_database(database)
	tab_container.current_tab = 0


func _on_new_recipe_menu_id_pressed() -> void:
	database.add_recipe()
	save_file()
	load_database(database)
	tab_container.current_tab = 1


func _on_new_craft_station_menu_id_pressed() -> void:
	if database.stations_type.any(func(station_type) -> bool: return station_type.id == ""):
		push_warning("Craft station type with empty id can exist only once.")
		return
	var new_craft_station_type = CraftStationType.new()
	new_craft_station_type.name = "New Craft Station Type"
	database.stations_type.append(new_craft_station_type)
	save_file()
	load_database(database)
	tab_container.current_tab = 2


func _on_new_item_category_menu_id_pressed() -> void:
	if database.item_categories.any(func(item_category) -> bool: return item_category.id == ""):
		push_warning("Item category with empty id can exist only once.")
		return
	var new_item_category = ItemCategory.new()
	new_item_category.name = "New Item Category"
	database.item_categories.append(new_item_category)
	save_file()
	load_database(database)
	tab_container.current_tab = 3


func remove_item_definition(item : ItemDefinition):
	if item == null:
		return
	database.remove_item(item)
	save_file()


func duplicate_item_definition(item : ItemDefinition):
	if item.id == "":
		push_warning("Item definition with empty id cannot be duplicated.")
		return
	var new_item: ItemDefinition = item.duplicate()
	while database.has_item_id(new_item.id):
		new_item.id = new_item.id + "_duplicate"
	while database.items.any(func(item) -> bool: return item.name == new_item.name):
		new_item.name = new_item.name + "(2)"
	database.items.push_back(new_item)
	save_file()
	load_database(database)
	item_definitions_editor.select(new_item)


func remove_item_category(category : ItemCategory):
	database.remove_category(category)
	categories_editor.load_item_categories()
	categories_editor.data_changed.emit()
	save_file()


func duplicate_item_category(item : ItemCategory):
	if item.id == "":
		push_warning("Item category with empty id cannot be duplicated.")
		return
	var new_item: ItemCategory = item.duplicate()
	while database.has_item_category_id(new_item.id):
		new_item.id = new_item.id + "_duplicate"
	while database.item_categories.any(func(item) -> bool: return item.name == new_item.name):
		new_item.name = new_item.name + "(2)"
	database.item_categories.push_back(new_item)
	save_file()
	load_database(database)
	categories_editor.select(new_item)


func remove_craft_station_type(craft_station_type : CraftStationType):
	var index = database.stations_type.find(craft_station_type)
	if index == -1:
		return
	database.stations_type.remove_at(index)
	craft_station_types_editor.load_craft_station_types()
	craft_station_types_editor.data_changed.emit()
	save_file()


func duplicate_craft_station_type(item : CraftStationType):
	if item.id == "":
		push_warning("Craft station type with empty id cannot be duplicated.")
		return
	var new_item: CraftStationType = item.duplicate()
	while database.has_craft_station_type_id(new_item.id):
		new_item.id = new_item.id + "_duplicate"
	while database.stations_type.any(func(item) -> bool: return item.name == new_item.name):
		new_item.name = new_item.name + "(2)"
	database.stations_type.push_back(new_item)
	save_file()
	load_database(database)
	craft_station_types_editor.select(new_item)


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


func _on_save_inv_dialog_file_selected(path):
	database.export_json_file(path)
	

func _on_open_button_pressed():
	open_dialog.popup_centered()


func _on_open_dialog_file_selected(path):
	open_file(path)


func _on_open_inv_dialog_file_selected(path):
	import_inv_file(path)


func _on_open_button_about_to_popup():
	build_database_menu()

	
func _on_new_recipe_button_pressed():
	recipes_editor.new_recipe_pressed()

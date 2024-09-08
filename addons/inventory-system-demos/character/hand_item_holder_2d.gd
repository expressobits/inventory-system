class_name HandItemHolder2D
extends Node3D

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@export_node_path("Node2D") var default_hand_item_object_path = NodePath("DefaultHandItem")
@export_node_path("Hotbar") var hotbar_path = NodePath("../InventoryHandler/Hotbar")

@onready var default_hand_item_object := get_node(default_hand_item_object_path) 
@onready var hotbar : Hotbar = get_node(hotbar_path) 

@export_node_path var interactor_path = NodePath("../../CharacterInventorySystem/Interactor")
@onready var interactor : Interactor = get_node(interactor_path) 

var last_item : ItemDefinition = null
var objects_per_id : Dictionary


func _ready():
	hotbar.on_change_selection.connect(_on_change_selection)
	hotbar.on_update_selection_slot.connect(_on_update_selection_slot)
	_on_change_selection(hotbar.selection_index)


func _on_update_selection_slot():
	_on_change_selection(hotbar.selection_index)


func _on_change_selection(new_index : int):
	_clear_last_selection()
	if not hotbar.has_valid_item_id():
		interactor.actual_hand_object = null
		return
	var item = hotbar.get_selected_item()
	var item_definition = item.definition
	var hand_item_scene = null
	if item_definition.properties.has("hand_item"):
		var path = item_definition.properties["hand_item"]
		hand_item_scene = load(path)
	last_item = item_definition
	if hand_item_scene == null:
		default_hand_item_object.visible = true
		interactor.actual_hand_object = null
		return
	if objects_per_id.has(item_definition):
		objects_per_id[item_definition].visible = true
	else:
		var hand_item_obj = hand_item_scene.instantiate()
		hand_item_obj.interactor = interactor
		add_child(hand_item_obj)
		objects_per_id[item_definition] = hand_item_obj
	interactor.actual_hand_object = objects_per_id[item_definition]


func _clear_last_selection():
	default_hand_item_object.visible = false
	if last_item == null:
		return
	if objects_per_id.has(last_item):
		objects_per_id[last_item].visible = false		

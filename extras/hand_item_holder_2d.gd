extends Node3D
class_name HandItemHolder2D

@export_node_path("Node2D") var default_hand_item_object_path = NodePath("DefaultHandItem")
@export_node_path("Hotbar") var hotbar_path = NodePath("../InventoryHandler/Hotbar")

@onready var default_hand_item_object := get_node(default_hand_item_object_path) 
@onready var hotbar : Hotbar = get_node(hotbar_path) 

@export_node_path("Interactor") var interactor_path = NodePath("../../CharacterInventorySystem/Interactor")
@onready var interactor : InventoryInteractor = get_node(interactor_path) 

var last_item := InventoryItem.NONE
var objects_per_id : Dictionary


func _ready():
	hotbar.on_change_selection.connect(_on_change_selection.bind())
	_on_change_selection(hotbar.selection_index)


func _on_change_selection(new_index : int):
	_clear_last_selection()
	if not hotbar.has_valid_item_id():
		return
	var item_id = hotbar.get_selected_item()
	var hand_item_scene = hotbar.inventory.database.get_hand_item(item_id)
	last_item = item_id
	interactor.actual_hand_object = hand_item_scene
	if hand_item_scene == null:
		default_hand_item_object.visible = true
		return
	if objects_per_id.has(item_id):
		objects_per_id[item_id].visible = true
	else:
		var hand_item_obj = hand_item_scene.instantiate()
		add_child(hand_item_obj)
		objects_per_id[item_id] = hand_item_obj


func _clear_last_selection():
	default_hand_item_object.visible = false
	if last_item <= InventoryItem.NONE:
		return
	if objects_per_id.has(last_item):
		objects_per_id[last_item].visible = false	

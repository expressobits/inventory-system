extends Node3D
class_name HandItemHolder3D

@export_node_path("Node3D") var default_hand_item_object_path = NodePath("DefaultHandItem")
@export_node_path("Hotbar") var hotbar_path = NodePath("../../CharacterInventorySystem/Hotbar")

@onready var default_hand_item_object := get_node(default_hand_item_object_path) 
@onready var hotbar : Hotbar = get_node(hotbar_path) 

@export_node_path("InventoryInteractor") var interactor_path = NodePath("../../CharacterInventorySystem/InventoryInteractor")
@onready var interactor : InventoryInteractor = get_node(interactor_path) 

var last_item : InventoryItem = null
var objects_per_id : Dictionary


func _ready():
	hotbar.on_change_selection.connect(_on_change_selection.bind())
	_on_change_selection(hotbar.selection_index)


func _on_change_selection(new_index : int):
	_clear_last_selection()
	if not hotbar.has_valid_item_id():
		return
	var item = hotbar.get_selected_item()
	var hand_item_scene = null
	if item.properties.has("hand_item"):
		var path = item.properties["hand_item"]
		hand_item_scene = load(path)
	last_item = item
	if hand_item_scene == null:
		default_hand_item_object.visible = true
		interactor.actual_hand_object = null
		return
	if objects_per_id.has(item):
		objects_per_id[item].visible = true
	else:
		var hand_item_obj = hand_item_scene.instantiate()
		hand_item_obj.interactor = interactor
		add_child(hand_item_obj)
		objects_per_id[item] = hand_item_obj
	interactor.actual_hand_object = objects_per_id[item]


func _clear_last_selection():
	default_hand_item_object.visible = false
	if last_item == null:
		return
	if objects_per_id.has(last_item):
		objects_per_id[last_item].visible = false	

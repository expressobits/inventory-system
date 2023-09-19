extends Node3D
class_name VisualSlot3D

@export_node_path("Node3D") var default_hand_item_object_path = NodePath("DefaultItem3D")
@onready var default_hand_item_object := get_node(default_hand_item_object_path) 

var property_name_of_visual : String
var last_item : InventoryItem = null
var inventory : Inventory
var objects_per_id : Dictionary
var slot_index : int


func setup(inv : Inventory, new_slot_index : int, property := "visual_3d"):
	self.property_name_of_visual = property
	self.inventory = inv
	self.inventory.updated_slot.connect(_on_updated_slot.bind())
	self.slot_index = new_slot_index
	_on_updated_slot(slot_index)
	objects_per_id = {}


func _on_updated_slot(changed_slot_index : int):
	if self.slot_index != changed_slot_index:
		return
	_clear_last_visual()
	var item = inventory.slots[slot_index].item
	if item == null:
		return
	var item_scene = null
	if item.properties.has(property_name_of_visual):
		var path = item.properties[property_name_of_visual]
		item_scene = load(path)
	last_item = item
	if item_scene == null:
		default_hand_item_object.visible = true
		return
	if objects_per_id.has(item):
		objects_per_id[item].visible = true
	else:
		var item_obj = item_scene.instantiate()
		add_child(item_obj)
		objects_per_id[item] = item_obj


func _clear_last_visual():
	default_hand_item_object.visible = false
	if last_item == null:
		return
	if objects_per_id.has(last_item):
		objects_per_id[last_item].visible = false	

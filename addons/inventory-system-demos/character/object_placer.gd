class_name ObjectPlacer
extends NodeInventories

signal dropped
signal placed

@export var property_from_item_for_object_scene : String = "placeable"
@export var drop_node_path : NodePath = "../../../.."
@onready var drop_node_parent : Node = get_node(drop_node_path)
@export_node_path("GridInventory") var main_inventory_path := NodePath("../../Inventory")
@onready var main_inventory : GridInventory = get_node(main_inventory_path)


# TODO Add code to interactor child node (InteractorResponse)
func place_item(item_id : String, position : Vector3, rotation : Vector3):
	# TODO Add 3D Preview
	var definition = main_inventory.database.get_item(item_id)
	if !definition.properties.has(property_from_item_for_object_scene):
		return
	var path = definition.properties[property_from_item_for_object_scene]
	var res = load(path)
	if res is PackedScene:
		var scene = res as PackedScene
		if main_inventory.remove(item_id) <= 0:
			_instantiate_object(scene, position, rotation)
			placed.emit()
			return


func _instantiate_object(dropped_item : PackedScene, position : Vector3, rotation : Vector3):
	var obj = dropped_item.instantiate()
	drop_node_parent.add_child(obj)
	obj.position = position
	obj.rotation = rotation
	dropped.emit(obj)

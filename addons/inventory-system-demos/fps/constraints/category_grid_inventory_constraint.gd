extends GridInventoryConstraint
class_name CategoryGridInventoryConstraint

@export var category_position: Dictionary
@export var is_only_one_category_for_slots: bool
@export var one_category_for_slots: String

func _can_add_on_position(inventory: Node, position: Vector2i, item_id: String, _amount: int, _properties: Dictionary, _is_rotated: bool):
	var def: ItemDefinition = inventory.database.get_item(item_id)
	if def == null:
		return false
	
	var size: Vector2i
	if _is_rotated:
		size = Vector2i(def.size.y, def.size.x)
	else:
		size = def.size
	
	for x in range(position.x,position.x + size.x):
		for y in range(position.y,position.y + size.y):
			var pos = Vector2i(x, y)
			var category: String = get_category(pos)
			if not contains_category(def, category):
				return false
	return true


func contains_category(def: ItemDefinition, category: String):
	if category != "":
		for c in def.categories:
			if c.id == category:
				return true
		return false
	else:
		return true

func get_category(position: Vector2i) -> String:
	if is_only_one_category_for_slots:
		return one_category_for_slots
	else:
		if category_position.has(position):
			var category : String = category_position[position]
			return category
		return ""

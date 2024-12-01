extends GridInventoryConstraint
class_name CategoryGridInventoryConstraint

@export var category_position: Dictionary
@export var is_only_one_category_for_slots: bool
@export var one_category_for_slots: String

func _can_add_on_position(inventory: Node, position: Vector2i, item_id: String, amount: int, properties: Dictionary, is_rotated: bool):
	var category: String = get_category(position)
	if category != "":
		var def : ItemDefinition = inventory.database.get_item(item_id)
		for c in def.categories:
			if c.id == category:
				return true
		return false
	return true

func get_category(position: Vector2i) -> String:
	if is_only_one_category_for_slots:
		return one_category_for_slots
	else:
		if category_position.has(position):
			var category : String = category_position[position]
			return category
		return ""

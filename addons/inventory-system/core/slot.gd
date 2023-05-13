@tool
extends Resource
class_name Slot

@export var item : InventoryItem
@export var amount := 0
@export var accepted_categories : Array[ItemCategory]:
	set(new_categories):
		accepted_categories = new_categories
		accepted_categories_code = 0
		if not Engine.is_editor_hint():
			for c in accepted_categories:
				accepted_categories_code |= c.code
var accepted_categories_code := 0

func get_item_id() -> int:
	if item == null:
		return InventoryItem.NONE
	else:
		return item.id


func is_accept_category(other_item : InventoryItem) -> bool:
	return accepted_categories_code == 0 || contains_any_category(other_item.categories)


func contains_any_category(other_list : Array[ItemCategory]) -> bool:
	for c in other_list:
		if accepted_categories_code & c.code > 0:
			return true
	return false

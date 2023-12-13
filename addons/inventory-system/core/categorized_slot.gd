@tool
extends Slot
class_name CategorizedSlot

@export var accepted_categories : Array[ItemCategory]:
	set(new_categories):
		accepted_categories = new_categories
		accepted_categories_code = 0
		if not Engine.is_editor_hint():
			for c in accepted_categories:
				accepted_categories_code |= c.code
				
var accepted_categories_code := 0


func is_accept_category(other_item : InventoryItem) -> bool:
	return accepted_categories_code == 0 || contains_any_category(other_item.categories)


func contains_any_category(other_list : Array[ItemCategory]) -> bool:
	for c in other_list:
		if accepted_categories_code & c.code > 0:
			return true
	return false


func add(item : SlotItem, amount : int = 1):
	if not is_accept_category(item.definition):
		return amount
	return super.add(item, amount)

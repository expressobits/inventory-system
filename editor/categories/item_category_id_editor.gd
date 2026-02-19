@tool
class_name ItemCategoryIDEditor
extends ItemIDEditor

func has_in_database(id : String) -> bool:
	return database.has_item_category_id(id)

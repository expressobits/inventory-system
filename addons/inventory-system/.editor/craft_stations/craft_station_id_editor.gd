@tool
class_name CraftStationTypeIDEditor
extends ItemIDEditor

func has_in_database(id : String) -> bool:
	return database.has_craft_station_type_id(id)

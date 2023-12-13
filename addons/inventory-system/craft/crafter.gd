@tool
@icon("res://addons/inventory-system/icons/crafter.svg")
extends NodeInventorySystemBase
class_name Crafter


## Class of an agent that opens and closes craft-stations
##
## It also stores an internal craft-station, normally used for crafts that don't require a station that stays at its level.


## Emitted when craft station is opened by the crafter.
## Called when the [code]open()[/code] function is called and the craft station has not yet been opened
signal opened(craft_station : CraftStation)

## Emitted when craft_station is closed by the crafter.
## Called when the [code]close()[/code] function is called and the craft_station has not yet been closed
signal closed(craft_station : CraftStation)


## Internal craft-station used for crafts that don't need stations that are in level.
@export var main_station : CraftStation 


## All craft stations currently open by this crafter
var opened_stations : Array


## Opens an [CraftStation] that is closed.
## Returns [code]true[/code] if opened successfully.
func open(craft_station : CraftStation) -> bool:
	if opened_stations.has(craft_station):
		return false 
	if !craft_station.open():
		return false
	opened_stations.append(craft_station)
	opened.emit(craft_station)
	return true


## Close an [CraftStation] that is opened.
## Returns [code]true[/code] if closed successfully.
func close(craft_station : CraftStation) -> bool:
	var index = opened_stations.find(craft_station)
	if index == -1:
		return false
	if !craft_station.close():
		return false
	opened_stations.remove_at(index)
	closed.emit(craft_station)
	return true


## Returns [code]true[/code] if main [Inventory] is open.
func is_open_main_craft_station() -> bool:
	return is_open(main_station)


## Returns [code]true[/code] if any [Inventory] is open.
func is_open_any_station() -> bool:
	return opened_stations.size() > 0


## Returns [code]true[/code] if [Inventory] is open.
func is_open(craft_station : CraftStation) -> bool:
	return craft_station.is_open


## Open main [CraftStation]. Return [code]true[/code] if opened successfully.
func open_main_craft_station() -> bool:
	return open(main_station)


## Close main [CraftStation]. Return [code]true[/code] if closed successfully.
func close_main_craft_station() -> bool:
	return close(main_station)


## Close all open [CraftStation]s.
func close_all_craft_stations():
	for i in range(opened_stations.size() - 1, -1, -1):
		var station = opened_stations[0]
		close(station)

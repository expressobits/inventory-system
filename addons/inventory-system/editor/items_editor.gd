extends Panel
class_name ItemsEditor

var _database : InventoryDatabase

func load_database(database : InventoryDatabase):
	self._database = database
	load_items()


func load_items():
	var str = ""
	for item in _database.items:
		str += item.name
	$Label.text = str

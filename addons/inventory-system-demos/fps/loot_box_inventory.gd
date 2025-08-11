extends BoxInventory
class_name LootBoxInventory

@onready var loot_generator: LootGenerator = $LootGenerator

func _on_open():
	loot_generator.generate_loot()
	super._on_open()


func _on_close():
	inventory.clear()
	super._on_close()

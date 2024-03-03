extends Slot
class_name TestSlot

func _on_add(item, amount) -> int:
	print("GD _on_add")
	super.add(item, 2)
	return 0

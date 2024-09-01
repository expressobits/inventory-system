extends Node
class_name Openable

signal opened(character : Node)
signal closed(character : Node)

@export var is_open : bool

var characters : Array[Node] = []

func open(character : Node):
	characters.append(character)
	if is_open:
		return
	is_open = true
	opened.emit(character)
	
func close(character : Node):
	var index = characters.find(character)
	if index == -1:
		return
	characters.remove_at(index)
	if not characters.is_empty():
		return
	if !is_open:
		return
	is_open = false
	closed.emit(character)

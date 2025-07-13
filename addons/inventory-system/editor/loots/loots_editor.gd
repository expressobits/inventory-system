@tool
extends VBoxContainer

@export var database: Resource

@onready var loot_list = $HBoxContainer/LootList
@onready var loot_name = $HBoxContainer/VBoxContainer/LootName
@onready var loot_items_list = $HBoxContainer/VBoxContainer/LootItemsList
@onready var add_loot_btn = $HBoxContainer/VBoxContainer/AddLoot
@onready var remove_loot_btn = $HBoxContainer/VBoxContainer/RemoveLoot

var selected_loot: Loot = null

func load_from_database(db):
    database = db
    loot_list.clear()
    for loot in database.loots:
        loot_list.add_item(loot.name)
    selected_loot = null
    loot_name.text = ""
    loot_items_list.clear()

func _ready():
    loot_list.item_selected.connect(_on_LootList_item_selected)
    loot_name.text_changed.connect(_on_LootName_text_changed)
    add_loot_btn.pressed.connect(_on_AddLoot_pressed)
    remove_loot_btn.pressed.connect(_on_RemoveLoot_pressed)

func _on_LootList_item_selected(index):
    selected_loot = database.loots[index]
    loot_name.text = selected_loot.name
    loot_items_list.clear()
    for item in selected_loot.items:
        loot_items_list.add_item("%s x%d (weight %.2f)" % [item.item_id, item.amount, item.weight])

func _on_LootName_text_changed(new_text):
    if selected_loot:
        selected_loot.name = new_text

func _on_AddLoot_pressed():
    if selected_loot:
        var loot_item = LootItem.new()
        loot_item.item_id = ""
        loot_item.amount = 1
        loot_item.weight = 1.0
        selected_loot.items.append(loot_item)
        _on_LootList_item_selected(loot_list.get_selected_items()[0])

func _on_RemoveLoot_pressed():
    if selected_loot and selected_loot.items.size() > 0:
        selected_loot.items.pop_back()
        _on_LootList_item_selected(loot_list.get_selected_items()[0])
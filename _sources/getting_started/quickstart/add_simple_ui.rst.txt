##########################
Add Simple UI
##########################

This guide shows how to connect a UI to your inventory already created in the last guide. 
The idea of this plugin is for the developer to create their own UIs, as each game will have its own UI nuances, 
but UI scripts placed in the addons/inventory-system-demos/ui folder can be used as bases.

==============================
Adding a UI
==============================

Add a SimpleInventoryUI node to the previously created scene, switch to 2D view and place this node to fill the entire screen:

.. image:: ./images/add_simple_ui_to_scene.gif

.. warning::
    This script is only in the plugin demos, it is only downloaded from the addon branch or via godot asset lib, it is not included via git modules.

Now let's define the connection between the inventory and the UI. To do this, select the newly created Simple Inventory UI node and in the inspector, place the reference to the Inventory node:

.. image:: ./images/simple_ui_inspector.png
    :width: 50%

.. image:: ./images/simple_ui_inspector_filled.png
    :width: 50%

Done, now when you press play and use the :guilabel:`&F1` keys to add items, you will be able to see your items being displayed with their information such as name and icon:

.. image:: ./images/run_simple_ui.gif

.. note::
    Play around here, go to the inventory editor and change the stack that can be stored to check that this number implies how many items are in each stack of this inventory.

==============================================
Understanding the script simple_inventory_ui.gd
==============================================

Let's understand a little bit of the code previously added to our scene. In the ready code we can see that two nodes are created at runtime, 
a VBoxContainer and an ItemList, the first is used only to organize the content vertically in case we have additional buttons (This is not the case here), 
while the item list is used to place the items and populate the UI.

.. code-block:: gdscript

    if Engine.is_editor_hint():
		# Clean up, in case it is duplicated in the editor
		if is_instance_valid(_vbox_container):
			_vbox_container.queue_free()

	_vbox_container = VBoxContainer.new()
	_vbox_container.size_flags_horizontal = SIZE_EXPAND_FILL
	_vbox_container.size_flags_vertical = SIZE_EXPAND_FILL
	_vbox_container.anchor_right = 1.0
	_vbox_container.anchor_bottom = 1.0
	add_child(_vbox_container)

	_item_list = ItemList.new()
	_item_list.size_flags_horizontal = SIZE_EXPAND_FILL
	_item_list.size_flags_vertical = SIZE_EXPAND_FILL
	_item_list.item_activated.connect(_on_list_item_activated)
	_item_list.item_clicked.connect(_on_list_item_clicked)
	_item_list.select_mode = select_mode
	_vbox_container.add_child(_item_list)

	if has_node(inventory_path):
		inventory = get_node(inventory_path)

	_queue_refresh()


Here we can see that when an inventory is defined (and also at initialization) it connects the inventory change signals to call the _queue_refresh() and _update_stack() methods.
_queue_refresh() will update how many items there are and whether to remove or add new ones, while _update_stack() will update the contents of each item in the inventory. 

.. code-block:: gdscript

    var inventory: Inventory = null:
        set(new_inventory):
            if new_inventory == inventory:
                return
        
            _disconnect_inventory_signals()
            inventory = new_inventory
            _connect_inventory_signals()

            _queue_refresh()


    func _connect_inventory_signals() -> void:
        if !is_instance_valid(inventory):
            return
        if !inventory.contents_changed.is_connected(_queue_refresh):
            inventory.contents_changed.connect(_queue_refresh)
        if !inventory.updated_stack.is_connected(_on_stack_updated):
            inventory.updated_stack.connect(_on_stack_updated)

.. note::
    Understanding how the signals in a :ref:`class_inventory` are used is essential to make important and functional connections for your game, 
    such as updating the UI, making an automatic craft happen or needing to update the client-server in the case of a multiplayer game.
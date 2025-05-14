########################
Serialize Inventory
########################

To save your game and/or synchronize network games, you can serialize and deserialize an inventory using the following functions:

.. code-block::

    # Serialize Inventory 
    var serialized_inventory : Dictionary = inventory.serialize()

    # Deserialize Inventory
    inventory.deserialize(serialized_inventory)


.. warning::

    Attention: Deserialize inventory will remove all items in the inventory and replace them with the serialized data.
######################
Item Definition
######################

Editor
==============================

- :guilabel:`&ID`: String identification of the item, used to get the item based on a list of items easily using the id.
        You can use the button to edit the id correctly, it will indicate in red if the id already exists.

.. note:: 
    The base node (NodeInventories) has two methods that use the database field (InventoryDatabase) that use this id to facilitate access to items

.. code-block:: gdscript
    # Get id of item
    int get_id_from_item(item: ItemDefinition)

    # Get resource (ItemDefinition) from id param
    ItemDefinition get_item_from_id(id: int)
    

- :guilabel:`&Icon`: Texture to display your item in-game and in the editor
- :guilabel:`&Name`: Name to be displayed or used as additional identification (It is your responsibility to use this field)
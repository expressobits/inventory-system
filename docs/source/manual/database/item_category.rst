#######################
Item Category
#######################

You can use item categories to highlight a slot that only accepts that type of item.

.. image:: https://github.com/expressobits/inventory-system/assets/1673249/9294b461-8d61-41e0-9980-60c5dca21136

.. note::   
    Creating by inventory editor or without, results in the same resource database within the data.

=============================
Creating
=============================

1. Open Inventory Editor

.. image:: https://github.com/expressobits/inventory-system/assets/1673249/2bf12f88-031b-43b5-9b4c-65fa7aabc368

2. Click on "New Item Category" button

.. image:: ./images/menu_new_item_category.png

3. Edit a new category created

.. image:: https://github.com/expressobits/inventory-system/assets/1673249/129e12b2-a553-4b91-8f47-d0537545474c

=============================
Adding a Category to an Item
=============================

1. Open the item you want to put your category

.. image:: https://github.com/expressobits/inventory-system/assets/1673249/e5512cdd-48fd-4447-9b71-eeca8480e6d0

2. Check the categories this item has

.. image:: https://github.com/expressobits/inventory-system/assets/1673249/a987ead2-2c46-4642-ac0c-22e9e9f1f1fd

If the category contains custom properties and the item does not, they will be created for the item. Removing the category does not remove its related properties.

=============================
Viewing item category in code
=============================

The item category can be checked by the code:

.. code-block:: gdscript

    var item: ItemDefinition = db.get_item("my_item_id")
    var category: ItemCategory
    if item.is_in_category(category):
        print("Item has category")
    else:
        print("Item does not have category")


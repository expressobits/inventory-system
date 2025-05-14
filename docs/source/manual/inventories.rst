.. _inventories:

######################
Inventories
######################

:ref:`class_inventory` is node with stack container. Each stack contains data about amount, item_id and custom properties.
Stacks are stored in an Array, which can be accessed with inventory.stacks, but it is best to avoid changing data directly and instead use methods within this node,
such as add(), remove(), set_stack_content(), ...
The methods correctly call signals when necessary.
Some methods have their general version and a direct version of some [Stack] index, such as contains() and contains_at().

As demonstrated in the initial guide on adding items, it is also possible to remove them using the
:ref:`class_Inventory_method_remove` and :ref:`class_Inventory_method_remove_at` functions.

.. toctree::
   :maxdepth: 1
   :caption: Inventories
   
   inventories/serialize_inventory
   inventories/constraints

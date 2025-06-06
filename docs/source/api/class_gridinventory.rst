:github_url: hide

.. DO NOT EDIT THIS FILE!!!
.. Generated automatically from Godot engine sources.
.. Generator: https://github.com/godotengine/godot/tree/master/doc/tools/make_rst.py.
.. XML source: https://github.com/godotengine/godot/tree/master/doc_classes/GridInventory.xml.

.. _class_GridInventory:

GridInventory
=============

**Inherits:** :ref:`Inventory<class_Inventory>` **<** :ref:`NodeInventories<class_NodeInventories>` **<** ``Node``

.. container:: contribute

	There is currently no description for this class. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-reftable-group

Properties
----------

.. table::
   :widths: auto

   +--------------------------------------------------------------------------------------------+------------------------------------------------------------------------+--------------------+
   | :ref:`Array<class_Array>`\[:ref:`GridInventoryConstraint<class_GridInventoryConstraint>`\] | :ref:`grid_constraints<class_GridInventory_property_grid_constraints>` | ``[]``             |
   +--------------------------------------------------------------------------------------------+------------------------------------------------------------------------+--------------------+
   | ``Vector2i``                                                                               | :ref:`size<class_GridInventory_property_size>`                         | ``Vector2i(8, 8)`` |
   +--------------------------------------------------------------------------------------------+------------------------------------------------------------------------+--------------------+
   | :ref:`Array<class_Array>`\[``Vector2i``\]                                                  | :ref:`stack_positions<class_GridInventory_property_stack_positions>`   | ``[]``             |
   +--------------------------------------------------------------------------------------------+------------------------------------------------------------------------+--------------------+
   | :ref:`Array<class_Array>`\[``bool``\]                                                      | :ref:`stack_rotations<class_GridInventory_property_stack_rotations>`   | ``[]``             |
   +--------------------------------------------------------------------------------------------+------------------------------------------------------------------------+--------------------+

.. rst-class:: classref-reftable-group

Methods
-------

.. table::
   :widths: auto

   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``int``                                                        | :ref:`add_at_position<class_GridInventory_method_add_at_position>`\ (\ position\: ``Vector2i``, item_id\: ``String``, amount\: ``int`` = 1, properties\: ``Dictionary`` = {}, is_rotated\: ``bool`` = false\ )                                    |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``bool``                                                       | :ref:`can_rotate_item<class_GridInventory_method_can_rotate_item>`\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const|                                                                                                                       |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | :ref:`QuadTree<class_QuadTree>`                                | :ref:`get_quad_tree<class_GridInventory_method_get_quad_tree>`\ (\ ) |const|                                                                                                                                                                      |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | :ref:`ItemStack<class_ItemStack>`                              | :ref:`get_stack_at<class_GridInventory_method_get_stack_at>`\ (\ position\: ``Vector2i``\ ) |const|                                                                                                                                               |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``int``                                                        | :ref:`get_stack_index_at<class_GridInventory_method_get_stack_index_at>`\ (\ position\: ``Vector2i``\ ) |const|                                                                                                                                   |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``Vector2i``                                                   | :ref:`get_stack_position<class_GridInventory_method_get_stack_position>`\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const|                                                                                                                 |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``Rect2i``                                                     | :ref:`get_stack_rect<class_GridInventory_method_get_stack_rect>`\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const|                                                                                                                         |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``Vector2i``                                                   | :ref:`get_stack_size<class_GridInventory_method_get_stack_size>`\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const|                                                                                                                         |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | :ref:`Array<class_Array>`\[:ref:`ItemStack<class_ItemStack>`\] | :ref:`get_stacks_under<class_GridInventory_method_get_stacks_under>`\ (\ rect\: ``Rect2i``\ ) |const|                                                                                                                                             |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``bool``                                                       | :ref:`has_free_place<class_GridInventory_method_has_free_place>`\ (\ stack_size\: ``Vector2i``, exception\: :ref:`ItemStack<class_ItemStack>` = null\ ) |const|                                                                                   |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``bool``                                                       | :ref:`is_stack_rotated<class_GridInventory_method_is_stack_rotated>`\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const|                                                                                                                     |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``bool``                                                       | :ref:`rect_free<class_GridInventory_method_rect_free>`\ (\ rect\: ``Rect2i``, exception\: :ref:`ItemStack<class_ItemStack>` = null\ ) |const|                                                                                                     |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | |void|                                                         | :ref:`rotate<class_GridInventory_method_rotate>`\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ )                                                                                                                                                 |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | |void|                                                         | :ref:`set_quad_tree<class_GridInventory_method_set_quad_tree>`\ (\ quad_tree\: :ref:`QuadTree<class_QuadTree>`\ )                                                                                                                                 |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``bool``                                                       | :ref:`set_stack_position<class_GridInventory_method_set_stack_position>`\ (\ stack\: :ref:`ItemStack<class_ItemStack>`, new_position\: ``Vector2i``\ )                                                                                            |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``bool``                                                       | :ref:`sort<class_GridInventory_method_sort>`\ (\ )                                                                                                                                                                                                |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``bool``                                                       | :ref:`swap_stacks<class_GridInventory_method_swap_stacks>`\ (\ position\: ``Vector2i``, other_inventory\: :ref:`GridInventory<class_GridInventory>`, other_position\: ``Vector2i``\ )                                                             |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
   | ``int``                                                        | :ref:`transfer_to<class_GridInventory_method_transfer_to>`\ (\ from_position\: ``Vector2i``, destination\: :ref:`GridInventory<class_GridInventory>`, destination_position\: ``Vector2i``, amount\: ``int`` = 1, is_rotated\: ``bool`` = false\ ) |
   +----------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

.. rst-class:: classref-section-separator

----

.. rst-class:: classref-descriptions-group

Signals
-------

.. _class_GridInventory_signal_size_changed:

.. rst-class:: classref-signal

**size_changed**\ (\ ) :ref:`🔗<class_GridInventory_signal_size_changed>`

.. container:: contribute

	There is currently no description for this signal. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-section-separator

----

.. rst-class:: classref-descriptions-group

Property Descriptions
---------------------

.. _class_GridInventory_property_grid_constraints:

.. rst-class:: classref-property

:ref:`Array<class_Array>`\[:ref:`GridInventoryConstraint<class_GridInventoryConstraint>`\] **grid_constraints** = ``[]`` :ref:`🔗<class_GridInventory_property_grid_constraints>`

.. rst-class:: classref-property-setget

- |void| **set_grid_constraints**\ (\ value\: :ref:`Array<class_Array>`\[:ref:`GridInventoryConstraint<class_GridInventoryConstraint>`\]\ )
- :ref:`Array<class_Array>`\[:ref:`GridInventoryConstraint<class_GridInventoryConstraint>`\] **get_grid_constraints**\ (\ )

.. container:: contribute

	There is currently no description for this property. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_property_size:

.. rst-class:: classref-property

``Vector2i`` **size** = ``Vector2i(8, 8)`` :ref:`🔗<class_GridInventory_property_size>`

.. rst-class:: classref-property-setget

- |void| **set_size**\ (\ value\: ``Vector2i``\ )
- ``Vector2i`` **get_size**\ (\ )

.. container:: contribute

	There is currently no description for this property. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_property_stack_positions:

.. rst-class:: classref-property

:ref:`Array<class_Array>`\[``Vector2i``\] **stack_positions** = ``[]`` :ref:`🔗<class_GridInventory_property_stack_positions>`

.. rst-class:: classref-property-setget

- |void| **set_stack_positions**\ (\ value\: :ref:`Array<class_Array>`\[``Vector2i``\]\ )
- :ref:`Array<class_Array>`\[``Vector2i``\] **get_stack_positions**\ (\ )

.. container:: contribute

	There is currently no description for this property. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_property_stack_rotations:

.. rst-class:: classref-property

:ref:`Array<class_Array>`\[``bool``\] **stack_rotations** = ``[]`` :ref:`🔗<class_GridInventory_property_stack_rotations>`

.. rst-class:: classref-property-setget

- |void| **set_stack_rotations**\ (\ value\: :ref:`Array<class_Array>`\[``bool``\]\ )
- :ref:`Array<class_Array>`\[``bool``\] **get_stack_rotations**\ (\ )

.. container:: contribute

	There is currently no description for this property. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-section-separator

----

.. rst-class:: classref-descriptions-group

Method Descriptions
-------------------

.. _class_GridInventory_method_add_at_position:

.. rst-class:: classref-method

``int`` **add_at_position**\ (\ position\: ``Vector2i``, item_id\: ``String``, amount\: ``int`` = 1, properties\: ``Dictionary`` = {}, is_rotated\: ``bool`` = false\ ) :ref:`🔗<class_GridInventory_method_add_at_position>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_can_rotate_item:

.. rst-class:: classref-method

``bool`` **can_rotate_item**\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const| :ref:`🔗<class_GridInventory_method_can_rotate_item>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_get_quad_tree:

.. rst-class:: classref-method

:ref:`QuadTree<class_QuadTree>` **get_quad_tree**\ (\ ) |const| :ref:`🔗<class_GridInventory_method_get_quad_tree>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_get_stack_at:

.. rst-class:: classref-method

:ref:`ItemStack<class_ItemStack>` **get_stack_at**\ (\ position\: ``Vector2i``\ ) |const| :ref:`🔗<class_GridInventory_method_get_stack_at>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_get_stack_index_at:

.. rst-class:: classref-method

``int`` **get_stack_index_at**\ (\ position\: ``Vector2i``\ ) |const| :ref:`🔗<class_GridInventory_method_get_stack_index_at>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_get_stack_position:

.. rst-class:: classref-method

``Vector2i`` **get_stack_position**\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const| :ref:`🔗<class_GridInventory_method_get_stack_position>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_get_stack_rect:

.. rst-class:: classref-method

``Rect2i`` **get_stack_rect**\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const| :ref:`🔗<class_GridInventory_method_get_stack_rect>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_get_stack_size:

.. rst-class:: classref-method

``Vector2i`` **get_stack_size**\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const| :ref:`🔗<class_GridInventory_method_get_stack_size>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_get_stacks_under:

.. rst-class:: classref-method

:ref:`Array<class_Array>`\[:ref:`ItemStack<class_ItemStack>`\] **get_stacks_under**\ (\ rect\: ``Rect2i``\ ) |const| :ref:`🔗<class_GridInventory_method_get_stacks_under>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_has_free_place:

.. rst-class:: classref-method

``bool`` **has_free_place**\ (\ stack_size\: ``Vector2i``, exception\: :ref:`ItemStack<class_ItemStack>` = null\ ) |const| :ref:`🔗<class_GridInventory_method_has_free_place>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_is_stack_rotated:

.. rst-class:: classref-method

``bool`` **is_stack_rotated**\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) |const| :ref:`🔗<class_GridInventory_method_is_stack_rotated>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_rect_free:

.. rst-class:: classref-method

``bool`` **rect_free**\ (\ rect\: ``Rect2i``, exception\: :ref:`ItemStack<class_ItemStack>` = null\ ) |const| :ref:`🔗<class_GridInventory_method_rect_free>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_rotate:

.. rst-class:: classref-method

|void| **rotate**\ (\ stack\: :ref:`ItemStack<class_ItemStack>`\ ) :ref:`🔗<class_GridInventory_method_rotate>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_set_quad_tree:

.. rst-class:: classref-method

|void| **set_quad_tree**\ (\ quad_tree\: :ref:`QuadTree<class_QuadTree>`\ ) :ref:`🔗<class_GridInventory_method_set_quad_tree>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_set_stack_position:

.. rst-class:: classref-method

``bool`` **set_stack_position**\ (\ stack\: :ref:`ItemStack<class_ItemStack>`, new_position\: ``Vector2i``\ ) :ref:`🔗<class_GridInventory_method_set_stack_position>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_sort:

.. rst-class:: classref-method

``bool`` **sort**\ (\ ) :ref:`🔗<class_GridInventory_method_sort>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_swap_stacks:

.. rst-class:: classref-method

``bool`` **swap_stacks**\ (\ position\: ``Vector2i``, other_inventory\: :ref:`GridInventory<class_GridInventory>`, other_position\: ``Vector2i``\ ) :ref:`🔗<class_GridInventory_method_swap_stacks>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. rst-class:: classref-item-separator

----

.. _class_GridInventory_method_transfer_to:

.. rst-class:: classref-method

``int`` **transfer_to**\ (\ from_position\: ``Vector2i``, destination\: :ref:`GridInventory<class_GridInventory>`, destination_position\: ``Vector2i``, amount\: ``int`` = 1, is_rotated\: ``bool`` = false\ ) :ref:`🔗<class_GridInventory_method_transfer_to>`

.. container:: contribute

	There is currently no description for this method. Please help us by :ref:`contributing one <doc_updating_the_class_reference>`!

.. |virtual| replace:: :abbr:`virtual (This method should typically be overridden by the user to have any effect.)`
.. |const| replace:: :abbr:`const (This method has no side effects. It doesn't modify any of the instance's member variables.)`
.. |vararg| replace:: :abbr:`vararg (This method accepts any number of arguments after the ones described here.)`
.. |constructor| replace:: :abbr:`constructor (This method is used to construct a type.)`
.. |static| replace:: :abbr:`static (This method doesn't need an instance to be called, so it can be called directly using the class name.)`
.. |operator| replace:: :abbr:`operator (This method describes a valid operator to use with this type as left-hand operand.)`
.. |bitfield| replace:: :abbr:`BitField (This value is an integer composed as a bitmask of the following flags.)`
.. |void| replace:: :abbr:`void (No return value.)`

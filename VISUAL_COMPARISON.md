## Visual Comparison: Before vs After Fix

### Issue #131: I key should close inventory in examples

```
BEFORE FIX (Problematic Behavior):
=====================================

State: Nothing Open
User presses "I" → ✅ Inventory Opens

State: Inventory Open  
User presses "I" → ❌ Nothing happens (BUG!)
User must press "Esc" → Inventory Closes

State: Craft Station Open
User presses "I" → ✅ Nothing happens (correct)


AFTER FIX (Correct Behavior):
==============================

State: Nothing Open
User presses "I" → ✅ Inventory Opens

State: Inventory Open
User presses "I" → ✅ Inventory Closes (FIXED!)
User can also press "Esc" → Inventory Closes

State: Craft Station Open  
User presses "I" → ✅ Nothing happens (preserved)


Code Change Summary:
===================

OLD CODE:
---------
if Input.is_action_just_released(toggle_inventory_input):
    if not is_any_station_or_inventory_opened():
        open_main_inventory()

NEW CODE:
---------
if Input.is_action_just_released(toggle_inventory_input):
    if is_open_main_inventory():
        close_inventory(main_inventory)  # <-- ADDED: Close when open
    elif not is_any_station_or_inventory_opened():
        open_main_inventory()


Key Improvements:
================
✅ "I" key now properly toggles inventory (open ↔ closed)
✅ User experience is more intuitive 
✅ Maintains all existing behaviors
✅ Preserves craft station priority
✅ No breaking changes
```
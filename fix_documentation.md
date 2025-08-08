# Fix for Issue #131: I key should close inventory in examples

## Problem Description
In the demo(s), pressing "I" opens the inventory, but pressing "I" again does nothing. You must press "Esc" to close the inventory. The "I" key should also close the inventory in the examples.

## Root Cause
The issue was in the `inventory_inputs()` function in `character_inventory_system.gd`. The function only handled opening the inventory when the toggle key ("I") was pressed, but didn't handle closing it when the inventory was already open.

### Original Code (Problematic)
```gdscript
func inventory_inputs():
    if Input.is_action_just_released(toggle_inventory_input):
        if not is_any_station_or_inventory_opened():
            open_main_inventory()
```

This only opened the inventory if nothing was currently open, but didn't provide a way to close it using the same key.

### Fixed Code
```gdscript
func inventory_inputs():
    if Input.is_action_just_released(toggle_inventory_input):
        if is_open_main_inventory():
            # If inventory is open, close it
            close_inventory(main_inventory)
        elif not is_any_station_or_inventory_opened():
            # If nothing is open, open the inventory
            open_main_inventory()
```

## Solution Explanation
The fix adds proper toggle behavior by:

1. **Checking if the main inventory is currently open** - If it is, close it
2. **Only opening the inventory if nothing else is open** - This preserves the existing behavior where craft panels take priority

## Key Changes Made

### File: `addons/inventory-system-demos/character/character_inventory_system.gd`
- **Lines 109-112**: Modified the toggle inventory input handling to include closing logic
- **Added condition**: `if is_open_main_inventory():`
- **Added action**: `close_inventory(main_inventory)`

## Testing Scenarios

The fix should handle these scenarios correctly:

1. **Press "I" when nothing is open** → Opens inventory ✅
2. **Press "I" when inventory is open** → Closes inventory ✅ (NEW BEHAVIOR)
3. **Press "I" when craft panel is open** → Does nothing ✅ (PRESERVED)
4. **Press "Esc" at any time** → Closes all inventories and craft panels ✅ (PRESERVED)

## Edge Cases Considered

- **Craft stations are open**: The "I" key won't interfere because the condition checks `elif not is_any_station_or_inventory_opened()`
- **Multiple inventories**: Only the main inventory is toggled, other inventories maintain their existing behavior
- **Mouse capture state**: Properly maintained through existing `_check_inputs()` mechanism

## Implementation Notes

This is a minimal change that:
- ✅ Fixes the specific issue mentioned
- ✅ Preserves all existing functionality  
- ✅ Doesn't break any other interactions
- ✅ Uses existing functions (`close_inventory`, `is_open_main_inventory`)
- ✅ Maintains the same input handling pattern
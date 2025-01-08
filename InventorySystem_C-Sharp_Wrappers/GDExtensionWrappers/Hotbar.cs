using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class Hotbar : NodeInventories
{
    public new static readonly StringName GDExtensionName = "Hotbar";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Node), please use the Instantiate() method instead.")]
    protected Hotbar() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="Hotbar"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public new static Hotbar Instantiate()
    {
        return GDExtensionHelper.Instantiate<Hotbar>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="Hotbar"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="Hotbar"/> wrapper type,
    /// a new instance of the <see cref="Hotbar"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="Hotbar"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public new static Hotbar Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<Hotbar>(godotObject);
    }
#region Properties

    public NodePath Inventory
    {
        get => (NodePath)Get("inventory");
        set => Set("inventory", Variant.From(value));
    }

    public int MaxSlots
    {
        get => (int)Get("max_slots");
        set => Set("max_slots", Variant.From(value));
    }

    public int SelectionIndex
    {
        get => (int)Get("selection_index");
        set => Set("selection_index", Variant.From(value));
    }

#endregion

#region Signals

    public delegate void OnChangeSelectionHandler(int selectionIndex);

    private OnChangeSelectionHandler _onChangeSelection_backing;
    private Callable _onChangeSelection_backing_callable;
    public event OnChangeSelectionHandler OnChangeSelection
    {
        add
        {
            if(_onChangeSelection_backing == null)
            {
                _onChangeSelection_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _onChangeSelection_backing?.Invoke(arg0);
                    }
                );
                Connect("on_change_selection", _onChangeSelection_backing_callable);
            }
            _onChangeSelection_backing += value;
        }
        remove
        {
            _onChangeSelection_backing -= value;
            
            if(_onChangeSelection_backing == null)
            {
                Disconnect("on_change_selection", _onChangeSelection_backing_callable);
                _onChangeSelection_backing_callable = default;
            }
        }
    }

    public delegate void EquippedStackChangedHandler(int slotIndex);

    private EquippedStackChangedHandler _equippedStackChanged_backing;
    private Callable _equippedStackChanged_backing_callable;
    public event EquippedStackChangedHandler EquippedStackChanged
    {
        add
        {
            if(_equippedStackChanged_backing == null)
            {
                _equippedStackChanged_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _equippedStackChanged_backing?.Invoke(arg0);
                    }
                );
                Connect("equipped_stack_changed", _equippedStackChanged_backing_callable);
            }
            _equippedStackChanged_backing += value;
        }
        remove
        {
            _equippedStackChanged_backing -= value;
            
            if(_equippedStackChanged_backing == null)
            {
                Disconnect("equipped_stack_changed", _equippedStackChanged_backing_callable);
                _equippedStackChanged_backing_callable = default;
            }
        }
    }

    public delegate void EquippedHandler(int slotIndex);

    private EquippedHandler _equipped_backing;
    private Callable _equipped_backing_callable;
    public event EquippedHandler Equipped
    {
        add
        {
            if(_equipped_backing == null)
            {
                _equipped_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _equipped_backing?.Invoke(arg0);
                    }
                );
                Connect("equipped", _equipped_backing_callable);
            }
            _equipped_backing += value;
        }
        remove
        {
            _equipped_backing -= value;
            
            if(_equipped_backing == null)
            {
                Disconnect("equipped", _equipped_backing_callable);
                _equipped_backing_callable = default;
            }
        }
    }

    public delegate void UnequippedHandler(int slotIndex);

    private UnequippedHandler _unequipped_backing;
    private Callable _unequipped_backing_callable;
    public event UnequippedHandler Unequipped
    {
        add
        {
            if(_unequipped_backing == null)
            {
                _unequipped_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _unequipped_backing?.Invoke(arg0);
                    }
                );
                Connect("unequipped", _unequipped_backing_callable);
            }
            _unequipped_backing += value;
        }
        remove
        {
            _unequipped_backing -= value;
            
            if(_unequipped_backing == null)
            {
                Disconnect("unequipped", _unequipped_backing_callable);
                _unequipped_backing_callable = default;
            }
        }
    }

#endregion

#region Methods

    public void SetInventoryPath(NodePath inventory) => Call("set_inventory_path", inventory);

    public NodePath GetInventoryPath() => Call("get_inventory_path").As<NodePath>();

    public void ActiveSlot(int slotIndex) => Call("active_slot", slotIndex);

    public void DeactiveSlot(int slotIndex) => Call("deactive_slot", slotIndex);

    public bool IsActiveSlot(int slotIndex) => Call("is_active_slot", slotIndex).As<bool>();

    public void Equip(ItemStack stack, int slotIndex) => Call("equip", (Resource)stack, slotIndex);

    public void Unequip(int slotIndex) => Call("unequip", slotIndex);

    public void NextItem() => Call("next_item");

    public void PreviousItem() => Call("previous_item");

    public bool HasValidStackOnSlot(int slotIndex) => Call("has_valid_stack_on_slot", slotIndex).As<bool>();

    public bool HasValidStackOnSelection() => Call("has_valid_stack_on_selection").As<bool>();

    public ItemStack GetStackOnSlot(int slotIndex) => GDExtensionHelper.Bind<ItemStack>(Call("get_stack_on_slot", slotIndex).As<GodotObject>());

    public ItemStack GetStackOnSelection() => GDExtensionHelper.Bind<ItemStack>(Call("get_stack_on_selection").As<GodotObject>());

#endregion

}
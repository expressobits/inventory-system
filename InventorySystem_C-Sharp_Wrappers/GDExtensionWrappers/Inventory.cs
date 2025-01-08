using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class Inventory : NodeInventories
{
    public new static readonly StringName GDExtensionName = "Inventory";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Node), please use the Instantiate() method instead.")]
    protected Inventory() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="Inventory"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public new static Inventory Instantiate()
    {
        return GDExtensionHelper.Instantiate<Inventory>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="Inventory"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="Inventory"/> wrapper type,
    /// a new instance of the <see cref="Inventory"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="Inventory"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public new static Inventory Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<Inventory>(godotObject);
    }
#region Properties

    public Godot.Collections.Array<ItemStack> Stacks
    {
        get => GDExtensionHelper.Cast<ItemStack>((Godot.Collections.Array<Godot.GodotObject>)Get("stacks"));
        set => Set("stacks", Variant.From(value));
    }

    public string InventoryName
    {
        get => (string)Get("inventory_name");
        set => Set("inventory_name", Variant.From(value));
    }

    public Godot.Collections.Array<InventoryConstraint> Constraints
    {
        get => GDExtensionHelper.Cast<InventoryConstraint>((Godot.Collections.Array<Godot.GodotObject>)Get("constraints"));
        set => Set("constraints", Variant.From(value));
    }

#endregion

#region Signals

    public delegate void ContentsChangedHandler();

    private ContentsChangedHandler _contentsChanged_backing;
    private Callable _contentsChanged_backing_callable;
    public event ContentsChangedHandler ContentsChanged
    {
        add
        {
            if(_contentsChanged_backing == null)
            {
                _contentsChanged_backing_callable = Callable.From(
                    () =>
                    {
                        _contentsChanged_backing?.Invoke();
                    }
                );
                Connect("contents_changed", _contentsChanged_backing_callable);
            }
            _contentsChanged_backing += value;
        }
        remove
        {
            _contentsChanged_backing -= value;
            
            if(_contentsChanged_backing == null)
            {
                Disconnect("contents_changed", _contentsChanged_backing_callable);
                _contentsChanged_backing_callable = default;
            }
        }
    }

    public delegate void StackAddedHandler(int stackIndex);

    private StackAddedHandler _stackAdded_backing;
    private Callable _stackAdded_backing_callable;
    public event StackAddedHandler StackAdded
    {
        add
        {
            if(_stackAdded_backing == null)
            {
                _stackAdded_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _stackAdded_backing?.Invoke(arg0);
                    }
                );
                Connect("stack_added", _stackAdded_backing_callable);
            }
            _stackAdded_backing += value;
        }
        remove
        {
            _stackAdded_backing -= value;
            
            if(_stackAdded_backing == null)
            {
                Disconnect("stack_added", _stackAdded_backing_callable);
                _stackAdded_backing_callable = default;
            }
        }
    }

    public delegate void StackRemovedHandler(int stackIndex);

    private StackRemovedHandler _stackRemoved_backing;
    private Callable _stackRemoved_backing_callable;
    public event StackRemovedHandler StackRemoved
    {
        add
        {
            if(_stackRemoved_backing == null)
            {
                _stackRemoved_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _stackRemoved_backing?.Invoke(arg0);
                    }
                );
                Connect("stack_removed", _stackRemoved_backing_callable);
            }
            _stackRemoved_backing += value;
        }
        remove
        {
            _stackRemoved_backing -= value;
            
            if(_stackRemoved_backing == null)
            {
                Disconnect("stack_removed", _stackRemoved_backing_callable);
                _stackRemoved_backing_callable = default;
            }
        }
    }

    public delegate void ItemAddedHandler(string itemId, int amount);

    private ItemAddedHandler _itemAdded_backing;
    private Callable _itemAdded_backing_callable;
    public event ItemAddedHandler ItemAdded
    {
        add
        {
            if(_itemAdded_backing == null)
            {
                _itemAdded_backing_callable = Callable.From<Variant, Variant>(
                    (arg0_variant, arg1_variant) =>
                    {
                        var arg0 = arg0_variant.As<string>();
                        var arg1 = arg1_variant.As<int>();
                        _itemAdded_backing?.Invoke(arg0, arg1);
                    }
                );
                Connect("item_added", _itemAdded_backing_callable);
            }
            _itemAdded_backing += value;
        }
        remove
        {
            _itemAdded_backing -= value;
            
            if(_itemAdded_backing == null)
            {
                Disconnect("item_added", _itemAdded_backing_callable);
                _itemAdded_backing_callable = default;
            }
        }
    }

    public delegate void ItemRemovedHandler(string itemId, int amount);

    private ItemRemovedHandler _itemRemoved_backing;
    private Callable _itemRemoved_backing_callable;
    public event ItemRemovedHandler ItemRemoved
    {
        add
        {
            if(_itemRemoved_backing == null)
            {
                _itemRemoved_backing_callable = Callable.From<Variant, Variant>(
                    (arg0_variant, arg1_variant) =>
                    {
                        var arg0 = arg0_variant.As<string>();
                        var arg1 = arg1_variant.As<int>();
                        _itemRemoved_backing?.Invoke(arg0, arg1);
                    }
                );
                Connect("item_removed", _itemRemoved_backing_callable);
            }
            _itemRemoved_backing += value;
        }
        remove
        {
            _itemRemoved_backing -= value;
            
            if(_itemRemoved_backing == null)
            {
                Disconnect("item_removed", _itemRemoved_backing_callable);
                _itemRemoved_backing_callable = default;
            }
        }
    }

    public delegate void FilledHandler();

    private FilledHandler _filled_backing;
    private Callable _filled_backing_callable;
    public event FilledHandler Filled
    {
        add
        {
            if(_filled_backing == null)
            {
                _filled_backing_callable = Callable.From(
                    () =>
                    {
                        _filled_backing?.Invoke();
                    }
                );
                Connect("filled", _filled_backing_callable);
            }
            _filled_backing += value;
        }
        remove
        {
            _filled_backing -= value;
            
            if(_filled_backing == null)
            {
                Disconnect("filled", _filled_backing_callable);
                _filled_backing_callable = default;
            }
        }
    }

    public delegate void EmptiedHandler();

    private EmptiedHandler _emptied_backing;
    private Callable _emptied_backing_callable;
    public event EmptiedHandler Emptied
    {
        add
        {
            if(_emptied_backing == null)
            {
                _emptied_backing_callable = Callable.From(
                    () =>
                    {
                        _emptied_backing?.Invoke();
                    }
                );
                Connect("emptied", _emptied_backing_callable);
            }
            _emptied_backing += value;
        }
        remove
        {
            _emptied_backing -= value;
            
            if(_emptied_backing == null)
            {
                Disconnect("emptied", _emptied_backing_callable);
                _emptied_backing_callable = default;
            }
        }
    }

    public delegate void UpdatedStackHandler(int stackIndex);

    private UpdatedStackHandler _updatedStack_backing;
    private Callable _updatedStack_backing_callable;
    public event UpdatedStackHandler UpdatedStack
    {
        add
        {
            if(_updatedStack_backing == null)
            {
                _updatedStack_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _updatedStack_backing?.Invoke(arg0);
                    }
                );
                Connect("updated_stack", _updatedStack_backing_callable);
            }
            _updatedStack_backing += value;
        }
        remove
        {
            _updatedStack_backing -= value;
            
            if(_updatedStack_backing == null)
            {
                Disconnect("updated_stack", _updatedStack_backing_callable);
                _updatedStack_backing_callable = default;
            }
        }
    }

    public delegate void RequestDropObjHandler(string dropItemPackedScenePath, string itemId, int amount, Godot.Collections.Dictionary itemProperties);

    private RequestDropObjHandler _requestDropObj_backing;
    private Callable _requestDropObj_backing_callable;
    public event RequestDropObjHandler RequestDropObj
    {
        add
        {
            if(_requestDropObj_backing == null)
            {
                _requestDropObj_backing_callable = Callable.From<Variant, Variant, Variant, Variant>(
                    (arg0_variant, arg1_variant, arg2_variant, arg3_variant) =>
                    {
                        var arg0 = arg0_variant.As<string>();
                        var arg1 = arg1_variant.As<string>();
                        var arg2 = arg2_variant.As<int>();
                        var arg3 = arg3_variant.As<Godot.Collections.Dictionary>();
                        _requestDropObj_backing?.Invoke(arg0, arg1, arg2, arg3);
                    }
                );
                Connect("request_drop_obj", _requestDropObj_backing_callable);
            }
            _requestDropObj_backing += value;
        }
        remove
        {
            _requestDropObj_backing -= value;
            
            if(_requestDropObj_backing == null)
            {
                Disconnect("request_drop_obj", _requestDropObj_backing_callable);
                _requestDropObj_backing_callable = default;
            }
        }
    }

#endregion

#region Methods

    public void SetStackContent(int stackIndex, string itemId, int amount, Godot.Collections.Dictionary properties) => Call("set_stack_content", stackIndex, itemId, amount, properties);

    public bool IsEmpty() => Call("is_empty").As<bool>();

    public bool IsFull() => Call("is_full").As<bool>();

    public void Clear() => Call("clear");

    public bool Contains(string itemId, int amount) => Call("contains", itemId, amount).As<bool>();

    public bool ContainsAt(int stackIndex, string itemId, int amount) => Call("contains_at", stackIndex, itemId, amount).As<bool>();

    public bool ContainsCategory(ItemCategory category, int amount) => Call("contains_category", (Resource)category, amount).As<bool>();

    public bool HasSpaceFor(string item, int amount, Godot.Collections.Dictionary properties) => Call("has_space_for", item, amount, properties).As<bool>();

    public bool HasStack(ItemStack stack) => Call("has_stack", (Resource)stack).As<bool>();

    public int GetStackIndexWithAnItemOfCategory(ItemCategory category) => Call("get_stack_index_with_an_item_of_category", (Resource)category).As<int>();

    public int AmountOfItem(string itemId) => Call("amount_of_item", itemId).As<int>();

    public int GetAmountOfCategory(ItemCategory category) => Call("get_amount_of_category", (Resource)category).As<int>();

    public int GetAmount() => Call("get_amount").As<int>();

    public int Add(string itemId, int amount, Godot.Collections.Dictionary properties, bool dropExcess) => Call("add", itemId, amount, properties, dropExcess).As<int>();

    public int AddAtIndex(int stackIndex, string itemId, int amount, Godot.Collections.Dictionary properties) => Call("add_at_index", stackIndex, itemId, amount, properties).As<int>();

    public int AddOnNewStack(string itemId, int amount, Godot.Collections.Dictionary properties) => Call("add_on_new_stack", itemId, amount, properties).As<int>();

    public int Remove(string itemId, int amount) => Call("remove", itemId, amount).As<int>();

    public int RemoveAt(int stackIndex, string itemId, int amount) => Call("remove_at", stackIndex, itemId, amount).As<int>();

    public bool Split(int stackIndex, int amount) => Call("split", stackIndex, amount).As<bool>();

    public int TransferAt(int stackIndex, Inventory destination, int destinationStackIndex, int amount) => Call("transfer_at", stackIndex, (Node)destination, destinationStackIndex, amount).As<int>();

    public int Transfer(int stackIndex, Inventory destination, int amount) => Call("transfer", stackIndex, (Node)destination, amount).As<int>();

    public bool Drop(string itemId, int amount, Godot.Collections.Dictionary properties) => Call("drop", itemId, amount, properties).As<bool>();

    public void DropFromInventory(int stackIndex, int amount, Godot.Collections.Dictionary properties) => Call("drop_from_inventory", stackIndex, amount, properties);

    public int AddToStack(ItemStack stack, string itemId, int amount, Godot.Collections.Dictionary properties) => Call("add_to_stack", (Resource)stack, itemId, amount, properties).As<int>();

    public int RemoveFromStack(ItemStack stack, string itemId, int amount) => Call("remove_from_stack", (Resource)stack, itemId, amount).As<int>();

    public bool IsAcceptAnyCategories(int categoriesFlag, Godot.Collections.Array<ItemCategory> slot) => Call("is_accept_any_categories", categoriesFlag, slot).As<bool>();

    public bool ContainsCategoryInStack(ItemStack stack, ItemCategory category) => Call("contains_category_in_stack", (Resource)stack, (Resource)category).As<bool>();

    public Godot.Collections.Dictionary Serialize() => Call("serialize").As<Godot.Collections.Dictionary>();

    public void Deserialize(Godot.Collections.Dictionary data) => Call("deserialize", data);

    public bool CanAddNewStack(string itemId, int amount, Godot.Collections.Dictionary properties) => Call("can_add_new_stack", itemId, amount, properties).As<bool>();

    public void UpdateStack(int stackIndex) => Call("update_stack", stackIndex);

#endregion

}
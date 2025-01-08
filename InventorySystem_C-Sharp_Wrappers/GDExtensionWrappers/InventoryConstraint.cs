using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class InventoryConstraint : Resource
{
    public static readonly StringName GDExtensionName = "InventoryConstraint";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected InventoryConstraint() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="InventoryConstraint"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static InventoryConstraint Instantiate()
    {
        return GDExtensionHelper.Instantiate<InventoryConstraint>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="InventoryConstraint"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="InventoryConstraint"/> wrapper type,
    /// a new instance of the <see cref="InventoryConstraint"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="InventoryConstraint"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static InventoryConstraint Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<InventoryConstraint>(godotObject);
    }
#region Methods

    public virtual bool CanAddOnInventory(Node inventory, string itemId, int amount, Godot.Collections.Dictionary properties) => Call("_can_add_on_inventory", inventory, itemId, amount, properties).As<bool>();

    public virtual bool CanAddNewStackOnInventory(Node inventory, string itemId, int amount, Godot.Collections.Dictionary properties) => Call("_can_add_new_stack_on_inventory", inventory, itemId, amount, properties).As<bool>();

    public virtual int GetAmountToAdd(Node inventory, string itemId, int amount, Godot.Collections.Dictionary properties) => Call("_get_amount_to_add", inventory, itemId, amount, properties).As<int>();

    public virtual int GetMaxStack(Node inventory, string itemId, int amount, Godot.Collections.Dictionary properties) => Call("_get_max_stack", inventory, itemId, amount, properties).As<int>();

    public virtual bool IsOverrideMaxStack(Node inventory, string itemId, int amount, Godot.Collections.Dictionary properties) => Call("_is_override_max_stack", inventory, itemId, amount, properties).As<bool>();

#endregion

}
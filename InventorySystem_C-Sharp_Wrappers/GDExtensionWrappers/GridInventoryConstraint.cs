using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class GridInventoryConstraint : Resource
{
    public static readonly StringName GDExtensionName = "GridInventoryConstraint";

    //[Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected GridInventoryConstraint() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="GridInventoryConstraint"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static GridInventoryConstraint Instantiate()
    {
        return GDExtensionHelper.Instantiate<GridInventoryConstraint>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="GridInventoryConstraint"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="GridInventoryConstraint"/> wrapper type,
    /// a new instance of the <see cref="GridInventoryConstraint"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="GridInventoryConstraint"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static GridInventoryConstraint Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<GridInventoryConstraint>(godotObject);
    }
#region Methods

    public virtual bool CanAddOnPosition(Node inventory, Vector2I position, string itemId, int amount, Godot.Collections.Dictionary properties, bool isRotated) => Call("_can_add_on_position", inventory, position, itemId, amount, properties, isRotated).As<bool>();

#endregion

}
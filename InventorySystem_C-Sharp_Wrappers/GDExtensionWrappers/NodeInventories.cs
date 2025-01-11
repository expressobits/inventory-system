using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class NodeInventories : Node
{
    public static readonly StringName GDExtensionName = "NodeInventories";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Node), please use the Instantiate() method instead.")]
    protected NodeInventories() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="NodeInventories"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static NodeInventories Instantiate()
    {
        return GDExtensionHelper.Instantiate<NodeInventories>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="NodeInventories"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="NodeInventories"/> wrapper type,
    /// a new instance of the <see cref="NodeInventories"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="NodeInventories"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static NodeInventories Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<NodeInventories>(godotObject);
    }
#region Properties

    public InventoryDatabase Database
    {
        get => (InventoryDatabase)Get("database");
        set => Set("database", Variant.From(value));
    }

#endregion

#region Methods

    public ItemDefinition GetItemFromId(string id) => GDExtensionHelper.Bind<ItemDefinition>(Call("get_item_from_id", id).As<GodotObject>());

    public string GetIdFromItem(ItemDefinition item) => Call("get_id_from_item", (Resource)item).As<string>();

#endregion

}
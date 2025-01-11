using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class ItemDefinition : Resource
{
    public static readonly StringName GDExtensionName = "ItemDefinition";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected ItemDefinition() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="ItemDefinition"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static ItemDefinition Instantiate()
    {
        return GDExtensionHelper.Instantiate<ItemDefinition>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="ItemDefinition"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="ItemDefinition"/> wrapper type,
    /// a new instance of the <see cref="ItemDefinition"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="ItemDefinition"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static ItemDefinition Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<ItemDefinition>(godotObject);
    }
#region Properties

    public string Id
    {
        get => (string)Get("id");
        set => Set("id", Variant.From(value));
    }

    public bool CanStack
    {
        get => (bool)Get("can_stack");
        set => Set("can_stack", Variant.From(value));
    }

    public int MaxStack
    {
        get => (int)Get("max_stack");
        set => Set("max_stack", Variant.From(value));
    }

    public string Name
    {
        get => (string)Get("name");
        set => Set("name", Variant.From(value));
    }

    public Texture2D Icon
    {
        get => (Texture2D)Get("icon");
        set => Set("icon", Variant.From(value));
    }

    public float Weight
    {
        get => (float)Get("weight");
        set => Set("weight", Variant.From(value));
    }

    public Vector2I Size
    {
        get => (Vector2I)Get("size");
        set => Set("size", Variant.From(value));
    }

    public Godot.Collections.Dictionary Properties
    {
        get => (Godot.Collections.Dictionary)Get("properties");
        set => Set("properties", Variant.From(value));
    }

    public Godot.Collections.Array<String> DynamicProperties
    {
        get => (Godot.Collections.Array<String>)Get("dynamic_properties");
        set => Set("dynamic_properties", Variant.From(value));
    }

    public Godot.Collections.Array<ItemCategory> Categories
    {
        get => GDExtensionHelper.Cast<ItemCategory>((Godot.Collections.Array<Godot.GodotObject>)Get("categories"));
        set => Set("categories", Variant.From(value));
    }

#endregion

#region Methods

    public bool IsOfCategory(ItemCategory category) => Call("is_of_category", (Resource)category).As<bool>();

    public Vector2I GetRotatedSize() => Call("get_rotated_size").As<Vector2I>();

#endregion

}
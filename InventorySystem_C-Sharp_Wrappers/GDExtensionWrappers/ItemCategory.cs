using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class ItemCategory : Resource
{
    public static readonly StringName GDExtensionName = "ItemCategory";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected ItemCategory() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="ItemCategory"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static ItemCategory Instantiate()
    {
        return GDExtensionHelper.Instantiate<ItemCategory>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="ItemCategory"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="ItemCategory"/> wrapper type,
    /// a new instance of the <see cref="ItemCategory"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="ItemCategory"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static ItemCategory Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<ItemCategory>(godotObject);
    }
#region Properties

    public string Id
    {
        get => (string)Get("id");
        set => Set("id", Variant.From(value));
    }

    public string Name
    {
        get => (string)Get("name");
        set => Set("name", Variant.From(value));
    }

    public Color Color
    {
        get => (Color)Get("color");
        set => Set("color", Variant.From(value));
    }

    public Texture2D Icon
    {
        get => (Texture2D)Get("icon");
        set => Set("icon", Variant.From(value));
    }

    public Godot.Collections.Dictionary ItemProperties
    {
        get => (Godot.Collections.Dictionary)Get("item_properties");
        set => Set("item_properties", Variant.From(value));
    }

    public Godot.Collections.Array<String> ItemDynamicProperties
    {
        get => (Godot.Collections.Array<String>)Get("item_dynamic_properties");
        set => Set("item_dynamic_properties", Variant.From(value));
    }

#endregion

#region Methods

    public int GetCode() => Call("get_code").As<int>();

#endregion

}
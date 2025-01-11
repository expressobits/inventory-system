using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class Recipe : Resource
{
    public static readonly StringName GDExtensionName = "Recipe";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected Recipe() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="Recipe"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static Recipe Instantiate()
    {
        return GDExtensionHelper.Instantiate<Recipe>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="Recipe"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="Recipe"/> wrapper type,
    /// a new instance of the <see cref="Recipe"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="Recipe"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static Recipe Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<Recipe>(godotObject);
    }
#region Properties

    public Godot.Collections.Array<ItemStack> Products
    {
        get => GDExtensionHelper.Cast<ItemStack>((Godot.Collections.Array<Godot.GodotObject>)Get("products"));
        set => Set("products", Variant.From(value));
    }

    public float TimeToCraft
    {
        get => (float)Get("time_to_craft");
        set => Set("time_to_craft", Variant.From(value));
    }

    public CraftStationType Station
    {
        get => (CraftStationType)Get("station");
        set => Set("station", Variant.From(value));
    }

    public Godot.Collections.Array<ItemStack> Ingredients
    {
        get => GDExtensionHelper.Cast<ItemStack>((Godot.Collections.Array<Godot.GodotObject>)Get("ingredients"));
        set => Set("ingredients", Variant.From(value));
    }

    public Godot.Collections.Array<ItemStack> RequiredItems
    {
        get => GDExtensionHelper.Cast<ItemStack>((Godot.Collections.Array<Godot.GodotObject>)Get("required_items"));
        set => Set("required_items", Variant.From(value));
    }

#endregion

}
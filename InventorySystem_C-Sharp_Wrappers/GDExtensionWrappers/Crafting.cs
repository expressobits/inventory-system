using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class Crafting : Resource
{
    public static readonly StringName GDExtensionName = "Crafting";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected Crafting() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="Crafting"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static Crafting Instantiate()
    {
        return GDExtensionHelper.Instantiate<Crafting>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="Crafting"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="Crafting"/> wrapper type,
    /// a new instance of the <see cref="Crafting"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="Crafting"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static Crafting Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<Crafting>(godotObject);
    }
#region Properties

    public int RecipeIndex
    {
        get => (int)Get("recipe_index");
        set => Set("recipe_index", Variant.From(value));
    }

    public float Time
    {
        get => (float)Get("time");
        set => Set("time", Variant.From(value));
    }

#endregion

#region Methods

    public bool IsFinished() => Call("is_finished").As<bool>();

    public void Process(float delta) => Call("process", delta);

    public Godot.Collections.Dictionary Serialize() => Call("serialize").As<Godot.Collections.Dictionary>();

    public void Deserialize(Godot.Collections.Dictionary data) => Call("deserialize", data);

#endregion

}
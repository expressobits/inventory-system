using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class CraftStationType : Resource
{
    public static readonly StringName GDExtensionName = "CraftStationType";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected CraftStationType() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="CraftStationType"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static CraftStationType Instantiate()
    {
        return GDExtensionHelper.Instantiate<CraftStationType>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="CraftStationType"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="CraftStationType"/> wrapper type,
    /// a new instance of the <see cref="CraftStationType"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="CraftStationType"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static CraftStationType Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<CraftStationType>(godotObject);
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

    public Texture2D Icon
    {
        get => (Texture2D)Get("icon");
        set => Set("icon", Variant.From(value));
    }

#endregion

}
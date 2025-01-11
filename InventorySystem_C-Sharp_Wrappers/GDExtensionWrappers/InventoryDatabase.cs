using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class InventoryDatabase : Resource
{
    public static readonly StringName GDExtensionName = "InventoryDatabase";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Resource), please use the Instantiate() method instead.")]
    protected InventoryDatabase() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="InventoryDatabase"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public static InventoryDatabase Instantiate()
    {
        return GDExtensionHelper.Instantiate<InventoryDatabase>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="InventoryDatabase"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="InventoryDatabase"/> wrapper type,
    /// a new instance of the <see cref="InventoryDatabase"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="InventoryDatabase"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public static InventoryDatabase Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<InventoryDatabase>(godotObject);
    }
#region Properties

    public Godot.Collections.Array<ItemDefinition> Items
    {
        get => GDExtensionHelper.Cast<ItemDefinition>((Godot.Collections.Array<Godot.GodotObject>)Get("items"));
        set => Set("items", Variant.From(value));
    }

    public Godot.Collections.Array<Recipe> Recipes
    {
        get => GDExtensionHelper.Cast<Recipe>((Godot.Collections.Array<Godot.GodotObject>)Get("recipes"));
        set => Set("recipes", Variant.From(value));
    }

    public Godot.Collections.Array<CraftStationType> StationsType
    {
        get => GDExtensionHelper.Cast<CraftStationType>((Godot.Collections.Array<Godot.GodotObject>)Get("stations_type"));
        set => Set("stations_type", Variant.From(value));
    }

    public Godot.Collections.Array<ItemCategory> ItemCategories
    {
        get => GDExtensionHelper.Cast<ItemCategory>((Godot.Collections.Array<Godot.GodotObject>)Get("item_categories"));
        set => Set("item_categories", Variant.From(value));
    }

#endregion

#region Methods

    public void AddNewItem(ItemDefinition item) => Call("add_new_item", (Resource)item);

    public void RemoveItem(ItemDefinition item) => Call("remove_item", (Resource)item);

    public void AddNewCategory(ItemCategory category) => Call("add_new_category", (Resource)category);

    public void RemoveCategory(ItemCategory category) => Call("remove_category", (Resource)category);

    public ItemDefinition GetItem(string id) => GDExtensionHelper.Bind<ItemDefinition>(Call("get_item", id).As<GodotObject>());

    public bool HasItemCategoryId(string id) => Call("has_item_category_id", id).As<bool>();

    public bool HasItemId(string id) => Call("has_item_id", id).As<bool>();

    public bool HasCraftStationTypeId(string id) => Call("has_craft_station_type_id", id).As<bool>();

    public string GetValidId() => Call("get_valid_id").As<string>();

    public string GetNewValidId() => Call("get_new_valid_id").As<string>();

    public ItemCategory GetCategory(int code) => GDExtensionHelper.Bind<ItemCategory>(Call("get_category", code).As<GodotObject>());

    public Godot.Collections.Dictionary SerializeItemDefinition(ItemDefinition definition) => Call("serialize_item_definition", (Resource)definition).As<Godot.Collections.Dictionary>();

    public void DeserializeItemDefinition(ItemDefinition definition, Godot.Collections.Dictionary data) => Call("deserialize_item_definition", (Resource)definition, data);

    public Godot.Collections.Dictionary SerializeItemCategory(ItemCategory category) => Call("serialize_item_category", (Resource)category).As<Godot.Collections.Dictionary>();

    public void DeserializeItemCategory(ItemCategory category, Godot.Collections.Dictionary data) => Call("deserialize_item_category", (Resource)category, data);

    public Godot.Collections.Dictionary SerializeRecipe(Recipe recipe) => Call("serialize_recipe", (Resource)recipe).As<Godot.Collections.Dictionary>();

    public void DeserializeRecipe(Recipe recipe, Godot.Collections.Dictionary data) => Call("deserialize_recipe", (Resource)recipe, data);

    public Godot.Collections.Dictionary SerializeStationType(CraftStationType stationType) => Call("serialize_station_type", (Resource)stationType).As<Godot.Collections.Dictionary>();

    public void DeserializeStationType(CraftStationType stationType, Godot.Collections.Dictionary data) => Call("deserialize_station_type", (Resource)stationType, data);

    public ItemCategory GetCategoryFromId(string id) => GDExtensionHelper.Bind<ItemCategory>(Call("get_category_from_id", id).As<GodotObject>());

    public void AddItem() => Call("add_item");

    public void AddItemCategory() => Call("add_item_category");

    public void AddRecipe() => Call("add_recipe");

    public void AddCraftStationType() => Call("add_craft_station_type");

    public string ExportToInvdata() => Call("export_to_invdata").As<string>();

    public void ImportFromInvdata(string json) => Call("import_from_invdata", json);

    public void ClearCurrentData() => Call("clear_current_data");

    public int ImportJsonFile(string path) => Call("import_json_file", path).As<int>();

    public int ExportJsonFile(string path) => Call("export_json_file", path).As<int>();

    public Godot.Collections.Dictionary CreateDynamicProperties(string itemId) => Call("create_dynamic_properties", itemId).As<Godot.Collections.Dictionary>();

#endregion

}
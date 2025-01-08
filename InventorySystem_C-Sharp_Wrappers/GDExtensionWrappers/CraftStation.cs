using System;
using Godot;

namespace GDExtension.Wrappers;

public partial class CraftStation : NodeInventories
{
    public new static readonly StringName GDExtensionName = "CraftStation";

    [Obsolete("Wrapper classes cannot be constructed with Ctor (it only instantiate the underlying Node), please use the Instantiate() method instead.")]
    protected CraftStation() { }

    /// <summary>
    /// Creates an instance of the GDExtension <see cref="CraftStation"/> type, and attaches the wrapper script to it.
    /// </summary>
    /// <returns>The wrapper instance linked to the underlying GDExtension type.</returns>
    public new static CraftStation Instantiate()
    {
        return GDExtensionHelper.Instantiate<CraftStation>(GDExtensionName);
    }

    /// <summary>
    /// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="CraftStation"/> wrapper type,
    /// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="CraftStation"/> wrapper type,
    /// a new instance of the <see cref="CraftStation"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
    /// </summary>
    /// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
    /// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
    /// <returns>The existing or a new instance of the <see cref="CraftStation"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
    public new static CraftStation Bind(GodotObject godotObject)
    {
        return GDExtensionHelper.Bind<CraftStation>(godotObject);
    }
#region Properties

    public Godot.Collections.Array<Inventory> InputInventories
    {
        get => GDExtensionHelper.Cast<Inventory>((Godot.Collections.Array<Godot.GodotObject>)Get("input_inventories"));
        set => Set("input_inventories", Variant.From(value));
    }

    public Godot.Collections.Array<Inventory> OutputInventories
    {
        get => GDExtensionHelper.Cast<Inventory>((Godot.Collections.Array<Godot.GodotObject>)Get("output_inventories"));
        set => Set("output_inventories", Variant.From(value));
    }

    public bool HasLimitCrafts
    {
        get => (bool)Get("has_limit_crafts");
        set => Set("has_limit_crafts", Variant.From(value));
    }

    public int LimitNumberCrafts
    {
        get => (int)Get("limit_number_crafts");
        set => Set("limit_number_crafts", Variant.From(value));
    }

    public bool CanProcessingCraftings
    {
        get => (bool)Get("can_processing_craftings");
        set => Set("can_processing_craftings", Variant.From(value));
    }

    public bool CanFinishCraftings
    {
        get => (bool)Get("can_finish_craftings");
        set => Set("can_finish_craftings", Variant.From(value));
    }

    public CraftStationType Type
    {
        get => (CraftStationType)Get("type");
        set => Set("type", Variant.From(value));
    }

    public string TypeId
    {
        get => (string)Get("type_id");
        set => Set("type_id", Variant.From(value));
    }

    public bool OnlyRemoveIngredientsAfterCraft
    {
        get => (bool)Get("only_remove_ingredients_after_craft");
        set => Set("only_remove_ingredients_after_craft", Variant.From(value));
    }

    public bool AutoCraft
    {
        get => (bool)Get("auto_craft");
        set => Set("auto_craft", Variant.From(value));
    }

    /// <summary>
    /// Had to change this enum to int
    /// </summary>
    public long ProcessingMode
    {
        get => (long)Get("processing_mode").As<Int64>();
        set => Set("processing_mode", Variant.From(value));
    }

    public long /*Process,PhysicProcess,Custom*/ TickUpdateMethod
    {
        get => (long /*Process,PhysicProcess,Custom*/)Get("tick_update_method").As<Int64>();
        set => Set("tick_update_method", Variant.From(value));
    }

    public Godot.Collections.Array<Crafting> Craftings
    {
        get => GDExtensionHelper.Cast<Crafting>((Godot.Collections.Array<Godot.GodotObject>)Get("craftings"));
        set => Set("craftings", Variant.From(value));
    }

    public Godot.Collections.Array ValidRecipes
    {
        get => (Godot.Collections.Array)Get("valid_recipes");
        set => Set("valid_recipes", Variant.From(value));
    }

#endregion

#region Signals

    public delegate void OnCraftedHandler(int recipeIndex);

    private OnCraftedHandler _onCrafted_backing;
    private Callable _onCrafted_backing_callable;
    public event OnCraftedHandler OnCrafted
    {
        add
        {
            if(_onCrafted_backing == null)
            {
                _onCrafted_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _onCrafted_backing?.Invoke(arg0);
                    }
                );
                Connect("on_crafted", _onCrafted_backing_callable);
            }
            _onCrafted_backing += value;
        }
        remove
        {
            _onCrafted_backing -= value;
            
            if(_onCrafted_backing == null)
            {
                Disconnect("on_crafted", _onCrafted_backing_callable);
                _onCrafted_backing_callable = default;
            }
        }
    }

    public delegate void OnRequestCraftHandler(int recipeIndex);

    private OnRequestCraftHandler _onRequestCraft_backing;
    private Callable _onRequestCraft_backing_callable;
    public event OnRequestCraftHandler OnRequestCraft
    {
        add
        {
            if(_onRequestCraft_backing == null)
            {
                _onRequestCraft_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _onRequestCraft_backing?.Invoke(arg0);
                    }
                );
                Connect("on_request_craft", _onRequestCraft_backing_callable);
            }
            _onRequestCraft_backing += value;
        }
        remove
        {
            _onRequestCraft_backing -= value;
            
            if(_onRequestCraft_backing == null)
            {
                Disconnect("on_request_craft", _onRequestCraft_backing_callable);
                _onRequestCraft_backing_callable = default;
            }
        }
    }

    public delegate void CraftingAddedHandler(int craftingIndex);

    private CraftingAddedHandler _craftingAdded_backing;
    private Callable _craftingAdded_backing_callable;
    public event CraftingAddedHandler CraftingAdded
    {
        add
        {
            if(_craftingAdded_backing == null)
            {
                _craftingAdded_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _craftingAdded_backing?.Invoke(arg0);
                    }
                );
                Connect("crafting_added", _craftingAdded_backing_callable);
            }
            _craftingAdded_backing += value;
        }
        remove
        {
            _craftingAdded_backing -= value;
            
            if(_craftingAdded_backing == null)
            {
                Disconnect("crafting_added", _craftingAdded_backing_callable);
                _craftingAdded_backing_callable = default;
            }
        }
    }

    public delegate void CraftingRemovedHandler(int craftingIndex);

    private CraftingRemovedHandler _craftingRemoved_backing;
    private Callable _craftingRemoved_backing_callable;
    public event CraftingRemovedHandler CraftingRemoved
    {
        add
        {
            if(_craftingRemoved_backing == null)
            {
                _craftingRemoved_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<int>();
                        _craftingRemoved_backing?.Invoke(arg0);
                    }
                );
                Connect("crafting_removed", _craftingRemoved_backing_callable);
            }
            _craftingRemoved_backing += value;
        }
        remove
        {
            _craftingRemoved_backing -= value;
            
            if(_craftingRemoved_backing == null)
            {
                Disconnect("crafting_removed", _craftingRemoved_backing_callable);
                _craftingRemoved_backing_callable = default;
            }
        }
    }

    public delegate void InputInventoryAddedHandler(NodePath inputInventoryPath);

    private InputInventoryAddedHandler _inputInventoryAdded_backing;
    private Callable _inputInventoryAdded_backing_callable;
    public event InputInventoryAddedHandler InputInventoryAdded
    {
        add
        {
            if(_inputInventoryAdded_backing == null)
            {
                _inputInventoryAdded_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<NodePath>();
                        _inputInventoryAdded_backing?.Invoke(arg0);
                    }
                );
                Connect("input_inventory_added", _inputInventoryAdded_backing_callable);
            }
            _inputInventoryAdded_backing += value;
        }
        remove
        {
            _inputInventoryAdded_backing -= value;
            
            if(_inputInventoryAdded_backing == null)
            {
                Disconnect("input_inventory_added", _inputInventoryAdded_backing_callable);
                _inputInventoryAdded_backing_callable = default;
            }
        }
    }

    public delegate void InputInventoryRemovedHandler(NodePath inputInventoryPath);

    private InputInventoryRemovedHandler _inputInventoryRemoved_backing;
    private Callable _inputInventoryRemoved_backing_callable;
    public event InputInventoryRemovedHandler InputInventoryRemoved
    {
        add
        {
            if(_inputInventoryRemoved_backing == null)
            {
                _inputInventoryRemoved_backing_callable = Callable.From<Variant>(
                    (arg0_variant) =>
                    {
                        var arg0 = arg0_variant.As<NodePath>();
                        _inputInventoryRemoved_backing?.Invoke(arg0);
                    }
                );
                Connect("input_inventory_removed", _inputInventoryRemoved_backing_callable);
            }
            _inputInventoryRemoved_backing += value;
        }
        remove
        {
            _inputInventoryRemoved_backing -= value;
            
            if(_inputInventoryRemoved_backing == null)
            {
                Disconnect("input_inventory_removed", _inputInventoryRemoved_backing_callable);
                _inputInventoryRemoved_backing_callable = default;
            }
        }
    }

#endregion

#region Methods

    public void LoadValidRecipes() => Call("load_valid_recipes");

    public void Tick(float delta) => Call("tick", delta);

    public bool IsCrafting() => Call("is_crafting").As<bool>();

    public bool CanCraft(Recipe recipe) => Call("can_craft", (Resource)recipe).As<bool>();

    public bool ContainsIngredients(Recipe recipe) => Call("contains_ingredients", (Resource)recipe).As<bool>();

    public void Craft(int recipeIndex) => Call("craft", recipeIndex);

    public void CancelCraft(int craftingIndex) => Call("cancel_craft", craftingIndex);

    public void AddCrafting(int recipeIndex, Recipe recipe) => Call("add_crafting", recipeIndex, (Resource)recipe);

    public void RemoveCrafting(int craftingIndex) => Call("remove_crafting", craftingIndex);

    public void FinishCrafting(int craftingIndex) => Call("finish_crafting", craftingIndex);

    public Inventory GetInputInventory(int index) => GDExtensionHelper.Bind<Inventory>(Call("get_input_inventory", index).As<GodotObject>());

    public Godot.Collections.Dictionary Serialize() => Call("serialize").As<Godot.Collections.Dictionary>();

    public void Deserialize(Godot.Collections.Dictionary data) => Call("deserialize", data);

    public void AddInputInventory(Inventory inputInventory) => Call("add_input_inventory", (Node)inputInventory);

    public void RemoveInputInventory(Inventory inputInventory) => Call("remove_input_inventory", (Node)inputInventory);

#endregion

}
#include "networked_craft_station.h"

void NetworkedCraftStation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_craftings_data_sync", "craftings_data_sync"), &NetworkedCraftStation::set_craftings_data_sync);
    ClassDB::bind_method(D_METHOD("get_craftings_data_sync"), &NetworkedCraftStation::get_craftings_data_sync);
    ClassDB::bind_method(D_METHOD("craft_rpc", "recipe_index"), &NetworkedCraftStation::craft_rpc);
    ClassDB::bind_method(D_METHOD("cancel_craft_rpc", "crafting_index"), &NetworkedCraftStation::cancel_craft_rpc);
    ClassDB::bind_method(D_METHOD("_update_craftings_rpc", "craftings_data"), &NetworkedCraftStation::_update_craftings_rpc);
    ClassDB::bind_method(D_METHOD("_crafting_added_rpc", "recipe_index"), &NetworkedCraftStation::_crafting_added_rpc);
    ClassDB::bind_method(D_METHOD("_crafting_removed_rpc", "crafting_index"), &NetworkedCraftStation::_crafting_removed_rpc);
    ClassDB::bind_method(D_METHOD("_opened_rpc"), &NetworkedCraftStation::_opened_rpc);
    ClassDB::bind_method(D_METHOD("_closed_rpc"), &NetworkedCraftStation::_closed_rpc);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "craftings_data_sync"), "set_craftings_data_sync", "get_craftings_data_sync");
}

void NetworkedCraftStation::_on_opened() {
    if (!get_multiplayer()->is_server())
        return;
    rpc("_opened_rpc");
}

void NetworkedCraftStation::_on_closed() {
    if (!get_multiplayer()->is_server())
        return;
    rpc("_closed_rpc");
}

void NetworkedCraftStation::_on_crafting_added(const int crafting_index) {
    if (!get_multiplayer()->is_server())
        return;
    Ref<Crafting> crafting = craftings[crafting_index];
    rpc("_crafting_added_rpc", crafting->get_recipe_index());
    craftings_data_sync.append(crafting->to_data());
}

void NetworkedCraftStation::_on_crafting_removed(const int crafting_index) {
    if (!get_multiplayer()->is_server())
        return;
    rpc("_crafting_removed_rpc", crafting_index);
    craftings_data_sync.remove_at(crafting_index);
}

void NetworkedCraftStation::_on_connected(const int peer_id) {
    if (!get_multiplayer()->is_server())
        return;
    if(get_is_open()) {
        rpc_id(peer_id, "_opened_rpc");
    }
    rpc_id(peer_id, "_update_craftings_rpc", craftings_data_sync);
}

void NetworkedCraftStation::_ready() {
	get_multiplayer()->connect("peer_connected", callable_mp(this, &NetworkedCraftStation::_on_connected));
    connect("crafting_added", callable_mp(this, &NetworkedCraftStation::_on_crafting_added));
    // connect("updated_crafting", callable_mp(this, &NetworkedCraftStation::_on_updated_crafting));
    connect("crafting_removed", callable_mp(this, &NetworkedCraftStation::_on_crafting_removed));
    connect("opened", callable_mp(this, &NetworkedCraftStation::_on_opened));
    connect("closed", callable_mp(this, &NetworkedCraftStation::_on_closed));

    _create_rpc_config("craft_rpc", MultiplayerAPI::RPC_MODE_ANY_PEER);
    _create_rpc_config("cancel_craft_rpc", MultiplayerAPI::RPC_MODE_ANY_PEER);
    _create_rpc_config("_update_craftings_rpc");
    _create_rpc_config("_crafting_added_rpc");
    _create_rpc_config("_crafting_removed_rpc");
    _create_rpc_config("_opened_rpc");
    _create_rpc_config("_closed_rpc");
    CraftStation::_ready();
}

void NetworkedCraftStation::set_craftings_data_sync(const Array new_craftings_data_sync) {
    craftings_data_sync = new_craftings_data_sync;
}

Array NetworkedCraftStation::get_craftings_data_sync() {
	return craftings_data_sync;
}

void NetworkedCraftStation::craft(int recipe_index) {
    if (!get_multiplayer()->is_server()) {
        rpc_id(1, "craft_rpc", recipe_index);
    }else{
        craft_rpc(recipe_index);
    }
}

void NetworkedCraftStation::cancel_craft(int crafting_index) {
    if (!get_multiplayer()->is_server()) {
        rpc_id(1, "cancel_craft_rpc", crafting_index);
    }else{
        cancel_craft_rpc(crafting_index);
    }
}

void NetworkedCraftStation::finish_crafting(int crafting_index) {
    if (get_multiplayer()->is_server())
        CraftStation::finish_crafting(crafting_index);
}

void NetworkedCraftStation::craft_rpc(const int recipe_index) {
    if (!get_multiplayer()->is_server())
        return;
    CraftStation::craft(recipe_index);    
}

void NetworkedCraftStation::cancel_craft_rpc(const int crafting_index) {
    if (!get_multiplayer()->is_server())
        return;
    CraftStation::cancel_craft(crafting_index);  
}

void NetworkedCraftStation::_update_craftings_rpc(const Array craftings_data) {
    for (size_t i = 0; i < craftings_data.size(); i++)
    {
        Variant data = craftings_data[i];
        Ref<Crafting> crafting = memnew(Crafting());
        crafting->from_data(data);
        craftings.append(crafting);
    }
}

void NetworkedCraftStation::_crafting_added_rpc(const int recipe_index) {
    if (get_multiplayer()->is_server())
        return;
    Ref<Recipe> recipe = get_database()->get_recipes()[recipe_index];
    add_crafting(recipe_index, recipe);
}

void NetworkedCraftStation::_crafting_removed_rpc(const int crafting_index) {
    if (get_multiplayer()->is_server())
        return;
    remove_crafting(crafting_index);
}

void NetworkedCraftStation::_opened_rpc() {
    if (get_multiplayer()->is_server())
        return;
    open();
}

void NetworkedCraftStation::_closed_rpc() {
    if (get_multiplayer()->is_server())
        return;
    close();
}

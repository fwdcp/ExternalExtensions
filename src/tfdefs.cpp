/*
 *  tfdefs.cpp
 *  ExternalExtensions project
 *  
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "tfdefs.h"

const std::map<TFClassType, std::string> TFDefinitions::classNames = { { TFClass_Scout, "scout" }, { TFClass_Soldier, "soldier" }, { TFClass_Pyro, "pyro" }, { TFClass_DemoMan, "demoman" }, { TFClass_Heavy, "heavy" }, { TFClass_Engineer, "engineer" }, { TFClass_Medic, "medic" }, { TFClass_Sniper, "sniper" }, { TFClass_Spy, "spy" } };
const std::array<std::string, 9> TFDefinitions::itemSlots = { "primary", "secondary", "melee", "pda", "pda2", "building", "head", "misc", "action" };
const std::map<TFClassType, int> TFDefinitions::normalClassOrdinal = { { TFClass_Unknown, 0 }, { TFClass_Scout, 1 }, { TFClass_Soldier, 2 }, { TFClass_Pyro, 3 }, { TFClass_DemoMan, 4 }, { TFClass_Heavy, 5 }, { TFClass_Engineer, 6 }, { TFClass_Medic, 7 }, { TFClass_Sniper, 8 }, { TFClass_Spy, 9 } };
const std::map<int, std::vector<std::string>> TFDefinitions::slotKillIcons = { { 0, { "sniperrrifle", "tf_projectile_rocket", "shotgun_primary", "tf_projectile_pipe", "syringegun_medic", "minigun", "flamethrower", "revolver", "scattergun", "headshot", "deflect_promode", "deflect_sticky", "deflect_rocket", "deflect_flare", "ambassador", "huntsman", "huntsman_burning", "huntsman_flyingburn", "huntsman_headshot", "ambassador_headshot", "taunt_sniper", "deflect_arrow", "rocketlauncher_directhit", "deflect_huntsman_headshot", "huntsman_flyingburn_headshot", "samrevolver", "natascha", "force_a_nature", "deflect_huntsman_flyingburn_headshot", "deflect_huntsman_flyingburn", "deflect_ball", "taunt_guitar_kill", "frontier_kill", "degreaser", "letranger", "short_stop", "blackbox", "loch_n_load", "iron_curtain", "crusaders_crossbow", "battleneedle", "sydney_sleeper", "cow_mangler", "brass_beast", "blutsauger", "backburner", "tomislav", "soda_popper", "bazaar_bargain", "liberty_launcher", "proto_syringe", "enforcer", "quake_rl", "diamondback", "widowmaker", "machina", "player_penetration", "headshot_player_penetration", "phlogistinator", "pomson", "rainblower", "armageddon", "pro_rifle", "pep_brawlerblaster", "dumpster_device", "long_heatmaker", "loose_cannon_impact", "loose_cannon_explosion", "rescue_ranger", "loose_cannon_reflect", "rescue_ranger_reflect", "the_classic", "back_scatter", "airstrike", "ai_flamethrower" } }, { 1, { "pistol", "pistol_scout", "smg", "shotgun_soldier", "shotgun_hwg", "shotgun_pyro", "flaregun", "tf_projectile_pipe_remote", "taunt_pyro", "obj_attachment_sapper", "demoshield", "sticky_resistance", "maxgun", "wrangler_kill", "righteous_bison", "the_winger", "detonator", "family_business", "reserve_shooter", "splendid_screen", "mantreads", "short_circuit", "manmelter", "scorchshot", "pro_smg", "pep_pistol", "guillotine", "recorder", "p_sapper", "tide_turner", "snack_attack" } }, { 2, { "obj_sentrygun", "obj_sentrygun2", "obj_sentrygun3", "bat", "club", "shovel", "bottle", "pipe", "fists", "fireaxe", "bonesaw", "knife", "wrench", "backstab", "ubersaw", "axtinguisher", "bat_wood", "ball", "taunt_heavy", "taunt_scout", "gloves", "taunt_spy", "taunt_soldier", "taunt_demoman", "sword", "pickaxe", "battleaxe", "tribalkukri", "sledgehammer", "paintrain", "taunt_medic", "wrench_golden", "southern_comfort_kill", "robot_arm_kill", "robot_arm_combo_kill", "robot_arm_blender_kill", "powerjack", "eternal_reward", "holy_mackerel", "headtaker", "fryingpan", "back_scratcher", "boston_basher", "claidheamohmor", "steel_fists", "ullapool_caber_explosion", "ullapool_caber", "candy_cane", "amputator", "wrench_jag", "warrior_spirit", "bushwacka", "taunt_soldier_lumbricus", "gloves_running_urgently", "sandman", "lava_bat", "lava_axe", "obj_minisentry", "kunai", "warfan", "demokatana", "scout_sword", "the_maul", "saxxy", "market_gardener", "atomizer", "big_earner", "eviction_notice", "persian_persuader", "shahanshah", "disciplinary_action", "solemn_vow", "nessieclub", "mailbox", "nonnonviolent_protest", "scotland_shard", "unarmed_combat", "voodoo_pin", "apocofists", "sharp_dresser", "sharp_dresser_backstab", "thirddegree", "eureka_effect", "holiday_punch", "spy_cicle", "wrap_assassin", "black_rose", "lollichop", "annihilator", "freedom_staff", "memory_maker", "ham_shank", "golden_fryingpan", "bread_bite", "necro_smasher", "crossing_guard" } } };
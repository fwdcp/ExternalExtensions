ExternalExtensions
==================

plugin exposing an API to the Team Fortress 2 client

Modules
-------

### Console ###

#### Commands ####
*executes a command*

**Call Info**
* `type`: `command`
* `command`: command to be executed

**Example Call**
```json
{
    "type": "command",
    "command": "hud_reloadscheme"
}
```

#### Printing ####
*outputs everything printed to the console*

**Automatic Message Info**
* `type`: `consoleprint`
* `message`
  * `type`: `color`, `regular`, `debug`
  * `text`: message to be printed
  * `color` (only with `type`: `color`)
    * `r`: red value of message to be printed
	* `g`: green value of message to be printed
	* `b`: blue value of message to be printed
	* `a`: alpha value of message to be printed

**Example Global Message**
```json
{
    "message": {
        "color": {
            "a": 255,
            "b": 255,
            "g": 255,
            "r": 255
        },
        "text": " 0:Stopped sound ui\\buttonclickrelease.wav\n",
        "type": "color"
    },
    "type": "consoleprint"
}
```

#### Autocomplete ####
*returns autocomplete results for a command*

**Call Info**
* `type`: `autocomplete`
* `partial`: string to check for autocomplete results

**Example Call**
```json
{
    "type": "autocomplete",
    "partial": "playdemo "
}
```

**Response Info**
* `type`: `autocompleteresults`
* `partial`: the string for which autocomplete results were compiled
* `results`: an array of strings which possible command completions

**Example Response**
```json
{
    "partial": "playdemo ",
    "results": [
        "playdemo 2014-09-21-15-04-bk_vs_coolclan-koth_pro_viaduct_r",
        "playdemo 2014-09-21-15-05-ggwp_vs_425-koth_pro_viaduct_rc4",
        "playdemo 2014-09-21-15-13-maincall_vs_c2c-koth_pro_viaduct_",
        "playdemo 2014-09-21-15-14-fnx_vs_furbo-koth_pro_viaduct_rc4",
        "playdemo 2014-09-21-15-49-ggwp_vs_maincall-cp_badlands",
        "playdemo 2014-09-21-15-53-coolclan_vs_furbo-cp_badlands",
        "playdemo 2014-09-21-15-58-c2c_vs_425-cp_badlands",
        "playdemo 2014-09-21-16-30-c2c_vs_425-cp_badlands",
        "playdemo 2014-09-21-16-49-maincall_vs_bk-cp_process_final",
        "playdemo 2014-09-21-16-53-fnx_vs_425-cp_process_final",
        "playdemo 2014-09-21-16-54-furbo_vs_ggwp-cp_process_final",
        "playdemo 2014-09-21-17-42-425_vs_ggwp-cp_gullywash_final1",
        "playdemo 2014-09-21-18-30-bk_vs_425-cp_gullywash_final1",
        "playdemo 2014-09-21-18-38-bk_vs_425-cp_badlands",
        "playdemo 2014-09-21-19-13-bk_vs_425-cp_snakewater_final1",
        "playdemo 2014-09-21-21-09-maincall_vs_bk-cp_badlands",
        "playdemo 2014-09-21-21-52-maincall_vs_bk-koth_pro_viaduct_r",
        "playdemo 2014-10-01-21-20-coolclan_vs_maincall-cp_badlands",
        "playdemo 2014-10-01-21-55-coolclan_vs_maincall-cp_process_f",
        "playdemo 20140815_2142_cp_gullywash_final1_swag_blu",
        "playdemo 20140815_2221_cp_snakewater_final1_ussr_wonton",
        "playdemo 20140919_2245_cp_badlands_yolo_9gag",
        "playdemo 20140920_0010_cp_snakewater_final1_dream_blu",
        "playdemo 20140920_0133_cp_snakewater_final1_randl_gg",
        "playdemo auto-de3-20140930-192315-cp_badlands-lbc_vs_burger",
        "playdemo benchmark1",
        "playdemo esea_match_3739222",
        "playdemo esea_match_3774605",
        "playdemo esea_match_3780773",
        "playdemo esea_match_3797534"
    ],
    "type": "autocompleteresults"
}
```
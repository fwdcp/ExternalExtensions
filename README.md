ExternalExtensions
==================

plugin exposing an API to the Team Fortress 2 client

Changelog
---------

*0.0.1*
* general
  * initial version
* console
  * added module

General Info
------------
This plugin exposes a WebSockets server on port 2006. Information on communications from and to it can be found below.

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
        "playdemo esea_match_3739222",
        "playdemo esea_match_3774605",
        "playdemo esea_match_3780773",
        "playdemo esea_match_3797534"
    ],
    "type": "autocompleteresults"
}
```
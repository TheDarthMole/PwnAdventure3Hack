# Pwn Adventure 3: Pwnie Island Hack

The original game can be downloaded from the [pwnadventure.com](https://www.pwnadventure.com) website

This project was made for a demonstration for the [Sheffield Ethical Student Hackers Society](https://shefesh.com) on game hacking.

## Building

Use Microsoft Visual Studio 2019 using the x86 architecture when building this project.

## Injecting

I used [Guided Hacking's Injector](https://github.com/multikill/GH_Injector_MSVC_2019_QT_5_15_0/releases) for this cheat, however I assume other injectors will probably work just as well.


## Usage

```
[Numpad 1] for Test Script              [Toggle]
[Numpad 2] for Fly Hack                 [Toggle]
[Numpad 3] for Infinite Ammo            [Toggle]
[Numpad 4] for Teleport to Fire Spell
[Numpad 5] for Teleport to Gun Shop
[Numpad 6] for Teleport to Rats
[Numpad 7] for Speed Hack
```
The key binds can be changed in the source code if you don't have a numpad on your keyboard. Search for `GetAsyncKeyState` in the `dllmain.cpp` file and edit the `VK_[key]` value.
 
## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## Credits

[pwnadventure.com](https://www.pwnadventure.com) for making the original game

[guidedhacking.com](https://guidedhacking.com/) for the code snippets used in `proc`, `mem` and `dllmain`

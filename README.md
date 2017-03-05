# InsTux

This is InsTux, a feature-packed multihack for Insurgency on Linux.

# Features
See post on UnknownCheats.me:
[InsTux hack for Insurgency on Linux](https://www.unknowncheats.me/forum/insurgency/204814-instux-trainer-insurgency-linux-windows.html)

# Installation
(example commands here are for Ubuntu/Debian)

0. install some basic dependencies
    ```
    sudo apt-get install cmake g++ gdb git libsdl2-dev zlib1g-dev
    ```

1. make sure your `g++` version is at least 6.0. Here is AimTux's tutorial page for updating g++
    [How to update g++](https://github.com/AimTuxOfficial/AimTux/wiki/Updating-your-compiler)
    You will also need to install the corresponding g++ multilib
    
2. clone the repository and cd to its root directory
    ```
    git clone https://github.com/flint201/InsTux
    cd InsTux
    ```

3. generate the Makefile 
    ```
    cmake .
    ```

4. compile 
    ```
    make
    ```

At this point if you see a file called `libInsTux.so` in you directory you have succeessfully compiled the hack.
To use, start the game and wait until it reaches main menu, then run `./load` in the repo directory.
```
Insurgency PID: 12229

[New LWP 12230]
...
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
0xf77cdc89 in __kernel_vsyscall ()
$1 = (void *) 0x40d6eb70
$2 = 0x0
```

If the last line of the output shows `$2 = 0x0` like this it means the hack has successfully loaded. You should also see the following output in your game console.

```
++++ InsTux starting...
++++ InsTux loading complete! +++++++++++++++++++++++++++++++++++++++
```

# Config
After running the hack, a config file will be generated in your home directory `~/.instux.cfg`, you can edit the file to change the settings. You will need to reload the hack in order for the changes to take effect. If you wish to restore the default config, simply delete or rename `~/.instux.cfg`, and it will be generated for you. You may have to re-generate the config file after updating, since the items in the config file may change.

# Updating
This project is undergoing active development, run `./update` in your repo directory to get the latest version.

# Disclaimer
This project is for education purpose only. And the code will be provided as-is, the author does not assume any responsibility for damage of any kind cause by the use of this software.

# Credits
Special thanks to contributors of project AimTux for open source such an awesome project, I learned a lot from their code.
Also, tons of thanks to [aixxe](aixxe.net), the blog is very informative and helpful.

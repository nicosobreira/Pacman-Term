# Pacman Term

A clone of the OG *Pacman* in the **terminal**, made in **C**!

## Build

Dependencies, on Ubuntu/Debian:

``` bash
sudo apt install make libncurses-dev
```

Building:

``` bash
make
```

Running:

``` bash
make run
```

Or ...

``` bash
./build/pacman
```

## Todo

- [x] Create the point system (little dots on the ground)
- [x] Add *colors*!
    - **Yellow** for the points
    - **Purple** for the walls
- [ ] Add ghost's (**I HAVE NO IDEA**)
    - [x] Add a target system
- [ ] Add multiple arenas
    - Using text files
    - Need to load them

## Extra

### LSP config

If you don't work with [clangd](https://clangd.llvm.org/) (LSP tool for C/C++) you can skip this part, else you should use the `bear` command to create the `compile_commands.json`, you will gain some quality of life features, like better code navigation and completion.

You can install `bear` in Ubuntu/Debian with the following command:

``` bash
sudo apt install bear
```

Then run in the root folder:

``` bash
make clean && bear -- make
```

### GDB

``` bash
sudo apt install gdb gdbserver -y
```

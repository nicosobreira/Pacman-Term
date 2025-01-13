# Pacman Term

## Build

### Linux

### Dependencies

``` bash
sudo apt install cmake
```

Enter the following command on the terminal for creating the `MAKEFILE`:

``` bash
mkdir build && cd build
cmake ../
```

Then build and run with:

``` bash
make -j$(nproc)
./pacman
```

> If any errors occurs try to run `make` **without** `-j$(nproc)`

Note:
You may need to install *ncurses* using the following command

``` bash
sudo apt install libncurses-dev
```

## TODO

- [ ] Create the point system (little dots on the ground)
- [x] Add *colors*!
    - **Yellow** for the points
    - **Purple** for the walls
- [ ] Add ghost's (**I HAVE NO IDEA**)
    - [ ] Add a target system
- [ ] Add multiple arenas
    - Using text files
    - Need to load them

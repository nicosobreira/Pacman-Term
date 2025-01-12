# Pacman Term

## Build

### Linux

#### Dependencies

On Ubuntu/Debian
``` bash
sudo apt install cmake
```

Enter the following command on the terminal
``` bash
mkdir build && cd build
cmake ../
make -j$(nproc)
```

Then run `./pacman`.

If any errors occurs try to run `make` **without** `-j$(nproc)`

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

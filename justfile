run day:
    mkdir -p build
    clang++ -std=c++23 ./src/day{{day}}.cpp -o build/day{{day}}
    ./build/day{{day}}

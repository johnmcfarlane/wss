# wss
Wordscapes Solver

## build

```sh
conan remote add my-conan-repo https://api.bintray.com/conan/bincrafters/public-conan
conan install --build=missing path/to/wss
cmake path/to/wss
cmake --build .
```

## Invocation

```sh
./scrabble some?letters
```

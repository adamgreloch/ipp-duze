# ipp-duze

Duże zadanie zaliczeniowe, Indywidualny Projekt Programistyczny

## Kompilacja

Wersję release kompiluje się za pomocą sekwencji poleceń:
```
mkdir release
cd release
cmake ..
make
make doc
```

Wersję debug kompiluje się za pomocą sekwencji poleceń:
```
mkdir debug
cd debug
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
make doc
```

W wyniku kompilacji powinny powstać pliki wykonywalne oraz dokumentacja.
Domyślnym plikiem wykonywalnym jest `phone_forward`. Pliki
`phone_forward_test` oraz `phone_forward_instrumented` służą do sprawdzania
poprawności działania na podstawie oficjalnego zestawu testów.


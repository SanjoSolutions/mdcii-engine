[![Total alerts](https://img.shields.io/lgtm/alerts/g/siredmar/mdcii-engine.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/siredmar/mdcii-engine/alerts/) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/siredmar/mdcii-engine.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/siredmar/mdcii-engine/context:cpp)


MDCII Game Engine
=================

Ziel des MDCII-Projekts ist eine unabhängige Neuimplementierung der Engine des PC-Spiels **ANNO 1602** unter freier Lizenz.

Gegenwärtig enthält das Projekt die Programme [`mdcii-bshdump`](doc/mdcii-bshdump.md), [`mdcii-bshpacker`](doc/mdcii-bshpacker.md), `mdcii-codcat`, `mdcii-inselbmp`, [`mdcii-sdltest`](doc/mdcii-sdltest.md) und `mdcii-weltbmp`.

Das komplexeste dieser Programme ist [`mdcii-sdltest`](doc/mdcii-sdltest.md), das Spielstände von ANNO 1602 animiert und navigierbar darstellen kann.

How to build
------------
Clone the repo
```
mkdir build
cd build
cmake ..
make -j8
```
The main program is called `mdcii-sdltest`.

Installation
------------

Die Programme erwarten in ihrem Ordner bestimmte Ordner und Dateien aus der ANNO-1602-Installation beziehungsweise vom ANNO-1602-Datenträger.

Die erforderlichen Ordner sind:

- Grafikordner
  - `sgfx`: Grafiken der kleinsten Vergrößerungsstufe
  - `mgfx`: Grafiken der mittleren Vergrößerungsstufe
  - `gfx`: Grafiken der höchsten Vergrößerungsstufe
  - `toolgfx`: Schriftarten und Menügrafiken (letztere ungenutzt)
- Inselordner
  - `nord`: Nördliche Inseln
  - `sued`: Südliche Inseln

Lizenz
------

GPL Version 2 oder neuer, siehe [COPYING](COPYING).

# Third-party notices

The root MIT license covers PotatoField's own source code. It does not relicense the media or libraries below.

## Game media

The music, sound effects, sprite sheet and HUD images in `assets/audio/` and `assets/img/` come from external sources. The project author confirms permission to distribute them with this project and that no attribution is required. Their original source names and license texts were not retained. No separate right to redistribute these files as an asset pack is asserted here.

The two background tracks were converted from WAV to Ogg Vorbis; the short sound effects remain WAV.

## Font

`assets/fonts/RobotoMono.ttf` is the unmodified Roboto Mono Regular font, Version 3.000. Its embedded metadata identifies:

- Copyright 2015 The Roboto Mono Project Authors (https://github.com/googlefonts/robotomono).
- Designer: Christian Robertson.
- License: Apache License, Version 2.0. See [Apache-2.0.txt](third_party/Apache-2.0.txt).

This notice follows the license embedded in this particular font file.

## Libraries

| Component | Use | License / included notice |
| --- | --- | --- |
| [SFML 2.6.2](https://github.com/SFML/SFML/tree/2.6.2) | Window, graphics and audio | [zlib](third_party/SFML-zlib.md) |
| [GLM 1.0.1](https://github.com/g-truc/glm/tree/1.0.1) | Vectors and quaternions | MIT option of [GLM's dual license](third_party/GLM.txt) |
| FreeType, through SFML | Font rasterization | [FreeType License](third_party/FreeType-FTL.txt) |
| libogg / libvorbis, through SFML | Ogg Vorbis decoding | [libogg BSD](third_party/libogg-BSD.txt), [libvorbis BSD](third_party/libvorbis-BSD.txt) |
| libFLAC, through SFML | Audio decoder | [BSD](third_party/libFLAC-BSD.txt) |
| stb_image / stb_image_write, through SFML | Image loading / writing | [stb_image](third_party/stb-image.txt), [stb_image_write](third_party/stb-image-write.txt) |
| minimp3, through SFML | MP3 decoding | [CC0 dedication](third_party/minimp3-CC0.txt) |
| glad / Khronos headers, through SFML | SFML's platform graphics support | [glad and Khronos notices](third_party/glad-Khronos.txt), [Apache 2.0](third_party/Apache-2.0.txt) for Vulkan headers |
| OpenAL Soft 1.18.1, through SFML | Windows audio backend (`openal32.dll`) | [GNU Library GPL v2 or later](third_party/OpenAL-Soft-LGPL-2.0.txt) |

Portions of this software are copyright © 1996–2014 The FreeType Project (https://www.freetype.org). All rights reserved.

The Windows build uses SFML's unmodified external dependency binaries. The OpenAL Soft version is identified by the shipped DLL's version string. It remains a separate, replaceable DLL. Its unmodified corresponding source, including build files, is included as `third_party/openal-soft-1.18.1.tar.bz2`, downloaded from [the upstream release archive](https://openal-soft.org/openal-releases/openal-soft-1.18.1.tar.bz2). SHA-256: `2d51a6529526ef22484f51567e31a5c346a599767991a3dc9d4dcd9d9cec71dd`.

The library notices above apply independently of the game's MIT license. See the included texts for their full terms.

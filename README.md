# Dear ImGUI extension for Defold
This project adds support for [Dear ImGUI](https://github.com/ocornut/imgui) in Defold. Dear ImGUI is described as a "Bloat-free Graphical User interface for C++ with minimal dependencies". It is perfect for quickly creating a debug interface or for creation of in-game tools and settings.

![imgui1](https://user-images.githubusercontent.com/1300688/118636082-983cc000-b7d4-11eb-9a02-55785e4651ac.png)
![imgui2](https://user-images.githubusercontent.com/1300688/118636087-996ded00-b7d4-11eb-88e6-e90d15ede189.png)


## Why no auto-generated bindings?
Projects such as [cimgui](https://github.com/cimgui/cimgui) offers Dear ImGUI bindings for a number of different languages, including Lua. It could be used  to generate the bindings for this extension, BUT I'm doing this as a way to learn more about the Dear ImGUI API and as such it makes more sense for me to write the bindings by hand.


## IMPORTANT
* The project currently only has bindings for a small subset of the Dear ImGUI API.
  * More bindings are added as I need them. Pull requests are accepted!
* The project is not cross-platform - See below for list of supported platforms
  * The current version uses the `imgui_impl_opengl3.cpp`


## Platforms support
The current state of platform support:

* macOS - supported
* Linux - supported
* Windows - supported
* Android - supported
* iOS - supported
* HTML5 - supported


## Usage
Refer to [example/example.script](/example/example.script) to learn how to use the extension. Also check the bindings in `LuaInit()` in `extension_imgui.cpp`.

# win-gcc-zero

Example of building a diet runtime-less C/C++ executable with GCC on Windows. Doesn't try to do stuff like
change the section alignment to get the smallest possible executable; we just want a normal executable that
isn't bloated.

Get a GCC build for Windows from: https://winlibs.com/

## Building

Try building with the following command:

`gcc main.cpp -O2 -flto -nostdlib -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -fno-rtti -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -s -Qn -Wl,--entry=WinMainCRTStartup -Wl,--subsystem=windows:6.0 -Wl,--stack 0x100000,0x100000 -lkernel32 -luser32 -lshell32`

(Tested on MinGW-w64 GCC 11.2.0): Generates a 3.5kb executable with no dependencies.

## Explanation

| Flag                                                                           | Description |
| ------------------------------------------------------------------------------ | ----------- |
| `-O2`                                                                          | Enables optimization level 2. |
| `-flto`                                                                        | Enables link-time optimization. |
| `-nostdlib`                                                                    | Avoids linking to crt0 and libc (i.e. no runtime). |
| `-fno-stack-check -fno-stack-protector -mno-stack-arg-probe`                   | Disables stack probes that try to make sure enough of the stack is committed at runtime (since we commit all of it upfront anyway, see below). Same as `/Gs9999999` on MSVC. |
| `-fno-rtti -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables` | Disables RTTI and C++ exceptions. Probably not needed unless you're linking to C++ libraries. |
| `-s`                                                                           | Strips unnecessary (debug?) information from the executable. |
| `-Qn`                                                                          | Strips the super useful `GCC (7.2.7): Ultra-cool compiler!! xDDD` comment from the executable. |
| `-Wl,--entry=WinMainCRTStartup`                                                | Tells the linker to use `WinMainCRTStartup` as the entry point, since that's the default for MSVC's linker. Can be changed to anything you want. |
| `-Wl,--subsystem=windows:6.0`                                                  | Tells the linker to set the subsystem in the PE file to `windows` (as opposed to `console`) which gets rid of the console window. Also sets the OS version to 6.0, which fixes ugly window decorations on modern versions of Windows. |
| `-Wl,--stack 0x100000,0x100000`                                                | Tells the linker to set the stack reserve and stack commit entries in the PE file to 1 megabyte each, which also means we don't need stack probes, since we're committing as much stack as we have. |

## License

Anything here is public domain.
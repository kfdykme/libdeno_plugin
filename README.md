## libdeno plugin


## mac library

unzip macos/arm.zip
unzip macos/x86.zip
lipo -create macos/arm/liblibdeno.dylib macos/x86/liblibdeno.dylib -o macos/liblibdeno.dylib
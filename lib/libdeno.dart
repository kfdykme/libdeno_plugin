library libdeno;

import 'dart:io';

import 'package:flutter/services.dart';

const kMethodChannelName = "libdeno";

class Libdeno {
  late MethodChannel mMethodChannel;
  Libdeno() {
    mMethodChannel = const MethodChannel(kMethodChannelName);
  }
  static String GetPath() {
    if (Platform.isMacOS) {
      var executableDirPath = Platform.resolvedExecutable
          .substring(0, Platform.resolvedExecutable.lastIndexOf('/denkuitop'));
      var runableJsPath =
          "${executableDirPath + '/../Resources/denkui.bundle.js'}";
      return runableJsPath;
    } else if (Platform.isWindows) {
      var executableDirPath = Platform.resolvedExecutable
          .substring(0, Platform.resolvedExecutable.lastIndexOf('\\'));
      print("GetPath Windows ${executableDirPath}");
      return executableDirPath;
    }

    return '';
  }

  load() {
    mMethodChannel.invokeMethod("loadLibrary", <String, dynamic>{
      "path": Libdeno.GetPath() + "\\libdeno.dll"
    }).then((value) => {print(value)});
  }

  run(String cmd) {
    mMethodChannel.invokeMethod("run", <String, dynamic>{"cmd": cmd});
  }
}

import Cocoa
import FlutterMacOS



public class LibdenoPlugin: NSObject, FlutterPlugin {

  static func onReceive(message: String ) {
    LibdenoPlugin.channel?.invokeMethod("onReceive", arguments:message);
  }

  static var channel:FlutterMethodChannel? = nil


  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "libdeno_plugin", binaryMessenger: registrar.messenger)
    let instance = LibdenoPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
    // test()

    LibdenoPlugin.channel = channel
    set_ls_message_callback({
      if let ptr = $0 {
          let str = String(cString: ptr)
          LibdenoPlugin.onReceive(message: str)
      }
    })
  }

  func getInt(argva: Any, key: String) -> Int {
    let val = getString(argva: argva, key: key)
    if (val != "") {
      return Int(val as! String)!
    } else {
      return -1
    }
  }

  func getString(argva: Any, key:String) -> String {
    let argv: [String: Any] =  argva as! [String: Any]
    let val =  argv[key]
    if (val == nil) {
      return "";
    } else {
      return val as! String
    }
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    let argv:[String:Any] = call.arguments as! [String: Any]
    switch call.method {
    case "getPlatformVersion":
      result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
    case "loadLibrary":
      let path = getString(argva: argv, key: "path")
      let executablePath = Bundle.main.privateFrameworksPath!
      let dylibPath = executablePath + "/liblibdeno.dylib"
      // executablePath.replacingCharacters(in: executablePath.range(of: ""), with: StringProtocol)
      print(dylibPath)
      let res2 = load_libdeno_by_path(dylibPath)
      // print(path)
      print(res2)
      result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
    case "run":
      let cmd = getString(argva: argv, key: "cmd")
      // let executablePath = Bundle.main.privateFrameworksPath!
      // let indexTsPath = "deno run " + executablePath + "/libdeno_plugin.framework/Resources/index.ts"

      // let cmd = getString(argva: argv, key: "cmd")
      lib_main_libdeno(cmd)
    case "send":
      let msg = getString(argva: argv, key: "msg")
      send_ls_msg(msg);
    default:
      result(FlutterMethodNotImplemented)
    }
  }
}

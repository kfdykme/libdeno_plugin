#include "include/libdeno_plugin/libdeno_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include <thread>
#include "iostream"

namespace
{

  std::string getString(const flutter::EncodableMap *args, std::string key)
  {
    auto status_it = args->find(flutter::EncodableValue(key));
    std::string res = "";
    if (status_it != args->end())
    {
      auto str = std::get<std::string>(status_it->second);
      std::cout << "get " << key.c_str() << ": " << str.c_str() << std::endl;
      res = str;
    }
    return res;
  }

  int getInt(const flutter::EncodableMap *args, std::string key)
  {
    int res = 0;
    auto res_str = getString(args, key);
    if (!res_str.empty())
    {
      res = std::stoi(res_str);
    }
    return res;
  }

  class LibdenoPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    LibdenoPlugin();

    virtual ~LibdenoPlugin();

    bool LoadLibrary(std::string path);

  private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    HMODULE lib_handle_ = NULL;
    bool is_loaded_ = false;// unused
    void (*libmain_)(int8_t *cmd, size_t length);
    std::thread *lib_thread_ = nullptr;
  };

  // static
  void LibdenoPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "libdeno",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<LibdenoPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  LibdenoPlugin::LibdenoPlugin()
  {
    // load dynamic library
  }

  LibdenoPlugin::~LibdenoPlugin() {}

  bool LibdenoPlugin::LoadLibrary(std::string path)
  {
    if (lib_handle_ != NULL)
    {
      auto is_free_lib_suc = FreeLibrary(lib_handle_);
      std::cout << "is_free_lib_suc:" << (int)is_free_lib_suc << std::endl;
    }
    lib_handle_ = ::LoadLibraryA(path.c_str());
    if (lib_handle_ == NULL)
    {
      return false;
    }
    libmain_ = (void (*)(int8_t *, size_t))GetProcAddress(lib_handle_, "lib_main");
    is_loaded_ = true;
    return true;
  }
  void LibdenoPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("loadLibrary") == 0)
    {
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
      if (arguments)
      {

        std::string path = getString(arguments, "path");
        if (LoadLibrary(path))
        {
          std::cout << "LoadLibrary suc" << std::endl;
          result->Success(flutter::EncodableValue("ok")); 
        }
        // if (lib_thread_ != nullptr)
        // {
        //   auto is_terminate_suc = TerminateThread(lib_thread_->native_handle(), 0); // Dangerous source of errors!
        //   auto is_close_suc = CloseHandle(lib_thread_->native_handle());
        //   lib_thread_ = nullptr;
        //   std::cout << "is_terminate_suc:" << (int)is_terminate_suc << ", is_close_suc:" << (int)is_close_suc << std::endl;
        //   result->Success(flutter::EncodableValue("ok")); 
        // }
        // else
        // {
        //   result->Success(flutter::EncodableValue("error"));
        // }
      }
    }
    else if (method_call.method_name().compare("run") == 0)
    {
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
      if (arguments)
      {
        if (lib_thread_ != nullptr)
        {

          result->Success(flutter::EncodableValue("already has deno thread"));
          return;
        }
        std::string cmd = getString(arguments, "cmd");
        int8_t *cmd_i_p = new int8_t[cmd.length() + 1];
        for (int x = 0; x < cmd.length(); x++)
        {
          cmd_i_p[x] = (int8_t)cmd[x];
        }
        // memcpy(cmd_i_p, (const char *)cmd.c_str(), cmd.length());
        cmd_i_p[cmd.length()] = (int8_t)0;
        lib_thread_ = new std::thread(libmain_, cmd_i_p, cmd.length());
        lib_thread_->detach();
      }
      result->Success(flutter::EncodableValue("ok"));
    }
    else
    {
      result->NotImplemented();
    }
  }

} // namespace

void LibdenoPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  LibdenoPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

#include "DesktopWidget.h"
#include <Windows.h>
#include <winuser.h>
#include <string>
#include <iostream>

#define WINDOW_WIDTH  650
#define WINDOW_HEIGHT 450

RefPtr<Overlay> overlay2_;
HWND dekstopHandle = NULL;
bool bSwitch = false;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    HWND p = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL);
    HWND* ret = (HWND*)lParam;

    if (p) {
        *ret = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
    }
    return true;
}

HWND GetDesktopWindowHandle()
{
    HWND wallpaperHwnd = nullptr;
    HWND progman = FindWindow("ProgMan", NULL);
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    EnumWindows(EnumWindowsProc, (LPARAM)&wallpaperHwnd);
    return wallpaperHwnd;
}

DesktopWidget::DesktopWidget() {
  app = App::Create();
   
  AllocConsole();
  freopen("CONOUT$", "w", stdout);

  window = Window::Create(app->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
    false, kWindowFlags_Titled | kWindowFlags_Borderless);

  dekstopHandle = GetDesktopWindowHandle();

  app->set_window(*window.get());
  overlay = Overlay::Create(*window.get(), 1, 1, 0, 0);

  OnResize(window->width(), window->height());

  overlay->view()->LoadURL("file:///index.html");
  overlay2_ = overlay;

  app->set_listener(this);
  window->set_listener(this);

  overlay->view()->set_load_listener(this);
  overlay->view()->set_view_listener(this);
}

DesktopWidget::~DesktopWidget() {
}

void DesktopWidget::Run() {
  app->Run();
}

void DesktopWidget::OnUpdate() {
#ifdef DEBUG
    if (GetAsyncKeyState(VK_F5) & 1)
    {
        overlay->view()->Reload();
    }

    if (GetAsyncKeyState(VK_F1) & 1)
    {
        bSwitch = !bSwitch;
        SetParent((HWND)window->native_handle(), bSwitch ? dekstopHandle : NULL);
    }
#endif // DEBUG
}

void DesktopWidget::OnClose() {
}

JSValue DesktopWidget::OnButtonClick(const JSObject& thisObject, const JSArgs& args) {
    return JSValue("Test");
}

void DesktopWidget::OnResize(uint32_t width, uint32_t height) {
  overlay->Resize(width, height);
}

void DesktopWidget::OnFinishLoading(ultralight::View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) {
}

void DesktopWidget::OnDOMReady(ultralight::View* caller,
                       uint64_t frame_id,
                       bool is_main_frame,
                       const String& url) {

    Ref<JSContext> context = caller->LockJSContext();
    SetJSContext(context.get());
    JSObject global = JSGlobalObject();

    global["OnButtonClick"] = BindJSCallbackWithRetval(&DesktopWidget::OnButtonClick);
}

void DesktopWidget::OnChangeCursor(ultralight::View* caller,
    Cursor cursor) {

    window->SetCursor(cursor);
}

void DesktopWidget::OnChangeTitle(ultralight::View* caller,
    const String& title) {

    window->SetTitle(title.utf8().data());
}

inline std::string ToUTF8(const String& str) {
    String8 utf8 = str.utf8();
    return std::string(utf8.data(), utf8.length());
}

inline const char* Stringify(MessageSource source) {
    switch (source) {
    case kMessageSource_XML: return "XML";
    case kMessageSource_JS: return "JS";
    case kMessageSource_Network: return "Network";
    case kMessageSource_ConsoleAPI: return "ConsoleAPI";
    case kMessageSource_Storage: return "Storage";
    case kMessageSource_AppCache: return "AppCache";
    case kMessageSource_Rendering: return "Rendering";
    case kMessageSource_CSS: return "CSS";
    case kMessageSource_Security: return "Security";
    case kMessageSource_ContentBlocker: return "ContentBlocker";
    case kMessageSource_Other: return "Other";
    default: return "";
    }
}

inline const char* Stringify(MessageLevel level) {
    switch (level) {
    case kMessageLevel_Log: return "Log";
    case kMessageLevel_Warning: return "Warning";
    case kMessageLevel_Error: return "Error";
    case kMessageLevel_Debug: return "Debug";
    case kMessageLevel_Info: return "Info";
    default: return "";
    }
}

void DesktopWidget::OnAddConsoleMessage(View* caller,
    MessageSource source,
    MessageLevel level,
    const String& message,
    uint32_t line_number,
    uint32_t column_number,
    const String& source_id) {

    std::cout << "[Console]: [" << Stringify(source) << "] ["
        << Stringify(level) << "] " << ToUTF8(message);

    if (source == kMessageSource_JS) {
        std::cout << " (" << ToUTF8(source_id) << " @ line " << line_number
            << ", col " << column_number << ")";
    }

    std::cout << std::endl;

}
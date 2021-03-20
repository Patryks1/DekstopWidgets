#pragma once
#include "pch.h"

using namespace ultralight;

class DesktopBackground : public AppListener,
    public WindowListener,
    public LoadListener,
    public ViewListener {
public:
    DesktopBackground(RefPtr<App> app);
    virtual ~DesktopBackground();

    virtual void OnClose(ultralight::Window* window) override;

    virtual void OnResize(ultralight::Window* window, uint32_t width_px, uint32_t height_px) override;

    virtual void OnFinishLoading(ultralight::View* caller,
        uint64_t frame_id,
        bool is_main_frame,
        const String& url) override;

    virtual void OnDOMReady(ultralight::View* caller,
        uint64_t frame_id,
        bool is_main_frame,
        const String& url) override;

    virtual void OnChangeCursor(ultralight::View* caller,
        Cursor cursor) override;

    virtual void OnChangeTitle(ultralight::View* caller,
        const String& title) override;

    JSValue OnButtonClick(const JSObject& thisObject, const JSArgs& args);

    virtual void OnAddConsoleMessage(View* caller,
        MessageSource source,
        MessageLevel level,
        const String& message,
        uint32_t line_number,
        uint32_t column_number,
        const String& source_id) override;

    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

    HWND GetDesktopWindowHandle();

    bool IsInForeGround() {
        return bSwitch;
    }

    HWND GetDesktopHandle() {
        return dekstopHandle;
    }

    void SetInForeGround(bool newValue) {
        bSwitch = newValue;
    }

protected:
    RefPtr<Window> window;
    RefPtr<Overlay> overlay;

    // Background process
    HWND dekstopHandle = NULL;
    bool bSwitch = false;
};

#pragma once
#include <AppCore/AppCore.h>

using namespace ultralight;

class DesktopWidget : public AppListener,
    public WindowListener,
    public LoadListener,
    public ViewListener {
public:
    DesktopWidget();
    virtual ~DesktopWidget();

    virtual void Run();
    virtual void OnUpdate() override;
    virtual void OnClose() override;

    virtual void OnResize(uint32_t width, uint32_t height) override;

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

protected:
    RefPtr<App> app;
    RefPtr<Window> window;
    RefPtr<Overlay> overlay;
};

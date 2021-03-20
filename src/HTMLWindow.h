#pragma once

using namespace ultralight;

#define INSPECTOR_HEIGHT 400

class HTMLWindow : public WindowListener,
    public ViewListener {
    RefPtr<Window> window_;
    RefPtr<Overlay> overlay_;

    RefPtr<Overlay> inspectorOverlay;
    uint32_t container_width_, container_height_;

public:
    HTMLWindow(const char* title, const char* url, int x, int y, int width, int height) {
        window_ = Window::Create(App::instance()->main_monitor(), width, height, false,
            kWindowFlags_Titled | kWindowFlags_Resizable | kWindowFlags_Hidden);
        window_->MoveTo(x, y);
        window_->SetTitle(title);
        window_->Show();
        window_->set_listener(this);

        overlay_ = Overlay::Create(*window_, window_->width(), window_->height(), 0, 0);
        overlay_->view()->LoadURL(url);
        overlay_->view()->set_view_listener(this);

        container_width_ = width;
        container_height_ = height;
    }

    void ToggleInspector() {
        if (!inspectorOverlay) {
            inspectorOverlay = Overlay::Create(*window_, *view()->inspector(), 0, 0);
        }
        else {
            if (inspectorOverlay->is_hidden())
                inspectorOverlay->Show();
            else
                inspectorOverlay->Hide();
        }

        // Force resize to update layout
        Resize(container_width_, container_height_);
    }

    void Resize(uint32_t width, uint32_t height) {
        container_width_ = width;
        container_height_ = height;

        uint32_t content_height = container_height_;
        if (inspectorOverlay && !inspectorOverlay->is_hidden()) {
            uint32_t inspector_height_px = (uint32_t)std::round(INSPECTOR_HEIGHT * window_->scale());
            inspectorOverlay->Resize(container_width_, inspector_height_px);
            content_height -= inspector_height_px;
        }

        if (content_height < 1)
            content_height = 1;

        overlay_->Resize(container_width_, content_height);

        if (inspectorOverlay && !inspectorOverlay->is_hidden())
            inspectorOverlay->MoveTo(0, overlay_->y() + overlay_->height());
    }

    inline Ref<View> view() { return overlay_->view(); }
    inline Ref<Window> window() { return *window_; }
    inline Ref<Overlay> overlay() { return *overlay_; }

    bool IsFocused() {
        HWND foreGroundWindow = GetForegroundWindow();
        return window()->native_handle() == foreGroundWindow;
    }

    virtual void OnClose(ultralight::Window* window) override {
        window->Close();
    }

    virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override {
        overlay_->Resize(width, height);
        Resize(width, height);
    }

    virtual void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) override {
        window_->SetCursor(cursor);
    }
};
#pragma once

#include "pch.h"
#include "DesktopBackground.h"
#include "HTMLWindow.h"
#include "EditorWindow.h"
#include "HTMLWindow.h"

using namespace ultralight;

class MyApp : public EditorListener,
    public AppListener {
    RefPtr<App> app;
    std::unique_ptr<DesktopBackground> backgroundWindow;
    std::unique_ptr<HTMLWindow> optionsWindow;

    // To make widgets
    std::unique_ptr<EditorWindow> editorWindow;
    std::unique_ptr<HTMLWindow> previewWindow;
public:
    MyApp() {
        app = App::Create();

        app->set_listener(this);
        //backgroundWindow.reset(new DesktopBackground(app));
        //optionsWindow.reset(new HTMLWindow("asdasd", "file:///test.html", 700, 50, 600, 700));

        editorWindow.reset(new EditorWindow("Widget Editor", "file:///widgetCreator/editor.html", 50, 50, 600, 700));
        editorWindow->set_editor_listener(this);
    }

    virtual ~MyApp() {}

    virtual void OnUpdate() override {
        if (GetAsyncKeyState(VK_F5) & 1)
        {
            if (editorWindow->IsFocused())
                editorWindow->view()->Reload();

            if (previewWindow && previewWindow->IsFocused())
                previewWindow->view()->Reload();
        }

        /*if (GetAsyncKeyState(VK_F1) & 1)
        {
            bSwitch = !bSwitch;
            SetParent((HWND)window->native_handle(), bSwitch ? dekstopHandle : NULL);
        }*/

        if (GetAsyncKeyState(VK_F1) & 1)
        {
            if (editorWindow->window()->is_visible() && editorWindow->IsFocused())
                editorWindow->ToggleInspector();

            if (previewWindow && previewWindow->window()->is_visible() && previewWindow->IsFocused())
                previewWindow->ToggleInspector();
        }
    };

    virtual void OnUpdateEditor(const ultralight::String& content) {
        if (!previewWindow)
            return;

        previewWindow->view()->Reload();

        Ref<JSContext> context = previewWindow->view()->LockJSContext();
        JSContextRef ctx = context.get();
        SetJSContext(ctx);
        JSObject global = JSGlobalObject();
        global["html_content"] = content;

        previewWindow->view()->EvaluateScript("document.open(); document.write(html_content); delete html_content; document.close();");
    }

    virtual void OnShowPreviewWindow(bool isShowing) {
        // Creates the window if one does not exsist yet. 
        if (!previewWindow) {
            previewWindow.reset(new HTMLWindow("Widget Preview", "file:///widgetCreator/preview.html", 700, 50, 600, 700));
            previewWindow->window()->Hide();
        }
        if (isShowing) {
            previewWindow->window()->Show();
        } else {
            previewWindow->window()->Hide();
        }
    }

    void Run() {
        app->Run();
    }
};

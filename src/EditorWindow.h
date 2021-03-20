#pragma once

class EditorListener {
public:
    EditorListener() {}
    virtual ~EditorListener() {}
    virtual void OnUpdateEditor(const ultralight::String& content) = 0;
    virtual void OnShowPreviewWindow(bool isShowing) = 0;
};

class EditorWindow : public HTMLWindow,
    public LoadListener {
    EditorListener* editor_listener_ = nullptr;

    bool isPreviewWindowShowing = false;
    std::string openFilePath;
    JSString content;
public:
    EditorWindow(const char* title, const char* url, int x, int y, int width, int height)
        : HTMLWindow(title, url, x, y, width, height) {
        view()->set_load_listener(this);
    }

    void set_editor_listener(EditorListener* listener) { editor_listener_ = listener; }

    EditorListener* editor_listener() { return editor_listener_; }

    virtual void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
        const String& url) override {

        Ref<JSContext> context = caller->LockJSContext();
        SetJSContext(context.get());

        JSObject global = JSGlobalObject();

        global["UpdateEditor"] = BindJSCallbackWithRetval(&EditorWindow::UpdateEditor);
        global["ToggleShowPreviewWindow"] = BindJSCallbackWithRetval(&EditorWindow::ShowPreviewWindowClickButton);
        global["OpenFile"] = BindJSCallbackWithRetval(&EditorWindow::OpenFileButton);
        global["SaveFile"] = BindJSCallbackWithRetval(&EditorWindow::SaveFileButton);
        global["SaveFileAs"] = BindJSCallbackWithRetval(&EditorWindow::SaveFileAsButton);

        caller->EvaluateScript("var content = editor.getSession().getValue(); UpdateEditor(content);");
    }

private:
    JSValue UpdateEditor(const JSObject& thisObject, const JSArgs& args) {
        if (editor_listener_ && args.size() == 1) {
            editor_listener_->OnUpdateEditor(args[0].ToString());
            content = args[0].ToString();
        }

        return JSValue();
    }

    JSValue ShowPreviewWindowClickButton(const JSObject& thisObject, const JSArgs& args) {
        isPreviewWindowShowing = !isPreviewWindowShowing;

        Ref<JSContext> context = this->view()->LockJSContext();
        JSContextRef ctx = context.get();
        
        std::string value = isPreviewWindowShowing ? "green" : "red";
        std::string scriptStr = StringHelper::string_format("document.getElementById('preview-window').style.backgroundColor = '%s'", value.c_str());
        JSStringRef script = JSStringCreateWithUTF8CString(scriptStr.c_str());

        JSEvaluateScript(ctx, script, 0, 0, 0, 0);
        JSStringRelease(script);

        if (editor_listener_)
            editor_listener_->OnShowPreviewWindow(isPreviewWindowShowing);

        return JSValueMakeNull(ctx);
    }

    JSValue SaveFileButton(const JSObject& thisObject, const JSArgs& args) {
        Ref<JSContext> context = this->view()->LockJSContext();
        JSContextRef ctx = context.get();

        if (!openFilePath.empty()) {
            SaveFile(openFilePath);
        }
        else {
            std::string saveLocation = SaveDialog();
            SaveFile(saveLocation);
        }
        
        return JSValueMakeNull(ctx);
    }

    JSValue SaveFileAsButton(const JSObject& thisObject, const JSArgs& args) {
        Ref<JSContext> context = this->view()->LockJSContext();
        JSContextRef ctx = context.get();

        std::string saveLocation = SaveDialog();
        SaveFile(saveLocation);

        return JSValueMakeNull(ctx);
    }

    JSValue OpenFileButton(const JSObject& thisObject, const JSArgs& args) {
        Ref<JSContext> context = this->view()->LockJSContext();
        JSContextRef ctx = context.get();

        std::string openFileName = OpenDialog();

        if (!openFileName.empty()) {
            std::string loadedFile = LoadFile(openFileName);

            SetJSContext(ctx);
            JSObject global = JSGlobalObject();
            global["loadedFile"] = JSValue(loadedFile.c_str());

            JSStringRef script = JSStringCreateWithUTF8CString("editor.setValue(loadedFile); delete loadedFile;");
            JSEvaluateScript(ctx, script, 0, 0, 0, 0);
            JSStringRelease(script);

            openFilePath = openFileName;
        }

        return JSValueMakeNull(ctx);
    }
    
    std::string OpenDialog(const char* filter = "All Files (*.*)\0*.*\0", HWND owner = NULL) {
        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "";
        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = owner;
        ofn.lpstrFilter = filter;
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "";

        std::string fileNameStr;

        if (GetOpenFileName(&ofn))
            fileNameStr = fileName;

        return fileNameStr;
    }

    std::string SaveDialog(const char* filter = "All Files (*.*)\0*.*\0", HWND owner = NULL) {
        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "";
        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = owner;
        ofn.lpstrFilter = filter;
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "";

        std::string fileNameStr;

        if (GetSaveFileName(&ofn))
            fileNameStr = fileName;

        return fileNameStr;
    }

    std::string LoadFile(std::string fileName) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        return buffer.str();
    }

    void WriteFile(std::string fileName, std::string content) {
        std::ofstream file(fileName);
        file << content;

        file.close();
    }

    void SaveFile(std::string filePath) {
        String stringContent = (String)content;
        const char* actualContent = stringContent.utf8().data();
        WriteFile(filePath, actualContent);
    }
};

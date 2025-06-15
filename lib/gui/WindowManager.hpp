#pragma once
#include "Window.hpp"
#include <vector>

namespace Gui {

/// Gestisce lo stack di finestre (push / pop)
class WindowManager {
public:
    static WindowManager& instance();

    void push(Window::Ptr win);    ///< Mostra nuova finestra
    void pop();                    ///< Torna indietro (se >1)
    Window::Ptr current() const;
    bool dispatchKey(uint32_t key);

private:
    WindowManager() = default;
    std::vector<Window::Ptr> stack_;
};

} // namespace Gui
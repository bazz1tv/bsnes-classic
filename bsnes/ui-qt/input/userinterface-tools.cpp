InputGroup userInterfaceTools(InputCategory::UserInterface, "Tools");

namespace UserInterfaceTools {

struct Debugger : HotkeyInput {
  void pressed() {
    mainWindow->showDebugger();
  }

  Debugger() : HotkeyInput("Debugger", "input.userInterface.tools.debugger") {
    name = "Super+KB0::D";
    userInterfaceTools.attach(this);
  }
} debugger;

struct StateManager : HotkeyInput {
  void pressed() {
    mainWindow->showStateManager();
  }

  StateManager() : HotkeyInput("State Manager", "input.userInterface.tools.stateManager") {
    name = "Super+KB0::M";
    userInterfaceTools.attach(this);
  }
} stateManager;
}

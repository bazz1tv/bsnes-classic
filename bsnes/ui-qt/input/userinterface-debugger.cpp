InputGroup userInterfaceDebugger(InputCategory::UserInterface, "Debugger");
namespace UserInterfaceDebugger {

struct Disassembler : HotkeyInput {
  void pressed() {
    disassembler->show();
  }

  Disassembler() : HotkeyInput("Disassembler", "input.userInterface.debugger.disassembler", AssocWindow::Debugger) {
    name = "None";
    userInterfaceDebugger.attach(this);
  }
} disassembler;

struct BreakpointEditor : HotkeyInput {
  void pressed() {
    breakpointEditor->show();
  }

  BreakpointEditor() : HotkeyInput("Breakpoint Editor", "input.userInterface.debugger.breakpointEditor", AssocWindow::Debugger) {
    name = "Super+KB0::B";
    userInterfaceDebugger.attach(this);
  }
} breakpointEditor;

struct MemoryEditor : HotkeyInput {
  void pressed() {
    memoryEditor->show();
  }

  MemoryEditor() : HotkeyInput("Memory Editor", "input.userInterface.debugger.memoryEditor", AssocWindow::Debugger) {
    name = "Super+KB0::M";
    userInterfaceDebugger.attach(this);
  }
} memoryEditor;

struct PropertiesViewer : HotkeyInput {
  void pressed() {
    propertiesViewer->show();
  }

  PropertiesViewer() : HotkeyInput("Properties Viewer", "input.userInterface.debugger.propertiesViewer", AssocWindow::Debugger) {
    name = "Super+KB0::P";
    userInterfaceDebugger.attach(this);
  }
} propertiesViewer;

struct RunBreak : HotkeyInput {
  void pressed() {
    debugger->toggleRunStatus();
  }

  RunBreak() : HotkeyInput("Run/Break", "input.userInterface.debugger.runBreak", AssocWindow::Debugger) {
    name = "KB0::Return";
    userInterfaceDebugger.attach(this);
  }
} runBreak;

struct StepInstruction : HotkeyInput {
  void pressed() {
    if (debugger->stepInstruction->isEnabled())
      debugger->stepAction();
  }

  StepInstruction() : HotkeyInput("Step Instruction", "input.userInterface.debugger.stepInstruction", AssocWindow::Debugger) {
    name = "KB0::Spacebar";
    userInterfaceDebugger.attach(this);
  }
} stepInstruction;

struct StepCPU : HotkeyInput {
  void pressed() {
    debugger->stepCPU->toggle();
    debugger->synchronize();
  }

  StepCPU() : HotkeyInput("Step S-CPU", "input.userInterface.debugger.stepCPU", AssocWindow::Debugger) {
    name = "KB0::C";
    userInterfaceDebugger.attach(this);
  }
} stepCPU;

struct StepSMP : HotkeyInput {
  void pressed() {
    debugger->stepSMP->toggle();
    debugger->synchronize();
  }

  StepSMP() : HotkeyInput("Step S-SMP", "input.userInterface.debugger.stepSMP", AssocWindow::Debugger) {
    name = "KB0::S";
    userInterfaceDebugger.attach(this);
  }
} stepSMP;
}
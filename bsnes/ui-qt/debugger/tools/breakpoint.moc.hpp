class BreakpointItem : public QWidget {
  Q_OBJECT

public:
  QHBoxLayout *layout;
  QCheckBox *enabled;
  QLineEdit *addr;
  QLineEdit *data;
  QSpinBox *numbreaks;
  QComboBox *mode;
  QComboBox *source;
  BreakpointItem(unsigned id);

public slots:
  void toggle();
  void setNumBreaks(int n);

private:
  const unsigned id;
};

class BreakpointEditor : public Window {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  BreakpointItem *breakpoint[SNES::Debugger::Breakpoints];

  BreakpointEditor();
};

extern BreakpointEditor *breakpointEditor;

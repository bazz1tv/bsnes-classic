class BreakpointItem : public QWidget {
  Q_OBJECT

public:
  QCheckBox *enabled;
  QLineEdit *addr;
  QLineEdit *data;
  QSpinBox *numbreaks;
  QComboBox *mode;
  QComboBox *source;
  BreakpointItem(unsigned id, QGridLayout *layout);

public slots:
  void toggle();
  void setNumBreaks(int n);

private:
  const unsigned id;
};

class BreakpointDesc : public QWidget {
  Q_OBJECT

public:
  QLabel *enabled;
  QLabel *addr;
  QLabel *data;
  QLabel *numbreaks;
  QLabel *mode;
  QLabel *source;
  BreakpointDesc(QGridLayout *layout);
};

class BreakpointEditor : public Window {
  Q_OBJECT

public:
  QGridLayout *layout;
  BreakpointDesc *desc;
  BreakpointItem *breakpoint[SNES::Debugger::Breakpoints];

  BreakpointEditor();
};

extern BreakpointEditor *breakpointEditor;

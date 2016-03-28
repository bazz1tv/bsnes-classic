#include "breakpoint.moc"
BreakpointEditor *breakpointEditor;

BreakpointItem::BreakpointItem(unsigned id_, QGridLayout *layout) : id(id_) {
  unsigned n=0;
  enabled = new QCheckBox;
  layout->addWidget(enabled, id+1, n++, Qt::AlignHCenter);

  addr = new QLineEdit;
  addr->setFixedWidth(80);
  layout->addWidget(addr, id+1, n++, Qt::AlignHCenter);

  data = new QLineEdit;
  data->setFixedWidth(40);
  layout->addWidget(data, id+1, n++, Qt::AlignHCenter);

  numbreaks = new QSpinBox;
  numbreaks->setMinimum(1);
  numbreaks->setValue(1);
  layout->addWidget(numbreaks, id+1, n++, Qt::AlignHCenter);


  mode = new QComboBox;
  mode->addItem("Exec");
  mode->addItem("Read");
  mode->addItem("Write");
  layout->addWidget(mode, id+1, n++, Qt::AlignHCenter);

  source = new QComboBox;
  source->addItem("S-CPU bus");
  source->addItem("S-SMP bus");
  source->addItem("S-PPU VRAM");
  source->addItem("S-PPU OAM");
  source->addItem("S-PPU CGRAM");
  layout->addWidget(source, id+1, n++, Qt::AlignLeft);

  connect(enabled, SIGNAL(stateChanged(int)), this, SLOT(toggle()));
  connect(numbreaks, SIGNAL(valueChanged(int)), this, SLOT(setNumBreaks(int)));
}

BreakpointDesc::BreakpointDesc(QGridLayout *layout)
{
  unsigned n=0;
  enabled = new QLabel("Enable", this);
  layout->addWidget(enabled, 0, n++, Qt::AlignHCenter);
  addr = new QLabel("Address", this);
  layout->addWidget(addr, 0, n++, Qt::AlignHCenter);
  data = new QLabel("Data", this);
  layout->addWidget(data, 0, n++, Qt::AlignHCenter);
  numbreaks = new QLabel("Numbreaks", this);
  layout->addWidget(numbreaks, 0, n++, Qt::AlignHCenter);
  mode = new QLabel("Mode", this);
  layout->addWidget(mode, 0, n++, Qt::AlignHCenter);
  source = new QLabel("Source", this);
  layout->addWidget(source, 0, n++, Qt::AlignHCenter);
}

void BreakpointItem::setNumBreaks(int n)
{
  SNES::debugger.breakpoint[id].numbreaks = n;
  SNES::debugger.breakpoint[id].numbreaks_last = 0;
}

void BreakpointItem::toggle() {
  bool state = enabled->isChecked();

  if(state) {
    SNES::debugger.breakpoint[id].enabled = true;
    SNES::debugger.breakpoint[id].addr = hex(addr->text().toUtf8().data()) & 0xffffff;
    SNES::debugger.breakpoint[id].data = hex(data->text().toUtf8().data()) & 0xff;
    if(data->text().length() == 0) SNES::debugger.breakpoint[id].data = -1;
    SNES::debugger.breakpoint[id].mode = (SNES::Debugger::Breakpoint::Mode)mode->currentIndex();
    SNES::debugger.breakpoint[id].source = (SNES::Debugger::Breakpoint::Source)source->currentIndex();
    SNES::debugger.breakpoint[id].counter = 0;
    // numbreaks and numbreaks_last set in QT SLOT setNumBreaks immediately after GUI element is altered

    addr->setEnabled(false);
    data->setEnabled(false);
    mode->setEnabled(false);
    source->setEnabled(false);
    //numbreaks->setEnabled(false);
  } else {
    SNES::debugger.breakpoint[id].enabled = false;

    addr->setEnabled(true);
    data->setEnabled(true);
    mode->setEnabled(true);
    source->setEnabled(true);
    numbreaks->setEnabled(true);
  }
}

BreakpointEditor::BreakpointEditor() {
  setObjectName("breakpoint-editor");
  setWindowTitle("Breakpoint Editor");
  setGeometryString(&config().geometry.breakpointEditor);
  application.windowList.append(this);

  layout = new QGridLayout(this);
  setLayout(layout);

  desc = new BreakpointDesc(layout);

  for(unsigned n = 0; n < SNES::Debugger::Breakpoints; n++) {
    breakpoint[n] = new BreakpointItem(n, layout);
  }
}

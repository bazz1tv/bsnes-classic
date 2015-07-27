class Debugger {
public:
  enum class BreakEvent : unsigned {
    None,
    BreakpointHit,
    CPUStep,
    SMPStep,
  } break_event;

  int autobreak;
  bool frameBreak=false;

  enum { Breakpoints = 8 };
  struct Breakpoint {
    bool enabled;
    unsigned addr;
    signed data;  //-1 = unused
    enum class Mode : unsigned { Exec, Read, Write } mode;
    enum class Source : unsigned { CPUBus, APURAM, VRAM, OAM, CGRAM } source;
    unsigned counter;  //number of times breakpoint has been hit since being set
    unsigned numbreaks, numbreaks_last=0;
  } breakpoint[Breakpoints+1];

  unsigned breakpoint_hit;
  void breakpoint_test(Breakpoint::Source source, Breakpoint::Mode mode, unsigned addr, uint8 data);

  bool step_cpu;
  bool step_smp;
  bool bus_access;

  enum class MemorySource : unsigned { CPUBus, APUBus, APURAM, VRAM, OAM, CGRAM };
  uint8 read(MemorySource, unsigned addr);
  void write(MemorySource, unsigned addr, uint8 data);

  Debugger();
};

extern Debugger debugger;

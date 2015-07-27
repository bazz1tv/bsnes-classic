#ifdef SYSTEM_CPP

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define OS_UNIX defined(__APPLE__) || defined(__linux__) || defined(__unix) || defined(__posix)

Debugger debugger;

void Debugger::breakpoint_test(Debugger::Breakpoint::Source source, Debugger::Breakpoint::Mode mode, unsigned addr, uint8 data) {
  for(unsigned i = 0; i < Breakpoints; i++) {
    if(breakpoint[i].enabled == false) continue;
    if(breakpoint[i].source != source) continue;
    if(breakpoint[i].mode != mode) continue;
    if(breakpoint[i].data != -1 && breakpoint[i].data != data) continue;

    if(source == Debugger::Breakpoint::Source::CPUBus) {
      if(bus.is_mirror(breakpoint[i].addr, addr) == false) continue;
    } else {
      if(breakpoint[i].addr != addr) continue;
    }

    breakpoint[i].counter++;
    breakpoint[i].numbreaks_last++;
    breakpoint_hit = i;

    if (!autobreak)
    {
      if (breakpoint[i].numbreaks_last == breakpoint[i].numbreaks)
      {
        breakpoint[i].numbreaks_last = 0;
        break_event = BreakEvent::BreakpointHit;
        scheduler.exit(Scheduler::ExitReason::DebuggerEvent);
      }
    }
    else
    {
      char t[256];
      unsigned n = breakpoint_hit;
      // a user might want the below functionality printed as well
#if OS_UNIX
      printf ("%s", KNRM);
#endif

      printf ("Breakpoint %d hit (%d).\n",n,SNES::debugger.breakpoint[n].counter);
      //printf ("%s", K);

      if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::Source::CPUBus) {
        step_cpu = true;
        SNES::cpu.disassemble_opcode(t, SNES::cpu.opcode_pc);
#if OS_UNIX
        printf("%s%s\n",KRED,t);
#else
        printf("%s\n",t);
#endif
        step_cpu = false;
      }

      if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::Source::APURAM) {
        step_smp = true;
        SNES::smp.disassemble_opcode(t, SNES::smp.opcode_pc);
#if OS_UNIX
        printf("%s%s\n",KRED,t);
#else
        printf("%s\n",t);
#endif
        step_smp = false;
      }
#if OS_UNIX
      printf ("%s", KNRM);
#endif
      fflush(stdout);
    }
    break;
  }
  if (frameBreak)
  {
    frameBreak = false;
    breakpoint_hit = Breakpoints;
    breakpoint[Breakpoints].counter += breakpoint[Breakpoints].numbreaks;
    break_event = BreakEvent::BreakpointHit;
    scheduler.exit(Scheduler::ExitReason::DebuggerEvent);
  }
}

uint8 Debugger::read(Debugger::MemorySource source, unsigned addr) {
  switch(source) {
    case MemorySource::CPUBus: {
      return bus.read(addr & 0xffffff);
    } break;

    case MemorySource::APUBus: {
      if((addr & 0xffc0) == 0xffc0) return smp.iplrom[addr & 0x3f];
      return memory::apuram.read(addr & 0xffff);
    } break;

    case MemorySource::APURAM: {
      return memory::apuram.read(addr & 0xffff);
    } break;

    case MemorySource::VRAM: {
      return memory::vram.read(addr & 0xffff);
    } break;

    case MemorySource::OAM: {
      if(addr & 0x0200) return memory::oam.read(0x0200 + (addr & 0x1f));
      return memory::oam.read(addr & 0x01ff);
    } break;

    case MemorySource::CGRAM: {
      return memory::cgram.read(addr & 0x01ff);
    } break;
  }

  return 0x00;
}

void Debugger::write(Debugger::MemorySource source, unsigned addr, uint8 data) {
  switch(source) {
    case MemorySource::CPUBus: {
      bus.write(addr & 0xffffff, data);
    } break;

    case MemorySource::APURAM: {
      memory::apuram.write(addr & 0xffff, data);
    } break;

    case MemorySource::VRAM: {
      memory::vram.write(addr & 0xffff, data);
    } break;

    case MemorySource::OAM: {
      if(addr & 0x0200) memory::oam.write(0x0200 + (addr & 0x1f), data);
      else memory::oam.write(addr & 0x01ff, data);
    } break;

    case MemorySource::CGRAM: {
      memory::cgram.write(addr & 0x01ff, data);
    } break;
  }
}

Debugger::Debugger() {
  break_event = BreakEvent::None;
  autobreak = 0;

  for(unsigned n = 0; n < Breakpoints; n++) {
    breakpoint[n].enabled = false;
    breakpoint[n].addr = 0;
    breakpoint[n].data = -1;
    breakpoint[n].mode = Breakpoint::Mode::Exec;
    breakpoint[n].source = Breakpoint::Source::CPUBus;
    breakpoint[n].counter = 0;
    breakpoint[n].numbreaks = 1;
  }
  breakpoint_hit = 0;

  step_cpu = false;
  step_smp = false;
  bus_access = false;
}

#endif


#ifndef ZSP_CPU_H
#define ZSP_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CPU_CHANGE_NONE = 0,
    CPU_CHANGE      = 1,
    CPU_CHANGE_REGS = 2,
    CPU_CHANGE_MEM  = 4,
    CPU_CHANGE_A    = 8,
    CPU_CHANGE_X    = 16,
    CPU_CHANGE_Y    = 32,
    CPU_CHANGE_SP   = 64,
    CPU_CHANGE_STACK = 128,
    CPU_CHANGE_FLAGS = 256
} CPU_CHANGES;

typedef struct cpu_6510_s {
    // -- regs
    unsigned short pc;

    unsigned char a;
    unsigned char x;
    unsigned char y;

    unsigned char flags;
    unsigned short sp;

    unsigned char mem[0x10000];

    // we double the entries to be able to detect changes
    unsigned short old_pc;
    unsigned char old_a;
    unsigned char old_x;
    unsigned char old_y;
    unsigned char old_flags;
    unsigned char old_sp;
    unsigned char old_mem[0x10000];

    unsigned int cycles;

    unsigned char detect_mem_changes;
} CPU_6510;

void cpu_init(CPU_6510      *cpu, 
              unsigned short newpc, 
              unsigned char  newa,
              unsigned char  newx, 
              unsigned char  newy,
              unsigned char disable_memchk);

int  cpu_step(CPU_6510 *cpu);

// -- dbg related funcs

void cpu_dmp_regs(CPU_6510 *cpu, char *task);
void cpu_test(CPU_6510 *cpu);

CPU_CHANGES cpu_reg_changed(CPU_6510 *cpu);

// -- 

#define FN 0x80
#define FV 0x40
#define FB 0x10
#define FD 0x08
#define FI 0x04
#define FZ 0x02
#define FC 0x01

#define MEM(address) (cpu->mem[address])
#define LO() (MEM(cpu->pc))
#define HI() (MEM(cpu->pc + 1))
#define FETCH() (MEM(cpu->pc++))
#define SETPC(newpc) (cpu->pc = (newpc))
#define PUSH(data) (MEM(0x100 + (cpu->sp--)) = (data))
#define POP() (MEM(0x100 + (++cpu->sp)))

#define IMMEDIATE() (LO())
#define ABSOLUTE() (LO() | (HI() << 8))
#define ABSOLUTEX() (((LO() | (HI() << 8)) + cpu->x) & 0xffff)
#define ABSOLUTEY() (((LO() | (HI() << 8)) + cpu->y) & 0xffff)
#define ZEROPAGE() (LO() & 0xff)
#define ZEROPAGEX() ((LO() + cpu->x) & 0xff)
#define ZEROPAGEY() ((LO() + cpu->y) & 0xff)
#define INDIRECTX()                                                            \
  (MEM((LO() + cpu->x) & 0xff) | (MEM((LO() + cpu->x + 1) & 0xff) << 8))
#define INDIRECTY()                                                            \
  (((MEM(LO()) | (MEM((LO() + 1) & 0xff) << 8)) + cpu->y) & 0xffff)
#define INDIRECTZP()                                                           \
  (((MEM(LO()) | (MEM((LO() + 1) & 0xff) << 8)) + 0) & 0xffff)

#define WRITE(address)                                                         \
  { /* cpuwritemap[(address) >> 6] = 1; */                                     \
  }

#define EVALPAGECROSSING(baseaddr, realaddr)                                   \
  ((((baseaddr) ^ (realaddr)) & 0xff00) ? 1 : 0)
#define EVALPAGECROSSING_ABSOLUTEX() (EVALPAGECROSSING(ABSOLUTE(), ABSOLUTEX()))
#define EVALPAGECROSSING_ABSOLUTEY() (EVALPAGECROSSING(ABSOLUTE(), ABSOLUTEY()))
#define EVALPAGECROSSING_INDIRECTY()                                           \
  (EVALPAGECROSSING(INDIRECTZP(), INDIRECTY()))

#define BRANCH()                                                               \
  {                                                                            \
    ++cpu->cycles;                                                             \
    temp = FETCH();                                                            \
    if (temp < 0x80) {                                                         \
      cpu->cycles += EVALPAGECROSSING(cpu->pc, cpu->pc + temp);                \
      SETPC(cpu->pc + temp);                                                   \
    } else {                                                                   \
      cpu->cycles += EVALPAGECROSSING(cpu->pc, cpu->pc + temp - 0x100);        \
      SETPC(cpu->pc + temp - 0x100);                                           \
    }                                                                          \
  }

#define SETFLAGS(data)                                                         \
  {                                                                            \
    if (!(data))                                                               \
      cpu->flags = (cpu->flags & ~FN) | FZ;                                    \
    else                                                                       \
      cpu->flags = (cpu->flags & ~(FN | FZ)) | ((data)&FN);                    \
  }

#define ASSIGNSETFLAGS(dest, data)                                             \
  {                                                                            \
    dest = data;                                                               \
    if (!dest)                                                                 \
      cpu->flags = (cpu->flags & ~FN) | FZ;                                    \
    else                                                                       \
      cpu->flags = (cpu->flags & ~(FN | FZ)) | (dest & FN);                    \
  }

#define ADC(data)                                                              \
  {                                                                            \
    unsigned tempval = data;                                                   \
                                                                               \
    if (cpu->flags & FD) {                                                     \
      temp = (cpu->a & 0xf) + (tempval & 0xf) + (cpu->flags & FC);             \
      if (temp > 0x9)                                                          \
        temp += 0x6;                                                           \
      if (temp <= 0x0f)                                                        \
        temp = (temp & 0xf) + (cpu->a & 0xf0) + (tempval & 0xf0);              \
      else                                                                     \
        temp = (temp & 0xf) + (cpu->a & 0xf0) + (tempval & 0xf0) + 0x10;       \
      if (!((cpu->a + tempval + (cpu->flags & FC)) & 0xff))                    \
        cpu->flags |= FZ;                                                      \
      else                                                                     \
        cpu->flags &= ~FZ;                                                     \
      if (temp & 0x80)                                                         \
        cpu->flags |= FN;                                                      \
      else                                                                     \
        cpu->flags &= ~FN;                                                     \
      if (((cpu->a ^ temp) & 0x80) && !((cpu->a ^ tempval) & 0x80))            \
        cpu->flags |= FV;                                                      \
      else                                                                     \
        cpu->flags &= ~FV;                                                     \
      if ((temp & 0x1f0) > 0x90)                                               \
        temp += 0x60;                                                          \
      if ((temp & 0xff0) > 0xf0)                                               \
        cpu->flags |= FC;                                                      \
      else                                                                     \
        cpu->flags &= ~FC;                                                     \
    } else {                                                                   \
      temp = tempval + cpu->a + (cpu->flags & FC);                             \
      SETFLAGS(temp & 0xff);                                                   \
      if (!((cpu->a ^ tempval) & 0x80) && ((cpu->a ^ temp) & 0x80))            \
        cpu->flags |= FV;                                                      \
      else                                                                     \
        cpu->flags &= ~FV;                                                     \
      if (temp > 0xff)                                                         \
        cpu->flags |= FC;                                                      \
      else                                                                     \
        cpu->flags &= ~FC;                                                     \
    }                                                                          \
    cpu->a = temp;                                                             \
  }

#define SBC(data)                                                              \
  {                                                                            \
    unsigned tempval = data;                                                   \
    temp = cpu->a - tempval - ((cpu->flags & FC) ^ FC);                        \
                                                                               \
    if (cpu->flags & FD) {                                                     \
      unsigned tempval2;                                                       \
      tempval2 = (cpu->a & 0xf) - (tempval & 0xf) - ((cpu->flags & FC) ^ FC);  \
      if (tempval2 & 0x10)                                                     \
        tempval2 = ((tempval2 - 6) & 0xf) |                                    \
                   ((cpu->a & 0xf0) - (tempval & 0xf0) - 0x10);                \
      else                                                                     \
        tempval2 = (tempval2 & 0xf) | ((cpu->a & 0xf0) - (tempval & 0xf0));    \
      if (tempval2 & 0x100)                                                    \
        tempval2 -= 0x60;                                                      \
      if (temp < 0x100)                                                        \
        cpu->flags |= FC;                                                      \
      else                                                                     \
        cpu->flags &= ~FC;                                                     \
      SETFLAGS(temp & 0xff);                                                   \
      if (((cpu->a ^ temp) & 0x80) && ((cpu->a ^ tempval) & 0x80))             \
        cpu->flags |= FV;                                                      \
      else                                                                     \
        cpu->flags &= ~FV;                                                     \
      cpu->a = tempval2;                                                       \
    } else {                                                                   \
      SETFLAGS(temp & 0xff);                                                   \
      if (temp < 0x100)                                                        \
        cpu->flags |= FC;                                                      \
      else                                                                     \
        cpu->flags &= ~FC;                                                     \
      if (((cpu->a ^ temp) & 0x80) && ((cpu->a ^ tempval) & 0x80))             \
        cpu->flags |= FV;                                                      \
      else                                                                     \
        cpu->flags &= ~FV;                                                     \
      cpu->a = temp;                                                           \
    }                                                                          \
  }

#define CMP(src, data)                                                         \
  {                                                                            \
    temp = (src - data) & 0xff;                                                \
                                                                               \
    cpu->flags = (cpu->flags & ~(FC | FN | FZ)) | (temp & FN);                 \
                                                                               \
    if (!temp)                                                                 \
      cpu->flags |= FZ;                                                        \
    if (src >= data)                                                           \
      cpu->flags |= FC;                                                        \
  }

#define ASL(data)                                                              \
  {                                                                            \
    temp = data;                                                               \
    temp <<= 1;                                                                \
    if (temp & 0x100)                                                          \
      cpu->flags |= FC;                                                        \
    else                                                                       \
      cpu->flags &= ~FC;                                                       \
    ASSIGNSETFLAGS(data, temp);                                                \
  }

#define LSR(data)                                                              \
  {                                                                            \
    temp = data;                                                               \
    if (temp & 1)                                                              \
      cpu->flags |= FC;                                                        \
    else                                                                       \
      cpu->flags &= ~FC;                                                       \
    temp >>= 1;                                                                \
    ASSIGNSETFLAGS(data, temp);                                                \
  }

#define ROL(data)                                                              \
  {                                                                            \
    temp = data;                                                               \
    temp <<= 1;                                                                \
    if (cpu->flags & FC)                                                       \
      temp |= 1;                                                               \
    if (temp & 0x100)                                                          \
      cpu->flags |= FC;                                                        \
    else                                                                       \
      cpu->flags &= ~FC;                                                       \
    ASSIGNSETFLAGS(data, temp);                                                \
  }

#define ROR(data)                                                              \
  {                                                                            \
    temp = data;                                                               \
    if (cpu->flags & FC)                                                       \
      temp |= 0x100;                                                           \
    if (temp & 1)                                                              \
      cpu->flags |= FC;                                                        \
    else                                                                       \
      cpu->flags &= ~FC;                                                       \
    temp >>= 1;                                                                \
    ASSIGNSETFLAGS(data, temp);                                                \
  }

#define DEC(data)                                                              \
  {                                                                            \
    temp = data - 1;                                                           \
    ASSIGNSETFLAGS(data, temp);                                                \
  }

#define INC(data)                                                              \
  {                                                                            \
    temp = data + 1;                                                           \
    ASSIGNSETFLAGS(data, temp);                                                \
  }

#define EOR(data)                                                              \
  {                                                                            \
    cpu->a ^= data;                                                            \
    SETFLAGS(cpu->a);                                                          \
  }

#define ORA(data)                                                              \
  {                                                                            \
    cpu->a |= data;                                                            \
    SETFLAGS(cpu->a);                                                          \
  }

#define AND(data)                                                              \
  {                                                                            \
    cpu->a &= data;                                                            \
    SETFLAGS(cpu->a)                                                           \
  }

#define BIT(data)                                                              \
  {                                                                            \
    cpu->flags = (cpu->flags & ~(FN | FV)) | (data & (FN | FV));               \
    if (!(data & cpu->a))                                                      \
      cpu->flags |= FZ;                                                        \
    else                                                                       \
      cpu->flags &= ~FZ;                                                       \
  }

#ifdef __cplusplus
}
#endif

#endif


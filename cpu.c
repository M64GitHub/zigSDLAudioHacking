#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

static const int cpucycles_table[] = {
    7, 6, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6, 2, 5, 0, 8, 4, 4, 6, 6,
    2, 4, 2, 7, 4, 4, 7, 7, 6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
    2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, 6, 6, 0, 8, 3, 3, 5, 5,
    3, 2, 2, 2, 3, 4, 6, 6, 2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6, 2, 5, 0, 8, 4, 4, 6, 6,
    2, 4, 2, 7, 4, 4, 7, 7, 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
    2, 6, 0, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5, 2, 6, 2, 6, 3, 3, 3, 3,
    2, 2, 2, 2, 4, 4, 4, 4, 2, 5, 0, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
    2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, 2, 5, 0, 8, 4, 4, 6, 6,
    2, 4, 2, 7, 4, 4, 7, 7, 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
    2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7};

void init_cpu(CPU_6510 *cpu, unsigned short newpc, unsigned char newa,
              unsigned char newx, unsigned char newy) {
  cpu->pc = newpc;
  cpu->a = newa;
  cpu->x = newx;
  cpu->y = newy;
  cpu->flags = 0;
  cpu->sp = 0xff;
  cpu->cycles = 0;
}

void debug_cpu_regs(CPU_6510 *cpu) {
  printf("[DBG][CPU] PC: %04x A:%02x X:%02x Y:%02x    FLAGS: %02x\n", cpu->pc,
         cpu->a, cpu->x, cpu->y, cpu->flags);
}

int run_cpu(CPU_6510 *cpu) {
  unsigned temp;

  unsigned char op = FETCH();
  printf("[DBG][CPU] PC: %04x OP: %02x A:%02x X:%02x Y:%02x     FLAGS: %02x\n",
         cpu->pc - 1, op, cpu->a, cpu->x, cpu->y, cpu->flags);
  cpu->cycles += cpucycles_table[op];
  switch (op) {
  case 0xa7:
    ASSIGNSETFLAGS(cpu->a, MEM(ZEROPAGE()));
    cpu->x = cpu->a;
    cpu->pc++;
    break;

  case 0xb7:
    ASSIGNSETFLAGS(cpu->a, MEM(ZEROPAGEY()));
    cpu->x = cpu->a;
    cpu->pc++;
    break;

  case 0xaf:
    ASSIGNSETFLAGS(cpu->a, MEM(ABSOLUTE()));
    cpu->x = cpu->a;
    cpu->pc += 2;
    break;

  case 0xa3:
    ASSIGNSETFLAGS(cpu->a, MEM(INDIRECTX()));
    cpu->x = cpu->a;
    cpu->pc++;
    break;

  case 0xb3:
    cpu->cycles += EVALPAGECROSSING_INDIRECTY();
    ASSIGNSETFLAGS(cpu->a, MEM(INDIRECTY()));
    cpu->x = cpu->a;
    cpu->pc++;
    break;

  case 0x1a:
  case 0x3a:
  case 0x5a:
  case 0x7a:
  case 0xda:
  case 0xfa:
    break;

  case 0x80:
  case 0x82:
  case 0x89:
  case 0xc2:
  case 0xe2:
  case 0x04:
  case 0x44:
  case 0x64:
  case 0x14:
  case 0x34:
  case 0x54:
  case 0x74:
  case 0xd4:
  case 0xf4:
    cpu->pc++;
    break;

  case 0x0c:
  case 0x1c:
  case 0x3c:
  case 0x5c:
  case 0x7c:
  case 0xdc:
  case 0xfc:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    cpu->pc += 2;
    break;

  case 0x69:
    ADC(IMMEDIATE());
    cpu->pc++;
    break;

  case 0x65:
    ADC(MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0x75:
    ADC(MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0x6d:
    ADC(MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0x7d:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    ADC(MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0x79:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEY();
    ADC(MEM(ABSOLUTEY()));
    cpu->pc += 2;
    break;

  case 0x61:
    ADC(MEM(INDIRECTX()));
    cpu->pc++;
    break;

  case 0x71:
    cpu->cycles += EVALPAGECROSSING_INDIRECTY();
    ADC(MEM(INDIRECTY()));
    cpu->pc++;
    break;

  case 0x29:
    AND(IMMEDIATE());
    cpu->pc++;
    break;

  case 0x25:
    AND(MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0x35:
    AND(MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0x2d:
    AND(MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0x3d:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    AND(MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0x39:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEY();
    AND(MEM(ABSOLUTEY()));
    cpu->pc += 2;
    break;

  case 0x21:
    AND(MEM(INDIRECTX()));
    cpu->pc++;
    break;

  case 0x31:
    cpu->cycles += EVALPAGECROSSING_INDIRECTY();
    AND(MEM(INDIRECTY()));
    cpu->pc++;
    break;

  case 0x0a:
    ASL(cpu->a);
    break;

  case 0x06:
    ASL(MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0x16:
    ASL(MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0x0e:
    ASL(MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0x1e:
    ASL(MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0x90:
    if (!(cpu->flags & FC))
      BRANCH()
    else
      cpu->pc++;
    break;

  case 0xb0:
    if (cpu->flags & FC)
      BRANCH()
    else
      cpu->pc++;
    break;

  case 0xf0:
    if (cpu->flags & FZ)
      BRANCH()
    else
      cpu->pc++;
    break;

  case 0x24:
    BIT(MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0x2c:
    BIT(MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0x30:
    if (cpu->flags & FN)
      BRANCH()
    else
      cpu->pc++;
    break;

  case 0xd0:
    if (!(cpu->flags & FZ))
      BRANCH()
    else
      cpu->pc++;
    break;

  case 0x10:
    if (!(cpu->flags & FN))
      BRANCH()
    else
      cpu->pc++;
    break;

  case 0x50:
    if (!(cpu->flags & FV))
      BRANCH()
    else
      cpu->pc++;
    break;

  case 0x70:
    if (cpu->flags & FV)
      BRANCH()
    else
      cpu->pc++;
    break;

  case 0x18:
    cpu->flags &= ~FC;
    break;

  case 0xd8:
    cpu->flags &= ~FD;
    break;

  case 0x58:
    cpu->flags &= ~FI;
    break;

  case 0xb8:
    cpu->flags &= ~FV;
    break;

  case 0xc9:
    CMP(cpu->a, IMMEDIATE());
    cpu->pc++;
    break;

  case 0xc5:
    CMP(cpu->a, MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0xd5:
    CMP(cpu->a, MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0xcd:
    CMP(cpu->a, MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0xdd:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    CMP(cpu->a, MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0xd9:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEY();
    CMP(cpu->a, MEM(ABSOLUTEY()));
    cpu->pc += 2;
    break;

  case 0xc1:
    CMP(cpu->a, MEM(INDIRECTX()));
    cpu->pc++;
    break;

  case 0xd1:
    cpu->cycles += EVALPAGECROSSING_INDIRECTY();
    CMP(cpu->a, MEM(INDIRECTY()));
    cpu->pc++;
    break;

  case 0xe0:
    CMP(cpu->x, IMMEDIATE());
    cpu->pc++;
    break;

  case 0xe4:
    CMP(cpu->x, MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0xec:
    CMP(cpu->x, MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0xc0:
    CMP(cpu->y, IMMEDIATE());
    cpu->pc++;
    break;

  case 0xc4:
    CMP(cpu->y, MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0xcc:
    CMP(cpu->y, MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0xc6:
    DEC(MEM(ZEROPAGE()));
    WRITE(ZEROPAGE());
    cpu->pc++;
    break;

  case 0xd6:
    DEC(MEM(ZEROPAGEX()));
    WRITE(ZEROPAGEX());
    cpu->pc++;
    break;

  case 0xce:
    DEC(MEM(ABSOLUTE()));
    WRITE(ABSOLUTE());
    cpu->pc += 2;
    break;

  case 0xde:
    DEC(MEM(ABSOLUTEX()));
    WRITE(ABSOLUTEX());
    cpu->pc += 2;
    break;

  case 0xca:
    cpu->x--;
    SETFLAGS(cpu->x);
    break;

  case 0x88:
    cpu->y--;
    SETFLAGS(cpu->y);
    break;

  case 0x49:
    EOR(IMMEDIATE());
    cpu->pc++;
    break;

  case 0x45:
    EOR(MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0x55:
    EOR(MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0x4d:
    EOR(MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0x5d:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    EOR(MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0x59:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEY();
    EOR(MEM(ABSOLUTEY()));
    cpu->pc += 2;
    break;

  case 0x41:
    EOR(MEM(INDIRECTX()));
    cpu->pc++;
    break;

  case 0x51:
    cpu->cycles += EVALPAGECROSSING_INDIRECTY();
    EOR(MEM(INDIRECTY()));
    cpu->pc++;
    break;

  case 0xe6:
    INC(MEM(ZEROPAGE()));
    WRITE(ZEROPAGE());
    cpu->pc++;
    break;

  case 0xf6:
    INC(MEM(ZEROPAGEX()));
    WRITE(ZEROPAGEX());
    cpu->pc++;
    break;

  case 0xee:
    INC(MEM(ABSOLUTE()));
    WRITE(ABSOLUTE());
    cpu->pc += 2;
    break;

  case 0xfe:
    INC(MEM(ABSOLUTEX()));
    WRITE(ABSOLUTEX());
    cpu->pc += 2;
    break;

  case 0xe8:
    cpu->x++;
    SETFLAGS(cpu->x);
    break;

  case 0xc8:
    cpu->y++;
    SETFLAGS(cpu->y);
    break;

  case 0x20:
    PUSH((cpu->pc + 1) >> 8);
    PUSH((cpu->pc + 1) & 0xff);
    cpu->pc = ABSOLUTE();
    break;

  case 0x4c:
    cpu->pc = ABSOLUTE();
    break;

  case 0x6c: {
    unsigned short adr = ABSOLUTE();
    cpu->pc = (MEM(adr) | (MEM(((adr + 1) & 0xff) | (adr & 0xff00)) << 8));
  } break;

  case 0xa9:
    ASSIGNSETFLAGS(cpu->a, IMMEDIATE());
    cpu->pc++;
    break;

  case 0xa5:
    ASSIGNSETFLAGS(cpu->a, MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0xb5:
    ASSIGNSETFLAGS(cpu->a, MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0xad:
    ASSIGNSETFLAGS(cpu->a, MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0xbd:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    ASSIGNSETFLAGS(cpu->a, MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0xb9:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEY();
    ASSIGNSETFLAGS(cpu->a, MEM(ABSOLUTEY()));
    cpu->pc += 2;
    break;

  case 0xa1:
    ASSIGNSETFLAGS(cpu->a, MEM(INDIRECTX()));
    cpu->pc++;
    break;

  case 0xb1:
    cpu->cycles += EVALPAGECROSSING_INDIRECTY();
    ASSIGNSETFLAGS(cpu->a, MEM(INDIRECTY()));
    cpu->pc++;
    break;

  case 0xa2:
    ASSIGNSETFLAGS(cpu->x, IMMEDIATE());
    cpu->pc++;
    break;

  case 0xa6:
    ASSIGNSETFLAGS(cpu->x, MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0xb6:
    ASSIGNSETFLAGS(cpu->x, MEM(ZEROPAGEY()));
    cpu->pc++;
    break;

  case 0xae:
    ASSIGNSETFLAGS(cpu->x, MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0xbe:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEY();
    ASSIGNSETFLAGS(cpu->x, MEM(ABSOLUTEY()));
    cpu->pc += 2;
    break;

  case 0xa0:
    ASSIGNSETFLAGS(cpu->y, IMMEDIATE());
    cpu->pc++;
    break;

  case 0xa4:
    ASSIGNSETFLAGS(cpu->y, MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0xb4:
    ASSIGNSETFLAGS(cpu->y, MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0xac:
    ASSIGNSETFLAGS(cpu->y, MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0xbc:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    ASSIGNSETFLAGS(cpu->y, MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0x4a:
    LSR(cpu->a);
    break;

  case 0x46:
    LSR(MEM(ZEROPAGE()));
    WRITE(ZEROPAGE());
    cpu->pc++;
    break;

  case 0x56:
    LSR(MEM(ZEROPAGEX()));
    WRITE(ZEROPAGEX());
    cpu->pc++;
    break;

  case 0x4e:
    LSR(MEM(ABSOLUTE()));
    WRITE(ABSOLUTE());
    cpu->pc += 2;
    break;

  case 0x5e:
    LSR(MEM(ABSOLUTEX()));
    WRITE(ABSOLUTEX());
    cpu->pc += 2;
    break;

  case 0xea:
    break;

  case 0x09:
    ORA(IMMEDIATE());
    cpu->pc++;
    break;

  case 0x05:
    ORA(MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0x15:
    ORA(MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0x0d:
    ORA(MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0x1d:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    ORA(MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0x19:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEY();
    ORA(MEM(ABSOLUTEY()));
    cpu->pc += 2;
    break;

  case 0x01:
    ORA(MEM(INDIRECTX()));
    cpu->pc++;
    break;

  case 0x11:
    cpu->cycles += EVALPAGECROSSING_INDIRECTY();
    ORA(MEM(INDIRECTY()));
    cpu->pc++;
    break;

  case 0x48:
    PUSH(cpu->a);
    break;

  case 0x08:
    PUSH(cpu->flags | 0x30);
    break;

  case 0x68:
    ASSIGNSETFLAGS(cpu->a, POP());
    break;

  case 0x28:
    cpu->flags = POP();
    break;

  case 0x2a:
    ROL(cpu->a);
    break;

  case 0x26:
    ROL(MEM(ZEROPAGE()));
    WRITE(ZEROPAGE());
    cpu->pc++;
    break;

  case 0x36:
    ROL(MEM(ZEROPAGEX()));
    WRITE(ZEROPAGEX());
    cpu->pc++;
    break;

  case 0x2e:
    ROL(MEM(ABSOLUTE()));
    WRITE(ABSOLUTE());
    cpu->pc += 2;
    break;

  case 0x3e:
    ROL(MEM(ABSOLUTEX()));
    WRITE(ABSOLUTEX());
    cpu->pc += 2;
    break;

  case 0x6a:
    ROR(cpu->a);
    break;

  case 0x66:
    ROR(MEM(ZEROPAGE()));
    WRITE(ZEROPAGE());
    cpu->pc++;
    break;

  case 0x76:
    ROR(MEM(ZEROPAGEX()));
    WRITE(ZEROPAGEX());
    cpu->pc++;
    break;

  case 0x6e:
    ROR(MEM(ABSOLUTE()));
    WRITE(ABSOLUTE());
    cpu->pc += 2;
    break;

  case 0x7e:
    ROR(MEM(ABSOLUTEX()));
    WRITE(ABSOLUTEX());
    cpu->pc += 2;
    break;

  case 0x40:
    if (cpu->sp == 0xff)
      return 0;
    cpu->flags = POP();
    cpu->pc = POP();
    cpu->pc |= POP() << 8;
    break;

  case 0x60:
    if (cpu->sp == 0xff)
      return 0;
    cpu->pc = POP();
    cpu->pc |= POP() << 8;
    cpu->pc++;
    break;

  case 0xe9:
  case 0xeb:
    SBC(IMMEDIATE());
    cpu->pc++;
    break;

  case 0xe5:
    SBC(MEM(ZEROPAGE()));
    cpu->pc++;
    break;

  case 0xf5:
    SBC(MEM(ZEROPAGEX()));
    cpu->pc++;
    break;

  case 0xed:
    SBC(MEM(ABSOLUTE()));
    cpu->pc += 2;
    break;

  case 0xfd:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEX();
    SBC(MEM(ABSOLUTEX()));
    cpu->pc += 2;
    break;

  case 0xf9:
    cpu->cycles += EVALPAGECROSSING_ABSOLUTEY();
    SBC(MEM(ABSOLUTEY()));
    cpu->pc += 2;
    break;

  case 0xe1:
    SBC(MEM(INDIRECTX()));
    cpu->pc++;
    break;

  case 0xf1:
    cpu->cycles += EVALPAGECROSSING_INDIRECTY();
    SBC(MEM(INDIRECTY()));
    cpu->pc++;
    break;

  case 0x38:
    cpu->flags |= FC;
    break;

  case 0xf8:
    cpu->flags |= FD;
    break;

  case 0x78:
    cpu->flags |= FI;
    break;

  case 0x85:
    MEM(ZEROPAGE()) = cpu->a;
    WRITE(ZEROPAGE());
    cpu->pc++;
    break;

  case 0x95:
    MEM(ZEROPAGEX()) = cpu->a;
    WRITE(ZEROPAGEX());
    cpu->pc++;
    break;

  case 0x8d:
    MEM(ABSOLUTE()) = cpu->a;
    WRITE(ABSOLUTE());
    cpu->pc += 2;
    break;

  case 0x9d:
    MEM(ABSOLUTEX()) = cpu->a;
    WRITE(ABSOLUTEX());
    cpu->pc += 2;
    break;

  case 0x99:
    MEM(ABSOLUTEY()) = cpu->a;
    WRITE(ABSOLUTEY());
    cpu->pc += 2;
    break;

  case 0x81:
    MEM(INDIRECTX()) = cpu->a;
    WRITE(INDIRECTX());
    cpu->pc++;
    break;

  case 0x91:
    MEM(INDIRECTY()) = cpu->a;
    WRITE(INDIRECTY());
    cpu->pc++;
    break;

  case 0x86:
    MEM(ZEROPAGE()) = cpu->x;
    WRITE(ZEROPAGE());
    cpu->pc++;
    break;

  case 0x96:
    MEM(ZEROPAGEY()) = cpu->x;
    WRITE(ZEROPAGEY());
    cpu->pc++;
    break;

  case 0x8e:
    MEM(ABSOLUTE()) = cpu->x;
    WRITE(ABSOLUTE());
    cpu->pc += 2;
    break;

  case 0x84:
    MEM(ZEROPAGE()) = cpu->y;
    WRITE(ZEROPAGE());
    cpu->pc++;
    break;

  case 0x94:
    MEM(ZEROPAGEX()) = cpu->y;
    WRITE(ZEROPAGEX());
    cpu->pc++;
    break;

  case 0x8c:
    MEM(ABSOLUTE()) = cpu->y;
    WRITE(ABSOLUTE());
    cpu->pc += 2;
    break;

  case 0xaa:
    ASSIGNSETFLAGS(cpu->x, cpu->a);
    break;

  case 0xba:
    ASSIGNSETFLAGS(cpu->x, cpu->sp);
    break;

  case 0x8a:
    ASSIGNSETFLAGS(cpu->a, cpu->x);
    break;

  case 0x9a:
    cpu->sp = cpu->x;
    break;

  case 0x98:
    ASSIGNSETFLAGS(cpu->a, cpu->y);
    break;

  case 0xa8:
    ASSIGNSETFLAGS(cpu->y, cpu->a);
    break;

  case 0x00:
    return 0;

  case 0x02:
    printf("Error: CPU halt at %04X\n", cpu->pc - 1);
    exit(1);
    break;

  default:
    printf("Error: Unknown opcode $%02X at $%04X\n", op, cpu->pc - 1);
    exit(1);
    break;
  }
  return 1;
}

void setpc(CPU_6510 *cpu, unsigned short newpc) { cpu->pc = newpc; }

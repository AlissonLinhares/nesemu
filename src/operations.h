#ifndef OPERATIONS_H
#define OPERATIONS_H

// cout << hex << setw(2) << (unsigned) opcode << endl;

// #define _BEGIN(id) \
// 	void id() { \
// 		std::cout << #id << std::endl;

#define _BEGIN(id) \
	void id() {
#define _END }
// #define _END char key; std::cin >> key;}

#define _ADC_DECL(id, addressing_mode) _BEGIN(id) \
	uint8_t data = io.load(addressing_mode()); \
	uint16_t result = a + data + status.flags.c; \
	\
	bool fa = a & 0x80; \
	bool fb = data & 0x80; \
	bool fc = result & 0x80; \
	\
	a = (uint8_t) result; \
	\
	status.flags.v = (fa == fb) & (fc != fa); \
	status.flags.c = result > 0xFF; \
	status.flags.z = (a == 0); \
	status.flags.n = ((a & 0x80) == 0x80); \
_END

#define _AND_DECL(id, addressing_mode) _BEGIN(id) \
	uint8_t data = io.load(addressing_mode()); \
	a = a & data; \
	status.flags.z = (a == 0); \
	status.flags.n = ((a & 0x80) == 0x80); \
_END

#define _ASL_DECL(id, addressing_mode) _BEGIN(id) \
	uint16_t addr = addressing_mode(); \
	uint16_t data = io.load(addr); \
	data = data << 1; \
	io.store(addr, (uint8_t) data); \
	status.flags.z = (data & 0xFF) == 0; \
	status.flags.c = ((data & 0x100) == 0x100); \
	status.flags.n = ((data & 0x80) == 0x80); \
_END

#define _ASLA_DECL(id) _BEGIN(id) \
	uint16_t data = a; \
	data = data << 1; \
	a = (uint8_t) data; \
	status.flags.z = (a == 0); \
	status.flags.c = ((data & 0x100) == 0x100); \
	status.flags.n = ((a & 0x80) == 0x80); \
	pc++; \
_END

#define _LSR_DECL(id, addressing_mode) _BEGIN(id) \
	uint16_t addr = addressing_mode(); \
	uint8_t data = io.load(addr); \
	status.flags.c = data & 0x1; \
	data = data >> 1; \
	status.flags.z = (data == 0); \
	status.flags.n = 0; \
	io.store(addr, data); \
_END

#define _LSRA_DECL(id) _BEGIN(id) \
	uint8_t data = a; \
	status.flags.c = data & 0x1; \
	data = data >> 1; \
	status.flags.z = (data == 0); \
	status.flags.n = 0; \
	a = data; \
	pc++; \
_END

#define _BRANCH_DECL(id, condition) _BEGIN(id) \
	if (condition) \
		pc += (int8_t) io.load(pc + 1); \
	pc += 2; \
_END

#define _CLI_DECL(id) _BEGIN(id) \
	status.flags.i = 0; \
	pc++; \
_END

#define _CLV_DECL(id) _BEGIN(id) \
	status.flags.v = 0; \
	pc++; \
_END

#define _CLD_DECL(id) _BEGIN(id) \
	status.flags.d = 0; \
	pc++; \
_END

#define _CLC_DECL(id) _BEGIN(id) \
	status.flags.c = 0; \
	pc++; \
_END

#define _CMP_DECL(id, r, addressing_mode) _BEGIN(id) \
	uint8_t data = io.load(addressing_mode()); \
	status.flags.c = (r >= data); \
	status.flags.z = (r == data); \
	status.flags.n = (r < data); \
_END

#define _SBC_DECL(id, addressing_mode) _BEGIN(id) \
	uint8_t data = io.load(addressing_mode()); \
	uint16_t result = a - data - (1 - status.flags.c); \
	\
	bool fa = a & 0x80; \
	bool fb = data & 0x80; \
	bool fc = result & 0x80; \
	\
	a = (uint8_t) result; \
	\
	status.flags.v = (fa == fb) & (fc != fa); \
	status.flags.c = result <= 0xFF; \
	status.flags.z = (a == 0); \
	status.flags.n = ((a & 0x80) == 0x80); \
_END

#define _LD_DECL(id, out, addressing_mode) _BEGIN(id) \
	out = io.load(addressing_mode()); \
	status.flags.z = (out == 0); \
	status.flags.n = ((out & 0x80) == 0x80); \
_END

#define _ST_DECL(id, in, addressing_mode) _BEGIN(id) \
	io.store(addressing_mode(), in); \
_END

#define _ORA_DECL(id, addressing_mode) _BEGIN(id) \
	a = a | io.load(addressing_mode()); \
	status.flags.z = (a == 0); \
	status.flags.n = ((a & 0x80) == 0x80); \
_END

#define _EOR_DECL(id, addressing_mode) _BEGIN(id) \
	a = a ^ io.load(addressing_mode()); \
	status.flags.z = (a == 0); \
	status.flags.n = ((a & 0x80) == 0x80); \
_END

#define _SEI_DECL(id) _BEGIN(id) \
	status.flags.i = 1; \
	pc++; \
_END

#define _SED_DECL(id) _BEGIN(id) \
	status.flags.d = 1; \
	pc++; \
_END

#define _SEC_DECL(id) _BEGIN(id) \
	status.flags.c = 1; \
	pc++; \
_END

#define _JMP_DECL(id, addressing_mode) _BEGIN(id) \
	pc = (uint16_t) addressing_mode(); \
_END

#define _BIT_DECL(id, addressing_mode) _BEGIN(id) \
	uint8_t temp = io.load(addressing_mode()); \
	status.flags.z = ((a & temp) == 0); \
	status.flags.n = (temp & 0x80) == 0x80; \
	status.flags.v = (temp & 0x40) == 0x40; \
_END

#define _NMI_DECL(id) _BEGIN(id) \
	status.flags.b = 1; \
	io.store16(sp - 1, pc + 1); \
	io.store(sp - 2, status.value); \
	sp -= 3; \
	pc = io.load16(0xFFFA); \
_END

#define _RESET_DECL(id) _BEGIN(id) \
	status.flags.b = 1; \
	io.store16(sp - 1, pc + 1); \
	io.store(sp - 2, status.value); \
	sp -= 3; \
	pc = io.load16(0xFFFC); \
_END

#define _BRK_DECL(id) _BEGIN(id) \
	status.flags.b = 1; \
	io.store16(sp - 1, pc + 1); \
	io.store(sp - 2, status.value); \
	sp -= 3; \
	pc = io.load16(0xFFFE); \
_END

#define _RTI_DECL(id) _BEGIN(id) \
	sp += 3; \
	pc = io.load16(sp - 1); \
	status.value &= 0b00110000; \
	status.value |= io.load(sp - 2); \
_END

#define _DEC_DECL(id, addressing_mode) _BEGIN(id) \
	uint16_t addr = addressing_mode(); \
	uint8_t data = io.load(addr); \
	data--; \
	io.store(addr,data); \
	status.flags.z = (data == 0); \
	status.flags.n = ((data & 0x80) == 0x80); \
_END

#define _DER_DECL(id, r) _BEGIN(id) \
	r--; \
	status.flags.z = (r == 0); \
	status.flags.n = ((r & 0x80) == 0x80); \
	pc++; \
_END

#define _INC_DECL(id, addressing_mode) _BEGIN(id) \
	uint16_t addr = addressing_mode(); \
	uint8_t data = io.load(addr); \
	data++; \
	io.store(addr,data); \
	status.flags.z = (data == 0); \
	status.flags.n = ((data & 0x80) == 0x80); \
_END

#define _INR_DECL(id, r) _BEGIN(id) \
	r++; \
	status.flags.z = (r == 0); \
	status.flags.n = ((r & 0x80) == 0x80); \
	pc++; \
_END

#define _JSR_DECL(id, addressing_mode) _BEGIN(id) \
	io.store16(sp - 1, pc + 2); \
	pc = addressing_mode(); \
	sp -= 2; \
_END

#define _RTS_DECL(id) _BEGIN(id) \
	sp += 2; \
	pc = io.load16(sp - 1) + 1; \
_END

#define _TX_DECL(id, src, dst) _BEGIN(id) \
	dst = src; \
	status.flags.z = (dst == 0); \
	status.flags.n = ((dst & 0x80) == 0x80); \
	pc++; \
_END

#define _PUSH_DECL(id, r) _BEGIN(id) \
	io.store(sp, r); \
	sp--; \
	pc++; \
_END

#define _POP_DECL(id, r) _BEGIN(id) \
	sp++; \
	r = io.load(sp); \
	pc++; \
_END

#define _PLP_DECL(id) _BEGIN(id) \
	sp++; \
	status.value &= 0b00110000; \
	status.value |= io.load(sp); \
	pc++; \
_END

#define _ROL_DECL(id, addressing_mode) _BEGIN(id) \
	uint16_t addr = addressing_mode(); \
	uint16_t data = io.load(addr); \
	data = (data << 1) | status.flags.c; \
	io.store(addr, (uint8_t) data); \
	status.flags.z = ((data & 0xFF) == 0); \
	status.flags.c = ((data & 0x100) == 0x100); \
	status.flags.n = ((data & 0x80) == 0x80); \
_END

#define _ROLA_DECL(id) _BEGIN(id) \
	uint16_t data = a; \
	data = (data << 1) | status.flags.c; \
	a = (uint8_t) data; \
	status.flags.z = (a == 0); \
	status.flags.c = ((data & 0x100) == 0x100); \
	status.flags.n = ((a & 0x80) == 0x80); \
	pc++; \
_END

#define _ROR_DECL(id, addressing_mode) _BEGIN(id) \
	uint16_t addr = addressing_mode(); \
	uint8_t data = io.load(addr); \
	bool bit0 = data & 1; \
	data = (data >> 1) | status.flags.c << 7; \
	io.store(addr, data); \
	status.flags.z = ((data & 0xFF) == 0); \
	status.flags.c = bit0; \
	status.flags.n = ((data & 0x80) == 0x80); \
_END

#define _RORA_DECL(id) _BEGIN(id) \
	bool bit0 = a & 1; \
	a = (a >> 1) | status.flags.c << 7; \
	status.flags.z = ((a & 0xFF) == 0); \
	status.flags.c = bit0; \
	status.flags.n = ((a & 0x80) == 0x80); \
	pc++; \
_END

#endif

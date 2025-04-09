// obelisk.me.uk/6502/reference.html
// skilldrick.github.io/easy6502/
// http://obelisk.me.uk/6502/
// http://users.telenet.be/kim1-6502/6502/proman.html
// https://nesdev.com/
// https://patater.com/nes-asm-tutorials/day-1/
// http://nintendoage.com/forum/messageview.cfm?catid=22&threadid=7155

#include <operations.h>
#include <iobus.h>
#include <iointerface.h>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

using namespace std;

#ifndef CPU_H
#define CPU_H

namespace nesemu {

	void tick(list<IOInterface*> devices) {
		while (true) {
			auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(100);

			for (auto it=devices.begin(); it != devices.end(); ++it)
				(*it)->update();

			std::this_thread::sleep_until(x);
		}
	}

	class CPU {
	private:
		typedef void (CPU::*tInstruction)();

	public:
		CPU(IOBus &io) : io(io) {
			a = 0;
			x = 0;
			y = 0;
			sp = 0x01FF;

			status.flags.c = 0;
			status.flags.z = 0;
			status.flags.i = 0;
			status.flags.d = 0;
			status.flags.b = 0;
			status.flags.r = 1;
			status.flags.v = 0;
			status.flags.n = 0;

			pc = io.load16(0xFFFC);
		}

		inline void run() {
			// thread(tick, devices).detach();

			while (true) {
 				usleep(100);
				uint8_t opcode = io.load(pc);
				switch (opcode) {
					case 0x00:
						if (io.load16(0xFFFE) == 0) {
							abort();
							return;
						} else {
							brk();
						}
						break;
					case 0x01: ora_indexed_indirect(); break;
					case 0x05: ora_zero();             break;
					case 0x06: asl_zero();             break;
					case 0x08: php();                  break;
					case 0x09: ora_imm();              break;
					case 0x0A: asl_accumulator();      break;
					case 0x0D: ora_absolute();         break;
					case 0x0E: asl_absolute();         break;
					case 0x10: bpl();                  break;
					case 0x11: ora_indirect_indexed(); break;
					case 0x15: ora_zero_x();           break;
					case 0x16: asl_zero_x();           break;
					case 0x18: clc();                  break;
					case 0x19: ora_absolute_y();       break;
					case 0x1D: ora_absolute_x();       break;
					case 0x1E: asl_absolute_x();       break;
					case 0x20: jsr();                  break;
					case 0x21: and_indexed_indirect(); break;
					case 0x24: bit_zero();             break;
					case 0x25: and_zero();             break;
					case 0x26: rol_zero();             break;
					case 0x28: plp();                  break;
					case 0x29: and_imm();              break;
					case 0x2A: rol_accumulator();      break;
					case 0x2C: bit_absolute();         break;
					case 0x2D: and_absolute();         break;
					case 0x2E: rol_absolute();         break;
					case 0x30: bmi();                  break;
					case 0x31: and_indirect_indexed(); break;
					case 0x35: and_zero_x();           break;
					case 0x36: rol_zero_x();           break;
					case 0x38: sec();                  break;
					case 0x39: and_absolute_y();       break;
					case 0x3D: and_absolute_x();       break;
					case 0x3E: rol_absolute_x();       break;
					case 0x40: rti();                  break;
					case 0x41: eor_indexed_indirect(); break;
					case 0x45: eor_zero();             break;
					case 0x46: lsr_zero();             break;
					case 0x48: pha();                  break;
					case 0x49: eor_imm();              break;
					case 0x4A: lsr_accumulator();      break;
					case 0x4C: jmp_absolute();         break;
					case 0x4D: eor_absolute();         break;
					case 0x4E: lsr_absolute();         break;
					case 0x50: bvc();                  break;
					case 0x51: eor_indirect_indexed(); break;
					case 0x55: eor_zero_x();           break;
					case 0x56: lsr_zero_x();           break;
					case 0x58: cli();                  break;
					case 0x59: eor_absolute_y();       break;
					case 0x5D: eor_absolute_x();       break;
					case 0x5E: lsr_absolute_x();       break;
					case 0x60: rts();                  break;
					case 0x61: adc_indexed_indirect(); break;
					case 0x65: adc_zero();             break;
					case 0x66: ror_zero();             break;
					case 0x68: pla();                  break;
					case 0x69: adc_imm();              break;
					case 0x6A: ror_accumulator();      break;
					case 0x6C: jmp_indirect();         break;
					case 0x6D: adc_absolute();         break;
					case 0x6E: ror_absolute();         break;
					case 0x70: bvs();                  break;
					case 0x71: adc_indirect_indexed(); break;
					case 0x75: adc_zero_x();           break;
					case 0x76: ror_zero_x();           break;
					case 0x78: sei();                  break;
					case 0x79: adc_absolute_y();       break;
					case 0x7D: adc_absolute_x();       break;
					case 0x7E: ror_absolute_x();       break;
					case 0x81: sta_indexed_indirect(); break;
					case 0x84: sty_zero();             break;
					case 0x85: sta_zero();             break;
					case 0x86: stx_zero();             break;
					case 0x88: dey();                  break;
					case 0x8A: txa();                  break;
					case 0x8C: sty_absolute();         break;
					case 0x8D: sta_absolute();         break;
					case 0x8E: stx_absolute();         break;
					case 0x90: bcc();                  break;
					case 0x91: sta_indirect_indexed(); break;
					case 0x94: sty_zero_x();           break;
					case 0x95: sta_zero_x();           break;
					case 0x96: stx_zero_y();           break;
					case 0x98: tya();                  break;
					case 0x99: sta_absolute_y();       break;
					case 0x9A: txs();                  break;
					case 0x9D: sta_absolute_x();       break;
					case 0xA0: ldy_imm();              break;
					case 0xA1: lda_indexed_indirect(); break;
					case 0xA2: ldx_imm();              break;
					case 0xA4: ldy_zero();             break;
					case 0xA5: lda_zero();             break;
					case 0xA6: ldx_zero();             break;
					case 0xA8: tay();                  break;
					case 0xA9: lda_imm();              break;
					case 0xAA: tax();                  break;
					case 0xAC: ldy_absolute();         break;
					case 0xAD: lda_absolute();         break;
					case 0xAE: ldx_absolute();         break;
					case 0xB0: bcs();                  break;
					case 0xB1: lda_indirect_indexed(); break;
					case 0xB4: ldy_zero_x();           break;
					case 0xB5: lda_zero_x();           break;
					case 0xB6: ldx_zero_y();           break;
					case 0xB8: clv();                  break;
					case 0xB9: lda_absolute_y();       break;
					case 0xBA: tsx();                  break;
					case 0xBC: ldy_absolute_x();       break;
					case 0xBD: lda_absolute_x();       break;
					case 0xBE: ldx_absolute_y();       break;
					case 0xC0: cpy_imm();              break;
					case 0xC1: cmp_indexed_indirect(); break;
					case 0xC4: cpy_zero();             break;
					case 0xC5: cmp_zero();             break;
					case 0xC6: dec_zero();             break;
					case 0xC8: iny();                  break;
					case 0xC9: cmp_imm();              break;
					case 0xCA: dex();                  break;
					case 0xCC: cpy_absolute();         break;
					case 0xCD: cmp_absolute();         break;
					case 0xCE: dec_absolute();         break;
					case 0xD0: bne();                  break;
					case 0xD1: cmp_indirect_indexed(); break;
					case 0xD5: cmp_zero_x();           break;
					case 0xD6: dec_zero_x();           break;
					case 0xD8: cld();                  break;
					case 0xD9: cmp_absolute_y();       break;
					case 0xDD: cmp_absolute_x();       break;
					case 0xDE: dec_absolute_x();       break;
					case 0xE0: cpx_imm();              break;
					case 0xE1: sbc_indexed_indirect(); break;
					case 0xE4: cpx_zero();             break;
					case 0xE5: sbc_zero();             break;
					case 0xE6: inc_zero();             break;
					case 0xE8: inx();                  break;
					case 0xE9: sbc_imm();              break;
					case 0xEA: nop();                  break;
					case 0xEC: cpx_absolute();         break;
					case 0xED: sbc_absolute();         break;
					case 0xEE: inc_absolute();         break;
					case 0xF0: beq();                  break;
					case 0xF1: sbc_indirect_indexed(); break;
					case 0xF5: sbc_zero_x();           break;
					case 0xF6: inc_zero_x();           break;
					case 0xF8: sed();                  break;
					case 0xF9: sbc_absolute_y();       break;
					case 0xFD: sbc_absolute_x();       break;
					case 0xFE: inc_absolute_x();       break;
					default: abort();                 return;
				}

				io.update();

				// if (io.getNMI())
				// 	nmi();
			}
		}

		inline void dump() const {
			cout << setfill('0'); // fill with 0s
			cout << "----- Registers -----" << endl;
			cout << "a = 0x" << hex << setw(2) << (unsigned) a << endl;
			cout << "x = 0x" << hex << setw(2) << (unsigned) x << endl;
			cout << "y = 0x" << hex << setw(2) << (unsigned) y << endl;
			cout << "sp = 0x" << hex << setw(4) << sp << endl;
			cout << "pc = 0x" << hex << setw(4) << pc << endl;
			cout << "status = " << bitset<8>(status.value) << endl;
			cout << "         " << "NV-BDIZC" << endl;
			cout << "---------------------" << endl;
		}

		_NMI_DECL(nmi);
		_NMI_DECL(reset);

		virtual ~CPU() {

		}

	private:
		void nop() {
#if DEBUG == 1
			dump();
#endif
			pc++;
		}

		void abort() {
			dump();
		}

		_RTS_DECL(rts);

		_BRK_DECL(brk);

		_RTI_DECL(rti);

		_CLI_DECL(cli);
		_CLV_DECL(clv);
		_CLD_DECL(cld);
		_CLC_DECL(clc);

		_SEI_DECL(sei);
		_SED_DECL(sed);
		_SEC_DECL(sec);

		_PLP_DECL(plp);
		_POP_DECL(pla                 , a);
		_PUSH_DECL(pha                , a);
		_PUSH_DECL(php                , status.value | 0b00110000);

		_TX_DECL(tax                  , a, x);
		_TX_DECL(tay                  , a, y);
		_TX_DECL(tsx                  , ((uint8_t) sp), x);
		_TX_DECL(txa                  , x, a);
		_TX_DECL(txs                  , (uint16_t) (x | 0x0100), sp);
		_TX_DECL(tya                  , y, a);

		_INR_DECL(inx                 , x);
		_INR_DECL(iny                 , y);

		_DER_DECL(dex                 , x);
		_DER_DECL(dey                 , y);

		_DEC_DECL(dec_zero            , zero_page);
		_DEC_DECL(dec_zero_x          , zero_page_x);
		_DEC_DECL(dec_absolute        , absolute);
		_DEC_DECL(dec_absolute_x      , absolute_x);

		_INC_DECL(inc_zero            , zero_page);
		_INC_DECL(inc_zero_x          , zero_page_x);
		_INC_DECL(inc_absolute        , absolute);
		_INC_DECL(inc_absolute_x      , absolute_x);

		_JSR_DECL(jsr                 , absolute);

		_BIT_DECL(bit_zero            , zero_page);
		_BIT_DECL(bit_absolute        , absolute);

		_ADC_DECL(adc_imm             , immediate);
		_ADC_DECL(adc_zero            , zero_page);
		_ADC_DECL(adc_zero_x          , zero_page_x);
		_ADC_DECL(adc_absolute        , absolute);
		_ADC_DECL(adc_absolute_x      , absolute_x);
		_ADC_DECL(adc_absolute_y      , absolute_y);
		_ADC_DECL(adc_indexed_indirect, indexed_indirect);
		_ADC_DECL(adc_indirect_indexed, indirect_indexed);

		_SBC_DECL(sbc_imm             , immediate);
		_SBC_DECL(sbc_zero            , zero_page);
		_SBC_DECL(sbc_zero_x          , zero_page_x);
		_SBC_DECL(sbc_absolute        , absolute);
		_SBC_DECL(sbc_absolute_x      , absolute_x);
		_SBC_DECL(sbc_absolute_y      , absolute_y);
		_SBC_DECL(sbc_indexed_indirect, indexed_indirect);
		_SBC_DECL(sbc_indirect_indexed, indirect_indexed);

		_AND_DECL(and_imm             , immediate);
		_AND_DECL(and_zero            , zero_page);
		_AND_DECL(and_zero_x          , zero_page_x);
		_AND_DECL(and_absolute        , absolute);
		_AND_DECL(and_absolute_x      , absolute_x);
		_AND_DECL(and_absolute_y      , absolute_y);
		_AND_DECL(and_indexed_indirect, indexed_indirect);
		_AND_DECL(and_indirect_indexed, indirect_indexed);

		_ASLA_DECL(asl_accumulator);
		_ASL_DECL(asl_zero            , zero_page);
		_ASL_DECL(asl_zero_x          , zero_page_x);
		_ASL_DECL(asl_absolute        , absolute);
		_ASL_DECL(asl_absolute_x      , absolute_x);

		_ROLA_DECL(rol_accumulator);
		_ROL_DECL(rol_zero            , zero_page);
		_ROL_DECL(rol_zero_x          , zero_page_x);
		_ROL_DECL(rol_absolute        , absolute);
		_ROL_DECL(rol_absolute_x      , absolute_x);

		_RORA_DECL(ror_accumulator);
		_ROR_DECL(ror_zero            , zero_page);
		_ROR_DECL(ror_zero_x          , zero_page_x);
		_ROR_DECL(ror_absolute        , absolute);
		_ROR_DECL(ror_absolute_x      , absolute_x);

		_LSRA_DECL(lsr_accumulator);
		_LSR_DECL(lsr_zero            , zero_page);
		_LSR_DECL(lsr_zero_x          , zero_page_x);
		_LSR_DECL(lsr_absolute        , absolute);
		_LSR_DECL(lsr_absolute_x      , absolute_x);

		_BRANCH_DECL(bcc              , status.flags.c == 0);
		_BRANCH_DECL(bcs              , status.flags.c == 1);
		_BRANCH_DECL(beq              , status.flags.z == 1);
		_BRANCH_DECL(bmi              , status.flags.n == 1);
		_BRANCH_DECL(bne              , status.flags.z == 0);
		_BRANCH_DECL(bpl              , status.flags.n == 0);
		_BRANCH_DECL(bvc              , status.flags.v == 0);
		_BRANCH_DECL(bvs              , status.flags.v == 1);

		_JMP_DECL(jmp_absolute        , absolute);
		_JMP_DECL(jmp_indirect        , indirect);

		_LD_DECL(lda_imm              , a, immediate);
		_LD_DECL(lda_zero             , a, zero_page);
		_LD_DECL(lda_zero_x           , a, zero_page_x);
		_LD_DECL(lda_absolute         , a, absolute);
		_LD_DECL(lda_absolute_x       , a, absolute_x);
		_LD_DECL(lda_absolute_y       , a, absolute_y);
		_LD_DECL(lda_indexed_indirect , a, indexed_indirect);
		_LD_DECL(lda_indirect_indexed , a, indirect_indexed);

		_LD_DECL(ldx_imm              , x, immediate);
		_LD_DECL(ldx_zero             , x, zero_page);
		_LD_DECL(ldx_zero_y           , x, zero_page_y);
		_LD_DECL(ldx_absolute         , x, absolute);
		_LD_DECL(ldx_absolute_y       , x, absolute_y);

		_LD_DECL(ldy_imm              , y, immediate);
		_LD_DECL(ldy_zero             , y, zero_page);
		_LD_DECL(ldy_zero_x           , y, zero_page_x);
		_LD_DECL(ldy_absolute         , y, absolute);
		_LD_DECL(ldy_absolute_x       , y, absolute_x);

		_ST_DECL(sta_zero             , a, zero_page);
		_ST_DECL(sta_zero_x           , a, zero_page_x);
		_ST_DECL(sta_absolute         , a, absolute);
		_ST_DECL(sta_absolute_x       , a, absolute_x);
		_ST_DECL(sta_absolute_y       , a, absolute_y);
		_ST_DECL(sta_indexed_indirect , a, indexed_indirect);
		_ST_DECL(sta_indirect_indexed , a, indirect_indexed);

		_ST_DECL(stx_zero             , x, zero_page);
		_ST_DECL(stx_zero_y           , x, zero_page_y);
		_ST_DECL(stx_absolute         , x, absolute);

		_ST_DECL(sty_zero             , y, zero_page);
		_ST_DECL(sty_zero_x           , y, zero_page_x);
		_ST_DECL(sty_absolute         , y, absolute);

		_EOR_DECL(eor_imm             , immediate);
		_EOR_DECL(eor_zero            , zero_page);
		_EOR_DECL(eor_zero_x          , zero_page_x);
		_EOR_DECL(eor_absolute        , absolute);
		_EOR_DECL(eor_absolute_x      , absolute_x);
		_EOR_DECL(eor_absolute_y      , absolute_y);
		_EOR_DECL(eor_indexed_indirect, indexed_indirect);
		_EOR_DECL(eor_indirect_indexed, indirect_indexed);

		_ORA_DECL(ora_imm             , immediate);
		_ORA_DECL(ora_zero            , zero_page);
		_ORA_DECL(ora_zero_x          , zero_page_x);
		_ORA_DECL(ora_absolute        , absolute);
		_ORA_DECL(ora_absolute_x      , absolute_x);
		_ORA_DECL(ora_absolute_y      , absolute_y);
		_ORA_DECL(ora_indexed_indirect, indexed_indirect);
		_ORA_DECL(ora_indirect_indexed, indirect_indexed);

		_CMP_DECL(cmp_imm             , a, immediate);
		_CMP_DECL(cmp_zero            , a, zero_page);
		_CMP_DECL(cmp_zero_x          , a, zero_page_x);
		_CMP_DECL(cmp_absolute        , a, absolute);
		_CMP_DECL(cmp_absolute_x      , a, absolute_x);
		_CMP_DECL(cmp_absolute_y      , a, absolute_y);
		_CMP_DECL(cmp_indexed_indirect, a, indexed_indirect);
		_CMP_DECL(cmp_indirect_indexed, a, indirect_indexed);

		_CMP_DECL(cpx_imm             , x, immediate);
		_CMP_DECL(cpx_zero            , x, zero_page);
		_CMP_DECL(cpx_absolute        , x, absolute);

		_CMP_DECL(cpy_imm             , y, immediate);
		_CMP_DECL(cpy_zero            , y, zero_page);
		_CMP_DECL(cpy_absolute        , y, absolute);

	protected:
		inline const uint16_t immediate() {
			uint16_t imm = pc + 1;
			pc += 2;
			return imm;
		}

		inline const uint8_t zero_page() {
			uint8_t addr = io.load(pc + 1);
			pc += 2;
			return addr;
		}

		inline const uint8_t zero_page_x() {
			uint8_t addr = io.load(pc + 1) + x;
			pc += 2;
			return addr;
		}

		inline const uint8_t zero_page_y() {
			uint8_t addr = io.load(pc + 1) + y;
			pc += 2;
			return addr;
		}

		inline const uint16_t absolute() {
			uint16_t addr = io.load16(pc + 1);
			pc += 3;
			return addr;
		}

		inline const uint16_t absolute_x() {
			uint16_t addr = io.load16(pc + 1) + x;
			pc += 3;
			return addr;
		}

		inline const uint16_t absolute_y() {
			uint16_t addr = io.load16(pc + 1) + y;
			pc += 3;
			return addr;
		}

		inline const uint16_t indexed_indirect() {
			uint16_t addr = io.load16(io.load(pc + 1) + x);
			pc += 2;
			return addr;
		}

		inline const uint16_t indirect_indexed() {
			uint16_t addr = io.load16(io.load(pc + 1)) + y;
			pc += 2;
			return addr;
		}

		inline const uint16_t indirect() {
			uint16_t addr = io.load16(io.load16(pc + 1));
			pc += 3;
			return addr;
		}

	private:
		IOBus& io;
		uint8_t a, x, y;
		uint16_t sp, pc;

		union {
			uint8_t value;
			struct {
				unsigned c : 1; // Carry Flag
				unsigned z : 1; // Zero Flag
				unsigned i : 1; // Interrupt Disable
				unsigned d : 1; // Decimal Mode (unused)
				unsigned b : 1; // Break Command
				unsigned r : 1; // Reserved Flag
				unsigned v : 1; // Overflow Flag
				unsigned n : 1; // Negative Flag
			} flags;
		} status;
	};
}

 #endif

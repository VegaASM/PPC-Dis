/*
    Barebones PPC Disassembler written in C++
    Copyright (C) 2023 Vega

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/gpl-3.0.txt
*/


#define FMT_HEADER_ONLY // Must be included if using fmt library in header mode
#include <iostream> // To cout and cin
#include <string> // To allow basic string usage
#include <cstdint> // To allow uint32_t usage
#include <fmt/format.h> // To allow special fmt library use
#include <fmt/color.h> // To allow colored terminal output
using namespace std; // To allow removal of "::"

// TODO put in sp and rtoc

//Declare cin arg that needs to be global
    uint32_t ppcinstruction;

// Primary AND Masks; read only
    const uint32_t addX = 0x7C000214;
    const uint32_t addcX = 0x7C000014;
    const uint32_t addeX = 0x7C000114;
    const uint32_t addi = 0x38000000;
    const uint32_t addic = 0x30000000;
    const uint32_t addicRC = 0x34000000;
    const uint32_t addis = 0x3C000000;
    const uint32_t addmeX = 0x7C0001D4;
    const uint32_t addzeX = 0x7C000194;
    const uint32_t andX = 0x7C000038;
    const uint32_t andcX = 0x7C000078;
    const uint32_t andiRC = 0x70000000;
    const uint32_t andisRC = 0x74000000;
    const uint32_t bX = 0x48000000;
    const uint32_t bcX = 0x40000000;
    const uint32_t bcctrX = 0x4C000420;
    const uint32_t bclrX = 0x4C000020;
    const uint32_t cmpw = 0x7C000000;
    const uint32_t cmpwi = 0x2C000000;
    const uint32_t cmplw = 0x7C000040;
    const uint32_t cmplwi = 0x28000000;
    const uint32_t cntlzwX = 0x7C000034;
    const uint32_t crand = 0x4C000202;
    const uint32_t crandc = 0x4C000102;
    const uint32_t creqv = 0x4C000242;
    const uint32_t crnand = 0x4C0001C2;
    const uint32_t crnor = 0x4C000042;
    const uint32_t cror = 0x4C000382;
    const uint32_t crorc = 0x4C000342;
    const uint32_t crxor = 0x4C000182;
    const uint32_t dcbf = 0x7C0000AC;
    const uint32_t dcbi = 0x7C0003AC;
    const uint32_t dcbst = 0x7C00006C;
    const uint32_t dcbt = 0x7C00022C;
    const uint32_t dcbtst = 0x7C0001EC;
    const uint32_t dcbz = 0x7C0007EC;
    const uint32_t dcbz_l = 0x100007EC;
    const uint32_t divwX = 0x7C0003D6;
    const uint32_t divwuX = 0x7C000396;
    const uint32_t eciwx = 0x7C00026C;
    const uint32_t ecowx = 0x7C00036C;
    const uint32_t eieio = 0x7C0006AC;
    const uint32_t eqvX = 0x7C000238;
    const uint32_t extsbX = 0x7C000774;
    const uint32_t extshX = 0x7C000734;
    const uint32_t fabsX = 0xFC000210;
    const uint32_t faddX = 0xFC00002A;
    const uint32_t faddsX = 0xEC00002A;
    const uint32_t fcmpo = 0xFC000040;
    const uint32_t fcmpu = 0xFC000000;
    const uint32_t fctiwX = 0xFC00001C;
    const uint32_t fctiwzX = 0xFC00001E;
    const uint32_t fdivX = 0xFC000024;
    const uint32_t fdivsX = 0xEC000024;
    const uint32_t fmaddX = 0xFC00003A;
    const uint32_t fmaddsX = 0xEC00003A;
    const uint32_t fmrX = 0xFC000090;
    const uint32_t fmsubX = 0xFC000038;
    const uint32_t fmsubsX = 0xEC000038;
    const uint32_t fmulX = 0xFC000032;
    const uint32_t fmulsX = 0xEC000032;
    const uint32_t fnabsX = 0xFC000110;
    const uint32_t fnegX = 0xFC000050;
    const uint32_t fnmaddX = 0xFC00003E;
    const uint32_t fnmaddsX = 0xEC00003E;
  const uint32_t fnmsubX = 0xFC00003C;
  const uint32_t fnmsubsX = 0xEC00003C;
  const uint32_t fresX = 0xEC000030;
  const uint32_t frspX = 0xFC000018;
  const uint32_t frsqrteX = 0xFC000034;
  const uint32_t fselX = 0xFC00002E;
  const uint32_t fsubX = 0xFC000028;
  const uint32_t fsubsX = 0xEC000028;
  const uint32_t icbi = 0x7C0007AC;
  const uint32_t isync = 0x4C00012C;
  const uint32_t lbz = 0x88000000;
  const uint32_t lbzu = 0x8C000000;
  const uint32_t lbzux = 0x7C0000EE;
  const uint32_t lbzx = 0x7C0000AE;
  const uint32_t lfd = 0xC8000000;
  const uint32_t lfdu = 0xCC000000;
  const uint32_t lfdux = 0x7C0004EE;
  const uint32_t lfdx = 0x7C0004AE;
  const uint32_t lfs = 0xC0000000;
  const uint32_t lfsu = 0xC4000000;
  const uint32_t lfsux = 0x7C00046E;
  const uint32_t lfsx = 0x7C00042E;
  const uint32_t lha = 0xA8000000;
  const uint32_t lhau = 0xAC000000;
  const uint32_t lhaux = 0x7C0002EE;
  const uint32_t lhax = 0x7C0002AE;
  const uint32_t lhbrx = 0x7C00062C;
  const uint32_t lhz = 0xA0000000;
  const uint32_t lhzu = 0xA4000000;
  const uint32_t lhzux = 0x7C00026E;
  const uint32_t lhzx = 0x7C00022E;
  const uint32_t lmw = 0xB8000000;
  const uint32_t lswi = 0x7C0004AA;
  const uint32_t lswx = 0x7C00042A;
  const uint32_t lwarx = 0x7C000028;
  const uint32_t lwbrx = 0x7C00042C;
  const uint32_t lwz = 0x80000000;
  const uint32_t lwzu = 0x84000000;
  const uint32_t lwzux = 0x7C00006E;
  const uint32_t lwzx = 0x7C00002E;
  const uint32_t mcrf = 0x4C000000;
  const uint32_t mcrfs = 0xFC000080;
  const uint32_t mcrxr = 0x7C000400;
  const uint32_t mfcr = 0x7C000026;
  const uint32_t mffsX = 0xFC00048E;
  const uint32_t mfmsr = 0x7C0000A6;
  const uint32_t mfspr = 0x7C0002A6;
  const uint32_t mfsr = 0x7C0004A6;
  const uint32_t mfsrin = 0x7C000526;
  const uint32_t mftb = 0x7C0002E6;
  const uint32_t mtcrf = 0x7C000120;
  const uint32_t mtfsb0X = 0xFC00008C;
  const uint32_t mtfsb1X = 0xFC00004C;
  const uint32_t mtfsfX = 0xFC00058E;
  const uint32_t mtfsfiX = 0xFC00010C;
  const uint32_t mtmsr = 0x7C000124;
  const uint32_t mtspr = 0x7C0003A6;
  const uint32_t mtsr = 0x7C0001A4;
  const uint32_t mtsrin = 0x7C0001E4;
  const uint32_t mulhwX = 0x7C000096;
  const uint32_t mulhwuX = 0x7C000016;
  const uint32_t mulli = 0x1C000000;
  const uint32_t mullwX = 0x7C0001D6;
  const uint32_t nandX = 0x7C0003B8;
  const uint32_t negX = 0x7C0000D0;
  const uint32_t norX = 0x7C0000F8;
  const uint32_t orX = 0x7C000378;
  const uint32_t orcX = 0x7C000338;
  const uint32_t ori = 0x60000000;
  const uint32_t oris = 0x64000000;
  const uint32_t psq_l = 0xE0000000;
  const uint32_t psq_lu = 0xE4000000;
  const uint32_t psq_lux = 0x1000004C;
  const uint32_t psq_lx = 0x1000000C;
  const uint32_t psq_st = 0xF0000000;
  const uint32_t psq_stu = 0xF4000000;
  const uint32_t psq_stux = 0x1000004E;
  const uint32_t psq_stx = 0x1000000E;
  const uint32_t ps_absX = 0x10000210;
  const uint32_t ps_addX = 0x1000002A;
  const uint32_t ps_cmpo0 = 0x10000040;
  const uint32_t ps_cmpo1 = 0x100000C0;
  const uint32_t ps_cmpu0 = 0x10000000;
  const uint32_t ps_cmpu1 = 0x10000080;
  const uint32_t ps_divX = 0x10000024;
  const uint32_t ps_maddX = 0x1000003A;
  const uint32_t ps_madds0X = 0x1000001C;
  const uint32_t ps_madds1X = 0x1000001E;
  const uint32_t ps_merge00X = 0x10000420;
  const uint32_t ps_merge01X = 0x10000460;
  const uint32_t ps_merge10X = 0x100004A0;
  const uint32_t ps_merge11X = 0x100004E0;
  const uint32_t ps_mrX = 0x10000090;
  const uint32_t ps_msubX = 0x10000038;
  const uint32_t ps_mulX = 0x10000032;
  const uint32_t ps_muls0X = 0x10000018;
  const uint32_t ps_muls1X = 0x1000001A;
  const uint32_t ps_nabsX = 0x10000110;
  const uint32_t ps_negX = 0x10000050;
  const uint32_t ps_nmaddX = 0x1000003E;
  const uint32_t ps_nmsubX = 0x1000003C;
  const uint32_t ps_resX = 0x10000030;
  const uint32_t ps_rsqrteX = 0x10000034;
  const uint32_t ps_selX = 0x1000002E;
  const uint32_t ps_subX = 0x10000028;
  const uint32_t ps_sum0X = 0x10000014;
  const uint32_t ps_sum1X = 0x10000016;
  const uint32_t rfi = 0x4C000064;
  const uint32_t rlwimiX = 0x50000000;
  const uint32_t rlwinmX = 0x54000000;
  const uint32_t rlwnmX = 0x5C000000;
  const uint32_t sc = 0x44000002;
  const uint32_t slwX = 0x7C000030;
  const uint32_t srawX = 0x7C000630;
  const uint32_t srawiX = 0x7C000670;
  const uint32_t srwX = 0x7C000430;
  const uint32_t stb = 0x98000000;
  const uint32_t stbu = 0x9C000000;
  const uint32_t stbux = 0x7C0001EE;
  const uint32_t stbx = 0x7C0001AE;
  const uint32_t stfd = 0xD8000000;
  const uint32_t stfdu = 0xDC000000;
  const uint32_t stfdux = 0x7C0005EE;
  const uint32_t stfdx = 0x7C0005AE;
  const uint32_t stfiwX = 0x7C0007AE;
  const uint32_t stfs = 0xD0000000;
  const uint32_t stfsu = 0xD4000000;
  const uint32_t stfsux = 0x7C00056E;
  const uint32_t stfsx = 0x7C00052E;
  const uint32_t sth = 0xB0000000;
  const uint32_t sthbrx = 0x7C00072C;
  const uint32_t sthu = 0xB4000000;
  const uint32_t sthux = 0x7C00036E;
  const uint32_t sthx = 0x7C00032E;
  const uint32_t stmw = 0xBC000000;
  const uint32_t stswi = 0x7C0005AA;
  const uint32_t stswx = 0x7C00052A;
  const uint32_t stw = 0x90000000;
  const uint32_t stwbrx = 0x7C00052C;
  const uint32_t stwcxRC = 0x7C00012D;
  const uint32_t stwu = 0x94000000;
  const uint32_t stwux = 0x7C00016E;
  const uint32_t stwx = 0x7C00012E;
  const uint32_t subX = 0x7C000050;
  const uint32_t subcX = 0x7C000010;
  const uint32_t subfeX = 0x7C000110;
  const uint32_t subfic = 0x20000000;
  const uint32_t subfmeX = 0x7C0001D0;
  const uint32_t subfzeX = 0x7C000190;
  const uint32_t sync = 0x7C0004AC;
  const uint32_t tlbie = 0x7C000264;
  const uint32_t tlbsync = 0x7C00046C;
  const uint32_t tw = 0x7C000008;
  const uint32_t twi = 0x0C000000;
  const uint32_t xorX = 0x7C000278;
  const uint32_t xori = 0x68000000;
  const uint32_t xoris = 0x6C000000;
  
// Declare Secondary AND Masks; read only
  const uint32_t check_cmps1 = 0x006007FF; //cmpw, fcmpu, ps_cmpu0
  const uint32_t check_cmps2 = 0x00600001; //cmplw, fcmpo, ps_cmpo0, ps_cmpo1, ps_cmpu1
  const uint32_t check_cache = 0x03E00001; //for all cache related instructions
  const uint32_t check_syncs = 0x03FFF801; //for eieio, rfi, and all sync-type instructions
  const uint32_t check_res_sqrt = 0x001F07C0; //for res and rsqrte type instructions
  const uint32_t check_mcrf = 0x0063FFFF; //mcrf
  const uint32_t check_mcrfs = 0x0063F801; //mcrfs
  const uint32_t check_mcrxr = 0x007FF801; //mcrxr
  const uint32_t check_cr_msr = 0x001FF801; //mfcr, mfmsr, mtmsr
  const uint32_t check_fs_fsb = 0x001FF800; //mffsX, mtfsb0X, mtfsb1X
  const uint32_t check_sr = 0x0010F801; //mfsr, mtsr
  const uint32_t check_srin = 0x001F0001; //mfsrin, mtsrin
  const uint32_t check_mtcrf = 0x00100801; //mtcrf
  const uint32_t check_mtfsfiX = 0x007F0800; //mtfsfiX
  const uint32_t check_sc = 0x03FFFFFD; //sc
  const uint32_t check_tlbie = 0x03FF0001; //tlbie
  
// Declare Secondary AND Masks that do not require whole word descrip, just separated here for personal preference

    const uint32_t check_nullrA = 0x001F0000; // For instructions with null rA or fA
    const uint32_t check_nullrB = 0x0000F800; // For instructions with null rB or fB
    const uint32_t check_nullfC = 0x000007C0; // For instructions with null fC
    const uint32_t check_cmpXwi = 0x00600000; // For cmpwi and cmplwi
    const uint32_t check_cr_ops = 0x00000001; // For CR ops
    const uint32_t check_mtfsfX = 0x02010000; // For mtfsfX
    const uint32_t check_b21 = 0x00000200; // For mulhwX and mulhwuX
    const uint32_t check_b31 = 0x00000001; // For non-CR ops, naming preference

// insbitsmask arg for 1st check func, 5 diff options
  const uint32_t ppcinstructb21_30 = 0xFC0007FE; // For instructions that basically have just rc
  const uint32_t ppcinstructb22_30 = 0xFC0003FE; // For instructions that basically have oe and rc
  const uint32_t ppcinstructb25_30 = 0xFC00007E; // For some psq instructions
  const uint32_t ppcinstructb26_30 = 0xFC00003E; // For some float and ps instructions
  const uint32_t ppcinstructb30 = 0xFC000002; // For sc
  const uint32_t ppcClrOpcode = 0xFC000000; // For instructions where every non-opcode bit is non-sticky
  
// First check func
int first_check (uint32_t insbitsmask, uint32_t primarymask) { 
    uint32_t insbits_anding_result = (ppcinstruction & insbitsmask);
    if (insbits_anding_result == primarymask) {
        return 0; //0 return = successful initial find
    } else {
        return 1;
    }
}

// Second check func; for instructions that require a secondary Mask Logical AND check
int second_check (uint32_t secondarymask) {
    uint32_t second_check_and_result = (secondarymask & ppcinstruction);
    if (second_check_and_result == 0) {
        return 0; //0 return = successful find
    } else {
        return 1;
    }
}

// Special check for lbzu, lbzux, lhau, lhaux, lhzu, lhzux, lwzu, and lwzux
int lXzu_lXzux_special_check () {
    uint32_t rD_lispc = ((ppcinstruction | 0U) >> 21) & 0x1FU;
    uint32_t rA_lispc = ((ppcinstruction | 0U) >> 16) & 0x1FU;
    if (rD_lispc == rA_lispc || rA_lispc == 0) {
        return 1;
    } else {
        return 0; //0 = proper load instruction
    }
}

// Special check for..
// lfdu, lfsu, psq_lu, psq_stu, stbu, stfdu, stfsu, sthu, stwu
// lfdux, lfsux, psq_lux, psq_stux, stbux, stfdux, stfsux, sthux, stwux
int float_ps_update_and_store_update_special_check () {
    uint32_t rA_fpsspc = ((ppcinstruction | 0U) >> 16) & 0x1FU;
    if (rA_fpsspc == 0) {
        return 1;
    } else {
        return 0;  //0 = proper load instruction
    }
}

int lmw_specialcheck () {
    uint32_t rD_lmw = ((ppcinstruction | 0U) >> 21) & 0x1FU;
    uint32_t rA_lmw = ((ppcinstruction | 0U) >> 16) & 0x1FU;
    if (rA_lmw >= rD_lmw) {
        return 1;
    } else {
        return 0; //0 = proper lmw instruction
    }
}

//*NOTE* check this in depth later, seems ok at 1st glance
int lswi_specialcheck () {
    uint32_t rD_lswi = ((ppcinstruction | 0U) >> 21) & 0x1FU;
    uint32_t rA_lswi = ((ppcinstruction | 0U) >> 16) & 0x1FU;
    uint32_t NB_lswi = ((ppcinstruction | 0U) >> 11) & 0x1FU;
    uint32_t NBspill_limit = ((rA_lswi - rD_lswi) * 4U);
    if (NBspill_limit > NB_lswi) {
        return 1;
    } else {
        return 0; //0 = proper lswi instruction
    }
}

//Func to get standard 16-bit SIMM (will be 32-bit if negative)
uint32_t getSIMM () {
    uint32_t SIMM_forhere = (ppcinstruction & 0x0000FFFFU);
    if (SIMM_forhere > 0x00007FFFU) {
        return SIMM_forhere | 0xFFFF0000U;
    } else {
        return SIMM_forhere;
    }
}

//Func to get Unconditional Branchs' 26-bit SIMM that defaults to 4-byte alignment
uint32_t getSIMM26 () {
    uint32_t SIMM26temp = (ppcinstruction & 0x03FFFFFCU);
    if (SIMM26temp > 0x01FFFFFCU) {
        return SIMM26temp | 0xFC000000U;
    } else {
        return SIMM26temp;
    }
}

//Func to get Conditional Branchs' 16-bit SIMM that defaults to 4-byte alignment
uint32_t getSIMMcondb () {
    uint32_t SIMMcondb_temp = (ppcinstruction & 0x0000FFFCU);
    if (SIMMcondb_temp > 0x00007FFCU) {
        return SIMMcondb_temp | 0xFFFF0000U;
    } else {
        return SIMMcondb_temp;
    }
}

//Func to get PS instructions' signed IMM
uint32_t getSIMMps () {
    uint32_t SIMMps_temp = (ppcinstruction & 0x00000FFFU);
    if (SIMMps_temp > 0x000007FFU) {
        return SIMMps_temp | 0xFFFFF000U;
    } else {
        return SIMMps_temp;
    }
} 

//Start disassemble func
string disassemble (uint32_t ppcinstruction) {
    
    // Declare all integer values for all right justified instruction inputs
  uint32_t rD = ((ppcinstruction | 0U) >> 21) & 0x1FU;
  uint32_t rS = rD;
  //uint32_t BO = rD; // "rD" in conditional branches, not needed in this source as BOw/hint vs BOw/ohint is split into two variables
  uint32_t crbD = rD;
  uint32_t fD = rD;
  uint32_t fS = rD;
  uint32_t TO = rD; // For tw and twi
  
  uint32_t crfD = ((ppcinstruction | 0U) >> 23) & 0x7U; // For comparison instructions and mcrf+mcrfs
  uint32_t crfS = ((ppcinstruction | 0U) >> 18) & 0x7U; // For mcrf, mcrfs
  
  uint32_t rA = ((ppcinstruction | 0U) >> 16) & 0x1FU;
  uint32_t BI = rA;
  uint32_t crbA = rA;
  uint32_t fA = rA;
  
  uint32_t rB = ((ppcinstruction | 0U) >> 11) & 0x1FU;
  uint32_t fB = rB;
  uint32_t NB = rB; // For lswi and stswi
  uint32_t crbB = rB;
  uint32_t SH = rB;
  
  uint32_t fC = ((ppcinstruction | 0U) >> 6) & 0x1FU;
  uint32_t MB = fC;
  
  uint32_t ME = ((ppcinstruction | 0U) >> 1) & 0x1FU;
  
 
  uint32_t UIMM = (ppcinstruction & 0x0000FFFFU);
  uint32_t SIMM = getSIMM();
  uint32_t d = SIMM;
  uint32_t LI = getSIMM26(); // Unconditional branch signed jump amount
  uint32_t BD = getSIMMcondb();  // Conditional branch signed jump amount
  uint32_t ps_SIMM = getSIMMps(); // psq non indexed loads and stores
  uint32_t IMM = ((ppcinstruction | 0U) >> 12) & 0xFU; // For mtfsfiX
  
  uint32_t SPR_lowerfield = ((ppcinstruction | 0U) >> 16) & 0x1FU; // Breaking this apart so others can see how the SPR fields fit within the instruction
  uint32_t SPR_upperfield = ((ppcinstruction | 0U) >> 6) & 0x3E0U; // Breaking this apart so others can see how the SPR fields fit within the instruction
  uint32_t SPR = (SPR_upperfield | SPR_lowerfield);
  uint32_t TBR = SPR; //For mftb
  
  uint32_t SR = ((ppcinstruction | 0U) >> 16) & 0xFU; // For mfsr, mtsr
  uint32_t CRM = ((ppcinstruction | 0U) >> 12) & 0xFFU; // For mtcrf
  uint32_t FM = ((ppcinstruction | 0U) >> 17) & 0xFFU; // For mtfsfX
  
  uint32_t W = ((ppcinstruction | 0U) >> 15) & 0x1U; // For non indexed ps load and stores
  uint32_t I = ((ppcinstruction | 0U) >> 12) & 0x7U; // For non indexed ps load and stores
  uint32_t Wx = ((ppcinstruction | 0U) >> 10) & 0x1U; // For indexed ps load and stores
  uint32_t Ix = ((ppcinstruction | 0U) >> 7) & 0x7U; // For indexed ps load and stores
 
  uint32_t clrrwi_n = (31U - ME); // For clrrwi & clrrwi. simplified mnemonics, the n in "clrrwi(.) rA, rS, n" is (31 minus ME)
  
// Store results of various branch stuff we need for the many branch simplified mnemonics
    uint32_t BO_nohint = ((ppcinstruction | 0U) >> 22) & 0xFU;
    uint32_t BO_onlyhint = ((ppcinstruction | 0U) >> 21) & 0x1U; //For any conditional branch that is *NOT* bcX
    //bcX's branch hint is flipped if BD is negative value
    //All we need to do is simply XOR the Hint Bit with BD's Sign bit and voila! Humble brag :)
    uint32_t BD_signbit = ((ppcinstruction | 0U) >> 15) & 0x1U;
    uint32_t bcX_BOhint = (BO_onlyhint ^ BD_signbit);
    uint32_t crfD_manual = ((ppcinstruction | 0U) >> 18) & 0x7U; // BI divided by 4, then round down. We can do this in a single "instruction". This will give us the crf Number for BI
    uint32_t findcrbtype = ((ppcinstruction | 0U) >> 16) & 0x3U; // Shift BI normally by 16 to right, but only look at last 2 bits, this will give us lt vs gt vs eq vs so crB type

// Store results of ANDing for aa, oe, lk, and rc values
    uint32_t oerc = (ppcinstruction & 0x401U);
    uint32_t aalk = (ppcinstruction & 0x3U);
    uint32_t rc = (ppcinstruction & 0x1U);
    uint32_t lk = (ppcinstruction & 0x1U);

// Declare string for final cout, declare string for illegal instruction
    std::string illegal = fmt::format("Illegal instruction 0x{:08X}", ppcinstruction);

// Declare strings for standard instruction mnemonics
  string add_str = fmt::format("add r{}, r{}, r{}", rD, rA, rB);
  string addRC_str = fmt::format("add. r{}, r{}, r{}", rD, rA, rB);
  string addOE_str = fmt::format("addo r{}, r{}, r{}", rD, rA, rB);
  string addOERC_str = fmt::format("addo. r{}, r{}, r{}", rD, rA, rB);
  string addc_str = fmt::format("addc r{}, r{}, r{}", rD, rA, rB);
  string addcRC_str = fmt::format("addc. r{}, r{}, r{}", rD, rA, rB);
  string addcOE_str = fmt::format("addco r{}, r{}, r{}", rD, rA, rB);
  string addcOERC_str = fmt::format("addco. r{}, r{}, r{}", rD, rA, rB);
  string adde_str = fmt::format("adde r{}, r{}, r{}", rD, rA, rB);
  string addeRC_str = fmt::format("adde. r{}, r{}, r{}", rD, rA, rB);
  string addeOE_str = fmt::format("addeo r{}, r{}, r{}", rD, rA, rB);
  string addeOERC_str = fmt::format("addeo. r{}, r{}, r{}", rD, rA, rB);
  string addi_str = fmt::format("addi r{}, r{}, 0x{:X}", rD, rA, SIMM);
  string addic_str = fmt::format("addic r{}, r{}, 0x{:X}", rD, rA, SIMM);
  string addicRC_str = fmt::format("addic. r{}, r{}, 0x{:X}", rD, rA, SIMM);
  string addis_str = fmt::format("addis r{}, r{}, 0x{:X}", rD, rA, SIMM);
  string addme_str = fmt::format("addme r{}, r{}", rD, rA);
  string addmeRC_str = fmt::format("addme. r{}, r{}", rD, rA);
  string addmeOE_str = fmt::format("addmeo r{}, r{}", rD, rA);
  string addmeOERC_str = fmt::format("addmeo. r{}, r{}", rD, rA);
  string addze_str = fmt::format("addze r{}, r{}", rD, rA);
  string addzeRC_str = fmt::format("addze. r{}, r{}", rD, rA);
  string addzeOE_str = fmt::format("addzeo r{}, r{}", rD, rA);
  string addzeOERC_str = fmt::format("addzeo. r{}, r{}", rD, rA);
  string and_str = fmt::format("and r{}, r{}, r{}", rA, rS, rB);
  string andRC_str = fmt::format("and. r{}, r{}, r{}", rA, rS, rB);
  string andc_str = fmt::format("andc r{}, r{}, r{}", rA, rS, rB);
  string andcRC_str = fmt::format("andc. r{}, r{}, r{}", rA, rS, rB);
  string andiRC_str = fmt::format("andi. r{}, r{}, 0x{:04X}", rA, rS, UIMM);
  string andisRC_str = fmt::format("andis. r{}, r{}, 0x{:04X}", rA, rS, UIMM);
  string b_str = fmt::format("b 0x{:X}", LI);
  string ba_str = fmt::format("ba 0x{:X}", LI);
  string bl_str = fmt::format("bl 0x{:X}", LI);
  string bla_str = fmt::format("bla 0x{:X}", LI);
  //Branch Conditional Standard Mnemonics not needed
  //string bc_str = fmt::format("bc {}, {}, 0x{:X}", BO, BI, BD);
  //string bca_str = fmt::format("bca {}, {}, 0x{:X}", BO, BI, BD);
  //string bcl_str = fmt::format("bcl {}, {}, 0x{:X}", BO, BI, BD);
  //string bcla_str = fmt::format("bcla {}, {}, 0x{:X}", BO, BI, BD);
  //string bcctr_str = fmt::format("bcctr {}, {}", BO, BI);
  //string bcctrl_str = fmt::format("bcctrl {}, {}", BO, BI);
  //string bclr_str = fmt::format("bclr {}, {}", BO, BI);
  //string bclrl_str = fmt::format("bclrl {}, {}", BO, BI);
  //*NOTE* cmp, cmpi cmpl, and cmpli not used as L has to be 0 because Broadway is 32-bit
  string cmpwcr0_str = fmt::format("cmpw r{}, r{}", rA, rB);
  string cmpw_str = fmt::format("cmpw cr{}, r{}, r{}", crfD, rA, rB);
  string cmpwicr0_str = fmt::format("cmpwi r{}, 0x{:X}", rA, SIMM);
  string cmpwi_str = fmt::format("cmpwi cr{}, r{}, 0x{:X}", crfD, rA, SIMM);
  string cmplwcr0_str = fmt::format("cmplw r{}, r{}", rA, rB);
  string cmplw_str = fmt::format("cmplw cr{}, r{}, r{}", crfD, rA, rB);
  string cmplwicr0_str = fmt::format("cmplwi r{}, 0x{:04X}", rA, UIMM);
  string cmplwi_str = fmt::format("cmplwi cr{}, r{}, 0x{:04X}", crfD, rA, UIMM);
  string cntlzw_str = fmt::format("cntlzw r{}, r{}", rA, rS);
  string cntlzwRC_str = fmt::format("cntlzw. r{}, r{}", rA, rS);
  string crand_str = fmt::format("crand {}, {}, {}", crbD, crbA, crbB);
  string crandc_str = fmt::format("crandc {}, {}, {}", crbD, crbA, crbB);
  string creqv_str = fmt::format("creqv {}, {}, {}", crbD, crbA, crbB);
  string crnand_str = fmt::format("crnand {}, {}, {}", crbD, crbA, crbB);
  string crnor_str = fmt::format("crnor {}, {}, {}", crbD, crbA, crbB);
  string cror_str = fmt::format("cror {}, {}, {}", crbD, crbA, crbB);
  string crorc_str = fmt::format("crorc {}, {}, {}", crbD, crbA, crbB);
  string crxor_str = fmt::format("crxor {}, {}, {}", crbD, crbA, crbB);
  string dcbf_str = fmt::format("dcbf r{}, r{}", rA, rB);
  string dcbi_str = fmt::format("dcbi r{}, r{}", rA, rB);
  string dcbst_str = fmt::format("dcbst r{}, r{}", rA, rB);
  string dcbt_str = fmt::format("dcbt r{}, r{}", rA, rB);
  string dcbtst_str = fmt::format("dcbtst r{}, r{}", rA, rB);
  string dcbz_str = fmt::format("dcbz r{}, r{}", rA, rB);
  string dcbz_l_str = fmt::format("dcbz_l r{}, r{}", rA, rB);
  string divw_str = fmt::format("divw r{}, r{}, r{}", rD, rA, rB);
  string divwRC_str = fmt::format("divw. r{}, r{}, r{}", rD, rA, rB);
  string divwOE_str = fmt::format("divwo r{}, r{}, r{}", rD, rA, rB);
  string divwOERC_str = fmt::format("divwo. r{}, r{}, r{}", rD, rA, rB);
  string divwu_str = fmt::format("divwu r{}, r{}, r{}", rD, rA, rB);
  string divwuRC_str = fmt::format("divwu. r{}, r{}, r{}", rD, rA, rB);
  string divwuOE_str = fmt::format("divwuo r{}, r{}, r{}", rD, rA, rB);
  string divwuOERC_str = fmt::format("divwuo. r{}, r{}, r{}", rD, rA, rB);
  string eciwx_str = fmt::format("eciwx r{}, r{}, r{}", rD, rA, rB);
  string ecowx_str = fmt::format("ecowx r{}, r{}, r{}", rS, rA, rB);
  string eieio_str = "eieio";
  string eqv_str = fmt::format("eqv r{}, r{}, r{}", rA, rS, rB);
  string eqvRC_str = fmt::format("eqv. r{}, r{}, r{}", rA, rS, rB);
  string extsb_str = fmt::format("extsb r{}, r{}", rA, rS);
  string extsbRC_str = fmt::format("extsb. r{}, r{}", rA, rS);
  string extsh_str = fmt::format("extsh r{}, r{}", rA, rS);
  string extshRC_str = fmt::format("extsh. r{}, r{}", rA, rS);
  string fabs_str = fmt::format("fabs f{}, f{}", fD, fB);
  string fabsRC_str = fmt::format("fabs. f{}, f{}", fD, fB);
  string fadd_str = fmt::format("fadd f{}, f{}, f{}", fD, fA, fB);
  string faddRC_str = fmt::format("fadd. f{}, f{}, f{}", fD, fA, fB);
  string fadds_str = fmt::format("fadds f{}, f{}, f{}", fD, fA, fB);
  string faddsRC_str = fmt::format("fadds. f{}, f{}, f{}", fD, fA, fB);
  string fcmpo_str = fmt::format("fcmpo cr{}, f{}, f{}", crfD, fA, fB);
  string fcmpu_str = fmt::format("fcmpu cr{}, f{}, f{}", crfD, fA, fB);
  string fctiw_str = fmt::format("fctiw f{}, f{}", fD, fB);
  string fctiwRC_str = fmt::format("fctiw. f{}, f{}", fD, fB);
  string fctiwz_str = fmt::format("fctiwz f{}, f{}", fD, fB);
  string fctiwzRC_str = fmt::format("fcitwz. f{}, f{}", fD, fB);
  string fdiv_str = fmt::format("fdiv f{}, f{}, f{}", fD, fA, fB);
  string fdivRC_str = fmt::format("fdiv. f{}, f{}, f{}", fD, fA, fB);
  string fdivs_str = fmt::format("fdivs f{}, f{}, f{}", fD, fA, fB);
  string fdivsRC_str = fmt::format("fdivs. f{}, f{}, f{}", fD, fA, fB);
  string fmadd_str = fmt::format("fmadd f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fmaddRC_str = fmt::format("fmadd. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fmadds_str = fmt::format("fmadds f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fmaddsRC_str = fmt::format("fmadds. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fmr_str = fmt::format("fmr f{}, f{}", fD, fB);
  string fmrRC_str = fmt::format("fmr. f{}, f{}", fD, fB);
  string fmsub_str = fmt::format("fmsub f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fmsubRC_str = fmt::format("fmsub. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fmsubs_str = fmt::format("fmsubs f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fmsubsRC_str = fmt::format("fmsubs. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fmul_str = fmt::format("fmul f{}, f{}, f{}", fD, fA, fC);
  string fmulRC_str = fmt::format("fmul. f{}, f{}, f{}", fD, fA, fC);
  string fmuls_str = fmt::format("fmuls f{}, f{}, f{}", fD, fA, fC);
  string fmulsRC_str = fmt::format("fmuls. f{}, f{}, f{}", fD, fA, fC);
  string fnabs_str = fmt::format("fnabs f{}, f{}", fD, fB);
  string fnabsRC_str = fmt::format("fnabs. f{}, f{}", fD, fB);
  string fneg_str = fmt::format("fneg f{}, f{}", fD, fB);
  string fnegRC_str = fmt::format("fneg. f{}, f{}", fD, fB);
  string fnmadd_str = fmt::format("fnmadd f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fnmaddRC_str = fmt::format("fnmadd. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fnmadds_str = fmt::format("fnmadds f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fnmaddsRC_str = fmt::format("fnmadds. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fnmsub_str = fmt::format("fnmsub f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fnmsubRC_str = fmt::format("fnmsub. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fnmsubs_str = fmt::format("fnmsubs f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fnmsubsRC_str = fmt::format("fnmsubs. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fres_str = fmt::format("fres f{}, f{}", fD, fB);
  string fresRC_str = fmt::format("fres. f{}, f{}", fD, fB);
  string frsp_str = fmt::format("frsp f{}, f{}", fD, fB);
  string frspRC_str = fmt::format("frsp. f{}, f{}", fD, fB);
  string frsqrte_str = fmt::format("frsqrte f{}, f{}", fD, fB);
  string frsqrteRC_str = fmt::format("frsqrte. f{}, f{}", fD, fB);
  string fsel_str = fmt::format("fsel f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fselRC_str = fmt::format("fsel. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string fsub_str = fmt::format("fsub f{}, f{}, f{}", fD, fA, fB);
  string fsubRC_str = fmt::format("fsub. f{}, f{}, f{}", fD, fA, fB);
  string fsubs_str = fmt::format("fsubs f{}, f{}, f{}", fD, fA, fB);
  string fsubsRC_str = fmt::format("fsubs. f{}, f{}, f{}", fD, fA, fB);
  string icbi_str = fmt::format("icbi r{}, r{}", rA, rB);
  string isync_str = "isync";
  string lbz_str = fmt::format("lbz r{}, 0x{:X}, (r{})", rD, d, rA);
  string lbzu_str = fmt::format("lbzu r{}, 0x{:X}, (r{})", rD, d, rA);
  string lbzux_str = fmt::format("lbzux r{}, r{}, r{}", rD, rA, rB);
  string lbzx_str = fmt::format("lbzx r{}, r{}, r{}", rD, rA, rB);
  string lfd_str = fmt::format("lfd f{}, 0x{:X}, (r{})", fD, d, rA);
  string lfdu_str = fmt::format("lfdu f{}, 0x{:X}, (r{})", fD, d, rA);
  string lfdux_str = fmt::format("lfdux f{}, r{}, r{}", fD, rA, rB);
  string lfdx_str = fmt::format("lfdx f{}, r{}, r{}", fD, rA, rB);
  string lfs_str = fmt::format("lfs f{}, 0x{:X}, (r{})", fD, d, rA);
  string lfsu_str = fmt::format("lfsu f{}, 0x{:X}, (r{})", fD, d, rA);
  string lfsux_str = fmt::format("lfsux f{}, r{}, r{}", fD, rA, rB);
  string lfsx_str = fmt::format("lfsx f{}, r{}, r{}", fD, rA, rB);
  string lha_str = fmt::format("lha r{}, 0x{:X}, (r{})", rD, d, rA);
  string lhau_str = fmt::format("lhau r{}, 0x{:X}, (r{})", rD, d, rA);
  string lhaux_str = fmt::format("lhaux r{}, r{}, r{}", rD, rA, rB);
  string lhax_str = fmt::format("lhax r{}, r{}, r{}", rD, rA, rB);
  string lhbrx_str = fmt::format("lhbrx r{}, r{}, r{}", rD, rA, rB);
  string lhz_str = fmt::format("lhz r{}, 0x{:X}, (r{})", rD, d, rA);
  string lhzu_str = fmt::format("lhzu r{}, 0x{:X}, (r{})", rD, d, rA);
  string lhzux_str = fmt::format("lhzux r{}, r{}, r{}", rD, rA, rB);
  string lhzx_str = fmt::format("lhzx r{}, r{}, r{}", rD, rA, rB);
  string lmw_str = fmt::format("lmw r{}, 0x{:X}, (r{})", rD, d, rA);
  string lswi_str = fmt::format("lswi r{}, r{}, {}", rD, rA, NB);
  string lswx_str = fmt::format("lswx r{}, r{}, r{}", rD, rA, rB);
  string lwarx_str = fmt::format("lwarx r{}, r{}, r{}", rD, rA, rB);
  string lwbrx_str = fmt::format("lwbrx r{}, r{}, r{}", rD, rA, rB);
  string lwz_str = fmt::format("lwz r{}, 0x{:X}, (r{})", rD, d, rA);
  string lwzu_str = fmt::format("lwzu r{}, 0x{:X}, (r{})", rD, d, rA);
  string lwzux_str = fmt::format("lwzux r{}, r{}, r{}", rD, rA, rB);
  string lwzx_str = fmt::format("lwzx r{}, r{}, r{}", rD, rA, rB);
  string mcrf_str = fmt::format("mcrf cr{}, cr{}", crfD, crfS);
  string mcrfs_str = fmt::format("mcrfs cr{}, cr{}", crfD, crfS);
  string mcrxr_str = fmt::format("mcrxr cr{}", crfD);
  string mfcr_str = fmt::format("mfcr r{}", rD);
  string mffs_str = fmt::format("mffs f{}", fD);
  string mffsRC_str = fmt::format("mffs. f{}", fD);
  string mfmsr_str = fmt::format("mfmsr r{}", rD);
  //string mfspr_str = fmt::format("mfspr r{}, {}", rD, SPR); //*NOTE* not needed
  string mfsr_str = fmt::format("mfsr r{}, {}", rD, SR);
  string mfsrin_str = fmt::format("mfsrin r{}, r{}", rD, rB);
  //string mftb = fmt::format("mftb r{}, {}", rD, SPR); *NOTE* not needed
  string mtcrf_str = fmt::format("mtcrf 0x{:02X}, r{}", CRM, rS);
  string mtfsb0_str = fmt::format("mtfsb0 {}", crbD);
  string mtfsb0RC_str = fmt::format("mtfsb0. {}", crbD);
  string mtfsb1_str = fmt::format("mtfsb1 {}", crbD);
  string mtfsb1RC_str = fmt::format("mtfsb1. {}", crbD);
  string mtfsf_str = fmt::format("mtfsf 0x{:02X}, f{}", FM, fB);
  string mtfsfRC_str = fmt::format("mtfsf. 0x{:02X}, f{}", FM, fB);
  string mtfsfi_str = fmt::format("mtfsfi cr{}, 0x{:01X}", crfD, IMM);
  string mtfsfiRC_str = fmt::format("mtfsfi. cr{}, 0x{:01X}", crfD, IMM);
  string mtmsr_str = fmt::format("mtmsr r{}", rS);
  //string mtspr_str = fmt::format("mtspr {}, r{}", SPR, rS); //*NOTE* not needed
  string mtsr_str = fmt::format("mtsr {}, r{}", SR, rS);
  string mtsrin_str = fmt::format("mtsrin r{}, r{}", rS, rB);
  string mulhw_str = fmt::format("mulhw r{}, r{}, r{}", rD, rA, rB);
  string mulhwRC_str = fmt::format("mulhw. r{}, r{}, r{}", rD, rA, rB);
  string mulhwu_str = fmt::format("mulhwu r{}, r{}, r{}", rD, rA, rB);
  string mulhwuRC_str = fmt::format("mulhwu. r{}, r{}, r{}", rD, rA, rB);
  string mulli_str = fmt::format("mulli r{}, r{}, 0x{:X}", rD, rA, SIMM);
  string mullw_str = fmt::format("mullw r{}, r{}, r{}", rD, rA, rB);
  string mullwRC_str = fmt::format("mullw. r{}, r{}, r{}", rD, rA, rB);
  string mullwOE_str = fmt::format("mullwo r{}, r{}, r{}", rD, rA, rB);
  string mullwOERC_str = fmt::format("mullwo. r{}, r{}, r{}", rD, rA, rB);
  string nand_str = fmt::format("nand r{}, r{}, r{}", rA, rS, rB);
  string nandRC_str = fmt::format("nand. r{}, r{}, r{}", rA, rS, rB);
  string neg_str = fmt::format("neg r{}, r{}", rD, rA);
  string negRC_str = fmt::format("neg. r{}, r{}", rD, rA);
  string negOE_str = fmt::format("nego r{}, r{}", rD, rA);
  string negOERC_str = fmt::format("nego. r{}, r{}", rD, rA);
  string nor_str = fmt::format("nor r{}, r{}, r{}", rA, rS, rB);
  string norRC_str = fmt::format("nor. r{}, r{}, r{}", rA, rS, rB);
  string or_str = fmt::format("or r{}, r{}, r{}", rA, rS, rB);
  string orRC_str = fmt::format("or. r{}, r{}, r{}", rA, rS, rB);
  string orc_str = fmt::format("orc r{}, r{}, r{}", rA, rS, rB);
  string orcRC_str = fmt::format("orc. r{}, r{}, r{}", rA, rS, rB);
  string ori_str = fmt::format("ori r{}, r{}, 0x{:04X}", rA, rS, UIMM);
  string oris_str = fmt::format("oris r{}, r{}, 0x{:04X}", rA, rS, UIMM);
  string psq_l_str = fmt::format("psq_l f{}, 0x{:X}, (r{}), {}, {}", fD, ps_SIMM, rA, W, I);
  string psq_lu_str = fmt::format("psq_lu f{}, 0x{:X}, (r{}), {}, {}", fD, ps_SIMM, rA, W, I);
  string psq_lux_str = fmt::format("psq_lux f{}, r{}, r{}, {}, {}", fD, rA, rB, Wx, Ix);
  string psq_lx_str = fmt::format("psq_lx f{}, r{}, r{}, {}, {}", fD, rA, rB, Wx, Ix);
  string psq_st_str = fmt::format("psq_st f{}, 0x{:X}, (r{}), {}, {}", fS, ps_SIMM, rA, W, I);
  string psq_stu_str = fmt::format("psq_stu f{}, 0x{:X}, (r{}), {}, {}", fS, ps_SIMM, rA, W, I);
  string psq_stux_str = fmt::format("psq_stux f{}, r{}, r{}, {}, {}", fS, rA, rB, Wx, Ix);
  string psq_stx_str = fmt::format("psq_stx f{}, r{}, r{}, {}, {}", fS, rA, rB, Wx, Ix);
  string ps_abs_str = fmt::format("ps_abs f{}, f{}", fD, fB);
  string ps_absRC_str = fmt::format("ps_abs. f{}, f{}", fD, fB);
  string ps_add_str = fmt::format("ps_add f{}, f{}, f{}", fD, fA, fB);
  string ps_addRC_str = fmt::format("ps_add. f{}, f{}, f{}", fD, fA, fB);
  string ps_cmpo0_str = fmt::format("ps_cmpo0 cr{}, f{}, f{}", crfD, fA, fB);
  string ps_cmpo1_str = fmt::format("ps_cmpo1 cr{}, f{}, f{}", crfD, fA, fB);
  string ps_cmpu0_str = fmt::format("ps_cmpu0 cr{}, f{}, f{}", crfD, fA, fB);
  string ps_cmpu1_str = fmt::format("ps_cmpu1 cr{}, f{}, f{}", crfD, fA, fB);
  string ps_div_str = fmt::format("ps_div f{}, f{}, f{}", fD, fA, fB);
  string ps_divRC_str = fmt::format("ps_div. f{}, f{}, f{}", fD, fA, fB);
  string ps_madd_str = fmt::format("ps_madd f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_maddRC_str = fmt::format("ps_madd. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_madds0_str = fmt::format("ps_madds0 f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_madds0RC_str = fmt::format("ps_madds0. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_madds1_str = fmt::format("ps_madds1 f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_madds1RC_str = fmt::format("ps_madds1. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_merge00_str = fmt::format("ps_merge00 f{}, f{}, f{}", fD, fA, fB);
  string ps_merge00RC_str = fmt::format("ps_merge00. f{}, f{}, f{}", fD, fA, fB);
  string ps_merge01_str = fmt::format("ps_merge01 f{}, f{}, f{}", fD, fA, fB);
  string ps_merge01RC_str = fmt::format("ps_merge01. f{}, f{}, f{}", fD, fA, fB);
  string ps_merge10_str = fmt::format("ps_merge10 f{}, f{}, f{}", fD, fA, fB);
  string ps_merge10RC_str = fmt::format("ps_merge10. f{}, f{}, f{}", fD, fA, fB);
  string ps_merge11_str = fmt::format("ps_merge11 f{}, f{}, f{}", fD, fA, fB);
  string ps_merge11RC_str = fmt::format("ps_merge11. f{}, f{}, f{}", fD, fA, fB);
  string ps_mr_str = fmt::format("ps_mr f{}, f{}", fD, fB);
  string ps_mrRC_str = fmt::format("ps_mr. f{}, f{}", fD, fB);
  string ps_msub_str = fmt::format("ps_msub f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_msubRC_str = fmt::format("ps_msub. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_mul_str = fmt::format("ps_mul f{}, f{}, f{}", fD, fA, fC);
  string ps_mulRC_str = fmt::format("ps_mul. f{}, f{}, f{}", fD, fA, fC);
  string ps_muls0_str = fmt::format("ps_muls0 f{}, f{}, f{}", fD, fA, fC);
  string ps_muls0RC_str = fmt::format("ps_muls0. f{}, f{}, f{}", fD, fA, fC);
  string ps_muls1_str = fmt::format("ps_muls1 f{}, f{}, f{}", fD, fA, fC);
  string ps_muls1RC_str = fmt::format("ps_muls1. f{}, f{}, f{}", fD, fA, fC);
  string ps_nabs_str = fmt::format("ps_nabs f{}, f{}", fD, fB);
  string ps_nabsRC_str = fmt::format("ps_nabs. f{}, f{}", fD, fB);
  string ps_neg_str = fmt::format("ps_neg f{}, f{}", fD, fB);
  string ps_negRC_str = fmt::format("ps_neg. f{}, f{}", fD, fB);
  string ps_nmadd_str = fmt::format("ps_nmadd f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_nmaddRC_str = fmt::format("ps_nmadd. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_nmsub_str = fmt::format("ps_nmsub f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_nmsubRC_str = fmt::format("ps_nmsub. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_res_str = fmt::format("ps_res f{}, f{}", fD, fB);
  string ps_resRC_str = fmt::format("ps_res. f{}, f{}", fD, fB);
  string ps_rsqrte_str = fmt::format("ps_rsqrte f{}, f{}", fD, fB);
  string ps_rsqrteRC_str = fmt::format("ps_rsqrte. f{}, f{}", fD, fB);
  string ps_sel_str = fmt::format("ps_sel f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_selRC_str = fmt::format("ps_sel. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_sub_str = fmt::format("ps_sub f{}, f{}, f{}", fD, fA, fB);
  string ps_subRC_str = fmt::format("ps_sub. f{}, f{}, f{}", fD, fA, fB);
  string ps_sum0_str = fmt::format("ps_sum0 f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_sum0RC_str = fmt::format("ps_sum0. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_sum1_str = fmt::format("ps_sum1 f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string ps_sum1RC_str = fmt::format("ps_sum1. f{}, f{}, f{}, f{}", fD, fA, fC, fB);
  string rfi_str = "rfi";
  string rlwimi_str = fmt::format("rlwimi r{}, r{}, {}, {}, {}", rA, rS, SH, MB, ME);
  string rlwimiRC_str = fmt::format("rlwimi. r{}, r{}, {}, {}, {}", rA, rS, SH, MB, ME);
  string rlwinm_str = fmt::format("rlwinm r{}, r{}, {}, {}, {}", rA, rS, SH, MB, ME);
  string rlwinmRC_str = fmt::format("rlwinm. r{}, r{}, {}, {}, {}", rA, rS, SH, MB, ME);
  string rlwnm_str = fmt::format("rlwnm r{}, r{}, r{}, {}, {}", rA, rS, rB, MB, ME);
  string rlwnmRC_str = fmt::format("rlwnm. r{}, r{}, r{}, {}, {}", rA, rS, rB, MB, ME);
  string sc_str = "sc";
  string slw_str = fmt::format("slw r{}, r{}, r{}", rA, rS, rB);
  string slwRC_str = fmt::format("slw. r{}, r{}, r{}", rA, rS, rB);
  string sraw_str = fmt::format("sraw r{}, r{}, r{}", rA, rS, rB);
  string srawRC_str = fmt::format("sraw. r{}, r{}, r{}", rA, rS, rB);
  string srawi_str = fmt::format("srawi r{}, r{}, {}", rA, rS, SH);
  string srawiRC_str = fmt::format("srawi. r{}, r{}, {}", rA, rS, SH);
  string srw_str = fmt::format("srw r{}, r{}, r{}", rA, rS, rB);
  string srwRC_str = fmt::format("srw. r{}, r{}, r{}", rA, rS, rB);
  string stb_str = fmt::format("stb r{}, 0x{:X}, (r{})", rS, d, rA);
  string stbu_str = fmt::format("stbu r{}, 0x{:X}, (r{})", rS, d, rA);
  string stbux_str = fmt::format("stbux r{}, r{}, r{}", rS, rA, rB);
  string stbx_str = fmt::format("stbx r{}, r{}, r{}", rS, rA, rB);
  string stfd_str = fmt::format("stfd f{}, 0x{:X}, (r{})", fS, d, rA);
  string stfdu_str = fmt::format("stfdu f{}, 0x{:X}, (r{})", fS, d, rA);
  string stfdux_str = fmt::format("stfdux f{}, r{}, r{}", fS, rA, rB);
  string stfdx_str = fmt::format("stfdx f{}, r{}, r{}", fS, rA, rB);
  string stfiwx_str = fmt::format("stfiwx f{}, r{}, r{}", fS, rA, rB);
  string stfs_str = fmt::format("stfs f{}, 0x{:X}, (r{})", fS, d, rA);
  string stfsu_str = fmt::format("stfsu f{}, 0x{:X}, (r{})", fS, d, rA);
  string stfsux_str = fmt::format("stfsux f{}, r{}, r{}", fS, rA, rB);
  string stfsx_str = fmt::format("stfsx f{}, r{}, r{}", fS, rA, rB);
  string sth_str = fmt::format("sth r{}, 0x{:X}, (r{})", rS, d, rA);
  string sthbrx_str = fmt::format("sthbrx r{}, r{}, r{}", rS, rA, rB);
  string sthu_str = fmt::format("sthu r{}, 0x{:X}, (r{})", rS, d, rA);
  string sthux_str = fmt::format("sthux r{}, r{}, r{}", rS, rA, rB);
  string sthx_str = fmt::format("sthx r{}, r{}, r{}", rS, rA, rB);
  string stmw_str = fmt::format("stmw r{}, 0x{:X}, (r{})", rS, d, rA);
  string stswi_str = fmt::format("stswi r{}, r{}, {}", rS, rA, NB);
  string stswx_str = fmt::format("stswx r{}, r{}, r{}", rS, rA, rB);
  string stw_str = fmt::format("stw r{}, 0x{:X}, (r{})", rS, d, rA);
  string stwbrx_str = fmt::format("stwbrx r{}, r{}, r{}", rS, rA, rB);
  string stwcxRC_str = fmt::format("stwcx. r{}, r{}, r{}", rS, rA, rB);
  string stwu_str = fmt::format("stwu r{}, 0x{:X}, (r{})", rS, d, rA);
  string stwux_str = fmt::format("stwux r{}, r{}, r{}", rS, rA, rB);
  string stwx_str = fmt::format("stwx r{}, r{}, r{}", rS, rA, rB);
  //*NOTE* subf, subf., subfo, subfo., subfc, subfc., subfco., and subfco. not needed
  /*
  string subf_str = fmt::format("subf r{}, r{}, r{}", rD, rA, rB);
  string subfRC_str = fmt::format("subf. r{}, r{}, r{}", rD, rA, rB);
  string subfOE_str = fmt::format("subfo r{}, r{}, r{}", rD, rA, rB);
  string subfOERC_str = fmt::format("subfo. r{}, r{}, r{}", rD, rA, rB);
  string subfc_str = fmt::format("subfc r{}, r{}, r{}", rD, rA, rB);
  string subfcRC_str = fmt::format("subfc. r{}, r{}, r{}", rD, rA, rB);
  string subfcOE_str = fmt::format("subfco r{}, r{}, r{}", rD, rA, rB);
  string subfcOERC_str = fmt::format("subfco. r{}, r{}, r{}", rD, rA, rB);
  */
  string subfe_str = fmt::format("subfe r{}, r{}, r{}", rD, rA, rB);
  string subfeRC_str = fmt::format("subfe. r{}, r{}, r{}", rD, rA, rB);
  string subfeOE_str = fmt::format("subfeo r{}, r{}, r{}", rD, rA, rB);
  string subfeOERC_str = fmt::format("subfeo. r{}, r{}, r{}", rD, rA, rB);
  string subfic_str = fmt::format("subfic r{}, r{}, 0x{:X}", rD, rA, SIMM);
  string subfme_str = fmt::format("subfme r{}, r{}", rD, rA);
  string subfmeRC_str = fmt::format("subfme. r{}, r{}", rD, rA);
  string subfmeOE_str = fmt::format("subfmeo r{}, r{}", rD, rA);
  string subfmeOERC_str = fmt::format("subfmeo. r{}, r{}", rD, rA);
  string subfze_str = fmt::format("subfze r{}, r{}", rD, rA);
  string subfzeRC_str = fmt::format("subfze. r{}, r{}", rD, rA);
  string subfzeOE_str = fmt::format("subfzeo r{}, r{}", rD, rA);
  string subfzeOERC_str = fmt::format("subfzeo. r{}, r{}", rD, rA);
  string sync_str = "sync";
  string tlbie_str = fmt::format("tlbie r{}", rB);
  string tlbsync_str = "tlbsync";
  string tw_str = fmt::format("tw {}, r{}, r{}", TO, rA, rB);
  string twi_str = fmt::format("twi {}, r{}, 0x{:X}", TO, rA, SIMM);
  string xor_str = fmt::format("xor r{}, r{}, r{}", rA, rS, rB);
  string xorRC_str = fmt::format("xor. r{}, r{}, r{}", rA, rS, rB);
  string xori_str = fmt::format("xori r{}, r{}, 0x{:04X}", rA, rS, UIMM);
  string xoris_str = fmt::format("xoris r{}, r{}, 0x{:04X}", rA, rS, UIMM);

// Declare strings for simplified instruction mnemonics
  
  // Start with all the branch mnemonics, yuck!
  //bcX when BO is 0b0000y
  string bdnzf_str = fmt::format("bdnzf- {}, 0x{:X}", BI, BD);
  string bdnzfplus_str = fmt::format("bdnzf+ {}, 0x{:X}", BI, BD);
  string bdnzfa_str = fmt::format("bdnzfa- {}, 0x{:X}", BI, BD);
  string bdnzfaplus_str = fmt::format("bdnzfa+ {}, 0x{:X}", BI, BD);
  string bdnzfl_str = fmt::format("bdnzfl- {}, 0x{:X}", BI, BD);
  string bdnzflplus_str = fmt::format("bdnzfl+ {}, 0x{:X}", BI, BD);
  string bdnzfla_str = fmt::format("bdnzfla- {}, 0x{:X}", BI, BD);
  string bdnzflaplus_str = fmt::format("bdnzfla+ {}, 0x{:X}", BI, BD);
  
  //bcX when BO is 0b0001y
  string bdzf_str = fmt::format("bdzf- {}, 0x{:X}", BI, BD);
  string bdzfplus_str = fmt::format("bdzf+ {}, 0x{:X}", BI, BD);
  string bdzfa_str = fmt::format("bdzfa- {}, 0x{:X}", BI, BD);
  string bdzfaplus_str = fmt::format("bdzfa+ {}, 0x{:X}", BI, BD);
  string bdzfl_str = fmt::format("bdzfl- {}, 0x{:X}", BI, BD);
  string bdzflplus_str = fmt::format("bdzfl+ {}, 0x{:X}", BI, BD);
  string bdzfla_str = fmt::format("bdzfla- {}, 0x{:X}", BI, BD);
  string bdzflaplus_str = fmt::format("bdzfla+ {}, 0x{:X}", BI, BD);
  
  //bcX when BO is 0b0010y
  string bge_strcr0 = fmt::format("bge- 0x{:X}", BD); //when BI uses cr0
  string bgeplus_strcr0 = fmt::format("bge+ 0x{:X}", BD); //when BI uses cr0
  string bgea_strcr0 = fmt::format("bgea- 0x{:X}", BD); //when BI uses cr0
  string bgeaplus_strcr0 = fmt::format("bgea+ 0x{:X}", BD); //when BI uses cr0
  string bgel_strcr0 = fmt::format("bgel- 0x{:X}", BD); //when BI uses cr0
  string bgelplus_strcr0 = fmt::format("bgel+ 0x{:X}", BD); //when BI uses cr0
  string bgela_strcr0 = fmt::format("bgela- 0x{:X}", BD); //when BI uses cr0
  string bgelaplus_strcr0 = fmt::format("bgela+ 0x{:X}", BD); //when BI uses cr0
  string bge_str = fmt::format("bge- cr{}, 0x{:X}", crfD_manual, BD);
  string bgeplus_str = fmt::format("bge+ cr{}, 0x{:X}", crfD_manual, BD);
  string bgea_str = fmt::format("bgea- cr{}, 0x{:X}", crfD_manual, BD);
  string bgeaplus_str = fmt::format("bgea+ cr{}, 0x{:X}", crfD_manual, BD);
  string bgel_str = fmt::format("bgel- cr{}, 0x{:X}", crfD_manual, BD);
  string bgelplus_str = fmt::format("bgel+ cr{}, 0x{:X}", crfD_manual, BD);
  string bgela_str = fmt::format("bgela- cr{}, 0x{:X}", crfD_manual, BD);
  string bgelaplus_str = fmt::format("bgela+ cr{}, 0x{:X}", crfD_manual, BD);
  
  string ble_strcr0 = fmt::format("ble- 0x{:X}", BD); //when BI uses cr0
  string bleplus_strcr0 = fmt::format("ble+ 0x{:X}", BD); //when BI uses cr0
  string blea_strcr0 = fmt::format("blea- 0x{:X}", BD); //when BI uses cr0
  string bleaplus_strcr0 = fmt::format("blea+ 0x{:X}", BD); //when BI uses cr0
  string blel_strcr0 = fmt::format("blel- 0x{:X}", BD); //when BI uses cr0
  string blelplus_strcr0 = fmt::format("blel+ 0x{:X}", BD); //when BI uses cr0
  string blela_strcr0 = fmt::format("blela- 0x{:X}", BD); //when BI uses cr0
  string blelaplus_strcr0 = fmt::format("blela+ 0x{:X}", BD); //when BI uses cr0
  string ble_str = fmt::format("ble- cr{}, 0x{:X}", crfD_manual, BD);
  string bleplus_str = fmt::format("ble+ cr{}, 0x{:X}", crfD_manual, BD);
  string blea_str = fmt::format("blea- cr{}, 0x{:X}", crfD_manual, BD);
  string bleaplus_str = fmt::format("blea+ cr{}, 0x{:X}", crfD_manual, BD);
  string blel_str = fmt::format("blel- cr{}, 0x{:X}", crfD_manual, BD);
  string blelplus_str = fmt::format("blel+ cr{}, 0x{:X}", crfD_manual, BD);
  string blela_str = fmt::format("blela- cr{}, 0x{:X}", crfD_manual, BD);
  string blelaplus_str = fmt::format("blela+ cr{}, 0x{:X}", crfD_manual, BD);
  
  string bne_strcr0 = fmt::format("bne- 0x{:X}", BD); //when BI uses cr0
  string bneplus_strcr0 = fmt::format("bne+ 0x{:X}", BD); //when BI uses cr0
  string bnea_strcr0 = fmt::format("bnea- 0x{:X}", BD); //when BI uses cr0
  string bneaplus_strcr0 = fmt::format("bnea+ 0x{:X}", BD); //when BI uses cr0
  string bnel_strcr0 = fmt::format("bnel- 0x{:X}", BD); //when BI uses cr0
  string bnelplus_strcr0 = fmt::format("bnel+ 0x{:X}", BD); //when BI uses cr0
  string bnela_strcr0 = fmt::format("bnela- 0x{:X}", BD); //when BI uses cr0
  string bnelaplus_strcr0 = fmt::format("bnela+ 0x{:X}", BD); //when BI uses cr0
  string bne_str = fmt::format("bne- cr{}, 0x{:X}", crfD_manual, BD);
  string bneplus_str = fmt::format("bne+ cr{}, 0x{:X}", crfD_manual, BD);
  string bnea_str = fmt::format("bnea- cr{}, 0x{:X}", crfD_manual, BD);
  string bneaplus_str = fmt::format("bnea+ cr{}, 0x{:X}", crfD_manual, BD);
  string bnel_str = fmt::format("bnel- cr{}, 0x{:X}", crfD_manual, BD);
  string bnelplus_str = fmt::format("bnel+ cr{}, 0x{:X}", crfD_manual, BD);
  string bnela_str = fmt::format("bnela- cr{}, 0x{:X}", crfD_manual, BD);
  string bnelaplus_str = fmt::format("bnela+ cr{}, 0x{:X}", crfD_manual, BD);
  
  string bno_strcr0 = fmt::format("bno- 0x{:X}", BD); //when BI uses cr0
  string bnoplus_strcr0 = fmt::format("bno+ 0x{:X}", BD); //when BI uses cr0
  string bnoa_strcr0 = fmt::format("bnoa- 0x{:X}", BD); //when BI uses cr0
  string bnoaplus_strcr0 = fmt::format("bnoa+ 0x{:X}", BD); //when BI uses cr0
  string bnol_strcr0 = fmt::format("bnol- 0x{:X}", BD); //when BI uses cr0
  string bnolplus_strcr0 = fmt::format("bnol+ 0x{:X}", BD); //when BI uses cr0
  string bnola_strcr0 = fmt::format("bnola- 0x{:X}", BD); //when BI uses cr0
  string bnolaplus_strcr0 = fmt::format("bnola+ 0x{:X}", BD); //when BI uses cr0
  string bno_str = fmt::format("bno- cr{}, 0x{:X}", crfD_manual, BD);
  string bnoplus_str = fmt::format("bno+ cr{}, 0x{:X}", crfD_manual, BD);
  string bnoa_str = fmt::format("bnoa- cr{}, 0x{:X}", crfD_manual, BD);
  string bnoaplus_str = fmt::format("bnoa+ cr{}, 0x{:X}", crfD_manual, BD);
  string bnol_str = fmt::format("bnol- cr{}, 0x{:X}", crfD_manual, BD);
  string bnolplus_str = fmt::format("bnol+ cr{}, 0x{:X}", crfD_manual, BD);
  string bnola_str = fmt::format("bnola- cr{}, 0x{:X}", crfD_manual, BD);
  string bnolaplus_str = fmt::format("bnola+ cr{}, 0x{:X}", crfD_manual, BD);
  
  //bcX when BO is 0b0100y
  string bdnzt_str = fmt::format("bdnzt- {}, 0x{:X}", BI, BD);
  string bdnztplus_str = fmt::format("bdnzt+ {}, 0x{:X}", BI, BD);
  string bdnzta_str = fmt::format("bdnzta- {}, 0x{:X}", BI, BD);
  string bdnztaplus_str = fmt::format("bdnzta+ {}, 0x{:X}", BI, BD);
  string bdnztl_str = fmt::format("bdnztl- {}, 0x{:X}", BI, BD);
  string bdnztlplus_str = fmt::format("bdnztl+ {}, 0x{:X}", BI, BD);
  string bdnztla_str = fmt::format("bdnztla- {}, 0x{:X}", BI, BD);
  string bdnztlaplus_str = fmt::format("bdnztla+ {}, 0x{:X}", BI, BD);
  
  //bcX when BO is 0b0101y
  string bdzt_str = fmt::format("bdzt- {}, 0x{:X}", BI, BD);
  string bdztplus_str = fmt::format("bdzt+ {}, 0x{:X}", BI, BD);
  string bdzta_str = fmt::format("bdzta- {}, 0x{:X}", BI, BD);
  string bdztaplus_str = fmt::format("bdzta+ {}, 0x{:X}", BI, BD);
  string bdztl_str = fmt::format("bdztl- {}, 0x{:X}", BI, BD);
  string bdztlplus_str = fmt::format("bdztl+ {}, 0x{:X}", BI, BD);
  string bdztla_str = fmt::format("bdztla- {}, 0x{:X}", BI, BD);
  string bdztlaplus_str = fmt::format("bdztla+ {}, 0x{:X}", BI, BD);
  
  //bcX when BO is 0b0110y
  string blt_strcr0 = fmt::format("blt- 0x{:X}", BD); //when BI uses cr0
  string bltplus_strcr0 = fmt::format("blt+ 0x{:X}", BD); //when BI uses cr0
  string blta_strcr0 = fmt::format("blta- 0x{:X}", BD); //when BI uses cr0
  string bltaplus_strcr0 = fmt::format("blta+ 0x{:X}", BD); //when BI uses cr0
  string bltl_strcr0 = fmt::format("bltl- 0x{:X}", BD); //when BI uses cr0
  string bltlplus_strcr0 = fmt::format("bltl+ 0x{:X}", BD); //when BI uses cr0
  string bltla_strcr0 = fmt::format("bltla- 0x{:X}", BD); //when BI uses cr0
  string bltlaplus_strcr0 = fmt::format("bltla+ 0x{:X}", BD); //when BI uses cr0
  string blt_str = fmt::format("blt- cr{}, 0x{:X}", crfD_manual, BD);
  string bltplus_str = fmt::format("blt+ cr{}, 0x{:X}", crfD_manual, BD);
  string blta_str = fmt::format("blta- cr{}, 0x{:X}", crfD_manual, BD);
  string bltaplus_str = fmt::format("blta+ cr{}, 0x{:X}", crfD_manual, BD);
  string bltl_str = fmt::format("bltl- cr{}, 0x{:X}", crfD_manual, BD);
  string bltlplus_str = fmt::format("bltl+ cr{}, 0x{:X}", crfD_manual, BD);
  string bltla_str = fmt::format("bltla- cr{}, 0x{:X}", crfD_manual, BD);
  string bltlaplus_str = fmt::format("bltla+ cr{}, 0x{:X}", crfD_manual, BD);
  
  string bgt_strcr0 = fmt::format("bgt- 0x{:X}", BD); //when BI uses cr0
  string bgtplus_strcr0 = fmt::format("bgt+ 0x{:X}", BD); //when BI uses cr0
  string bgta_strcr0 = fmt::format("bgt- 0x{:X}", BD); //when BI uses cr0
  string bgtaplus_strcr0 = fmt::format("bgt+ 0x{:X}", BD); //when BI uses cr0
  string bgtl_strcr0 = fmt::format("bgt- 0x{:X}", BD); //when BI uses cr0
  string bgtlplus_strcr0 = fmt::format("bgt+ 0x{:X}", BD); //when BI uses cr0
  string bgtla_strcr0 = fmt::format("bgt- 0x{:X}", BD); //when BI uses cr0
  string bgtlaplus_strcr0 = fmt::format("bgt+ 0x{:X}", BD); //when BI uses cr0
  string bgt_str = fmt::format("bgt- cr{}, 0x{:X}", crfD_manual, BD);
  string bgtplus_str = fmt::format("bgt+ cr{}, 0x{:X}", crfD_manual, BD);
  string bgta_str = fmt::format("bgta- cr{}, 0x{:X}", crfD_manual, BD);
  string bgtaplus_str = fmt::format("bgta+ cr{}, 0x{:X}", crfD_manual, BD);
  string bgtl_str = fmt::format("bgtl- cr{}, 0x{:X}", crfD_manual, BD);
  string bgtlplus_str = fmt::format("bgtl+ cr{}, 0x{:X}", crfD_manual, BD);
  string bgtla_str = fmt::format("bgtla- cr{}, 0x{:X}", crfD_manual, BD);
  string bgtlaplus_str = fmt::format("bgtla+ cr{}, 0x{:X}", crfD_manual, BD);
  
  string beq_strcr0 = fmt::format("beq- 0x{:X}", BD); //when BI uses cr0
  string beqplus_strcr0 = fmt::format("beq+ 0x{:X}", BD); //when BI uses cr0
  string beqa_strcr0 = fmt::format("beqa- 0x{:X}", BD); //when BI uses cr0
  string beqaplus_strcr0 = fmt::format("beqa+ 0x{:X}", BD); //when BI uses cr0
  string beql_strcr0 = fmt::format("beql- 0x{:X}", BD); //when BI uses cr0
  string beqlplus_strcr0 = fmt::format("beql+ 0x{:X}", BD); //when BI uses cr0
  string beqla_strcr0 = fmt::format("beqla- 0x{:X}", BD); //when BI uses cr0
  string beqlaplus_strcr0 = fmt::format("beqla+ 0x{:X}", BD); //when BI uses cr0
  string beq_str = fmt::format("beq- cr{}, 0x{:X}", crfD_manual, BD);
  string beqplus_str = fmt::format("beq+ cr{}, 0x{:X}", crfD_manual, BD);
  string beqa_str = fmt::format("beqa- cr{}, 0x{:X}", crfD_manual, BD);
  string beqaplus_str = fmt::format("beqa+ cr{}, 0x{:X}", crfD_manual, BD);
  string beql_str = fmt::format("beql- cr{}, 0x{:X}", crfD_manual, BD);
  string beqlplus_str = fmt::format("beql+ cr{}, 0x{:X}", crfD_manual, BD);
  string beqla_str = fmt::format("beqla- cr{}, 0x{:X}", crfD_manual, BD);
  string beqlaplus_str = fmt::format("beqla+ cr{}, 0x{:X}", crfD_manual, BD);
  
  string bso_strcr0 = fmt::format("bso- 0x{:X}", BD); //when BI uses cr0
  string bsoplus_strcr0 = fmt::format("bso+ 0x{:X}", BD); //when BI uses cr0
  string bsoa_strcr0 = fmt::format("bsoa- 0x{:X}", BD); //when BI uses cr0
  string bsoaplus_strcr0 = fmt::format("bsoa+ 0x{:X}", BD); //when BI uses cr0
  string bsol_strcr0 = fmt::format("bsol- 0x{:X}", BD); //when BI uses cr0
  string bsolplus_strcr0 = fmt::format("bsol+ 0x{:X}", BD); //when BI uses cr0
  string bsola_strcr0 = fmt::format("bsola- 0x{:X}", BD); //when BI uses cr0
  string bsolaplus_strcr0 = fmt::format("bsola+ 0x{:X}", BD); //when BI uses cr0
  string bso_str = fmt::format("bso- cr{}, 0x{:X}", crfD_manual, BD);
  string bsoplus_str = fmt::format("bso+ cr{}, 0x{:X}", crfD_manual, BD);
  string bsoa_str = fmt::format("bsoa- cr{}, 0x{:X}", crfD_manual, BD);
  string bsoaplus_str = fmt::format("bsoa+ cr{}, 0x{:X}", crfD_manual, BD);
  string bsol_str = fmt::format("bsol- cr{}, 0x{:X}", crfD_manual, BD);
  string bsolplus_str = fmt::format("bsol+ cr{}, 0x{:X}", crfD_manual, BD);
  string bsola_str = fmt::format("bsola- cr{}, 0x{:X}", crfD_manual, BD);
  string bsolaplus_str = fmt::format("bsola+ cr{}, 0x{:X}", crfD_manual, BD);
  
  // bcX when BO is 0b1000y, BI ignored!
  string bdnz_str = fmt::format("bdnz- 0x{:X}", BD);
  string bdnzplus_str = fmt::format("bdnz+ 0x{:X}", BD);
  string bdnza_str = fmt::format("bdnza- 0x{:X}", BD);
  string bdnzaplus_str = fmt::format("bdnza+ 0x{:X}", BD);
  string bdnzl_str = fmt::format("bdnzl- 0x{:X}", BD);
  string bdnzlplus_str = fmt::format("bdnzl+ 0x{:X}", BD);
  string bdnzla_str = fmt::format("bdnzla- 0x{:X}", BD);
  string bdnzlaplus_str = fmt::format("bdnzla+ 0x{:X}", BD);
  
  // bcX when BO is 0b1001y, BI ignored!
  string bdz_str = fmt::format("bdz- 0x{:X}", BD);
  string bdzplus_str = fmt::format("bdz+ 0x{:X}", BD);
  string bdza_str = fmt::format("bdza- 0x{:X}", BD);
  string bdzaplus_str = fmt::format("bdza+ 0x{:X}", BD);
  string bdzl_str = fmt::format("bdzl- 0x{:X}", BD);
  string bdzlplus_str = fmt::format("bdzl+ 0x{:X}", BD);
  string bdzla_str = fmt::format("bdzla- 0x{:X}", BD);
  string bdzlaplus_str = fmt::format("bdzla+ 0x{:X}", BD);
  
  //bcX when BO is 0b101000, BI is ignored!
  string balt_str = fmt::format("b 0x{:X}", BD);
  string baalt_str = fmt::format("ba 0x{:X}", BD);
  string blalt_str = fmt::format("bl 0x{:X}", BD);
  string blaalt_str = fmt::format("bla 0x{:X}", BD);
  
  
  //bcctrX when BO is 0b0000y
  string bdnzfctr_str = fmt::format("bdnzfctr- {}", BI);
  string bdnzfctrplus_str = fmt::format("bdnzfctr+ {}", BI);
  string bdnzfctrl_str = fmt::format("bdnzfctrl- {}", BI);
  string bdnzfctrlplus_str = fmt::format("bdnzfctrl+ {}", BI);
  
  //bcctrX when BO is 0b0001y
  string bdzfctr_str = fmt::format("bdzfctr- {}", BI);
  string bdzfctrplus_str = fmt::format("bdzfctr+ {}", BI);
  string bdzfctrl_str = fmt::format("bdzfctrl- {}", BI);
  string bdzfctrlplus_str = fmt::format("bdzfctrl+ {}", BI);
  
  //bcctrX when BO is 0b0010y
  string bgectr_strcr0 = "bgectr-"; //when BI uses cr0
  string bgectrplus_strcr0 = "bgectr+"; //when BI uses cr0
  string bgectrl_strcr0 = "bgectrl-"; //when BI uses cr0
  string bgectrlplus_strcr0 = "bgectrl+"; //when BI uses cr0
  string bgectr_str = fmt::format("bgectr- cr{}", crfD_manual);
  string bgectrplus_str = fmt::format("bgectr+ cr{}", crfD_manual);
  string bgectrl_str = fmt::format("bgectrl- cr{}", crfD_manual);
  string bgectrlplus_str = fmt::format("bgectrl+ cr{}", crfD_manual);
  
  string blectr_strcr0 = "blectr-"; //when BI uses cr0
  string blectrplus_strcr0 = "blectr+"; //when BI uses cr0
  string blectrl_strcr0 = "blectrl-"; //when BI uses cr0
  string blectrlplus_strcr0 = "blectrl+"; //when BI uses cr0
  string blectr_str = fmt::format("blectr- cr{}", crfD_manual);
  string blectrplus_str = fmt::format("blectr+ cr{}", crfD_manual);
  string blectrl_str = fmt::format("blectrl- cr{}", crfD_manual);
  string blectrlplus_str = fmt::format("blectrl+ cr{}", crfD_manual);
  
  string bnectr_strcr0 = "bnectr-"; //when BI uses cr0
  string bnectrplus_strcr0 = "bnectr+"; //when BI uses cr0
  string bnectrl_strcr0 = "bnectrl-"; //when BI uses cr0
  string bnectrlplus_strcr0 = "bnectrl+"; //when BI uses cr0
  string bnectr_str = fmt::format("bnectr- cr{}", crfD_manual);
  string bnectrplus_str = fmt::format("bnectr+ cr{}", crfD_manual);
  string bnectrl_str = fmt::format("bnectrl- cr{},", crfD_manual);
  string bnectrlplus_str = fmt::format("bnectrl+ cr{}", crfD_manual);
  
  string bnoctr_strcr0 = "bnoctr-"; //when BI uses cr0
  string bnoctrplus_strcr0 = "bnoctr+"; //when BI uses cr0
  string bnoctrl_strcr0 = "bnoctrl-"; //when BI uses cr0
  string bnoctrlplus_strcr0 = "bnoctrl+"; //when BI uses cr0
  string bnoctr_str = fmt::format("bnoctr- cr{}", crfD_manual);
  string bnoctrplus_str = fmt::format("bnoctr+ cr{}", crfD_manual);
  string bnoctrl_str = fmt::format("bnoctrl- cr{}", crfD_manual);
  string bnoctrlplus_str = fmt::format("bnoctrl+ cr{}", crfD_manual);
  
  //bcctrX when BO is 0b0100y
  string bdnztctr_str = fmt::format("bdnztctr- {}", BI);
  string bdnztctrplus_str = fmt::format("bdnztctr+ {}", BI);
  string bdnztctrl_str = fmt::format("bdnztctrl- {}", BI);
  string bdnztctrlplus_str = fmt::format("bdnztctrl+ {}", BI);
  
  //bcctrX when BO is 0b0101y
  string bdztctr_str = fmt::format("bdztctr- {}", BI);
  string bdztctrplus_str = fmt::format("bdztctr+ {}", BI);
  string bdztctrl_str = fmt::format("bdztctrl- {}", BI);
  string bdztctrlplus_str = fmt::format("bdztctrl+ {}", BI);
  
  //bcctrX when BO is 0b0110y
  string bltctr_strcr0 = "bltctr-"; //when BI uses cr0
  string bltctrplus_strcr0 = "btlctr+"; //when BI uses cr0
  string bltctrl_strcr0 = "bltctrl-"; //when BI uses cr0
  string bltctrlplus_strcr0 = "bltctrl+"; //when BI uses cr0
  string bltctr_str = fmt::format("bltctr- cr{}", crfD_manual);
  string bltctrplus_str = fmt::format("bltctr+ cr{}", crfD_manual);
  string bltctrl_str = fmt::format("bltctrl- cr{}", crfD_manual);
  string bltctrlplus_str = fmt::format("bltctrl+ cr{}", crfD_manual);
  
  string bgtctr_strcr0 = "bgtctr-"; //when BI uses cr0
  string bgtctrplus_strcr0 = "bgtctr+"; //when BI uses cr0
  string bgtctrl_strcr0 = "bgtctrl-"; //when BI uses cr0
  string bgtctrlplus_strcr0 = "bgtctrl+"; //when BI uses cr0
  string bgtctr_str = fmt::format("bgtctr- cr{}", crfD_manual);
  string bgtctrplus_str = fmt::format("bgtctr+ cr{}", crfD_manual);
  string bgtctrl_str = fmt::format("bgtctrl- cr{}", crfD_manual);
  string bgtctrlplus_str = fmt::format("bgtctrl+ cr{}", crfD_manual);
  
  string beqctr_strcr0 = "beqctr-"; //when BI uses cr0
  string beqctrplus_strcr0 = "beqctr+"; //when BI uses cr0
  string beqctrl_strcr0 = "beqctrl-"; //when BI uses cr0
  string beqctrlplus_strcr0 = "beqctrl+"; //when BI uses cr0
  string beqctr_str = fmt::format("beqctr- cr{}", crfD_manual);
  string beqctrplus_str = fmt::format("beqctr+ cr{}", crfD_manual);
  string beqctrl_str = fmt::format("beqctrl- cr{},", crfD_manual);
  string beqctrlplus_str = fmt::format("beqctrl+ cr{}", crfD_manual);
  
  string bsoctr_strcr0 = "bsoctr-"; //when BI uses cr0
  string bsoctrplus_strcr0 = "bsoctr+"; //when BI uses cr0
  string bsoctrl_strcr0 = "bsoctrl-"; //when BI uses cr0
  string bsoctrlplus_strcr0 = "bsoctrl+"; //when BI uses cr0
  string bsoctr_str = fmt::format("bsoctr- cr{}", crfD_manual);
  string bsoctrplus_str = fmt::format("bsoctr+ cr{}", crfD_manual);
  string bsoctrl_str = fmt::format("bsoctrl- cr{}", crfD_manual);
  string bsoctrlplus_str = fmt::format("bsoctrl+ cr{}", crfD_manual);
  
  // bcctrX when BO is 0b1000y, BI ignored!
  string bdnzctr_str = "bdnzctr-";
  string bdnzctrplus_str = "bdnzctr+";
  string bdnzctrl_str = "bdnzctrl-";
  string bdnzctrlplus_str = "bdnzctrl+";
  
  // bcctrX when BO is 0b1001y, BI ignored!
  string bdzctr_str = "bdzctr-";
  string bdzctrplus_str = "bdzctr+";
  string bdzctrl_str = "bdzctrl-";
  string bdzctrlplus_str = "bdzctrl+";
  
  //bcctrX when BO is 0b10100, BI ignored!
  string bctr_str = "bctr";
  string bctrl_str = "bctrl";
  
  
  //bclrX when BO is 0b0000y
  string bdnzflr_str = fmt::format("bdnzflr- {}", BI);
  string bdnzflrplus_str = fmt::format("bdnzflr+ {}", BI);
  string bdnzflrl_str = fmt::format("bdnzflrl- {}", BI);
  string bdnzflrlplus_str = fmt::format("bdnzflrl+ {}", BI);
  
  //bclrX when BO is 0b0001y
  string bdzflr_str = fmt::format("bdzflr- {}", BI);
  string bdzflrplus_str = fmt::format("bdzflr+ {}", BI);
  string bdzflrl_str = fmt::format("bdzflrl- {}", BI);
  string bdzflrlplus_str = fmt::format("bdzflrl+ {}", BI);
  
  //bclrX when BO is 0b0010y
  string bgelr_strcr0 = "bgelr-"; //when BI uses cr0
  string bgelrplus_strcr0 = "bgelr+"; //when BI uses cr0
  string bgelrl_strcr0 = "bgelrl-"; //when BI uses cr0
  string bgelrlplus_strcr0 = "bgelrl+"; //when BI uses cr0
  string bgelr_str = fmt::format("bgelr- cr{}", crfD_manual);
  string bgelrplus_str = fmt::format("bgelr+ cr{}", crfD_manual);
  string bgelrl_str = fmt::format("bgelrl- cr{}", crfD_manual);
  string bgelrlplus_str = fmt::format("bgelrl+ cr{}", crfD_manual);
  
  string blelr_strcr0 = "blelr-"; //when BI uses cr0
  string blelrplus_strcr0 = "blelr+"; //when BI uses cr0
  string blelrl_strcr0 = "blelrl-"; //when BI uses cr0
  string blelrlplus_strcr0 = "blelrl+"; //when BI uses cr0
  string blelr_str = fmt::format("blelr- cr{}", crfD_manual);
  string blelrplus_str = fmt::format("blelr+ cr{}", crfD_manual);
  string blelrl_str = fmt::format("blelrl- cr{}", crfD_manual);
  string blelrlplus_str = fmt::format("blelrl+ cr{}", crfD_manual);
  
  string bnelr_strcr0 = "bnelr-"; //when BI uses cr0
  string bnelrplus_strcr0 = "bnelr+"; //when BI uses cr0
  string bnelrl_strcr0 = "bnelrl-"; //when BI uses cr0
  string bnelrlplus_strcr0 = "bnelrl+"; //when BI uses cr0
  string bnelr_str = fmt::format("bnelr- cr{}", crfD_manual);
  string bnelrplus_str = fmt::format("bnelr+ cr{}", crfD_manual);
  string bnelrl_str = fmt::format("bnelrl- cr{},", crfD_manual);
  string bnelrlplus_str = fmt::format("bnelrl+ cr{}", crfD_manual);
  
  string bnolr_strcr0 = "bnolr-"; //when BI uses cr0
  string bnolrplus_strcr0 = "bnolr+"; //when BI uses cr0
  string bnolrl_strcr0 = "bnolrl-"; //when BI uses cr0
  string bnolrlplus_strcr0 = "bnolrl+"; //when BI uses cr0
  string bnolr_str = fmt::format("bnolr- cr{}", crfD_manual);
  string bnolrplus_str = fmt::format("bnolr+ cr{}", crfD_manual);
  string bnolrl_str = fmt::format("bnolrl- cr{}", crfD_manual);
  string bnolrlplus_str = fmt::format("bnolrl+ cr{}", crfD_manual);
  
  //bclrX when BO is 0b0100y
  string bdnztlr_str = fmt::format("bdnztlr- {}", BI);
  string bdnztlrplus_str = fmt::format("bdnztlr+ {}", BI);
  string bdnztlrl_str = fmt::format("bdnztlrl- {}", BI);
  string bdnztlrlplus_str = fmt::format("bdnztlrl+ {}", BI);
  
  //bclrX when BO is 0b0101y
  string bdztlr_str = fmt::format("bdztlr- {}", BI);
  string bdztlrplus_str = fmt::format("bdztlr+ {}", BI);
  string bdztlrl_str = fmt::format("bdztlrl- {}", BI);
  string bdztlrlplus_str = fmt::format("bdztlrl+ {}", BI);
  
  //bclrX when BO is 0b0110y
  string bltlr_strcr0 = "bltlr-"; //when BI uses cr0
  string bltlrplus_strcr0 = "bltlr+"; //when BI uses cr0
  string bltlrl_strcr0 = "bltlrl-"; //when BI uses cr0
  string bltlrlplus_strcr0 = "bltlrl+"; //when BI uses cr0
  string bltlr_str = fmt::format("bltlr- cr{}", crfD_manual);
  string bltlrplus_str = fmt::format("bltlr+ cr{}", crfD_manual);
  string bltlrl_str = fmt::format("bltlrl- cr{}", crfD_manual);
  string bltlrlplus_str = fmt::format("bltlrl+ cr{}", crfD_manual);
  
  string bgtlr_strcr0 = "bgtlr-"; //when BI uses cr0
  string bgtlrplus_strcr0 = "bgtlr+"; //when BI uses cr0
  string bgtlrl_strcr0 = "bgtlrl-"; //when BI uses cr0
  string bgtlrlplus_strcr0 = "bgtlrl+"; //when BI uses cr0
  string bgtlr_str = fmt::format("bgtlr- cr{}", crfD_manual);
  string bgtlrplus_str = fmt::format("bgtlr+ cr{}", crfD_manual);
  string bgtlrl_str = fmt::format("bgtlrl- cr{}", crfD_manual);
  string bgtlrlplus_str = fmt::format("bgtlrl+ cr{}", crfD_manual);
  
  string beqlr_strcr0 = "beqlr-"; //when BI uses cr0
  string beqlrplus_strcr0 = "beqlr+"; //when BI uses cr0
  string beqlrl_strcr0 = "beqlrl-"; //when BI uses cr0
  string beqlrlplus_strcr0 = "beqlrl+"; //when BI uses cr0
  string beqlr_str = fmt::format("beqlr- cr{}", crfD_manual);
  string beqlrplus_str = fmt::format("beqlr+ cr{}", crfD_manual);
  string beqlrl_str = fmt::format("beqlrl- cr{},", crfD_manual);
  string beqlrlplus_str = fmt::format("beqlrl+ cr{}", crfD_manual);
  
  string bsolr_strcr0 = "bsolr-"; //when BI uses cr0
  string bsolrplus_strcr0 = "bsolr+"; //when BI uses cr0
  string bsolrl_strcr0 = "bsolrl-"; //when BI uses cr0
  string bsolrlplus_strcr0 = "bsolrl+"; //when BI uses cr0
  string bsolr_str = fmt::format("bsolr- cr{}", crfD_manual);
  string bsolrplus_str = fmt::format("bsolr+ cr{}", crfD_manual);
  string bsolrl_str = fmt::format("bsolrl- cr{}", crfD_manual);
  string bsolrlplus_str = fmt::format("bsolrl+ cr{}", crfD_manual);
  
  // bclrX when BO is 0b1000y, BI ignored!
  string bdnzlr_str = "bdnzlr-";
  string bdnzlrplus_str = "bdnzlr+";
  string bdnzlrl_str = "bdnzlrl-";
  string bdnzlrlplus_str = "bdnzlrl+";
  
  // bclrX when BO is 0b1001y, BI ignored!
  string bdzlr_str = "bdzlr-";
  string bdzlrplus_str = "bdzlr+";
  string bdzlrl_str = "bdzlrl-";
  string bdzlrlplus_str = "bdzlrl+";
  
  //bclrX when BO is 0b10100, BI ignored!
  string blr_str = "blr";
  string blrl_str = "blrl";
  
  
  //Non Branch simplified Mnemonics
  string clrlwi_str = fmt::format("clrlwi r{}, r{}, {}", rA, rS, MB); //clrlwi rA, rS, n where n = MB
  string clrlwiRC_str = fmt::format("clrlwi. r{}, r{}, {}", rA, rS, MB); //clrlwi rA, rS, n where n = MB
  string clrrwi_str = fmt::format("clrrwi r{}, r{}, {}", rA, rS, clrrwi_n); //clrrwi rA, rS, n where n = (31 - ME)
  string clrrwiRC_str = fmt::format("clrrwi. r{}, r{}, {}", rA, rS, clrrwi_n); //clrrwi. rA, rS, n where n = (31 - ME)
  //string clrlslwi_str = "clrlslwi"; //Personally don't like this mnemonic
  //string clrlslwiRC_str = "clrlslwi."; //Personally don't like this mnemonic
  string crclr_str = fmt::format("crclr {}", crbD);
  string crmove_str = fmt::format("crmove {}, {}", crbD, crbA);
  string crnot_str = fmt::format("crnot {}, {}", crbD, crbA);
  string crset_str = fmt::format("crset {}", crbD);
  //string extlwi_str = "extlwi"; //Personally don't like this mnemonic
  //string extlwiRC_str = "extlwi."; //Personally don't like this mnemonic
  //string extrwi_str = "extrwi"; //Personally don't like this mnemonic
  //string extrwiRC_str = "extrwi."; //Personally don't like this mnemonic
  //string inslwi_str = "inslwi"; //Personally don't like this mnemonic
  //string inslwiRC_str = "inslwi."; //Personally don't like this mnemonic
  //string insrwi_str = "insrwi"; //Personally don't like this mnemonic
  //string insrwiRC_str = "insrwi."; //Personally don't like this mnemonic
  string li_str = fmt::format("li r{}, 0x{:X}", rD, SIMM);
  string lis_str = fmt::format("lis r{}, 0x{:X}", rD, SIMM);
  string mfctr_str = fmt::format("mfctr r{}", rD);
  string mflr_str = fmt::format("mflr r{}", rD);
  string mftbu_str = fmt::format("mftbu r{}", rD);
  string mftbl_str = fmt::format("mftbl r{}", rD); //No point for mftb simplified mnemonic
  string mfxer_str = fmt::format("mfxer r{}", rD);
  
  //Start other mfspr simplified mnemonics
  string mfdsisr_str = fmt::format("mfdsisr r{}", rD);
  string mfdar_str = fmt::format("mfdar r{}", rD);
  string mfdec_str = fmt::format("mfdec r{}", rD);
  string mfsdr1_str = fmt::format("mfsdr1 r{}", rD);
  string mfsrr0_str = fmt::format("mfsrr0 r{}", rD);
  string mfsrr1_str = fmt::format("mfsrr1 r{}", rD);
  string mfsprg0_str = fmt::format("mfsprg0 r{}", rD);
  string mfsprg1_str = fmt::format("mfsprg1 r{}", rD);
  string mfsprg2_str = fmt::format("mfsprg2 r{}", rD);
  string mfsprg3_str = fmt::format("mfsprg3 r{}", rD);
  string mfear_str = fmt::format("mfear r{}", rD);
  string mfpvr_str = fmt::format("mfpvr r{}", rD);
  string mfibat0u_str = fmt::format("mfibat0u r{}", rD);
  string mfibat0l_str = fmt::format("mfibat0l r{}", rD);
  string mfibat1u_str = fmt::format("mfibat1u r{}", rD);
  string mfibat1l_str = fmt::format("mfibat1l r{}", rD);
  string mfibat2u_str = fmt::format("mfibat2u r{}", rD);
  string mfibat2l_str = fmt::format("mfibat2l r{}", rD);
  string mfibat3u_str = fmt::format("mfibat3u r{}", rD);
  string mfibat3l_str = fmt::format("mfibat3l r{}", rD);
  string mfibat4u_str = fmt::format("mfibat4u r{}", rD);
  string mfibat4l_str = fmt::format("mfibat4l r{}", rD);
  string mfibat5u_str = fmt::format("mfibat5u r{}", rD);
  string mfibat5l_str = fmt::format("mfibat5l r{}", rD);
  string mfibat6u_str = fmt::format("mfibat6u r{}", rD);
  string mfibat6l_str = fmt::format("mfibat6l r{}", rD);
  string mfibat7u_str = fmt::format("mfibat7u r{}", rD);
  string mfibat7l_str = fmt::format("mfibat7l r{}", rD);
  string mfdbat0u_str = fmt::format("mfdbat0u r{}", rD);
  string mfdbat0l_str = fmt::format("mfdbat0l r{}", rD);
  string mfdbat1u_str = fmt::format("mfdbat1u r{}", rD);
  string mfdbat1l_str = fmt::format("mfdbat1l r{}", rD);
  string mfdbat2u_str = fmt::format("mfdbat2u r{}", rD);
  string mfdbat2l_str = fmt::format("mfdbat2l r{}", rD);
  string mfdbat3u_str = fmt::format("mfdbat3u r{}", rD);
  string mfdbat3l_str = fmt::format("mfdbat3l r{}", rD);
  string mfdbat4u_str = fmt::format("mfdbat4u r{}", rD);
  string mfdbat4l_str = fmt::format("mfdbat4l r{}", rD);
  string mfdbat5u_str = fmt::format("mfdbat5u r{}", rD);
  string mfdbat5l_str = fmt::format("mfdbat5l r{}", rD);
  string mfdbat6u_str = fmt::format("mfdbat6u r{}", rD);
  string mfdbat6l_str = fmt::format("mfdbat6l r{}", rD);
  string mfdbat7u_str = fmt::format("mfdbat7u r{}", rD);
  string mfdbat7l_str = fmt::format("mfdbat7l r{}", rD);
  string mfgqr0_str = fmt::format("mfgqr0 r{}", rD);
  string mfgqr1_str = fmt::format("mfgqr1 r{}", rD);
  string mfgqr2_str = fmt::format("mfgqr2 r{}", rD);
  string mfgqr3_str = fmt::format("mfgqr3 r{}", rD);
  string mfgqr4_str = fmt::format("mfgqr4 r{}", rD);
  string mfgqr5_str = fmt::format("mfgqr5 r{}", rD);
  string mfgqr6_str = fmt::format("mfgqr6 r{}", rD);
  string mfgqr7_str = fmt::format("mfgqr7 r{}", rD);
  string mfhid2_str = fmt::format("mfhid2 r{}", rD);
  string mfwpar_str = fmt::format("mfwpar r{}", rD);
  string mfdma_u_str = fmt::format("mfdma_u r{}", rD);
  string mfdma_l_str = fmt::format("mfdma_l r{}", rD);
  string mfcidh_str = fmt::format("mfcidh r{}", rD); // Special Broadway Chip IDs supported
  string mfcidm_str = fmt::format("mfcidm r{}", rD); // Special Broadway Chip IDs supported
  string mfcidl_str = fmt::format("mfcidl r{}", rD); // Special Broadway Chip IDs supported
  string mfummcr0_str = fmt::format("mfummcr0 r{}", rD);
  string mfupmc1_str = fmt::format("mfupmc1 r{}", rD);
  string mfupmc2_str = fmt::format("mfupmc2 r{}", rD);
  string mfusia_str = fmt::format("mfusia r{}", rD);
  string mfummcr1_str = fmt::format("mfummcr1 r{}", rD);
  string mfupmc3_str = fmt::format("mfupmc3 r{}", rD);
  string mfupmc4_str = fmt::format("mfupmc4 r{}", rD);
  string mfusda_str = fmt::format("mfusda r{}", rD);
  string mfmmcr0_str = fmt::format("mfmmcr0 r{}", rD);
  string mfpmc1_str = fmt::format("mfpmc1 r{}", rD);
  string mfpmc2_str = fmt::format("mfpmc2 r{}", rD);
  string mfsia_str = fmt::format("mfsia r{}", rD);
  string mfmmcr1_str = fmt::format("mfmmcr1 r{}", rD);
  string mfpmc3_str = fmt::format("mfpmc3 r{}", rD);
  string mfpmc4_str = fmt::format("mfpmc4 r{}", rD);
  string mfsda_str = fmt::format("mfsda r{}", rD);
  string mfhid0_str = fmt::format("mfhid0 r{}", rD);
  string mfhid1_str = fmt::format("mfhid1 r{}", rD);
  string mfiabr_str = fmt::format("mfiabr r{}", rD);
  string mfhid4_str = fmt::format("mfhid4 r{}", rD);
  string mftdcl_str = fmt::format("mftdcl r{}", rD);
  string mfdabr_str = fmt::format("mfdabr r{}", rD);
  string mfl2cr_str = fmt::format("mfl2cr r{}", rD);
  string mftdch_str = fmt::format("mftdch r{}", rD);
  string mfictc_str = fmt::format("mfictc r{}", rD);
  string mfthrm1_str = fmt::format("mfthrm1 r{}", rD);
  string mfthrm2_str = fmt::format("mfthrm2 r{}", rD);
  string mfthrm3_str = fmt::format("mfthrm3 r{}", rD);
  
  //Continue on
  string mr_str = fmt::format("mr r{}, r{}", rA, rS);
  string mrRC_str = fmt::format("mr. r{}, r{}", rA, rS);
  string mtcr_str = fmt::format("mtcr r{}", rS);
  string mtctr_str = fmt::format("mtctr r{}", rD); //Technically this is rS but Broadway Manual uses "rD", lol
  string mtlr_str = fmt::format("mtlr r{}", rD); //Technically this is rS but Broadway Manual uses "rD", lol
  string mtxer_str = fmt::format("mtxer r{}", rD); //Technically this is rS but Broadway Manual uses "rD", lol
  
  //Start other mtspr simplified mnemonics. rS technically supposed to be used but whatever
  string mtdsisr_str = fmt::format("mtdsisr r{}", rD);
  string mtdar_str = fmt::format("mtdar r{}", rD);
  string mtdec_str = fmt::format("mtdec r{}", rD);
  string mtsdr1_str = fmt::format("mtsdr1 r{}", rD);
  string mtsrr0_str = fmt::format("mtsrr0 r{}", rD);
  string mtsrr1_str = fmt::format("mtsrr1 r{}", rD);
  string mtsprg0_str = fmt::format("mtsprg0 r{}", rD);
  string mtsprg1_str = fmt::format("mtsprg1 r{}", rD);
  string mtsprg2_str = fmt::format("mtsprg2 r{}", rD);
  string mtsprg3_str = fmt::format("mtsprg3 r{}", rD);
  string mtear_str = fmt::format("mtear r{}", rD);
  string mttbl_str = fmt::format("mttbl r{}", rD);
  string mttbu_str = fmt::format("mttbu r{}", rD);
  string mtibat0u_str = fmt::format("mtibat0u r{}", rD);
  string mtibat0l_str = fmt::format("mtibat0l r{}", rD);
  string mtibat1u_str = fmt::format("mtibat1u r{}", rD);
  string mtibat1l_str = fmt::format("mtibat1l r{}", rD);
  string mtibat2u_str = fmt::format("mtibat2u r{}", rD);
  string mtibat2l_str = fmt::format("mtibat2l r{}", rD);
  string mtibat3u_str = fmt::format("mtibat3u r{}", rD);
  string mtibat3l_str = fmt::format("mtibat3l r{}", rD);
  string mtibat4u_str = fmt::format("mtibat4u r{}", rD);
  string mtibat4l_str = fmt::format("mtibat4l r{}", rD);
  string mtibat5u_str = fmt::format("mtibat5u r{}", rD);
  string mtibat5l_str = fmt::format("mtibat5l r{}", rD);
  string mtibat6u_str = fmt::format("mtibat6u r{}", rD);
  string mtibat6l_str = fmt::format("mtibat6l r{}", rD);
  string mtibat7u_str = fmt::format("mtibat7u r{}", rD);
  string mtibat7l_str = fmt::format("mtibat7l r{}", rD);
  string mtdbat0u_str = fmt::format("mtdbat0u r{}", rD);
  string mtdbat0l_str = fmt::format("mtdbat0l r{}", rD);
  string mtdbat1u_str = fmt::format("mtdbat1u r{}", rD);
  string mtdbat1l_str = fmt::format("mtdbat1l r{}", rD);
  string mtdbat2u_str = fmt::format("mtdbat2u r{}", rD);
  string mtdbat2l_str = fmt::format("mtdbat2l r{}", rD);
  string mtdbat3u_str = fmt::format("mtdbat3u r{}", rD);
  string mtdbat3l_str = fmt::format("mtdbat3l r{}", rD);
  string mtdbat4u_str = fmt::format("mtdbat4u r{}", rD);
  string mtdbat4l_str = fmt::format("mtdbat4l r{}", rD);
  string mtdbat5u_str = fmt::format("mtdbat5u r{}", rD);
  string mtdbat5l_str = fmt::format("mtdbat5l r{}", rD);
  string mtdbat6u_str = fmt::format("mtdbat6u r{}", rD);
  string mtdbat6l_str = fmt::format("mtdbat6l r{}", rD);
  string mtdbat7u_str = fmt::format("mtdbat7u r{}", rD);
  string mtdbat7l_str = fmt::format("mtdbat7l r{}", rD);
  string mtgqr0_str = fmt::format("mtgqr0 r{}", rD);
  string mtgqr1_str = fmt::format("mtgqr1 r{}", rD);
  string mtgqr2_str = fmt::format("mtgqr2 r{}", rD);
  string mtgqr3_str = fmt::format("mtgqr3 r{}", rD);
  string mtgqr4_str = fmt::format("mtgqr4 r{}", rD);
  string mtgqr5_str = fmt::format("mtgqr5 r{}", rD);
  string mtgqr6_str = fmt::format("mtgqr6 r{}", rD);
  string mtgqr7_str = fmt::format("mtgqr7 r{}", rD);
  string mthid2_str = fmt::format("mthid2 r{}", rD);
  string mtwpar_str = fmt::format("mtwpar r{}", rD);
  string mtdma_u_str = fmt::format("mtdma_u r{}", rD);
  string mtdma_l_str = fmt::format("mtdma_l r{}", rD);
  string mtummcr0_str = fmt::format("mtummcr0 r{}", rD);
  string mtupmc1_str = fmt::format("mtupmc1 r{}", rD);
  string mtupmc2_str = fmt::format("mtupmc2 r{}", rD);
  string mtusia_str = fmt::format("mtusia r{}", rD);
  string mtummcr1_str = fmt::format("mtummcr1 r{}", rD);
  string mtupmc3_str = fmt::format("mtupmc3 r{}", rD);
  string mtupmc4_str = fmt::format("mtupmc4 r{}", rD);
  string mtusda_str = fmt::format("mtusda r{}", rD);
  string mtmmcr0_str = fmt::format("mtmmcr0 r{}", rD);
  string mtpmc1_str = fmt::format("mtpmc1 r{}", rD);
  string mtpmc2_str = fmt::format("mtpmc2 r{}", rD);
  string mtsia_str = fmt::format("mtsia r{}", rD);
  string mtmmcr1_str = fmt::format("mtmmcr1 r{}", rD);
  string mtpmc3_str = fmt::format("mtpmc3 r{}", rD);
  string mtpmc4_str = fmt::format("mtpmc4 r{}", rD);
  string mtsda_str = fmt::format("mtsda r{}", rD);
  string mthid0_str = fmt::format("mthid0 r{}", rD);
  string mthid1_str = fmt::format("mthid1 r{}", rD);
  string mtiabr_str = fmt::format("mtiabr r{}", rD);
  string mthid4_str = fmt::format("mthid4 r{}", rD);
  string mtdabr_str = fmt::format("mtdabr r{}", rD);
  string mtl2cr_str = fmt::format("mtl2cr r{}", rD);
  string mtictc_str = fmt::format("mtictc r{}", rD);
  string mtthrm1_str = fmt::format("mtthrm1 r{}", rD);
  string mtthrm2_str = fmt::format("mtthrm2 r{}", rD);
  string mtthrm3_str = fmt::format("mtthrm3 r{}", rD);
  
  //Continue on
  string nop_str = "nop";
  string not_str = fmt::format("not r{}, r{}", rA, rS);
  string notRC_str = fmt::format("not. r{}, r{}", rA, rS);
  string rotlw_str = fmt::format("rotlw r{}, r{}, r{}", rA, rS, rB); //no point doing rotrwX
  string rotlwRC_str = fmt::format("rotlw r{}, r{}, r{}", rA, rS, rB);
  string rotlwi_str = fmt::format("rotlwi r{}, r{}, {}", rA, rS, SH); //no point doing rotrwiX
  string rotlwiRC_str = fmt::format("rotlwi. r{}, r{}, {}", rA, rS, SH);
  string slwi_str = fmt::format("slwi r{}, r{}, {}", rA, rS, SH); //slwi rA, rS, n where n = SH
  string slwiRC_str = fmt::format("slwi. r{}, r{}, {}", rA, rS, SH); //slwi. rA rS, n where n = SH
  string srwi_str = fmt::format("srwi r{}, r{}, {}", rA, rS, MB); //srwi rA, rS, n where n = MB
  string srwiRC_str = fmt::format("srwi. r{}, r{}, {}", rA, rS, MB); //srwi. rA, rS, n where n = MB
  string sub_str = fmt::format("sub r{}, r{}, r{}", rD, rB, rA); //rA and rB swapped
  string subRC_str = fmt::format("sub. r{}, r{}, r{}", rD, rB, rA); //rA and rB swapped
  string subOE_str = fmt::format("subo r{}, r{}, r{}", rD, rB, rA); //rA and rB swapped
  string subOERC_str = fmt::format("subo. r{}, r{}, r{}", rD, rB, rA); //rA and rB swapped
  string subc_str = fmt::format("subc r{}, r{}, r{}", rD, rB, rA); //rA and rB swapped
  string subcRC_str = fmt::format("subc. r{}, r{}, r{}", rD, rB, rA); //rA and rB swapped
  string subcOE_str = fmt::format("subco r{}, r{}, r{}", rD, rB, rA); //rA and rB swapped
  string subcOERC_str = fmt::format("subco. r{}, r{}, r{}", rD, rB, rA); //rA and rB swapped
  string subic_str = fmt::format("subic r{}, r{}, 0x{:X}", rD, rA, SIMM);
  string subicRC_str = fmt::format("subic. r{}, r{}, 0x{:X}", rD, rA, SIMM);
  string trap_str = "trap";
  
    if (first_check(ppcinstructb22_30, addX) == 0) {
        goto addXfound;
    }
    else if (first_check(ppcinstructb22_30, addcX) == 0) {
        goto addcXfound;
    }
    else if (first_check(ppcinstructb22_30, addeX) == 0) {
        goto addeXfound;
    }
    else if (first_check(ppcClrOpcode, addi) == 0) {
        goto addifound;
    } 
    else if (first_check(ppcClrOpcode, addic) == 0) {
        goto addicfound;
    }
    else if (first_check(ppcClrOpcode, addicRC) == 0) {
        goto addicRCfound;
    }
    else if (first_check(ppcClrOpcode, addis) == 0) {
        goto addisfound;
    }
    else if ((first_check(ppcinstructb22_30, addmeX) == 0) && (second_check(check_nullrB) == 0))  {
        goto addmeXfound;
    }
    else if ((first_check(ppcinstructb22_30, addzeX) == 0) && (second_check(check_nullrB) == 0))  {
        goto addzeXfound;
    }
    else if (first_check(ppcinstructb21_30, andX) == 0) {
        goto andXfound;
    }
    else if (first_check(ppcinstructb21_30, andcX) == 0) {
        goto andcXfound;
    }
    else if (first_check(ppcClrOpcode, andiRC) == 0) {
        goto andiRCfound;
    }
    else if (first_check(ppcClrOpcode, andisRC) == 0) {
        goto andisRCfound;
    }
    else if (first_check(ppcClrOpcode, bX) == 0) {
        goto bXfound;
    }
    else if (first_check(ppcClrOpcode, bcX) == 0) {
        goto bcXfound;
    }
    else if ((first_check(ppcinstructb21_30, bcctrX) == 0) && (second_check(check_nullrB) == 0))  {
        goto bcctrXfound;
    }
    else if ((first_check(ppcinstructb21_30, bclrX) == 0) && (second_check(check_nullrB) == 0))  {
        goto bclrXfound;
    }
    else if ((first_check(ppcClrOpcode, cmpw) == 0) && (second_check(check_cmps1) == 0))  {
        goto cmpwfound;
    }
    else if ((first_check(ppcClrOpcode, cmpwi) == 0) && (second_check(check_cmpXwi) == 0))  {
        goto cmpwifound;
    }
    else if ((first_check(ppcinstructb21_30, cmplw) == 0) && (second_check(check_cmps2) == 0))  {
        goto cmplwfound;
    }
    else if ((first_check(ppcClrOpcode, cmplwi) == 0) && (second_check(check_cmpXwi) == 0))  {
        goto cmplwifound;
    }
    else if ((first_check(ppcinstructb21_30, cntlzwX) == 0) && (second_check(check_nullrB) == 0))  {
        goto cntlzwXfound;
    }
    else if ((first_check(ppcinstructb21_30, crand) == 0) && (second_check(check_cr_ops) == 0))  {
        goto crandfound;
    }
    else if ((first_check(ppcinstructb21_30, crandc) == 0) && (second_check(check_cr_ops) == 0))  {
        goto crandcfound;
    }
    else if ((first_check(ppcinstructb21_30, creqv) == 0) && (second_check(check_cr_ops) == 0))  {
        goto creqvfound;
    }
    else if ((first_check(ppcinstructb21_30, crnand) == 0) && (second_check(check_cr_ops) == 0))  {
        goto crnandfound;
    }
    else if ((first_check(ppcinstructb21_30, crnor) == 0) && (second_check(check_cr_ops) == 0))  {
        goto crnorfound;
    }
    else if ((first_check(ppcinstructb21_30, cror) == 0) && (second_check(check_cr_ops) == 0))  {
        goto crorfound;
    }
    else if ((first_check(ppcinstructb21_30, crorc) == 0) && (second_check(check_cr_ops) == 0))  {
        goto crorcfound;
    }
    else if ((first_check(ppcinstructb21_30, crxor) == 0) && (second_check(check_cr_ops) == 0))  {
        goto crxorfound;
    }
    else if ((first_check(ppcinstructb21_30, dcbf) == 0) && (second_check(check_cache) == 0))  {
        goto dcbffound;
    }
    else if ((first_check(ppcinstructb21_30, dcbi) == 0) && (second_check(check_cache) == 0))  {
        goto dcbifound;
    }
    else if ((first_check(ppcinstructb21_30, dcbst) == 0) && (second_check(check_cache) == 0))  {
        goto dcbstfound;
    }
    else if ((first_check(ppcinstructb21_30, dcbt) == 0) && (second_check(check_cache) == 0))  {
        goto dcbtfound;
    }
    else if ((first_check(ppcinstructb21_30, dcbtst) == 0) && (second_check(check_cache) == 0))  {
        goto dcbtstfound;
    }
    else if ((first_check(ppcinstructb21_30, dcbz) == 0) && (second_check(check_cache) == 0))  {
        goto dcbzfound;
    }
    else if ((first_check(ppcinstructb21_30, dcbz_l) == 0) && (second_check(check_cache) == 0))  {
        goto dcbz_lfound;
    }
    else if (first_check(ppcinstructb22_30, divwX) == 0) {
        goto divwXfound;
    }
    else if (first_check(ppcinstructb22_30, divwuX) == 0) {
        goto divwuXfound;
    }
    else if ((first_check(ppcinstructb21_30, eciwx) == 0) && (second_check(check_b31) == 0))  {
        goto eciwxfound;
    }
    else if ((first_check(ppcinstructb21_30, ecowx) == 0) && (second_check(check_b31) == 0))  {
        goto ecowxfound;
    }
    else if ((first_check(ppcinstructb21_30, eieio) == 0) && (second_check(check_syncs) == 0))  {
        goto eieiofound;
    }
    else if (first_check(ppcinstructb21_30, eqvX) == 0) {
        goto eqvXfound;
    }
    else if ((first_check(ppcinstructb21_30, extsbX) == 0) && (second_check(check_nullrB) == 0))  {
        goto extsbXfound;
    }
    else if ((first_check(ppcinstructb21_30, extshX) == 0) && (second_check(check_nullrB) == 0))  {
        goto extshXfound;
    }
    else if ((first_check(ppcinstructb21_30, fabsX) == 0) && (second_check(check_nullrA) == 0))  {
        goto fabsXfound;
    }
    else if ((first_check(ppcinstructb26_30, faddX) == 0) && (second_check(check_nullfC) == 0))  {
        goto faddXfound;
    }
    else if ((first_check(ppcinstructb26_30, faddsX) == 0) && (second_check(check_nullfC) == 0))  {
        goto faddsXfound;
    }
    else if ((first_check(ppcinstructb21_30, fcmpo) == 0) && (second_check(check_cmps2) == 0))  {
        goto fcmpofound;
    }
    else if ((first_check(ppcinstructb21_30, fcmpu) == 0) && (second_check(check_cmps1) == 0))  {
        goto fcmpufound;
    }
    else if ((first_check(ppcinstructb21_30, fctiwX) == 0) && (second_check(check_nullrA) == 0))  {
        goto fctiwXfound;
    }
    else if ((first_check(ppcinstructb21_30, fctiwzX) == 0) && (second_check(check_nullrA) == 0))  {
        goto fctiwzXfound;
    }
    else if ((first_check(ppcinstructb26_30, fdivX) == 0) && (second_check(check_nullfC) == 0))  {
        goto fdivXfound;
    }
    else if ((first_check(ppcinstructb26_30, fdivsX) == 0) && (second_check(check_nullfC) == 0))  {
        goto fdivsXfound;
    }
    else if (first_check(ppcinstructb26_30, fmaddX) == 0) {
        goto fmaddXfound;
    }
    else if (first_check(ppcinstructb26_30, fmaddsX) == 0) {
        goto fmaddsXfound;
    }
    else if ((first_check(ppcinstructb21_30, fmrX) == 0) && (second_check(check_nullrA) == 0))  {
        goto fmrXfound;
    }
    else if (first_check(ppcinstructb26_30, fmsubX) == 0) {
        goto fmsubXfound;
    }
    else if (first_check(ppcinstructb26_30, fmsubsX) == 0) {
        goto fmsubsXfound;
    }
    else if ((first_check(ppcinstructb26_30, fmulX) == 0) && (second_check(check_nullrB) == 0))  {
        goto fmulXfound;
    }
    else if ((first_check(ppcinstructb26_30, fmulsX) == 0) && (second_check(check_nullrB) == 0))  {
        goto fmulsXfound;
    }
    else if ((first_check(ppcinstructb21_30, fnabsX) == 0) && (second_check(check_nullrA) == 0))  {
        goto fnabsXfound;
    }
    else if ((first_check(ppcinstructb21_30, fnegX) == 0) && (second_check(check_nullrA) == 0))  {
        goto fnegXfound;
    }
    else if (first_check(ppcinstructb26_30, fnmaddX) == 0) {
        goto fnmaddXfound;
    }
    else if (first_check(ppcinstructb26_30, fnmaddsX) == 0) {
        goto fnmaddsXfound;
    }
    else if (first_check(ppcinstructb26_30, fnmsubX) == 0) {
        goto fnmsubXfound;
    }
    else if (first_check(ppcinstructb26_30, fnmsubsX) == 0) {
        goto fnmsubsXfound;
    }
    else if ((first_check(ppcinstructb26_30, fresX) == 0) && (second_check(check_res_sqrt) == 0))  {
        goto fresXfound;
    }
    else if ((first_check(ppcinstructb21_30, frspX) == 0) && (second_check(check_nullrA) == 0))  {
        goto frspXfound;
    }
    else if ((first_check(ppcinstructb26_30, frsqrteX) == 0) && (second_check(check_res_sqrt) == 0))  {
        goto frsqrteXfound;
    }
    else if (first_check(ppcinstructb26_30, fselX) == 0) {
        goto fselXfound;
    }
    else if ((first_check(ppcinstructb26_30, fsubX) == 0) && (second_check(check_nullfC) == 0))  {
        goto fsubXfound;
    }
    else if ((first_check(ppcinstructb26_30, fsubsX) == 0) && (second_check(check_nullfC) == 0))  {
        goto fsubsXfound;
    }
    else if ((first_check(ppcinstructb21_30, icbi) == 0) && (second_check(check_cache) == 0))  {
        goto icbifound;
    }
    else if ((first_check(ppcinstructb21_30, isync) == 0) && (second_check(check_syncs) == 0))  {
        goto isyncfound;
    }
    else if (first_check(ppcClrOpcode, lbz) == 0) {
        goto lbzfound;
    }
    else if ((first_check(ppcClrOpcode, lbzu) == 0) && (lXzu_lXzux_special_check() == 0))  {
        goto lbzufound;
    }
    else if ((first_check(ppcinstructb21_30, lbzux) == 0) && (second_check(check_b31) == 0) && (lXzu_lXzux_special_check() == 0))  {
        goto lbzuxfound;
    }
    else if ((first_check(ppcinstructb21_30, lbzx) == 0) && (second_check(check_b31) == 0))  {
        goto lbzxfound;
    }
    else if (first_check(ppcClrOpcode, lfd) == 0) {
        goto lfdfound;
    }
    else if ((first_check(ppcClrOpcode, lfdu) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto lfdufound;
    }
    else if ((first_check(ppcinstructb21_30, lfdux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto lfduxfound;
    }
    else if ((first_check(ppcinstructb21_30, lfdx) == 0) && (second_check(check_b31) == 0))  {
        goto lfdxfound;
    }
    else if (first_check(ppcClrOpcode, lfs) == 0) {
        goto lfsfound;
    }
    else if ((first_check(ppcClrOpcode, lfsu) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto lfsufound;
    }
    else if ((first_check(ppcinstructb21_30, lfsux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto lfsuxfound;
    }
    else if ((first_check(ppcinstructb21_30, lfsx) == 0) && (second_check(check_b31) == 0))  {
        goto lfsxfound;
    }
    else if (first_check(ppcClrOpcode, lha) == 0) {
        goto lhafound;
    }
    else if ((first_check(ppcClrOpcode, lhau) == 0) && (lXzu_lXzux_special_check() == 0))  {
        goto lhaufound;
    }
    else if ((first_check(ppcinstructb21_30, lhaux) == 0) && (second_check(check_b31) == 0) && (lXzu_lXzux_special_check() == 0))  {
        goto lhauxfound;
    }
    else if ((first_check(ppcinstructb21_30, lhax) == 0) && (second_check(check_b31) == 0))  {
        goto lhaxfound;
    }
    else if ((first_check(ppcinstructb21_30, lhbrx) == 0) && (second_check(check_b31) == 0))  {
        goto lhbrxfound;
    }
    else if (first_check(ppcClrOpcode, lhz) == 0) {
        goto lhzfound;
    }
    else if ((first_check(ppcClrOpcode, lhzu) == 0) && (lXzu_lXzux_special_check() == 0))  {
        goto lhzufound;
    }
    else if ((first_check(ppcinstructb21_30, lhzux) == 0) && (second_check(check_b31) == 0) && (lXzu_lXzux_special_check() == 0))  {
        goto lhzuxfound;
    }
    else if ((first_check(ppcinstructb21_30, lhzx) == 0) && (second_check(check_b31) == 0))  {
        goto lhzxfound;
    }
    else if ((first_check(ppcClrOpcode, lmw) == 0) && (lmw_specialcheck() == 0))  {
        goto lmwfound;
    }
    else if ((first_check(ppcinstructb21_30, lswi) == 0) && (second_check(check_b31) == 0) && (lswi_specialcheck() == 0))  {
        goto lswifound;
    }
    else if ((first_check(ppcinstructb21_30, lswx) == 0) && (second_check(check_b31) == 0))  {
        goto lswxfound;
    }
    else if ((first_check(ppcinstructb21_30, lwarx) == 0) && (second_check(check_b31) == 0))  {
        goto lwarxfound;
    }
    else if ((first_check(ppcinstructb21_30, lwbrx) == 0) && (second_check(check_b31) == 0))  {
        goto lwbrxfound;
    }
    else if (first_check(ppcClrOpcode, lwz) == 0) {
        goto lwzfound;
    }
    else if ((first_check(ppcClrOpcode, lwzu) == 0) && (lXzu_lXzux_special_check() == 0))  {
        goto lwzufound;
    }
    else if ((first_check(ppcinstructb21_30, lwzux) == 0) && (second_check(check_b31) == 0) && (lXzu_lXzux_special_check() == 0))  {
        goto lwzuxfound;
    }
    else if ((first_check(ppcinstructb21_30, lwzx) == 0) && (second_check(check_b31) == 0))  {
        goto lwzxfound;
    }
    else if ((first_check(ppcClrOpcode, mcrf) == 0) && (second_check(check_mcrf) == 0))  {
        goto mcrffound;
    }
    else if ((first_check(ppcinstructb21_30, mcrfs) == 0) && (second_check(check_mcrfs) == 0))  {
        goto mcrfsfound;
    }
    else if ((first_check(ppcinstructb21_30, mcrxr) == 0) && (second_check(check_mcrxr) == 0))  {
        goto mcrxrfound;
    }
    else if ((first_check(ppcinstructb21_30, mfcr) == 0) && (second_check(check_cr_msr) == 0))  {
        goto mfcrfound;
    }
    else if ((first_check(ppcinstructb21_30, mffsX) == 0) && (second_check(check_fs_fsb) == 0))  {
        goto mffsXfound;
    }
    else if ((first_check(ppcinstructb21_30, mfmsr) == 0) && (second_check(check_cr_msr) == 0))  {
        goto mfmsrfound;
    }
    else if ((first_check(ppcinstructb21_30, mfspr) == 0) && (second_check(check_b31) == 0))  {
        goto mfsprfound;
    }
    else if ((first_check(ppcinstructb21_30, mfsr) == 0) && (second_check(check_sr) == 0))  {
        goto mfsrfound;
    }
    else if ((first_check(ppcinstructb21_30, mfsrin) == 0) && (second_check(check_srin) == 0))  {
        goto mfsrinfound;
    }
    else if ((first_check(ppcinstructb21_30, mftb) == 0) && (second_check(check_b31) == 0))  {
        goto mftbfound;
    }
    else if ((first_check(ppcinstructb21_30, mtcrf) == 0) && (second_check(check_mtcrf) == 0))  {
        goto mtcrffound;
    }
    else if ((first_check(ppcinstructb21_30, mtfsb0X) == 0) && (second_check(check_fs_fsb) == 0))  {
        goto mtfsb0Xfound;
    }
    else if ((first_check(ppcinstructb21_30, mtfsb1X) == 0) && (second_check(check_fs_fsb) == 0))  {
        goto mtfsb1Xfound;
    }
    else if ((first_check(ppcinstructb21_30, mtfsfX) == 0) && (second_check(check_mtfsfX) == 0))  {
        goto mtfsfXfound;
    }
    else if ((first_check(ppcinstructb21_30, mtfsfiX) == 0) && (second_check(check_mtfsfiX) == 0))  {
        goto mtfsfiXfound;
    }
    else if ((first_check(ppcinstructb21_30, mtmsr) == 0) && (second_check(check_cr_msr) == 0))  {
        goto mtmsrfound;
    }
    else if ((first_check(ppcinstructb21_30, mtspr) == 0) && (second_check(check_b31) == 0))  {
        goto mtsprfound;
    }
    else if ((first_check(ppcinstructb21_30, mtsr) == 0) && (second_check(check_sr) == 0))  {
        goto mtsrfound;
    }
    else if ((first_check(ppcinstructb21_30, mtsrin) == 0) && (second_check(check_srin) == 0))  {
        goto mtsrinfound;
    }
    else if ((first_check(ppcinstructb22_30, mulhwX) == 0) && (second_check(check_b21) == 0))  {
        goto mulhwXfound;
    }
    else if ((first_check(ppcinstructb22_30, mulhwuX) == 0) && (second_check(check_b21) == 0))  {
        goto mulhwuXfound;
    }
    else if (first_check(ppcClrOpcode, mulli) == 0) {
        goto mullifound;
    }
    else if (first_check(ppcinstructb22_30, mullwX) == 0) {
        goto mullwXfound;
    }
    else if (first_check(ppcinstructb21_30, nandX) == 0) {
        goto nandXfound;
    }
    else if ((first_check(ppcinstructb22_30, negX) == 0) && (second_check(check_nullrB) == 0))  {
        goto negXfound;
    }
    else if (first_check(ppcinstructb21_30, norX) == 0) {
        goto norXfound;
    }
    else if (first_check(ppcinstructb21_30, orX) == 0) {
        goto orXfound;
    }
    else if (first_check(ppcinstructb21_30, orcX) == 0) {
        goto orcXfound;
    }
    else if (first_check(ppcClrOpcode, ori) == 0) {
        goto orifound;
    }
    else if (first_check(ppcClrOpcode, oris) == 0) {
        goto orisfound;
    }
    else if (first_check(ppcClrOpcode, psq_l) == 0) {
        goto psq_lfound;
    }
    else if ((first_check(ppcClrOpcode, psq_lu) == 0) && (float_ps_update_and_store_update_special_check() == 0)) {
        goto psq_lufound;
    }
    else if ((first_check(ppcinstructb25_30, psq_lux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto psq_luxfound;
    }
    else if ((first_check(ppcinstructb25_30, psq_lx) == 0) && (second_check(check_b31) == 0))  {
        goto psq_lxfound;
    }
    else if (first_check(ppcClrOpcode, psq_st) == 0) {
        goto psq_stfound;
    }
    else if ((first_check(ppcClrOpcode, psq_stu) == 0) && (float_ps_update_and_store_update_special_check() == 0)) {
        goto psq_stufound;
    }
    else if ((first_check(ppcinstructb25_30, psq_stux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto psq_stuxfound;
    }
    else if ((first_check(ppcinstructb25_30, psq_stx) == 0) && (second_check(check_b31) == 0))  {
        goto psq_stxfound;
    }
    else if ((first_check(ppcinstructb21_30, ps_absX) == 0) && (second_check(check_nullrA) == 0))  {
        goto ps_absXfound;
    }
    else if ((first_check(ppcinstructb26_30, ps_addX) == 0) && (second_check(check_nullfC) == 0))  {
        goto ps_addXfound;
    }
    else if ((first_check(ppcinstructb21_30, ps_cmpo0) == 0) && (second_check(check_cmps2) == 0))  {
        goto ps_cmpo0found;
    }
    else if ((first_check(ppcinstructb21_30, ps_cmpo1) == 0) && (second_check(check_cmps2) == 0))  {
        goto ps_cmpo1found;
    }
    else if ((first_check(ppcClrOpcode, ps_cmpu0) == 0) && (second_check(check_cmps1) == 0))  {
        goto ps_cmpu0found;
    }
    else if ((first_check(ppcinstructb21_30, ps_cmpu1) == 0) && (second_check(check_cmps2) == 0))  {
        goto ps_cmpu1found;
    }
    else if ((first_check(ppcinstructb26_30, ps_divX) == 0) && (second_check(check_nullfC) == 0))  {
        goto ps_divXfound;
    }
    else if (first_check(ppcinstructb26_30, ps_maddX) == 0) {
        goto ps_maddXfound;
    }
    else if (first_check(ppcinstructb26_30, ps_madds0X) == 0) {
        goto ps_madds0Xfound;
    }
    else if (first_check(ppcinstructb26_30, ps_madds1X) == 0) {
        goto ps_madds1Xfound;
    }
    else if (first_check(ppcinstructb21_30, ps_merge00X) == 0) {
        goto ps_merge00Xfound;
    }
    else if (first_check(ppcinstructb21_30, ps_merge01X) == 0) {
        goto ps_merge01Xfound;
    }
    else if (first_check(ppcinstructb21_30, ps_merge10X) == 0) {
        goto ps_merge10Xfound;
    }
    else if (first_check(ppcinstructb21_30, ps_merge11X) == 0) {
        goto ps_merge11Xfound;
    }
    else if ((first_check(ppcinstructb21_30, ps_mrX) == 0) && (second_check(check_nullrA) == 0))  {
        goto ps_mrXfound;
    }
    else if (first_check(ppcinstructb26_30, ps_msubX) == 0) {
        goto ps_msubXfound;
    }
    else if ((first_check(ppcinstructb26_30, ps_mulX) == 0) && (second_check(check_nullrB) == 0))  {
        goto ps_mulXfound;
    }
    else if ((first_check(ppcinstructb26_30, ps_muls0X) == 0) && (second_check(check_nullrB) == 0))  {
        goto ps_muls0Xfound;
    }
    else if ((first_check(ppcinstructb26_30, ps_muls1X) == 0) && (second_check(check_nullrB) == 0))  {
        goto ps_muls1Xfound;
    }
    else if ((first_check(ppcinstructb21_30, ps_nabsX) == 0) && (second_check(check_nullrA) == 0))  {
        goto ps_nabsXfound;
    }
    else if ((first_check(ppcinstructb21_30, ps_negX) == 0) && (second_check(check_nullrA) == 0))  {
        goto ps_negXfound;
    }
    else if (first_check(ppcinstructb26_30, ps_nmaddX) == 0) {
        goto ps_nmaddXfound;
    }
    else if (first_check(ppcinstructb26_30, ps_nmsubX) == 0) {
        goto ps_nmsubXfound;
    }
    else if ((first_check(ppcinstructb26_30, ps_resX) == 0) && (second_check(check_res_sqrt) == 0))  {
        goto ps_resXfound;
    }
    else if ((first_check(ppcinstructb26_30, ps_rsqrteX) == 0) && (second_check(check_res_sqrt) == 0))  {
        goto ps_rsqrteXfound;
    }
    else if (first_check(ppcinstructb26_30, ps_selX) == 0) {
        goto ps_selXfound;
    }
    else if ((first_check(ppcinstructb26_30, ps_subX) == 0) && (second_check(check_nullfC) == 0))  {
        goto ps_subXfound;
    }
    else if (first_check(ppcinstructb26_30, ps_sum0X) == 0) {
        goto ps_sum0Xfound;
    }
    else if (first_check(ppcinstructb26_30, ps_sum1X) == 0) {
        goto ps_sum1Xfound;
    }
    else if ((first_check(ppcinstructb21_30, rfi) == 0) && (second_check(check_syncs) == 0))  {
        goto rfifound;
    }
    else if (first_check(ppcClrOpcode, rlwimiX) == 0) {
        goto rlwimiXfound;
    }
    else if (first_check(ppcClrOpcode, rlwinmX) == 0) {
        goto rlwinmXfound;
    }
    else if (first_check(ppcClrOpcode, rlwnmX) == 0) {
        goto rlwnmXfound;
    }
    else if ((first_check(ppcinstructb30, sc) == 0) && (second_check(check_sc) == 0))  {
        goto scfound;
    }
    else if (first_check(ppcinstructb21_30, slwX) == 0) {
        goto slwXfound;
    }
    else if (first_check(ppcinstructb21_30, srawX) == 0) {
        goto srawXfound;
    }
    else if (first_check(ppcinstructb21_30, srawiX) == 0) {
        goto srawiXfound;
    }
    else if (first_check(ppcinstructb21_30, srwX) == 0) {
        goto srwXfound;
    }
    else if (first_check(ppcClrOpcode, stb) == 0) {
        goto stbfound;
    }
    else if ((first_check(ppcClrOpcode, stbu) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto stbufound;
    }
    else if ((first_check(ppcinstructb21_30, stbux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto stbuxfound;
    }
    else if ((first_check(ppcinstructb21_30, stbx) == 0) && (second_check(check_b31) == 0))  {
        goto stbxfound;
    }
    else if (first_check(ppcClrOpcode, stfd) == 0) {
        goto stfdfound;
    }
    else if ((first_check(ppcClrOpcode, stfdu) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto stfdufound;
    }
    else if ((first_check(ppcinstructb21_30, stfdux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto stfduxfound;
    }
    else if ((first_check(ppcinstructb21_30, stfdx) == 0) && (second_check(check_b31) == 0))  {
        goto stfdxfound;
    }
    else if ((first_check(ppcinstructb21_30, stfiwX) == 0) && (second_check(check_b31) == 0))  {
        goto stfiwxfound;
    }
    else if (first_check(ppcClrOpcode, stfs) == 0) {
        goto stfsfound;
    }
    else if ((first_check(ppcClrOpcode, stfsu) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto stfsufound;
    }
    else if ((first_check(ppcinstructb21_30, stfsux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto stfsuxfound;
    }
    else if ((first_check(ppcinstructb21_30, stfsx) == 0) && (second_check(check_b31) == 0))  {
        goto stfsxfound;
    }
    else if (first_check(ppcClrOpcode, sth) == 0) {
        goto sthfound;
    }
    else if ((first_check(ppcinstructb21_30, sthbrx) == 0) && (second_check(check_b31) == 0))  {
        goto sthbrxfound;
    }
    else if ((first_check(ppcClrOpcode, sthu) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto sthufound;
    }
    else if ((first_check(ppcinstructb21_30, sthux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto sthuxfound;
    }
    else if ((first_check(ppcinstructb21_30, sthx) == 0) && (second_check(check_b31) == 0))  {
        goto sthxfound;
    }
    else if (first_check(ppcClrOpcode, stmw) == 0) {
        goto stmwfound;
    }
    else if ((first_check(ppcinstructb21_30, stswi) == 0) && (second_check(check_b31) == 0))  {
        goto stswifound;
    }
    else if ((first_check(ppcinstructb21_30, stswx) == 0) && (second_check(check_b31) == 0))  {
        goto stswxfound;
    }
    else if (first_check(ppcClrOpcode, stw) == 0) {
        goto stwfound;
    }
    else if ((first_check(ppcinstructb21_30, stwbrx) == 0) && (second_check(check_b31) == 0))  {
        goto stwbrxfound;
    }
    else if (first_check(ppcinstructb21_30, stwcxRC) == 0) {
        goto stwcxRCfound;
    }
    else if ((first_check(ppcClrOpcode, stwu) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto stwufound;
    }
    else if ((first_check(ppcinstructb21_30, stwux) == 0) && (second_check(check_b31) == 0) && (float_ps_update_and_store_update_special_check() == 0))  {
        goto stwuxfound;
    }
    else if ((first_check(ppcinstructb21_30, stwx) == 0) && (second_check(check_b31) == 0))  {
        goto stwxfound;
    }
    else if (first_check(ppcinstructb22_30, subX) == 0) {
        goto subXfound;
    }
    else if (first_check(ppcinstructb22_30, subcX) == 0) {
        goto subcXfound;
    }
    else if (first_check(ppcinstructb22_30, subfeX) == 0) {
        goto subfeXfound;
    }
    else if (first_check(ppcClrOpcode, subfic) == 0) {
        goto subficfound;
    }
    else if ((first_check(ppcinstructb22_30, subfmeX) == 0) && (second_check(check_nullrB) == 0))  {
        goto subfmeXfound;
    }
    else if ((first_check(ppcinstructb22_30, subfzeX) == 0) && (second_check(check_nullrB) == 0))  {
        goto subfzeXfound;
    }
    else if ((first_check(ppcinstructb21_30, sync) == 0) && (second_check(check_syncs) == 0))  {
        goto syncfound;
    }
    else if ((first_check(ppcinstructb21_30, tlbie) == 0) && (second_check(check_tlbie) == 0))  {
        goto tlbiefound;
    }
    else if ((first_check(ppcinstructb21_30, tlbsync) == 0) && (second_check(check_syncs) == 0))  {
        goto tlbsyncfound;
    }
    else if ((first_check(ppcinstructb21_30, tw) == 0) && (second_check(check_b31) == 0))  {
        goto twfound;
    }
    else if (first_check(ppcClrOpcode, twi) == 0) {
        goto twifound;
    }
    else if (first_check(ppcinstructb21_30, xorX) == 0) {
        goto xorXfound;
    }
    else if (first_check(ppcClrOpcode, xori) == 0) {
        goto xorifound;
    }
    else if (first_check(ppcClrOpcode, xoris) == 0) {
        goto xorisfound;
    }
    else {
        invalid:
        return illegal;
    }

    addXfound:
    switch (oerc) {
        case 0:
            return add_str;
            break;
        case 1:
            return addRC_str;
            break;
        case 0x400:
            return addOE_str;
            break;
        default: // 0x0401
            return addOERC_str;
            break;
    }
    
    addcXfound:
    switch (oerc) {
        case 0:
            return addc_str;
            break;
        case 1:
            return addcRC_str;
            break;
        case 0x400:
            return addcOE_str;
            break;
        default: // 0x0401
            return addcOERC_str;
            break;
    }
    
    addeXfound:
     switch (oerc) {
        case 0:
            return adde_str;
            break;
        case 1:
            return addeRC_str;
            break;
        case 0x400:
            return addeOE_str;
            break;
        default: // 0x0401
            return addeOERC_str;
            break;
    }
    
    addifound:
    switch (rA) {
                case 0:
                    return li_str;
                    break;
                default:
                    return addi_str;
                    break;
    }
    
    addicfound:
    return addic_str;
    addicRCfound:
    return addicRC_str;
    
    addisfound:
    switch (rA) {
                case 0:
                    return lis_str;
                    break;
                default:
                    return addis_str;
                    break;
    }
    
    addmeXfound:
    switch (oerc) {
        case 0:
            return addme_str;
            break;
        case 1:
            return addmeRC_str;
            break;
        case 0x400:
            return addmeOE_str;
            break;
        default: // 0x0401
            return addmeOERC_str;
            break;
    }
    
    addzeXfound:
    switch (oerc) {
        case 0:
            return addze_str;
            break;
        case 1:
            return addzeRC_str;
            break;
        case 0x400:
            return addzeOE_str;
            break;
        default: // 0x0401
            return addzeOERC_str;
            break;
    }
    
    andXfound:
    switch (rc) {
        case 0:
            return and_str;
            break;
        default: // rc
            return andRC_str;
            break;
    }
    
    andcXfound:
    switch (rc) {
                case 0:
                    return andc_str;
                    break;
                default: // rc
                    return andcRC_str;
                    break;
    }
    
    andiRCfound:
    return andiRC_str;
    
    andisRCfound:
    return andisRC_str;
    
    bXfound:
    switch (aalk) {
                case 0:
                    return b_str;
                    break;
                case 1:
                    return bl_str;
                    break;
                case 2:
                    return ba_str;
                    break;
                default: // 3
                    return bla_str;
                    break;
    }
    
    bcXfound:
switch (BO_nohint) {
        case 0:
            if (bcX_BOhint == 0 && aalk == 0) {
                return bdnzf_str;
            }
            else if (bcX_BOhint == 1 && aalk == 0) {
                return bdnzfplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 1) {
                return bdnzfl_str;
            }
            else if (bcX_BOhint == 1 && aalk == 1) {
                return bdnzflplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 2) {
                return bdnzfa_str;
            }
            else if (bcX_BOhint == 1 && aalk == 2) {
                return bdnzfaplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 3) {
                return bdnzfla_str;
            }
            else { // bcX_BOhint == 1 && aalk == 3
                return bdnzflaplus_str;
            }
            break;
        case 1:
            if (bcX_BOhint == 0 && aalk == 0) {
                return bdzf_str;
            }
            else if (bcX_BOhint == 1 && aalk == 0) {
                return bdzfplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 1) {
                return bdzfl_str;
            }
            else if (bcX_BOhint == 1 && aalk == 1) {
                return bdzflplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 2) {
                return bdzfa_str;
            }
            else if (bcX_BOhint == 1 && aalk == 2) {
                return bdzfaplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 3) {
                return bdzfla_str;
            }
            else { // bcX_BOhint == 1 && aalk == 3
                return bdnzflaplus_str;
            }
            break;
        case 2:
            switch (crfD_manual) {
                case 0: //cr0
                    if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 0) {
                        return bge_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 0) {
                        return bgeplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 0) {
                        return bgel_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 0) {
                        return bgelplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 0) {
                        return bgea_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 0) {
                        return bgeaplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 0) {
                        return bgela_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 0) {
                        return bgelaplus_strcr0;
                    }
                    //
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 1) {
                        return ble_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 1) {
                        return bleplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 1) {
                        return blel_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 1) {
                        return blelplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 1) {
                        return blea_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 1) {
                        return bleaplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 1) {
                        return blela_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 1) {
                        return blelaplus_strcr0;
                    }
                    // 
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 2) {
                        return bne_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 2) {
                        return bneplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 2) {
                        return bnel_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 2) {
                        return bnelplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 2) {
                        return bnea_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 2) {
                        return bneaplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 2) {
                        return bnela_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 2) {
                        return bnelaplus_strcr0;
                    }
                    //
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 3) {
                        return bno_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 3) {
                        return bnoplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 3) {
                        return bnol_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 3) {
                        return bnolplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 3) {
                        return bnoa_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 3) {
                        return bnoaplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 3) {
                        return bnola_strcr0;
                    }
                    else { // bcX_BOhint == 1 && aalk == 3 && findcrbtype == 3
                        return bnolaplus_strcr0;
                    }
                    break;
                default: //not cr0
                    if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 0) {
                        return bge_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 0) {
                        return bgeplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 0) {
                        return bgel_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 0) {
                        return bgelplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 0) {
                        return bgea_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 0) {
                        return bgeaplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 0) {
                        return bgela_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 0) {
                        return bgelaplus_str;
                    }
                    //
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 1) {
                        return ble_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 1) {
                        return bleplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 1) {
                        return blel_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 1) {
                        return blelplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 1) {
                        return blea_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 1) {
                        return bleaplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 1) {
                        return blela_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 1) {
                        return blelaplus_str;
                    }
                    // 
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 2) {
                        return bne_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 2) {
                        return bneplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 2) {
                        return bnel_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 2) {
                        return bnelplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 2) {
                        return bnea_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 2) {
                        return bneaplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 2) {
                        return bnela_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 2) {
                        return bnelaplus_str;
                    }
                    //
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 3) {
                        return bno_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 3) {
                        return bnoplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 3) {
                        return bnol_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 3) {
                        return bnolplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 3) {
                        return bnoa_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 3) {
                        return bnoaplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 3) {
                        return bnola_str;
                    }
                    else { // bcX_BOhint == 1 && aalk == 3 && findcrbtype == 3
                        return bnolaplus_str;
                    }
                    break;
            }
            break;
        case 4:
            if (bcX_BOhint == 0 && aalk == 0) {
                return bdnzt_str;
            }
            else if (bcX_BOhint == 1 && aalk == 0) {
                return bdnztplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 1) {
                return bdnztl_str;
            }
            else if (bcX_BOhint == 1 && aalk == 1) {
                return bdnztlplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 2) {
                return bdnzta_str;
            }
            else if (bcX_BOhint == 1 && aalk == 2) {
                return bdnztaplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 3) {
                return bdnztla_str;
            }
            else { // bcX_BOhint == 1 && aalk == 3
                return bdnztlaplus_str;
            }
            break;
        case 5:
            if (bcX_BOhint == 0 && aalk == 0) {
                return bdzt_str;
            }
            else if (bcX_BOhint == 1 && aalk == 0) {
                return bdztplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 1) {
                return bdztl_str;
            }
            else if (bcX_BOhint == 1 && aalk == 1) {
                return bdztlplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 2) {
                return bdzta_str;
            }
            else if (bcX_BOhint == 1 && aalk == 2) {
                return bdztaplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 3) {
                return bdztla_str;
            }
            else { // bcX_BOhint == 1 && aalk == 3
                return bdztlaplus_str;
            }
            break;
        case 6:
            switch (crfD_manual) {
                case 0: //cr0
                    if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 0) {
                        return blt_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 0) {
                        return bltplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 0) {
                        return bltl_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 0) {
                        return bltlplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 0) {
                        return blta_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 0) {
                        return bltaplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 0) {
                        return bltla_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 0) {
                        return bltlaplus_strcr0;
                    }
                    //
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 1) {
                        return bgt_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 1) {
                        return bgtplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 1) {
                        return bgtl_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 1) {
                        return bgtlplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 1) {
                        return bgta_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 1) {
                        return bgtaplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 1) {
                        return bgtla_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 1) {
                        return bgtlaplus_strcr0;
                    }
                    // 
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 2) {
                        return beq_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 2) {
                        return beqplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 2) {
                        return beql_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 2) {
                        return beqlplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 2) {
                        return beqa_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 2) {
                        return beqaplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 2) {
                        return beqla_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 2) {
                        return beqlaplus_strcr0;
                    }
                    //
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 3) {
                        return bso_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 3) {
                        return bsoplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 3) {
                        return bsol_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 3) {
                        return bsolplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 3) {
                        return bsoa_strcr0;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 3) {
                        return bsoaplus_strcr0;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 3) {
                        return bsola_strcr0;
                    }
                    else { // bcX_BOhint == 1 && aalk == 3 && findcrbtype == 3
                        return bsolaplus_strcr0;
                    }
                    break;
                default: //not cr0
                    if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 0) {
                        return blt_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 0) {
                        return bltplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 0) {
                        return bltl_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 0) {
                        return bltlplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 0) {
                        return blta_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 0) {
                        return bltaplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 0) {
                        return bltla_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 0) {
                        return bltlaplus_str;
                    }
                    //
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 1) {
                        return bgt_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 1) {
                        return bgtplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 1) {
                        return bgtl_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 1) {
                        return bgtlplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 1) {
                        return bgta_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 1) {
                        return bgtaplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 1) {
                        return bgtla_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 1) {
                        return bgtlaplus_str;
                    }
                    // 
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 2) {
                        return beq_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 2) {
                        return beqplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 2) {
                        return beql_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 2) {
                        return beqlplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 2) {
                        return beqa_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 2) {
                        return beqaplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 2) {
                        return beqla_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 3 && findcrbtype == 2) {
                        return beqlaplus_str;
                    }
                    //
                    else if (bcX_BOhint == 0 && aalk == 0 && findcrbtype == 3) {
                        return bso_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 0 && findcrbtype == 3) {
                        return bsoplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 1 && findcrbtype == 3) {
                        return bsol_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 1 && findcrbtype == 3) {
                        return bsolplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 2 && findcrbtype == 3) {
                        return bsoa_str;
                    }
                    else if (bcX_BOhint == 1 && aalk == 2 && findcrbtype == 3) {
                        return bsoaplus_str;
                    }
                    else if (bcX_BOhint == 0 && aalk == 3 && findcrbtype == 3) {
                        return bsola_str;
                    }
                    else { // bcX_BOhint == 1 && aalk == 3 && findcrbtype == 3
                        return bsolaplus_str;
                    }
                    break;
            }
            break;
        case 8:
            if (bcX_BOhint == 0 && aalk == 0) {
                return bdnz_str;
            }
            else if (bcX_BOhint == 1 && aalk == 0) {
                return bdnzplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 1) {
                return bdnzl_str;
            }
            else if (bcX_BOhint == 1 && aalk == 1) {
                return bdnzlplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 2) {
                return bdnza_str;
            }
            else if (bcX_BOhint == 1 && aalk == 2) {
                return bdnzaplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 3) {
                return bdnzla_str;
            }
            else { // bcX_BOhint == 1 && aalk == 3
                return bdnzlaplus_str;
            }
            break;
        case 9:
            if (bcX_BOhint == 0 && aalk == 0) {
                return bdz_str;
            }
            else if (bcX_BOhint == 1 && aalk == 0) {
                return bdzplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 1) {
                return bdzl_str;
            }
            else if (bcX_BOhint == 1 && aalk == 1) {
                return bdzlplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 2) {
                return bdza_str;
            }
            else if (bcX_BOhint == 1 && aalk == 2) {
                return bdzaplus_str;
            }
            else if (bcX_BOhint == 0 && aalk == 3) {
                return bdzla_str;
            }
            else { // bcX_BOhint == 1 && aalk == 3
                return bdzlaplus_str;
            }
            break;
        case 0xA:
            if (aalk == 0) {
                return balt_str;
            }            
            else if (aalk == 1) {
                return blalt_str;
            }
            else if (aalk == 2) {
                return baalt_str;
            }
            else { // aalk == 3
                return blaalt_str;
            }
            break;
        default: //bad BO encoding, we will force requirement of z bits to be cleared 
            goto invalid;
            break;
    }
    
    bcctrXfound:
switch (BO_nohint) {
        case 0:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdnzfctr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdnzfctrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdnzfctrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdnzfctrlplus_str;
            }
            break;
        case 1:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdzfctr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdzfctrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdzfctrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdzfctrlplus_str;
            }
            break;
        case 2:
            switch (crfD_manual) {
                case 0: // cr0
                    if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 0) {
                        return bgectr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 0) {
                        return bgectrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 0) {
                        return bgectrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 0) {
                        return bgectrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 1) {
                        return blectr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 1) {
                        return blectrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 1) {
                        return blectrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 1) {
                        return blectrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 2) {
                        return bnectr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 2) {
                        return bnectrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 2) {
                        return bnectrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 2) {
                        return bnectrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 3) {
                        return bnoctr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 3) {
                        return bnoctrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 3) {
                        return bnoctrl_strcr0;
                    }
                    else { //(BO_onlyhint == 1 && lk == 1 && findcrbtype == 3)
                        return bnoctrlplus_strcr0;
                    }
                    break;
                default: // not cr0
                    if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 0) {
                        return bgectr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 0) {
                        return bgectrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 0) {
                        return bgectrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 0) {
                        return bgectrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 1) {
                        return blectr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 1) {
                        return blectrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 1) {
                        return blectrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 1) {
                        return blectrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 2) {
                        return bnectr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 2) {
                        return bnectrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 2) {
                        return bnectrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 2) {
                        return bnectrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 3) {
                        return bnoctr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 3) {
                        return bnoctrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 3) {
                        return bnoctrl_str;
                    }
                    else { //(BO_onlyhint == 1 && lk == 1 && findcrbtype == 3)
                        return bnoctrlplus_str;
                    }
                    break;
            }
            break;
        case 4:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdnztctr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdnztctrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdnztctrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdnztctrlplus_str;
            }
            break;
        case 5:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdztctr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdztctrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdztctrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdztctrlplus_str;
            }
            break;
        case 6:
            switch (crfD_manual) {
                case 0: // cr0
                    if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 0) {
                        return bltctr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 0) {
                        return bltctrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 0) {
                        return bltctrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 0) {
                        return bltctrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 1) {
                        return bgtctr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 1) {
                        return bgtctrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 1) {
                        return bgtctrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 1) {
                        return bgtctrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 2) {
                        return beqctr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 2) {
                        return beqctrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 2) {
                        return beqctrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 2) {
                        return beqctrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 3) {
                        return bsoctr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 3) {
                        return bsoctrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 3) {
                        return bsoctrl_strcr0;
                    }
                    else { //(BO_onlyhint == 1 && lk == 1 && findcrbtype == 3)
                        return bsoctrlplus_strcr0;
                    }
                    break;
                default: // not cr0
                    if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 0) {
                        return bltctr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 0) {
                        return bltctrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 0) {
                        return bltctrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 0) {
                        return bltctrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 1) {
                        return bgtctr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 1) {
                        return bgtctrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 1) {
                        return bgtctrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 1) {
                        return bgtctrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 2) {
                        return beqctr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 2) {
                        return beqctrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 2) {
                        return beqctrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 2) {
                        return beqctrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 3) {
                        return bsoctr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 3) {
                        return bsoctrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 3) {
                        return bsoctrl_str;
                    }
                    else { //(BO_onlyhint == 1 && lk == 1 && findcrbtype == 3)
                        return bsoctrlplus_str;
                    }
                    break;
            }
            break;
        case 8:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdnzctr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdnzctrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdnzctrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdnzctrlplus_str;
            }
        case 9:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdzctr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdzctrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdzctrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdzctrlplus_str;
            }
            break;
        case 0xA:
            if (lk == 0) {
                return bctr_str;
            }
            else { // lk == 1
                return bctrl_str;
            }
            break;
        default: //bad BO encoding, we will force requirement of z bits to be cleared 
            goto invalid;
            break;
    }
    
    bclrXfound:
switch (BO_nohint) {
        case 0:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdnzflr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdnzflrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdnzflrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdnzflrlplus_str;
            }
            break;
        case 1:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdzflr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdzflrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdzflrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdzflrlplus_str;
            }
            break;
        case 2:
            switch (crfD_manual) {
                case 0: // cr0
                    if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 0) {
                        return bgelr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 0) {
                        return bgelrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 0) {
                        return bgelrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 0) {
                        return bgelrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 1) {
                        return blelr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 1) {
                        return blelrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 1) {
                        return blelrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 1) {
                        return blelrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 2) {
                        return bnelr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 2) {
                        return bnelrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 2) {
                        return bnelrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 2) {
                        return bnelrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 3) {
                        return bnolr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 3) {
                        return bnolrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 3) {
                        return bnolrl_strcr0;
                    }
                    else { //(BO_onlyhint == 1 && lk == 1 && findcrbtype == 3)
                        return bnolrlplus_strcr0;
                    }
                    break;
                default: // not cr0
                    if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 0) {
                        return bgelr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 0) {
                        return bgelrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 0) {
                        return bgelrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 0) {
                        return bgelrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 1) {
                        return blelr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 1) {
                        return blelrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 1) {
                        return blelrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 1) {
                        return blelrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 2) {
                        return bnelr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 2) {
                        return bnelrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 2) {
                        return bnelrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 2) {
                        return bnelrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 3) {
                        return bnolr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 3) {
                        return bnolrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 3) {
                        return bnolrl_str;
                    }
                    else { //(BO_onlyhint == 1 && lk == 1 && findcrbtype == 3)
                        return bnolrlplus_str;
                    }
                    break;
            }
            break;
        case 4:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdnztlr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdnztlrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdnztlrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdnztlrlplus_str;
            }
            break;
        case 5:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdztlr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdztlrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdztlrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdztlrlplus_str;
            }
            break;
        case 6:
            switch (crfD_manual) {
                case 0: // cr0
                    if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 0) {
                        return bltlr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 0) {
                        return bltlrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 0) {
                        return bltlrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 0) {
                        return bltlrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 1) {
                        return bgtlr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 1) {
                        return bgtlrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 1) {
                        return bgtlrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 1) {
                        return bgtlrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 2) {
                        return beqlr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 2) {
                        return beqlrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 2) {
                        return beqlrl_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 2) {
                        return beqlrlplus_strcr0;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 3) {
                        return bsolr_strcr0;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 3) {
                        return bsolrplus_strcr0;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 3) {
                        return bsolrl_strcr0;
                    }
                    else { //(BO_onlyhint == 1 && lk == 1 && findcrbtype == 3)
                        return bsolrlplus_strcr0;
                    }
                    break;
                default: // not cr0
                    if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 0) {
                        return bltlr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 0) {
                        return bltlrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 0) {
                        return bltlrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 0) {
                        return bltlrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 1) {
                        return bgtlr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 1) {
                        return bgtlrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 1) {
                        return bgtlrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 1) {
                        return bgtlrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 2) {
                        return beqlr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 2) {
                        return beqlrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 2) {
                        return beqlrl_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 1 && findcrbtype == 2) {
                        return beqlrlplus_str;
                    }
                    //
                    else if (BO_onlyhint == 0 && lk == 0 && findcrbtype == 3) {
                        return bsolr_str;
                    }
                    else if (BO_onlyhint == 1 && lk == 0 && findcrbtype == 3) {
                        return bsolrplus_str;
                    }
                    else if (BO_onlyhint == 0 && lk == 1 && findcrbtype == 3) {
                        return bsolrl_str;
                    }
                    else { //(BO_onlyhint == 1 && lk == 1 && findcrbtype == 3)
                        return bsolrlplus_str;
                    }
                    break;
            }
            break;
        case 8:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdnzlr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdnzlrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdnzlrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdnzlrlplus_str;
            }
        case 9:
            if (BO_onlyhint == 0 && lk == 0) {
                return bdzlr_str;
            }
            else if (BO_onlyhint == 1 && lk == 0) {
                return bdzlrplus_str;
            }
            else if (BO_onlyhint == 0 && lk == 1) {
                return bdzlrl_str;
            }
            else { // BO_onlyhint == 1 && lk == 1
                return bdzlrlplus_str;
            }
            break;
        case 0xA:
            if (lk == 0) {
                return blr_str;
            }
            else { // lk == 1
                return blrl_str;
            }
            break;
        default: //bad BO encoding, we will force requirement of z bits to be cleared 
            goto invalid;
            break;
    }
    
    cmpwfound:
    if (crfD == 0) {
        return cmpwcr0_str;
    } else {
        return cmpw_str;
    }
    
    cmpwifound:
    if (crfD == 0) {
        return cmpwicr0_str;
    } else {
        return cmpwi_str;
    }
    
    cmplwfound:
    if (crfD == 0) {
        return cmplwcr0_str;
    } else {
        return cmplw_str;
    }
    
    cmplwifound:
    if (crfD == 0) {
        return cmplwicr0_str;
    } else {
        return cmplwi_str;
    }
    
    cntlzwXfound:
    switch (rc) {
                case 0:
                    return cntlzw_str;
                    break;
                default: // rc
                    return cntlzwRC_str;
                    break;
            }
            
    crandfound:
    return crand_str;
    
    crandcfound:
    return crandc_str;
    
    creqvfound:
    if ((crbA == crbB) && (crbD == crbB)) { // Check for crset simplified mnemonic
        return crset_str;
    } else {
        return creqv_str;
    }
    
    crnandfound:
    return crnand_str;
    
    crnorfound:
    if (crbA == crbB) { // Check for crnot simplified mnemonic
        return crnot_str;
    } else {
        return crnor_str;
    }
    
    crorfound:
    if (crbA == crbB) { // Check for crmove simplified mnemonic
        return crmove_str;
    } else {
        return cror_str;
    }
    
    crorcfound:
    return crorc_str;
    
    crxorfound:
    if ((crbA == crbB) && (crbD == crbB)) { // Check for crclr simplified mnemonic
        return crclr_str;
    } else {
        return crxor_str;
    }
    
    dcbffound:
    return dcbf_str;
    
    dcbifound:
    return dcbi_str;
    
    dcbstfound:
    return dcbst_str;
    
    dcbtfound:
    return dcbt_str;
    
    dcbtstfound:
    return dcbtst_str;
    
    dcbzfound:
    return dcbz_str;
    
    dcbz_lfound:
    return dcbz_l_str;
    
    divwXfound:
    switch (oerc) {
        case 0:
            return divw_str;
            break;
        case 1:
            return divwRC_str;
            break;
        case 0x400:
            return divwOE_str;
            break;
        default: // 0x0401
            return divwOERC_str;
            break;
    }
    
    divwuXfound:
    switch (oerc) {
        case 0:
            return divwu_str;
            break;
        case 1:
            return divwuRC_str;
            break;
        case 0x400:
            return divwuOE_str;
            break;
        default: // 0x0401
            return divwuOERC_str;
            break;
    }
    
    eciwxfound:
    return eciwx_str;
    
    ecowxfound:
    return ecowx_str;
    
    eieiofound:
    return eieio_str;
    
    eqvXfound:
    switch (rc) {
        case 0:
            return eqv_str;
            break;
        default: // rc
            return eqvRC_str;
            break;
    }
            
    extsbXfound:
    switch (rc) {
        case 0:
            return extsb_str;
            break;
        default: // rc
            return extsbRC_str;
            break;
    }
    
    extshXfound:
    switch (rc) {
        case 0:
            return extsh_str;
            break;
        default: // rc
            return extshRC_str;
            break;
    }   
    
    fabsXfound:
    switch (rc) {
        case 0:
            return fabs_str;
            break;
        default: // rc
            return fabsRC_str;
            break;
    } 
    
    faddXfound:
    switch (rc) {
        case 0:
            return fadd_str;
            break;
        default: // rc
            return faddRC_str;
            break;
    } 
    
    faddsXfound:
    switch (rc) {
        case 0:
            return fadds_str;
            break;
        default: // rc
            return faddsRC_str;
            break;
    } 
    
    fcmpofound:
    return fcmpo_str;
    
    fcmpufound:
    return fcmpu_str;
    
    fctiwXfound:
    switch (rc) {
        case 0:
            return fctiw_str;
            break;
        default: // rc
            return fctiwRC_str;
            break;
    } 
    
    fctiwzXfound:
    switch (rc) {
        case 0:
            return fctiwz_str;
            break;
        default: // rc
            return fctiwzRC_str;
            break;
    } 
    
    fdivXfound:
    switch (rc) {
        case 0:
            return fdiv_str;
            break;
        default: // rc
            return fdivRC_str;
            break;
    } 
    
    fdivsXfound:
    switch (rc) {
        case 0:
            return fdivs_str;
            break;
        default: // rc
            return fdivsRC_str;
            break;
    } 
    
    fmaddXfound:
    switch (rc) {
        case 0:
            return fmadd_str;
            break;
        default: // rc
            return fmaddRC_str;
            break;
    }
    
    fmaddsXfound:
    switch (rc) {
        case 0:
            return fmadds_str;
            break;
        default: // rc
            return fmaddsRC_str;
            break;
    }
    
    fmrXfound:
    switch (rc) {
        case 0:
            return fmr_str;
            break;
        default: // rc
            return fmrRC_str;
            break;
    }
    
    fmsubXfound:
    switch (rc) {
        case 0:
            return fmsub_str;
            break;
        default: // rc
            return fmsubRC_str;
            break;
    }
    
    fmsubsXfound:
    switch (rc) {
        case 0:
            return fmsubs_str;
            break;
        default: // rc
            return fmsubsRC_str;
            break;
    }
    
    fmulXfound:
    switch (rc) {
        case 0:
            return fmul_str;
            break;
        default: // rc
            return fmulRC_str;
            break;
    }
    
    fmulsXfound:
    switch (rc) {
        case 0:
            return fmuls_str;
            break;
        default: // rc
            return fmulsRC_str;
            break;
    }
    
    fnabsXfound:
    switch (rc) {
        case 0:
            return fnabs_str;
            break;
        default: // rc
            return fnabsRC_str;
            break;
    }
    
    fnegXfound:
    switch (rc) {
        case 0:
            return fneg_str;
            break;
        default: // rc
            return fnegRC_str;
            break;
    }
    
    fnmaddXfound:
    switch (rc) {
        case 0:
            return fnmadd_str;
            break;
        default: // rc
            return fnmaddRC_str;
            break;
    }
    
    fnmaddsXfound:
    switch (rc) {
        case 0:
            return fnmadds_str;
            break;
        default: // rc
            return fnmaddsRC_str;
            break;
    }
    
    fnmsubXfound:
    switch (rc) {
        case 0:
            return fnmsub_str;
            break;
        default: // rc
            return fnmsubRC_str;
            break;
    }
    
    fnmsubsXfound:
    switch (rc) {
        case 0:
            return fnmsubs_str;
            break;
        default: // rc
            return fnmsubsRC_str;
            break;
    }
    
    fresXfound:
    switch (rc) {
        case 0:
            return fres_str;
            break;
        default: // rc
            return fresRC_str;
            break;
    }
    
    frspXfound:
    switch (rc) {
        case 0:
            return frsp_str;
            break;
        default: // rc
            return frspRC_str;
            break;
    }
    
    frsqrteXfound:
    switch (rc) {
        case 0:
            return frsqrte_str;
            break;
        default: // rc
            return frsqrteRC_str;
            break;
    }
    
    fselXfound:
    switch (rc) {
        case 0:
            return fsel_str;
            break;
        default: // rc
            return fselRC_str;
            break;
    }
    
    fsubXfound:
    switch (rc) {
        case 0:
            return fsub_str;
            break;
        default: // rc
            return fsubRC_str;
            break;
    }
    
    fsubsXfound:
    switch (rc) {
        case 0:
            return fsubs_str;
            break;
        default: // rc
            return fsubsRC_str;
            break;
    }
    
    icbifound:
    return icbi_str;
    
    isyncfound:
    return isync_str;
    
    lbzfound:
    return lbz_str;
    
    lbzufound:
    return lbzu_str;
    
    lbzuxfound:
    return lbzux_str;
    
    lbzxfound:
    return lbzx_str;
    
    lfdfound:
    return lfd_str;
    
    lfdufound:
    return lfdu_str;
    
    lfduxfound:
    return lfdux_str;
    
    lfdxfound:
    return lfdx_str;
    
    lfsfound:
    return lfs_str;
    
    lfsufound:
    return lfsu_str;
    
    lfsuxfound:
    return lfsux_str;
    
    lfsxfound:
    return lfsx_str;
    
    lhafound:
    return lha_str;
    
    lhaufound:
    return lhau_str;
    
    lhauxfound:
    return lhaux_str;
    
    lhaxfound:
    return lhax_str;
    
    lhbrxfound:
    return lhbrx_str;
    
    lhzfound:
    return lhz_str;
    
    lhzufound:
    return lhzu_str;
    
    lhzuxfound:
    return lhzux_str;
    
    lhzxfound:
    return lhzx_str;
    
    lmwfound:
    return lmw_str;
    
    lswifound:
    return lswi_str;
    
    lswxfound:
    return lswx_str;
    
    lwarxfound:
    return lwarx_str;
    
    lwbrxfound:
    return lwbrx_str;
    
    lwzfound:
    return lwz_str;
    
    lwzufound:
    return lwzu_str;
    
    lwzuxfound:
    return lwzux_str;
    
    lwzxfound:
    return lwzx_str;
    
    mcrffound:
    return mcrf_str;
    
    mcrfsfound:
    return mcrfs_str;
    
    mcrxrfound:
    return mcrxr_str;
    
    mfcrfound:
    return mfcr_str;
    
    mffsXfound:
    switch (rc) {
        case 0:
            return mffs_str;
            break;
        default: // rc
            return mffsRC_str;
            break;
    }
    
    mfmsrfound:
    return mfmsr_str;
    
    mfsprfound:
    switch (SPR) {
        case 1:
            return mfxer_str;
            break;
        case 8:
            return mflr_str;
            break;
        case 9:
            return mfctr_str;
            break;
        case 18:
            return mfdsisr_str;
            break;
        case 19:
            return mfdar_str;
            break;
        case 22:
            return mfdec_str;
            break;
        case 25:
            return mfsdr1_str;
            break;
        case 26:
            return mfsrr0_str;
            break;
        case 27:
            return mfsrr1_str;
            break;
        case 272:
            return mfsprg0_str;
            break;
        case 273:
            return mfsprg1_str;
            break;
        case 274:
            return mfsprg2_str;
            break;
        case 275:
            return mfsprg3_str;
            break;
        case 282:
            return mfear_str;
            break;
        case 287:
            return mfpvr_str;
            break;
        case 528:
            return mfibat0u_str;
            break;
        case 529:
            return mfibat0l_str;
            break;
        case 530:
            return mfibat1u_str;
            break;
        case 531:
            return mfibat1l_str;
            break;
        case 532:
            return mfibat2u_str;
            break;
        case 533:
            return mfibat2l_str;
            break;
        case 534:
            return mfibat3u_str;
            break;
        case 535:
            return mfibat3l_str;
            break;
        case 560:
            return mfibat4u_str;
            break;
        case 561:
            return mfibat4l_str;
            break;
        case 562:
            return mfibat5u_str;
            break;
        case 563:
            return mfibat5l_str;
            break;
        case 564:
            return mfibat6u_str;
            break;
        case 565:
            return mfibat6l_str;
            break;
        case 566:
            return mfibat7u_str;
            break;
        case 567:
            return mfibat7l_str;
            break;
        case 536:
            return mfdbat0u_str;
            break;
        case 537:
            return mfdbat0l_str;
            break;
        case 538:
            return mfdbat1u_str;
            break;
        case 539:
            return mfdbat1l_str;
            break;
        case 540:
            return mfdbat2u_str;
            break;
        case 541:
            return mfdbat2l_str;
            break;
        case 542:
            return mfdbat3u_str;
            break;
        case 543:
            return mfdbat3l_str;
            break;
        case 568:
            return mfdbat4u_str;
            break;
        case 569:
            return mfdbat4l_str;
            break;
        case 570:
            return mfdbat5u_str;
            break;
        case 571:
            return mfdbat5l_str;
            break;
        case 572:
            return mfdbat6u_str;
            break;
        case 573:
            return mfdbat6l_str;
            break;
        case 574:
            return mfdbat7u_str;
            break;
        case 575:
            return mfdbat7l_str;
            break;
        case 912:
            return mfgqr0_str;
            break;
        case 913:
            return mfgqr1_str;
            break;
        case 914:
            return mfgqr2_str;
            break;
        case 915:
            return mfgqr3_str;
            break;
        case 916:
            return mfgqr4_str;
            break;
        case 917:
            return mfgqr5_str;
            break;
        case 918:
            return mfgqr6_str;
            break;
        case 919:
            return mfgqr7_str;
            break;
        case 920:
            return mfhid2_str;
            break;
        case 921:
            return mfwpar_str;
            break;
        case 922:
            return mfdma_u_str;
            break;
        case 923:
            return mfdma_l_str;
            break;
        case 925: //Special Broadway Chip IDs supported
            return mfcidh_str;
            break;
        case 926: //Special Broadway Chip IDs supported
            return mfcidm_str;
            break;
        case 927: //Special Broadway Chip IDs supported
            return mfcidl_str;
            break;
        case 936:
            return mfummcr0_str;
            break;
        case 937:
            return mfupmc1_str;
            break;
        case 938:
            return mfupmc2_str;
            break;
        case 939:
            return mfusia_str;
            break;
        case 940:
            return mfummcr1_str;
            break;
        case 941:
            return mfupmc3_str;
            break;
        case 942:
            return mfupmc4_str;
            break;
        case 943:
            return mfusda_str;
            break;
        case 952:
            return mfmmcr0_str;
            break;
        case 953:
            return mfpmc1_str;
            break;
        case 954:
            return mfpmc2_str;
            break;
        case 955:
            return mfsia_str;
            break;
        case 956:
            return mfmmcr1_str;
            break;
        case 957:
            return mfpmc3_str;
            break;
        case 958:
            return mfpmc4_str;
            break;
        case 959:
            return mfsda_str;
            break;
        case 1008:
            return mfhid0_str;
            break;
        case 1009:
            return mfhid1_str;
            break;
        case 1010:
            return mfiabr_str;
            break;
        case 1011:
            return mfhid4_str;
            break;
        case 1012:
            return mftdcl_str;
            break;
        case 1013:
            return mfdabr_str;
            break;
        case 1017:
            return mfl2cr_str;
            break;
        case 1018:
            return mftdch_str;
            break;
        case 1019:
            return mfictc_str;
            break;
        case 1020:
            return mfthrm1_str;
            break;
        case 1021:
            return mfthrm2_str;
            break;
        case 1022:
            return mfthrm3_str;
            break;
        default: //Invalid SPR number
            goto invalid;
            break;
    }
    
    mfsrfound:
    return mfsr_str;
    
    mfsrinfound:
    return mfsrin_str;
    
    mftbfound:
    switch (TBR) {
        case 269:
            return mftbu_str;
            break;
        case 268:
            return mftbl_str;
            break;
        default: //Invalid TPR number
            goto invalid;
            break;
    }
    
    mtcrffound:
    switch (CRM) {
        case 255: //0xFF
            return mtcr_str;
            break;
        default: //Standard mtcrf mnemonic
            return mtcrf_str;
            break;
    }
    
    mtfsb0Xfound:
    switch (rc) {
        case 0:
            return mtfsb0_str;
            break;
        default: // rc
            return mtfsb0RC_str;
            break;
    }
    
    mtfsb1Xfound:
    switch (rc) {
        case 0:
            return mtfsb1_str;
            break;
        default: // rc
            return mtfsb1RC_str;
            break;
    }
    
    mtfsfXfound:
    switch (rc) {
        case 0:
            return mtfsf_str;
            break;
        default: // rc
            return mtfsfRC_str;
            break;
    }
    
    mtfsfiXfound:
    switch (rc) {
        case 0:
            return mtfsfi_str;
            break;
        default: // rc
            return mtfsfiRC_str;
            break;
    }
    
    mtmsrfound:
    return mtmsr_str;
    
    mtsprfound:
    switch (SPR) {
        case 1:
            return mtxer_str;
            break;
        case 8:
            return mtlr_str;
            break;
        case 9:
            return mtctr_str;
            break;
        case 18:
            return mtdsisr_str;
            break;
        case 19:
            return mtdar_str;
            break;
        case 22:
            return mtdec_str;
            break;
        case 25:
            return mtsdr1_str;
            break;
        case 26:
            return mtsrr0_str;
            break;
        case 27:
            return mtsrr1_str;
            break;
        case 272:
            return mtsprg0_str;
            break;
        case 273:
            return mtsprg1_str;
            break;
        case 274:
            return mtsprg2_str;
            break;
        case 275:
            return mtsprg3_str;
            break;
        case 282:
            return mtear_str;
            break;
        case 284:
            return mttbl_str;
            break;
        case 285:
            return mttbu_str;
            break;
        case 528:
            return mtibat0u_str;
            break;
        case 529:
            return mtibat0l_str;
            break;
        case 530:
            return mtibat1u_str;
            break;
        case 531:
            return mtibat1l_str;
            break;
        case 532:
            return mtibat2u_str;
            break;
        case 533:
            return mtibat2l_str;
            break;
        case 534:
            return mtibat3u_str;
            break;
        case 535:
            return mtibat3l_str;
            break;
        case 560:
            return mtibat4u_str;
            break;
        case 561:
            return mtibat4l_str;
            break;
        case 562:
            return mtibat5u_str;
            break;
        case 563:
            return mtibat5l_str;
            break;
        case 564:
            return mtibat6u_str;
            break;
        case 565:
            return mtibat6l_str;
            break;
        case 566:
            return mtibat7u_str;
            break;
        case 567:
            return mtibat7l_str;
            break;
        case 536:
            return mtdbat0u_str;
            break;
        case 537:
            return mtdbat0l_str;
            break;
        case 538:
            return mtdbat1u_str;
            break;
        case 539:
            return mtdbat1l_str;
            break;
        case 540:
            return mtdbat2u_str;
            break;
        case 541:
            return mtdbat2l_str;
            break;
        case 542:
            return mtdbat3u_str;
            break;
        case 543:
            return mtdbat3l_str;
            break;
        case 568:
            return mtdbat4u_str;
            break;
        case 569:
            return mtdbat4l_str;
            break;
        case 570:
            return mtdbat5u_str;
            break;
        case 571:
            return mtdbat5l_str;
            break;
        case 572:
            return mtdbat6u_str;
            break;
        case 573:
            return mtdbat6l_str;
            break;
        case 574:
            return mtdbat7u_str;
            break;
        case 575:
            return mtdbat7l_str;
            break;
        case 912:
            return mtgqr0_str;
            break;
        case 913:
            return mtgqr1_str;
            break;
        case 914:
            return mtgqr2_str;
            break;
        case 915:
            return mtgqr3_str;
            break;
        case 916:
            return mtgqr4_str;
            break;
        case 917:
            return mtgqr5_str;
            break;
        case 918:
            return mtgqr6_str;
            break;
        case 919:
            return mtgqr7_str;
            break;
        case 920:
            return mthid2_str;
            break;
        case 921:
            return mtwpar_str;
            break;
        case 922:
            return mtdma_u_str;
            break;
        case 923:
            return mtdma_l_str;
            break;
        case 936:
            return mtummcr0_str;
            break;
        case 937:
            return mtupmc1_str;
            break;
        case 938:
            return mtupmc2_str;
            break;
        case 939:
            return mtusia_str;
            break;
        case 940:
            return mtummcr1_str;
            break;
        case 941:
            return mtupmc3_str;
            break;
        case 942:
            return mtupmc4_str;
            break;
        case 943:
            return mtusda_str;
            break;
        case 952:
            return mtmmcr0_str;
            break;
        case 953:
            return mtpmc1_str;
            break;
        case 954:
            return mtpmc2_str;
            break;
        case 955:
            return mtsia_str;
            break;
        case 956:
            return mtmmcr1_str;
            break;
        case 957:
            return mtpmc3_str;
            break;
        case 958:
            return mtpmc4_str;
            break;
        case 959:
            return mtsda_str;
            break;
        case 1008:
            return mthid0_str;
            break;
        case 1009:
            return mthid1_str;
            break;
        case 1010:
            return mtiabr_str;
            break;
        case 1011:
            return mthid4_str;
            break;
        case 1013:
            return mtdabr_str;
            break;
        case 1017:
            return mtl2cr_str;
            break;
        case 1019:
            return mtictc_str;
            break;
        case 1020:
            return mtthrm1_str;
            break;
        case 1021:
            return mtthrm2_str;
            break;
        case 1022:
            return mtthrm3_str;
            break;
        default: //Invalid SPR number
            goto invalid;
            break;
    }
    
    mtsrfound:
    return mtsr_str;
    
    mtsrinfound:
    return mtsrin_str;
    
    mulhwXfound:
    switch (rc) {
        case 0:
            return mulhw_str;
            break;
        default: // rc
            return mulhwRC_str;
            break;
    }
    
    mulhwuXfound:
    switch (rc) {
        case 0:
            return mulhwu_str;
            break;
        default: // rc
            return mulhwuRC_str;
            break;
    }
    
    mullifound:
    return mulli_str;
    
    mullwXfound:
    switch (rc) {
        case 0:
            return mullw_str;
            break;
        default: // rc
            return mullwRC_str;
            break;
    }
    
    nandXfound:
    switch (rc) {
        case 0:
            return nand_str;
            break;
        default: // rc
            return nandRC_str;
            break;
    }
    
    negXfound:
    switch (oerc) {
        case 0:
            return neg_str;
            break;
        case 1:
            return negRC_str;
            break;
        case 0x400:
            return negOE_str;
            break;
        default: // 0x0401
            return negOERC_str;
            break;
    }
    
    norXfound:
    switch (rc) {
        case 0:
            return nor_str;
            break;
        default: // rc
            return norRC_str;
            break;
    }
    
    orXfound:
    switch (rc) {
        case 0:
            if (rS == rB) { //Check for mr simplified mnemonic
                return mr_str;
            } else {
                return or_str;
            }
            break;
        default: // rc
            if (rS == rB) { //Check for mr. simplified mnemonic
                return mrRC_str;
            } else {
                return orRC_str;
            }
            break;
    }
    
    orcXfound:
    switch (rc) {
        case 0:
            return orc_str;
            break;
        default: // rc
            return orcRC_str;
            break;
    }
    
    orifound:
    if (ppcinstruction == 0x60000000) {
        return nop_str;
    } else {
        return ori_str;
    }
    
    orisfound:
    return oris_str;
    
    psq_lfound:
    return psq_l_str;
    
    psq_lufound:
    return psq_lu_str;
    
    psq_luxfound:
    return psq_lux_str;
    
    psq_lxfound:
    return psq_lx_str;
    
    psq_stfound:
    return psq_st_str;
    
    psq_stufound:
    return psq_stu_str;
    
    psq_stuxfound:
    return psq_stux_str;
    
    psq_stxfound:
    return psq_stx_str;
    
    ps_absXfound:
    switch (rc) {
        case 0:
            return ps_abs_str;
            break;
        default: // rc
            return ps_absRC_str;
            break;
    }
    
    ps_addXfound:
    switch (rc) {
        case 0:
            return ps_add_str;
            break;
        default: // rc
            return ps_addRC_str;
            break;
    }
    
    ps_cmpo0found:
    return ps_cmpo0_str;
    
    ps_cmpo1found:
    return ps_cmpo1_str;
    
    ps_cmpu0found:
    return ps_cmpu0_str;
    
    ps_cmpu1found:
    return ps_cmpu1_str;
    
    ps_divXfound:
    switch (rc) {
        case 0:
            return ps_div_str;
            break;
        default: // rc
            return ps_divRC_str;
            break;
    }
    
    ps_maddXfound:
    switch (rc) {
        case 0:
            return ps_madd_str;
            break;
        default: // rc
            return ps_maddRC_str;
            break;
    }
    
    ps_madds0Xfound:
    switch (rc) {
        case 0:
            return ps_madds0_str;
            break;
        default: // rc
            return ps_madds0RC_str;
            break;
    }
    
    ps_madds1Xfound:
    switch (rc) {
        case 0:
            return ps_madds1_str;
            break;
        default: // rc
            return ps_madds1RC_str;
            break;
    }
    
    ps_merge00Xfound:
    switch (rc) {
        case 0:
            return ps_merge00_str;
            break;
        default: // rc
            return ps_merge00RC_str;
            break;
    }
    
    ps_merge01Xfound:
    switch (rc) {
        case 0:
            return ps_merge01_str;
            break;
        default: // rc
            return ps_merge01RC_str;
            break;
    }
    
    ps_merge10Xfound:
    switch (rc) {
        case 0:
            return ps_merge10_str;
            break;
        default: // rc
            return ps_merge10RC_str;
            break;
    }
    
    ps_merge11Xfound:
    switch (rc) {
        case 0:
            return ps_merge11_str;
            break;
        default: // rc
            return ps_merge11RC_str;
            break;
    }
    
    ps_mrXfound:
    switch (rc) {
        case 0:
            return ps_mr_str;
            break;
        default: // rc
            return ps_mrRC_str;
            break;
    }
    
    ps_msubXfound:
    switch (rc) {
        case 0:
            return ps_msub_str;
            break;
        default: // rc
            return ps_msubRC_str;
            break;
    }
    
    ps_mulXfound:
    switch (rc) {
        case 0:
            return ps_mul_str;
            break;
        default: // rc
            return ps_mulRC_str;
            break;
    }
    
    ps_muls0Xfound:
    switch (rc) {
        case 0:
            return ps_muls0_str;
            break;
        default: // rc
            return ps_muls0RC_str;
            break;
    }
    
    ps_muls1Xfound:
    switch (rc) {
        case 0:
            return ps_muls1_str;
            break;
        default: // rc
            return ps_muls1RC_str;
            break;
    }
    
    ps_nabsXfound:
    switch (rc) {
        case 0:
            return ps_nabs_str;
            break;
        default: // rc
            return ps_nabsRC_str;
            break;
    }
    
    ps_negXfound:
    switch (rc) {
        case 0:
            return ps_neg_str;
            break;
        default: // rc
            return ps_negRC_str;
            break;
    }
    
    ps_nmaddXfound:
    switch (rc) {
        case 0:
            return ps_nmadd_str;
            break;
        default: // rc
            return ps_nmaddRC_str;
            break;
    }
    
    ps_nmsubXfound:
    switch (rc) {
        case 0:
            return ps_nmsub_str;
            break;
        default: // rc
            return ps_nmsubRC_str;
            break;
    }
    
    ps_resXfound:
    switch (rc) {
        case 0:
            return ps_res_str;
            break;
        default: // rc
            return ps_resRC_str;
            break;
    }
    
    ps_rsqrteXfound:
    switch (rc) {
        case 0:
            return ps_rsqrte_str;
            break;
        default: // rc
            return ps_rsqrteRC_str;
            break;
    }
    
    ps_selXfound:
    switch (rc) {
        case 0:
            return ps_sel_str;
            break;
        default: // rc
            return ps_selRC_str;
            break;
    }
    
    ps_subXfound:
    switch (rc) {
        case 0:
            return ps_sub_str;
            break;
        default: // rc
            return ps_subRC_str;
            break;
    }
    
    ps_sum0Xfound:
    switch (rc) {
        case 0:
            return ps_sum0_str;
            break;
        default: // rc
            return ps_sum0RC_str;
            break;
    }
    
    ps_sum1Xfound:
    switch (rc) {
        case 0:
            return ps_sum1_str;
            break;
        default: // rc
            return ps_sum1RC_str;
            break;
    }
    
    rfifound:
    return rfi_str;
    
    rlwimiXfound:
    switch (rc) {
        case 0:
            return rlwimi_str;
            break;
        default: // rc
            return rlwimiRC_str;
            break;
    }
    
    rlwinmXfound:
    switch (rc) {
        case 0:
            if ((MB == 0) && (SH + ME == 31)) {
                return slwi_str;
            }
            else if ((ME == 31) && (SH + MB == 32)) {
                return srwi_str;
            }
            else if ((SH == 0) && (ME == 31)) {
                return clrlwi_str;
            }
            else if ((SH == 0) && (MB == 0)) {
                return clrrwi_str;
            }
            else if ((MB == 0) && (ME == 31)) {
                return rotlwi_str;
            }
            else {
                return rlwinm_str;
            }
            break;
        default: // rc
            if ((MB == 0) && (SH + ME == 31)) {
                return slwiRC_str;
            }
            else if ((ME == 31) && (SH + MB == 32)) {
                return srwiRC_str;
            }
            else if ((SH == 0) && (ME == 31)) {
                return clrlwiRC_str;
            }
            else if ((SH == 0) && (MB == 0)) {
                return clrrwiRC_str;
            }
            else if ((MB == 0) && (ME == 31)) {
                return rotlwiRC_str;
            }
            else {
                return rlwinmRC_str;
            }
            break;
    }
    
    rlwnmXfound:
    switch (rc) {
        case 0:
            if ((MB == 0) && (ME == 31)) {
                return rotlw_str;
            }
            else {
                return rlwnm_str;
            }
            break;
        default: // rc
            if ((MB == 0) && (ME == 31)) {
                return rotlwRC_str;
            }
            else {
                return rlwnmRC_str;
            }
            break;
    }
    
    scfound:
    return sc_str;
    
    slwXfound:
    switch (rc) {
        case 0:
            return slw_str;
            break;
        default: // rc
            return slwRC_str;
            break;
    }
    
    srawXfound:
    switch (rc) {
        case 0:
            return sraw_str;
            break;
        default: // rc
            return srawRC_str;
            break;
    }
    
    srawiXfound:
    switch (rc) {
        case 0:
            return srawi_str;
            break;
        default: // rc
            return srawiRC_str;
            break;
    }
    
    srwXfound:
    switch (rc) {
        case 0:
            return srw_str;
            break;
        default: // rc
            return srwRC_str;
            break;
    }
    
    stbfound:
    return stb_str;
    
    stbufound:
    return stbu_str;
    
    stbuxfound:
    return stbux_str;
    
    stbxfound:
    return stbx_str;
    
    stfdfound:
    return stfd_str;
    
    stfdufound:
    return stfdu_str;
    
    stfduxfound:
    return stfdux_str;
    
    stfdxfound:
    return stfdx_str;
    
    stfiwxfound:
    return stfiwx_str;
    
    stfsfound:
    return stfs_str;
    
    stfsufound:
    return stfsu_str;
    
    stfsuxfound:
    return stfsux_str;
    
    stfsxfound:
    return stfsx_str;
    
    sthfound:
    return sth_str;
    
    sthbrxfound:
    return sthbrx_str;
    
    sthufound:
    return sthu_str;
    
    sthuxfound:
    return sthux_str;
    
    sthxfound:
    return sthx_str;
    
    stmwfound:
    return stmw_str;
    
    stswifound:
    return stswi_str;
    
    stswxfound:
    return stswx_str;
    
    stwfound:
    return stw_str;
    
    stwbrxfound:
    return stwbrx_str;
    
    stwcxRCfound:
    return stwcxRC_str;
    
    stwufound:
    return stwu_str;
    
    stwuxfound:
    return stwux_str;
    
    stwxfound:
    return stwx_str;
    
    subXfound:
    switch (oerc) {
        case 0:
            return sub_str;
            break;
        case 1:
            return subRC_str;
            break;
        case 0x400:
            return subOE_str;
            break;
        default: // 0x0401
            return subOERC_str;
            break;
    }
    
    subcXfound:
    switch (oerc) {
        case 0:
            return subc_str;
            break;
        case 1:
            return subcRC_str;
            break;
        case 0x400:
            return subcOE_str;
            break;
        default: // 0x0401
            return subcOERC_str;
            break;
    }
    
    subfeXfound:
    switch (oerc) {
        case 0:
            return subfe_str;
            break;
        case 1:
            return subfeRC_str;
            break;
        case 0x400:
            return subfeOE_str;
            break;
        default: // 0x0401
            return subfeOERC_str;
            break;
    }
    
    subficfound:
    return subfic_str;
    
    subfmeXfound:
    switch (oerc) {
        case 0:
            return subfme_str;
            break;
        case 1:
            return subfmeRC_str;
            break;
        case 0x400:
            return subfmeOE_str;
            break;
        default: // 0x0401
            return subfmeOERC_str;
            break;
    }
    
    subfzeXfound:
    switch (oerc) {
        case 0:
            return subfze_str;
            break;
        case 1:
            return subfzeRC_str;
            break;
        case 0x400:
            return subfzeOE_str;
            break;
        default: // 0x0401
            return subfzeOERC_str;
            break;
    }
    
    syncfound:
    return sync_str;
    
    tlbiefound:
    return tlbie_str;
    
    tlbsyncfound:
    return tlbsync_str;
    
    twfound:
    if (TO == 31) {
        return trap_str;
    }
    else {
        return tw_str;
    }
    
    twifound:
    return twi_str;
    
    xorXfound:
    switch (rc) {
        case 0:
            return xor_str;
            break;
        default: // rc
            return xorRC_str;
            break;
    }
    
    xorifound:
    return xori_str;
    
    xorisfound:
    return xoris_str;
    
}

// main
int main() {
    start:
    cout << "Enter a Broadway PPC instruction to disassemble.\n0x";
    cin >> hex >> ppcinstruction;
    string str;
    str = disassemble (ppcinstruction);
    fmt::print(bg(fmt::color::blue), str);
    cout << "\n\n";
    goto start;
    return 0;
}

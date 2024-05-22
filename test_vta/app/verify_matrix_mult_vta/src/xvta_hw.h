// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read)
//        bit 7  - auto_restart (Read/Write)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0  - enable ap_done interrupt (Read/Write)
//        bit 1  - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0  - ap_done (COR/TOW)
//        bit 1  - ap_ready (COR/TOW)
//        others - reserved
// 0x10 : Data signal of insn_count
//        bit 31~0 - insn_count[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of insns
//        bit 31~0 - insns[31:0] (Read/Write)
// 0x1c : reserved
// 0x20 : Data signal of uops
//        bit 31~0 - uops[31:0] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of inputs
//        bit 31~0 - inputs[31:0] (Read/Write)
// 0x2c : reserved
// 0x30 : Data signal of weights
//        bit 31~0 - weights[31:0] (Read/Write)
// 0x34 : reserved
// 0x38 : Data signal of biases
//        bit 31~0 - biases[31:0] (Read/Write)
// 0x3c : reserved
// 0x40 : Data signal of outputs
//        bit 31~0 - outputs[31:0] (Read/Write)
// 0x44 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#include "platform_info.h"

#define XVTA_CONTROL_ADDR_AP_CTRL         (I_VTA00_CONFIG_BASEADDR + 0x00)
//#define XVTA_CONTROL_ADDR_GIE             (I_AXI_HOST_SLAVE_BASEADDR + 0x04)
//#define XVTA_CONTROL_ADDR_IER             (I_AXI_HOST_SLAVE_BASEADDR + 0x08)
//#define XVTA_CONTROL_ADDR_ISR             (I_AXI_HOST_SLAVE_BASEADDR + 0x0c)
#define XVTA_CONTROL_ADDR_INSN_COUNT_DATA (I_VTA00_CONFIG_BASEADDR + 0x08)
#define XVTA_CONTROL_BITS_INSN_COUNT_DATA 32
#define XVTA_CONTROL_ADDR_INSNS_DATA      (I_VTA00_CONFIG_BASEADDR + 0x0c)
#define XVTA_CONTROL_BITS_INSNS_DATA      32
#define XVTA_CONTROL_ADDR_UOPS_DATA       (I_VTA00_CONFIG_BASEADDR + 0x10)
#define XVTA_CONTROL_BITS_UOPS_DATA       64
#define XVTA_CONTROL_ADDR_INPUTS_DATA     (I_VTA00_CONFIG_BASEADDR + 0x14)
#define XVTA_CONTROL_BITS_INPUTS_DATA     64
#define XVTA_CONTROL_ADDR_WEIGHTS_DATA    (I_VTA00_CONFIG_BASEADDR + 0x18)
#define XVTA_CONTROL_BITS_WEIGHTS_DATA    64
#define XVTA_CONTROL_ADDR_BIASES_DATA     (I_VTA00_CONFIG_BASEADDR + 0x1c)
#define XVTA_CONTROL_BITS_BIASES_DATA     64
#define XVTA_CONTROL_ADDR_OUTPUTS_DATA    (I_VTA00_CONFIG_BASEADDR + 0x20)
#define XVTA_CONTROL_BITS_OUTPUTS_DATA    64


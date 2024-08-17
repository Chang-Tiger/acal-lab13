---
title: ACAL 2024 Curriculum Lab 13 - SIMD Instruction Set Support for AI Computing
robots: noindex, nofollow
---

# <center>ACAL 2024 Curriculum Lab 13 <br /><font color="#1560bd">SIMD Instruction Set Support for AI Computing</font></center>

###### tags: `AIAS Spring 2024`

[toc]

Intorduction : Single Instruction Multiple Data (SIMD)
===
Section 1 : Data Parallelism
---
Many electronic devices incorporate graphical displays for user-friendly interfaces and enhanced user experiences. Typically, these graphical systems use 8-bit integers to represent the red, green, and blue colors in each pixel. To render a single frame for a screen with a 1920×1080 resolution, approximately 2 million pixels must be processed simultaneously. Achieving this within 0.02 seconds for a 60 FPS screen presents a significant challenge in microprocessor design.

Fortunately, many graphical applications involve performing identical operations on all data elements. The characteristic of workloads is called **Data Parallelism**. This allows for efficient acceleration using specialized computing units or ALUs.

Furthermore, given the substantial advancements in neural networks' accuracy in artificial intelligence, they have become a primary focus for acceleration in cutting-edge computing systems. In neural network applications, data is processed as tensors (also known as matrices), which naturally provides data parallelism, benefiting both software algorithms and hardware design.


Section 2 : Loop Unrolling
---
Loop Unrolling is an useful software technique for these optimization goal:

- Reducing branch overhead caused by loops.
- Reducing branch penalties caused by misprediction.
- Expressing parallelism and increasing parallelism degree to guide further optimization.

For this piece of code:

```cpp
for (i = 0; i < 8; i++)
    a[i] = a[i] + b[i] * c[i];
```

The `for` loop in the above program will recur 8 times to finish all computations. We can rewrite it as the following code:

```cpp
for (i = 0; i < 8; i += 4) {
    a[i]   = a[i]   + b[i]   * c[i];
    a[i+1] = a[i+1] + b[i+1] * c[i+1];
    a[i+2] = a[i+2] + b[i+2] * c[i+2];
    a[i+3] = a[i+3] + b[i+3] * c[i+3];
}
```

With the simple modification, we can reduce up to 75% of the branch instructions occurring during execution time. It is also obvious that the performance can be improved to 4x once the hardware execution units can perform four multiplications and additions in parallel.

Section 3 : Single Instruction Multiple Data (SIMD)
---
<center>
    <img src="https://johnnysswlab.com/wp-content/uploads/1_O4N5IlOJmtl_KLQJul4B_w.png" width="60%">
</center>

Several architectures have been developed to harness data parallelism in workloads for enhancing performance. One common approach in processor design is **Single Instruction Multiple Data (SIMD)**. SIMD instructions work on data vectors with multiple parallel Arithmetic Logic Units (ALUs). In SIMD machines, all parallel computing units are synchronized and controlled by a single instruction corresponding to one Programming Counter (PC).

Compared to **Single Instruction Single Data (SISD)** design, SIMD design typically shares a similar architecture and focuses on increasing the number of execution units, making SIMD microarchitecture easier to implement. Additionally, SIMD does not require additional instruction bandwidth, in contrast to **Multiple Instruction Multiple Data (MIMD)**, which necessitates fetching instructions for each execution pipeline.

Section 4 : Instruction Set Architecture (ISA) Design Flow
---

To design an ISA extension for AI acceleration, the tasks that need to be accomplished extend beyond defining instructions. Both hardware and software implementations, along with corresponding support, are essential to enable the effective utilization of this ISA extension in AI workload computations.

The whole design flow we demonstrate in this lab includes the following steps:

1. Define our SIMD instructions.
2. Design and implement the micro-architecture of hardware to support these SIMD instructions.
3. Facilitate the utilization of these new instructions and hardware features by offering kernel functions in the software, allowing users to invoke them seamlessly.
4. Implement operation functions within the kernel functions to enable AI workloads to leverage the new software/hardware design for maximum performance improvement.

Section 5 : Micro-Architecture Design Issues
---
- Vector registers
    - To optimize performance and broaden datatype support, vector registers are employed to store data operands for SIMD pipelines, thereby increasing the area and cost of the processor core.
    - In this lab, we will utilize the integer general purpose registers (GPRs) for SIMD operations, capping the SIMD pipeline's vector length at 32 bits (4 INT8s or 2 INT16s).
- Data dependency
    - Since there is more than 1 pipeline executing concurrently with SIMD design, the data dependency issues that occur between the SIMD pipeline and the original SISD pipeline should be managed properly to ensure correctness. This will require a more complex logic design to handle. 
    - Fortunately, this issue would be taken care of by the CFU framework in this lab, which means you can assume that the data received in Custom Function Units is free from dependency concerns.
- Data fetch bandwidth
    - In general, with increasing size of data can be processed by SIMD instructions in parallel, it also requires a corresponding increase in data bandwidth to sustain ALU throughput. Otherwise, these execution units may spend extra time waiting for data movement which causes inefficiencies. 
    - The data bandwidth involves the whole datapath between memory and execution units, including the design of
        - system level
            - data memory
            - system bus
            - data cache
        - processor core
            - load/store units
            - register file
            - arithmetic logic unit (ALU)
    - As a result, an effective SIMD design demands not only additional compute units but also careful system bandwidth planning to achieve optimal efficiency.
- Quantization
    - The precision requirements of arithmetic operations often need an expanded data width. For instance, basic operations like addition and subtraction of 32-bit data yield 33-bit results, leading to potential overflow issues. Similarly but more seriously, multiplication demands double data width to maintain precision, posing challenges with data supply rate due to instruction format limitations. 
    - To deal with this problem, quantization offers a viable approach to reduce the necessary data width for multiplication products while ensuring sufficient precision for subsequent computations.

Lab 13
===
Lab13-0 : CFU Playground Introduction
---
### Lab13-0-1 CFU Playground Bring-Up

In this lab, we will use Docker to set up the workspace. Please be aware that the workspace is only compatible with **AMD64** machines. **ARM64** machines are not compatible.

#### Build CFU Playground Workspace
> Not in ACAL Workspace !
```shell
# clone CFU Playground Workspace
user@workspace:~$ git clone ssh://git@course.playlab.tw:30022/acal-curriculum/cfu-playground-workspace.git
user@workspace:~$ cd cfu-playground-workspace

# build CFU Playground Workspace (image will be built at first time)
user@workspace:~/cfu-playground-workspace (main)$ ./run start
# In CFU _layground Workspace
user@AIAS-CFU-WORKSPACE:~/CFU-Playground/proj $ 
```
:::warning
- You may setup passwordless ssh login if you like. Please refer to [Use SSH keys to communicate with GitLab](https://docs.gitlab.com/ee/user/ssh.html)
- Also, if you would like to setup the SSH Key in our Container. Please refer to this [document](https://course.playlab.tw/md/CW_gy1XAR1GDPgo8KrkLgg#Set-up-the-SSH-Key) to set up the SSH Key in acal-curriculum and cfu-workspace.
:::

#### Environment and Repo Setup
```shell=
## bring up the CFU docker container
user@workspace:~/cfu-playground-workspace (main) $ ./run start
## clone the lab13 files
$  cd ~/CFU-Playground/proj
$  git clone ssh://git@course.playlab.tw:30022/acal-curriculum/lab13.git
$  cd lab13

## add your private upstream repositories
## make sure you have create project repo under your gitlab account
$  git remote add gitlab ssh://git@course.playlab.tw:30022/<your ldap name>/lab13.git

$  git remote -v
gitlab ssh://git@course.playlab.tw:30022/<your ldap name>/lab13.git (fetch)
gitlab ssh://git@course.playlab.tw:30022/<your ldap name>/lab13.git (push)
origin ssh://git@course.playlab.tw:30022/acal-curriculum/lab13.git (fetch)
origin ssh://git@course.playlab.tw:30022/acal-curriculum/lab13.git (push)
```
#### Simulation in CFU Playground (Lab13)
```shell
## setup CFU Project
user@AIAS-CFU-WORKSPACE:~/CFU-Playground/proj $ cd lab13

## build SIMD Hardware
user@AIAS-CFU-WORKSPACE:~/CFU-Playground/proj/lab13 $ cd chisel 
user@AIAS-CFU-WORKSPACE:~/CFU-Playground/proj/lab13/chisel $ sbt 'runMain simd.SIMDEngineApp'

## Start Simulation in Renode
user@AIAS-CFU-WORKSPACE:~/CFU-Playground/proj/lab13/chisel $ cd ..
user@AIAS-CFU-WORKSPACE:~/CFU-Playground/proj/lab13 (main)$ make ENABLE_TRACE_ARG=--trace renode-headless
```
### Lab13-0-2 How to run the Testbench In CFU-Playground.
- Test the Lab and Homework in CFU Playground.
```script
// Start Simulation in Renode
user@AIAS-CFU-WORKSPACE:~/CFU-Playground/proj/lab13 (main) $ make renode-headless
...
(monitor) s @digilent_arty.resc ;  uart_connect sysbus.uart
Redirecting the input to sysbus.uart, press <ESC> to quit...
// press <SPACE> to enter the CFU Playground Main Menu
CFU Playground
==============
 1: TfLM Models menu
 2: Functional CFU Tests
 3: Project menu
 4: Performance Counter Tests
 5: TFLite Unit Tests
 6: Benchmarks
 7: Util Tests
 8: Embench IoT
// press <3> to enter the CFU Playground Project Menu
main> 3
 0: test SIMD instruction - Integer Extension (Lab Edition)
 1: test SIMD instruction - Integer Extension (Homework Edition)
 a: test GEMM Operator    - with SIMD Extension (Lab Edition)
 b: test GEMM Operator    - with SIMD Extension (Homework Edition)
 c: test CONV Operator    - with SIMD Extension (Homework Edition)
 d: test MXPL Operator    - with SIMD Extension (Homework Edition)
 e: test RELU Operator    - with SIMD Extension (Homework Edition)
 f: test ALL  Operator    - with SIMD Extension (Homework Edition)
 g: test AlexNet (data0)  - with SIMD Extension (Homework Edition)
 h: test AlexNet (daat1)  - with SIMD Extension (Homework Edition)
 i: test All
 x: eXit to previous menu
 // press <0,1,a,b,c,d,e,f,g,h,i> to run the testbench
project> 

// press <ESC> to exit Renode Simulation
project> Disconnected from sysbus.uart
// press <CTRL+D> to end the Renode process
(digilent_arty) ^D

user@AIAS-CFU-WORKSPACE:~/CFU-Playground/proj/lab13 (main) $
```

#### How do I generate waveforms/traces in Lab13?
1. Chisel
    ```script
    sbt 'testOnly simd.hw.* -tbn verilator'
    ```
2. CFU-Playground
    ```script
    make ENABLE_TRACE_ARG=--trace VERILATOR_TRACE_DEPTH=4 VERILATOR_TRACE_PATH=</tmp/vtrace.vcd> renode-headless
    ```


### Lab13-0-3 CFU Introduction

CFU-Playground is a System-on-Chip (SoC) equipped with a 32-bit RISC-V CPU, a custom function unit, and a comprehensive software stack designed for executing Deep Neural Network (DNN) models. We have the capability to effortlessly develop our own ISA extensions and conduct RTL simulations for end-to-end model inference.

<center><img src="https://course.playlab.tw/md/uploads/6090c8b0-e66e-4489-8c1f-4f0c28b561ba.png" width="500" ></center>

#### Gateware
The following is the entire SoC in CFU Playground, which is built through LiteX, a framework that provides a convenient and efficient infrastructure for creating FPGA cores/SoCs.

<center><img src="https://course.playlab.tw/md/uploads/7fe4e4f8-0cfe-4124-8d4c-cd9d92553b6e.png" width="500" ></center>

The CPU in the project (VexRiscV) has an opcode allocated to the CFU(Custom Function Unit). When the CPU encounters this opcode, that instruction will be passed to the CFU with 2 source operands (cpu general-purpose registers), wait for the response, and move the result into the destination register.

![](https://course.playlab.tw/md/uploads/6c8d2903-4c3c-48a7-824d-a3acc8d6dc09.png)

```mipsasm
# in software.elf.dis
cfu_inst[funct3, funct7] rd, rs1, rs2
```

This is very similar to an ALU in general CPU, but it has additional variations due to the handshake mechanism of the ready/valid pair. More details of the CPU-CFU Interface - [Link](https://cfu-playground.readthedocs.io/en/latest/interface.html).

```
        >--- cmd_valid --------------->
        <--- cmd_ready ---------------<
        >--- cmd_function_id[9:0] ---->
        >--- cmd_inputs_0[31:0] ------>
        >--- cmd_inputs_1[31:0] ------>
CPU                                         CFU
        <--- rsp_valid ---------------<
        >--- rsp_ready --------------->
        <--- rsp_outputs_0[31:0] -----<
```

```verilog
module Cfu (
  input               cmd_valid,
  output              cmd_ready,
  input      [9:0]    cmd_payload_function_id,
  input      [31:0]   cmd_payload_inputs_0,
  input      [31:0]   cmd_payload_inputs_1,
  output              rsp_valid,
  input               rsp_ready,
  output     [31:0]   rsp_payload_outputs_0,
  input               clk,
  input               reset
);
endmodule
```

#### Software
The CFU Playground leverages Tensorflow Lite for Microcontroller (TFLite Micro) to do ML model inference. This enables us to easily rewrite the kernel functions and utilize the hardware we have designed. 

To make use of Custom Function Unit, the CFU Playground provides Inline-Assembly-Language-Liked APIs. Incorporating inline assembly language allows developers to write low-level instructions directly into their custom function units, providing a way to precisely control and optimize the execution of certain tasks.

- In [`CFU-Playground/common/src/cfu.h`](https://github.com/google/CFU-Playground/blob/fb43f80f44c32f81c9cdc1d7c015946f0265fa08/common/src/cfu.h), we can use `cfu_op` APIs to execute a specific operation.
    ```cpp
    // cfu.h
    #define cfu_op(funct3, funct7, rs1, rs2) cfu_op_hw(funct3, funct7, rs1, rs2)
    ```
- For example, this statement in software
    ```cpp
    int a = 1;
    int b = 2;
    int c = cfu_op_hw(0b000, 0b0100100, a, b);
    ```
    would be compiled into the following assembly code
    ```mipsasm
    li                  s6, 0x1
    li                  s7, 0x2
    cfu_inst[0x0, 0x24] s8, s6, s7  # assume the variable c is mapped to register s8
    ```

##### Kernel Functions vs. Operation Functions

Let's discuss these two terms that are crucial for ISA in AI acceleration but are often confused.

- Kernel Functions
    - A **hardware dependent** abstraction layer.
    - Allow users to use each instruction provided by hardware in the software.
    - Sometimes, a kernel function may also consist of more than one but only essential instructions to perform a fundamental computation or operation (e.g. an INT32 → INT64 multiplication).
- Operation Functions
    - A **hardware independent** abstraction layer.
    - Leveraging multiple kernel functions in the best manner to accomplish a complex operation (e.g. Conv2D, Gemm, etc.).
    - Can be easily mapped onto target workloads and applications for computing acceleration.

Lab13-1 : SIMD Extension Proposal and Function Unit Design
---
This section will demonstrate how to design the function unit for several SIMD instructions including addition, substraction, and multiplication operations.

Before hardware implementation in CFU Playgrpund, we should purpose the instructions that define how the CPU (hardware) is controlled by the software.

### Lab13-1-0 General Instruction Design
To design new instructions in an ISA extension, firstly we need to determine the specific functionalities or optimizations that the new instructions should provide. 

To design new instructions for a new ISA extension, the basic steps are listed below: 

1. Determine the specific functionalities or optimizations that the new instructions should provide.
2. Define the operation semantics, opcode, operand formats, and involved parameters for each new instruction. The impact on other instructions and the overall architecture should be considered.
3. Create the encoding scheme for the new instructions. Define the bit fields for opcodes, data operands, and control bits needed to differentiate instructions from each others.

Since most of the instructions that would be defined in this lab use two vectors of data elements to produce another vector of data, we decide to follow the R-type instruction format of RISC-V.


![](https://course.playlab.tw/md/uploads/5b663653-b516-4fdc-9d2e-9361090074d9.png)


### Lab13-1-1 SIMD ISA Extension Proposal

#### Signed Integer Multiplication
Before looking over the definition of instructions in the extension proposed for this lab, let us discuss about the output precision of arithmetic operations.

|  Operation  |      Lossless Bit-width      |
|:-----------:|:----------------------------:|
| `z = x + y` | `w(z) = max(w(x), w(y)) + 1` |
| `z = x - y` | `w(z) = max(w(x), w(y)) + 1` |
| `z = x * y` |     `w(z) = w(x) + w(y)`     |

In practice, an additional bit is required for addition and subtraction operations to hold full precision of result values. However, to prevent data width expansion, we typically disregard the most significant bit of computed results, potentially causing overflow issues. This technique is commonly applied to arithmetic instructions within the RV32I ISA.

However, this approach is not suitable for multiplication. As indicated in the table above, neglecting half of the output bits to maintain single-instruction computing may result in significant errors. Therefore, in the subsequent section, we introduced not only an INT8-to-INT8 multiplication instruction but also two INT8-to-INT16 multiplication instructions to ensure the return of complete results.

#### SIMD ISA Extension Definition
The details of all instructions proposed for this lab are described in [Appx. 13-3](#Appx-13-3-SIMD-Extension-Proposal-Instruction-List). The extension consists of the following categories:

- ADD: vector addition operations
- SUB: vector subtraction operations
- PMUL: precise(lossless) signed integer multiplication
- AMUL: approximate(lossy) signed integer multiplication
- QNT/CFG: quantization operations and corresponding configuration setup

To implement these instructions in CFU Playground, we should adjust the design of instructions to become compatible with this platform. Fortunately, the CFU Playground natively supports the R-type instruction format, so we only need to replace our opcode with the one that CFU Playground assigned.

:::success
In this lab, we will demonstrate the hardware and software implementation of the following instructions:

- **ADD**
    - `sADDI8I8S.vv`
    - `sADDI16I16S.vv`
- **SUB**
    - `sSUBI8I8S.vv`
    - `sSUBI16I16S.vv`
- **PMUL**
    - `sPMULI8I16S.vv.L`
    - `sPMULI8I16S.vv.H`
- **AMUL**
    - `sAMULI8I8S.vv.NQ`

The rest of the instructions should be implemented in your homework for practice.
:::

### Lab13-1-2 Function Unit Design
The following picture is the micro-architecture for our SIMD instrcutions in CFU Playground (the signals for handshaking between CPU and CFU are ignored).
![](https://course.playlab.tw/md/uploads/3a38b597-e4a0-4e11-b942-b074d31d4167.png)

To fulfill the required functions mentioned in [Lab 13-2-1](#13-2-1-SIMD-ISA-Extension-Proposal), the example design of the SIMD Execution Engine consists of these components: 

- SIMD Addition & Activation Unit
    - Perform arithmetic operations (addition & substraction), comparisons, and activation functions against `rs1` and `rs2` according to `opSel`.
- SIMD Multiplication Unit
    - Perform arithmetic operations (multiplication), comparisons against `rs1` and `rs2` according to `mulOpSel`. Output port `rd` should accurately present proper segments of the output based on executed instructions, while `rdMsb` consistently providing the upper 32 bits of the results.
- Register Unit
    - Buffer the higher 32 bits of the SIMD Multiplication Unit results to save power from redundant multiplications of repeated source operands.
- Controller
    - Decode operation selection signals for all execution units and generate handshaking signals (e.g. `cmd_ready` and `rsp_valid`).

#### SIMD Addition & Activation Unit Implementation
> `chisel/src/main/scala/simd/AddSubActivationUnit.scala`

- Define supported operations with `ChiselEnum`.
    ```scala
    object AddSubActivationOp extends ChiselEnum {
      val NONE                       = Value
      val ADDI8I8S_VV, ADDI16I16S_VV = Value
      val SUBI8I8S_VV, SUBI16I16S_VV = Value
    }
    ```
    With this definition, we can simply use `AddSubActivationOp.ADDI8S_VV` in the rest of our hardware implementation, and the Chisel compiler would automatically convert them to a proper value. More information can be found in [Chisel/FIRRTL: Enumerations](https://www.chisel-lang.org/chisel3/docs/explanations/chisel-enum.html).
- Implement supported operations.
    ```scala
    // 8-bit wire assignment
    for (i <- 0 until 4) {
      rs1ByteArray(i) := io.rs1(8 * i + 7, 8 * i)
      rs2ByteArray(i) := io.rs2(8 * i + 7, 8 * i)

      rdByteArray(i) := MuxLookup(
        io.opSel.asUInt,
        DontCare,
        Seq(
          AddSubActivationOp.ADDI8I8S_VV.asUInt -> (rs1ByteArray(i).asSInt + rs2ByteArray(i).asSInt).asUInt,
          AddSubActivationOp.SUBI8I8S_VV.asUInt -> (rs1ByteArray(i).asSInt - rs2ByteArray(i).asSInt).asUInt
        )
      )
    }

    // 16-bit wire assignment
    for (i <- 0 until 2) {
      rs1HalfArray(i) := io.rs1(16 * i + 15, 16 * i)
      rs2HalfArray(i) := io.rs2(16 * i + 15, 16 * i)

      rdHalfArray(i) := MuxLookup(
        io.opSel.asUInt,
        DontCare,
        Seq(
          AddSubActivationOp.ADDI16I16S_VV.asUInt -> (rs1HalfArray(i).asSInt + rs2HalfArray(i).asSInt).asUInt,
          AddSubActivationOp.SUBI16I16S_VV.asUInt -> (rs1HalfArray(i).asSInt - rs2HalfArray(i).asSInt).asUInt
        )
      )
    }

    rdByteConcat := Seq.range(3, -1, -1).map { i => rdByteArray(i) }.reduce(_ ## _)
    rdHalfConcat := Seq.range(1, -1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)
    ```
    - The last two lines concatenate four byte-elements and two half-elements into single 32-bit words respectively in specific sequence.
        - `Seq.range(3, -1, -1)` generates an array `(3, 2, 1, 0)`.
        - `.map { i => rdByteArray(i) }` maps the array `(3, 2, 1, 0)` to `(rdByteArray(3), rdByteArray(2), rdByteArray(1), rdByteArray(0))`.
        - `.reduce(_ ## _)` performs the operation `##` (concatenate) against all neighbor pairs.
- Return proper signals from corresponding wires.
    ```scala
    when(io.opSel.isOneOf(
      AddSubActivationOp.ADDI8I8S_VV,
      AddSubActivationOp.SUBI8I8S_VV
    )) {
      io.rd := rdByteConcat
    }.elsewhen(io.opSel.isOneOf(
      AddSubActivationOp.ADDI16I16S_VV,
      AddSubActivationOp.SUBI16I16S_VV
    )) {
      io.rd := rdHalfConcat
    }.otherwise {
      io.rd := DontCare
    }
    ```
    - The statement `io.opSel.isOneOf(A, B, C)` returns `true` if the signal `io.opSel` matches one of these arguments `A`, `B`, and `C`. Note that the `isOneOf()` is a special method of `ChiselEnum`.

#### SIMD Multiplication Unit Implementation
> `chisel/src/main/scala/simd/MulUnit.scala`

- Define supported operations.
    ```scala
    object MulOp extends ChiselEnum {
      val NONE                                           = Value
      val AMULI8I8S_VV, PMULI8I16S_VV_L, PMULI8I16S_VV_H = Value
    }
    ```
- Implement supported operations.
    ```scala
    // 8-bit wire assignment
    for (i <- 0 until 4) {
      rs1ByteArray(i) := io.rs1(8 * i + 7, 8 * i)
      rs2ByteArray(i) := io.rs2(8 * i + 7, 8 * i)

      when(io.opSel.isOneOf(MulOp.AMULI8I8S_VV, MulOp.PMULI8I16S_VV_L, MulOp.PMULI8I16S_VV_H)) {
        rdHalfArray(i) := (rs1ByteArray(i).asSInt * rs2ByteArray(i).asSInt).asUInt
      }.otherwise {
        rdHalfArray(i) := DontCare
      }
    }

    rdMsbByteConcat := Seq.range(3, -1, -1).map { i => rdHalfArray(i)(15, 8) }.reduce(_ ## _)
    rdLsbHalfConcat := Seq.range(1, -1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)
    rdMsbHalfConcat := Seq.range(3, 1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)
    ```
- Return proper signals from corresponding wires.
    ```scala
    // output assignment
    io.rdMsb := rdMsbHalfConcat
    io.rd    := MuxLookup(
      io.opSel.asUInt,
      DontCare,
      Seq(
        MulOp.AMULI8I8S_VV.asUInt    -> rdMsbByteConcat,
        MulOp.PMULI8I16S_VV_L.asUInt -> rdLsbHalfConcat,
        MulOp.PMULI8I16S_VV_H.asUInt -> rdMsbHalfConcat
      )
    )
    ```
#### Register Unit Implementation
> `chisel/src/main/scala/simd/Register.scala`

- Declare required registers.
    ```scala
    val rsReg    = RegInit(VecInit(Seq.fill(2)(0.U(32.W))))
    val rdMsbReg = RegInit(0.U(32.W))
    ```
- Implement supported operations.
    ```scala
    // register update
    rsReg(0) := Mux(io.wenRs, io.rs1, rsReg(0))
    rsReg(1) := Mux(io.wenRs, io.rs2, rsReg(1))
    rdMsbReg := Mux(io.wenRd, io.rdMsbIn, rdMsbReg)
    ```
- Return proper signals from corresponding wires.
    ```scala
    // output signals
    io.rdMsbOut := rdMsbReg
    io.rsMatch  := (io.rs1 === rsReg(0)) & (io.rs2 === rsReg(1))
    ```

#### Controller Implementation
> `chisel/src/main/scala/simd/Controller.scala`

- This module aims to generate controlling signals (i.e. operation selections) for all components by decoding the `funct7` and `funct3`.
- Another task of controller is to generate handshaking signals to CPU.
    ```scala
    // Output controll
    io.cmd_payload.ready   := true.B
    io.rsp_payload.valid   := io.cmd_payload.valid & io.rsp_payload.ready
    io.rsp_payload.bits.rd := DontCare
    ```
    Please note that due to the specifications of the Chisel syntax, it is necessary to assign some values to `io.rsp_payload.bits.rd` to prevent floating output. However, this signal is not actually generated by the Controller. Consequently, in the top module, we should connect the controller output to the top module output before the module that actually generates `io.rsp_payload.bits.rd`, ensuring that it is correctly overridden by the latter module.

#### SIMD Execution Engine (Top Module)
> `chisel/src/main/scala/simd/SIMDEngine.scala`

- To simplify the descriptions of the I/O signals, we declare the input and output signal bundles according to CPU↔︎CFU interface.
    ```scala
    class CfuInPayload extends Bundle {
      val funct7 = UInt(7.W)
      val funct3 = UInt(3.W)
      val rs1    = SInt(32.W)
      val rs2    = SInt(32.W)
    }

    class CfuOutPayload extends Bundle {
      val rd = SInt(32.W)
    }
    ```
- The top module is responsible to initiate submodules, connect their interfaces, and provide (or select) a correct source of the output data.
    ```scala
    io.rsp_payload.bits.rd := MuxLookup(
      controller.io.outputSel.asUInt,
      DontCare,
      Seq(
        OutputSel.ADDSUB.asUInt -> addSubActivationUnit.io.rd.asSInt,
        OutputSel.MUL.asUInt    -> mulUnit.io.rd.asSInt,
        OutputSel.REG.asUInt    -> register.io.rdMsbOut.asSInt
      )
    )
    ```
- Please ensure that the assignment of `io.rsp_payload.bits.rd` is placed after the bulk connection `controller.io.rsp_payload <> io.rsp_payload` because `io.rsp_payload.bits.rd` has previously been assigned to `DontCare` within the Controller.

### Lab13-1-3 Design Verification with Chisel Testbench
```shell
$ cd "/path/to/chisel"
$ sbt 'testOnly simd.lab.*'
```

The above command would test the hardware implementation using testbenches defined in `chisel/src/test/scala/simd/lab/lab2.scala`. The response should be:

```shell
[info] TestLab2Addition:
[info] - SIMD Execution Unit should execute sADDI8I8S.vv instructions
[info] - SIMD Execution Unit should execute sADDI16I16S.vv instructions
[info] TestLab2Substraction:
[info] - SIMD Execution Unit should execute sSUBI8I8S.vv instructions
[info] - SIMD Execution Unit should execute sSUBI16I16S.vv instructions
[info] TestLab2Multiplication:
[info] - SIMD Execution Unit should execute sAMULI8I8S.vv.NQ instructions
[info] - SIMD Execution Unit should execute sPMULI8I16S.vv.L instructions
[info] - SIMD Execution Unit should execute sPMULI8I16S.vv.H instructions
[info] Run completed in 9 seconds, 555 milliseconds.
[info] Total number of tests run: 7
[info] Suites: completed 3, aborted 0
[info] Tests: succeeded 7, failed 0, canceled 0, ignored 0, pending 0
[info] All tests passed.
[success] Total time: XX s, completed MM DD, YYYY, hh:mm:ss
```

### Lab13-1-4 Build Verilog Files for CFU Playground Integration
Use the `App` which wrapped the top module `SIMDEngine` to build its Verilog implementation.

```shell
$ cd "/path/to/chisel"
$ sbt 'runMain simd.SIMDEngineApp'
```

The generated verilog file would be stored as `chisel/build/SIMDEngine.v`.

Lab13-2 : AI Operator Acceleration
---

### Lab13-2-1 Kernel Function Implementation
After ISA and Function Unit have been purposed, in this section, we will demonstrate how to deploy the kernel functions for adding INT8/INT16 data and multiplying 4 INT8 data.

The source code mentioned in this section can be found in
1. `src/acal_lab/includes/instruction/simdInst.h`
2. `src/acal_lab/includes/instruction/simdFuse.h`
3. `src/acal_lab/libs/instruction/simdFuse.cc`

#### Signed Integer Addition & Substration
In CFU Playground, we leverage its API, `cfu_op();`. The following is how we implement the `sADDI8I8S.vv` / `sADDI16I16S.vv` and `sSUBI8S.vv` / `sSUBI16S.vv` in its software stack.

- `sADDI8I8S.vv` : INT8 vector-vector Addition
    ```cpp
    inline void simd_addi8i8s_vv(int8_t c[4], int8_t a[4], int8_t b[4])
    {
        *(int32_t *)c = cfu_op(0b000, 0b0000000, *(int32_t *)a, *(int32_t *)b);
    }
    ```
- `sADDI16I16S.vv` : INT16 vector-vector Addition
    ```cpp
    inline void simd_addi16i16s_vv(int16_t c[2], int16_t a[2], int16_t b[2])
    {
        *(int32_t *)c = cfu_op(0b001, 0b0000000, *(int32_t *)a, *(int32_t *)b);
    }
    ```
- `sSUBI8I8S.vv` : INT8 vector-vector Substration
    ```cpp
    inline void simd_subi8i8s_vv(int8_t c[4], int8_t a[4], int8_t b[4])
    {
        *(int32_t *)c = cfu_op(3b000, 7b0000001, *(int32_t *)a, *(int32_t *)b);
    }
    ```
- `sSUBI16I16S.vv` : INT16 vector-vector Substration
    ```cpp
    inline void simd_subi16s_vv(int16_t c[2], int16_t a[2], int16_t b[2])
    {
       *(int32_t *)c = cfu_op(3b001, 7b0000001, *(int32_t *)a, *(int32_t *)b);
    }
    ```
#### Signed Integer Precise(Lossless) Multiplication
Also, the following is how we implement the `sPMULI8I16S.vv.L` and `sPMULI8I16S.vv.H`   in its software stack.

- `sPMULI8I16S.vv.L` : Multiply INT8 and Return Lower 2 of the INT16 Elements
    ```cpp
    inline void simd_pmuli8i16s_vv_l(int16_t c[4], int8_t a[4], int8_t b[4])
    {
        // c[0] = (int16_t)(a[0]) s* (int16_t)(b[0]);
        // c[1] = (int16_t)(a[1]) s* (int16_t)(b[1]);
        *(int32_t *)c = cfu_op(0b100, 0b0000010, *(int32_t *)a, *(int32_t *)b);
    }
    ```
- `sPMULI8I16S.vv.H` : Multiply INT8 and Return Higher 2 of the INT16 Elements
    ```cpp
    inline void simd_pmuli8i16s_vv_h(int16_t c[4], int8_t a[4], int8_t b[4])
    {
        // c[2] = (int16_t)(a[2]) s* (int16_t)(b[2]);
        // c[3] = (int16_t)(a[3]) s* (int16_t)(b[3]);
        *(int32_t *)(c+2) = cfu_op(0b101, 0b0000010, *(int32_t *)a, *(int32_t *)b);
    }
    ```
- Precise(Lossless) Multiplication Kernel Function (`INT8 * INT8 = INT16`)
    ```cpp
    inline void simd_pmuli8i16s_vv(int16_t c[4], int8_t a[4], int8_t b[4])
    {
        simd_pmuli8i16s_vv_l(c, a, b);
        simd_pmuli8i16s_vv_h(c, a, b);
    }
    ```

#### Signed Integer Approximate(Lossy) Multiplication with Naive Quantization
- `sAMULI8I8S.vv.NQ` : Multiply INT8 and Return 4 of the INT8 Elements
    ```cpp
    inline void simd_amuli8i8s_vv(int8_t c[4], int8_t a[4], int8_t b[4])
    {
        // c[0] = a[0] + b[0];
        // c[1] = a[1] + b[1];
        // c[2] = a[2] + b[2];
        // c[3] = a[3] + b[3];
        *(int32_t *)c = cfu_op(0b000, 0b0000010, *(int32_t *)a, *(int32_t *)b);
    }
    ```
:::success
- Test Signed Integer Instruction (.vv) | Check Lab13-0-2 - Test in CFU Playground
    ```script
    > ~/CFU-Playground/proj/lab13 $ make renode-headless
    CFU Playground
    ==============
     ...
     3: Project menu
     ...     
    main> 3

    Running Project menu

    Project Menu
    ============
     0: test SIMD instruction - Integer Extension (Lab Edition)
     ...
    project> 0

    Running test SIMD instruction - Integer Extension (Lab Edition)
    =============== SIMD Vector-Vector ================
    [ TEST ] `sADDI8I8S_vv`     :               100/100
    [ TEST ] `sADDI16I16S_vv`   :               100/100
    [ TEST ] `sSUBI8I8S_vv`     :               100/100
    [ TEST ] `sSUBI16I16S_vv`   :               100/100
    [ TEST ] `sPMULI8I16S_vv_L` : only .L       100/100
    [ TEST ] `sPMULI8I16S_vv_H` : only .H       100/100
    [ TEST ] `sPMULI8I16S_vv`   : .L Before .H  100/100
    [ TEST ] `sPMULI8I16S_vv`   : .H Before .L  100/100
    [ TEST ] `sAMULI8I8S_vv_NQ` :               100/100
    ---------------------------------------------------
    SIMD Integer Extension : (Lab Edition)       | Pass
    ===================================================
    ```

- If the terminal displays "Pass", it indicates that the corresponding SIMD Instruction with SW/HW support has executed successfully.
:::
### Lab13-2-2 GEMM Acceleration

**GEMM** (GEneral Matrix Multiplication) is a fundamental building block for many operations in neural networks, for example fully-connected layers (FCN), recurrent layers such as RNNs or LSTMs, and convolutional layers(CNN).

The following "gemm" function is a common C implementation for matrix multiplication. All matrices are stored in memory in row-major order, and the computation results are also stored in row-major order. Our data and output are stored in the format of `int8_t`.
- General GEMM Operation :
    ```cpp
    void gemm_general(int8_t **A, int8_t **B, int8_t **C, int dimM, int dimK, int dimN)
    {
      for (int m = 0; m < dimM; m++)
        for (int n = 0; n < dimN; n ++)
          for (int k = 0; k < dimK; k++) 
            C[m][n] += A[m][k] * B[k][n];
    }
    ```
- With 'loop unrolling' methodology
    - Matrix A and B are stored in row-major order. As a consequence, we select "n"
        ```cpp
        void gemm_unroll(int8_t **A, int8_t **B, int8_t **C, int dimM, int dimK, int dimN)
        {
            int8_t temp_0, temp_1, temp_2, temp_3;
                for (int m = 0; m < dimM; m++)
                    for (int k = 0; k < dimK; k++)
                        for (int n = 0; n < dimN; n += 4) 
                        {
                            // C[m][n]   += A[m][k]*B[k][n];
                            temp_0 = A[m][k] * B[k][n];
                            C[m][n] += temp_0;
                            // C[m][n+1] += A[m][k]*B[k][n+1];
                            temp_1 = A[m][k] * B[k][n + 1];
                            C[m][n + 1] += temp_1;
                            // C[m][n+2] += A[m][k]*B[k][n+2];
                            temp_2 = A[m][k] * B[k][n + 2];
                            C[m][n + 2] += temp_2;
                            // C[m][n+3] += A[m][k]*B[k][n+3];
                            temp_3 = A[m][k] * B[k][n + 3];
                            C[m][n + 3] += temp_3;
                        }
        }
        ```
- re-order the C code
    ```cpp
    void gemm_reorder(int8_t **A, int8_t **B, int8_t **C, int dimM, int dimK, int dimN)
    {
        int16_t temp_0, temp_1, temp_2, temp_3;
        for (int m = 0; m < dimM; m++)
            for (int k = 0; k < dimK; k++)
                for (int n = 0; n < dimN; n += 4) 
                {
                    temp_0 = A[m][k] * B[k][n];
                    temp_1 = A[m][k] * B[k][n + 1];
                    temp_2 = A[m][k] * B[k][n + 2];
                    temp_3 = A[m][k] * B[k][n + 3];
                    C[m][n]   += temp_0;
                    C[m][n+1] += temp_1;
                    C[m][n+2] += temp_2;
                    C[m][n+3] += temp_3;
                }
    }
    ```
- replaced by SIMD instruction
    - In order to effectively utilize the `SMUL` SIMD instruction, it is essential to carefully prepare the respective input data sets, denoted as 4 elements of A and B.
    - The algorithm shows that A should be broadcasted to each B element.
    ```cpp=
    void gemm_simd(int8_t **A, int8_t **B, int8_t **C, int dimM, int dimK, int dimN)
    {
      int16_t temp_C[4] = {0, 0, 0, 0};
      int8_t  temp_A[4], temp_B[4];

      for (int m = 0; m < dimM; m++)
        for (int k = 0; k < dimK; k++)
          for (int n = 0; n < dimN; n += 4)
          {
            // temp_0 = A[m][k] * B[k][n];
            // temp_1 = A[m][k] * B[k][n + 1];
            // temp_2 = A[m][k] * B[k][n + 2];
            // temp_3 = A[m][k] * B[k][n + 3];
            for (int i = 0; i < 4; ++i) temp_A[i] = input->data[index_A + k];
            *(int32_t *)temp_B = *(int32_t *)&B[k][n];
            sPMULI8I8S_vv(temp_C, temp_A, temp_B);  // sPMULI8I16S.vv.[L/H]
            // C[m][n]   += temp_0;
            // C[m][n+1] += temp_1;
            // C[m][n+2] += temp_2;
            // C[m][n+3] += temp_3;
            sADDI8I8S_vv((int8_t *)&(C[m][n]), (int8_t *)&(C[m][n]), (int8_t *)(temp_C));  // sADDI8I8S.vv
          }
    }
    ```
- code can be found at `src/acal_lab/libs/op/simd/Gemm.cc`

:::success
- Test General Matrix Multiplication | Check Lab13-0-2 - Test in CFU Playground
    ```script
    > ~/CFU-Playground/proj/lab08 $ make renode-headless
    CFU Playground
    ==============
     1: TfLM Models menu
     2: Functional CFU Tests
     3: Project menu
     4: Performance Counter Tests
     5: TFLite Unit Tests
     6: Benchmarks
     7: Util Tests
     8: Embench IoT
    main> 3
    Running Project menu
    Project Menu
    ============
     ...
     a: test GEMM Operator    - with SIMD Extension (Lab Edition)
     ...
    project> a (<--- press 'a' in keyboard)

    Running test GEMM Operator    - with SIMD Extension (Lab Edition)
    ===================   GEMM with SIMD   ======================
    [ TEST ] `GEMM`  : per Operation  Naive  Quantization 100/100
    -------------------------------------------------------------
    `GEMM` with SIMD (LAB Edition)                         | Pass
    ```
- If the terminal displays "Pass", it indicates that the corresponding SIMD Instruction with SW/HW support has executed successfully.
:::

Lab13-3 : Quantization
---

### Lab13-3-1 Introduction
With certain mathematical operations, the resultant data require broader bit fields to hold their full precision. For example:

- Add (M bits, N bits) produce max(M, N) bits of output data.
- Multiply (M bits, N bits) produce M+N bits of output data.

In [Lab 13-1](#Lab-13-1--SIMD-Extension-Proposal-and-Function-Unit-Design), apart from `sMULI8I16S.vv.L` and `sMULI8I16S.vv.H`, we introduced the `sMULI8I8S.vv` instruction which eliminates half of the bits to yield outputs matching the bit-width of the inputs. This serves as an instance of quantization, wherein values from a larger set are mapped to a relatively smaller set. However, in Lab 8-2, we simply truncated the last 8 bits of the generated products, which may not produce meaningful values and could potentially lead to significant impacts on target workloads.

In this section, we are going to introduce another quantization approach to deal with the bit-width mismatch between input and output data.

![](https://miro.medium.com/v2/resize:fit:886/1*gqviJmE8NCoIMh_ewEbJCg.png)

The above diagram shows an example of mapping floating numbers with a broader range to 4-bit integer with only 16 distinct values. This operation usually requires two parameters for performing the quantization procedure: $S$ denoting scaling factors and $Z$ denoting zero point. The quantized value can be determined by

$$ 
Q = round(\frac{R}{S}+Z) 
$$

where $R$ represents original data and $Q$ represents quantized data.

In the homework sections, you should apply this mechanism on `sMULI8I8S.vv` and some other instructions specifically designed for quantization for further computing tasks.

### Lab13-3-2 Instruction for Quantization
To make our SIMD extension produce INT8 results from INT8 multiplications more properly as well as support quantization schemes, we defined `sQNT.INFO` instruction for programmers (and compilers) to configure quantization parameters $S$ and $Z$ so that the instructions involving quantization (e.g. `sMULI8I8S.vv`, `sMULI8I8S.vx`, and `sQNT.I.H2B`) can use them for further calculations.

For simplicity, we limit the SIMD extension to support scaling factors of only powers of two (i.e. $S = 2^{- rs1}$). The detailed definition of quantization instructions is listed in [Appendix 13-3-1-4-2](#13-3-1-4-2-Advance-Quantization) and [Appendix 13-3-2](#13-3-2-Quantization).

Homework 13
===

Hw13-1 : SIMD Extra Instruction Design
---
In Lab 13-1 and 13-2, we have demonstrated the hardware design for several vector-vector SIMD instructions. Another common SIMD instruction design is vector-scalar type, which performs a specific operation on a vector of data with a common scalar. These instructions are also useful in AI workloads. In this homework, please implement the following instructions in the Custom Function Unit of CFU Playground.

- Arithmetic Vector-Scalar Instructions
    - ADD: `sADDI8I8S.vx` / `sADDI16I16S.vx`
    - SUB: `sSUBI8I8S.vx` / `sSUBI16I16S.vx`
    - PMUL: `sPMULI8I16S.vx.L` / `sPMULI8I16S.vx.H`
    - AMUL: `sAMULI8I8S.vx.NQ`  / `sAMULI8I8S.vx.AQ`
- Arithmetic Vector-Vector Instructions
    - AMUL: `sAMULI8I8S.vv.AQ`
- Configuring Quantization Unit
    - CFG: `sQNT.INFO`
- SIMD Quantization
    - QNT: `sQNTI16I8S.vv.NQ` / `sQNTI16I8S.vv.AQ` 

### Design Verification

Please use chisel testbenches defined in `src/test/scala/simd/homework/hw8-2.scala` to verify your hardware design.

```shell
$ cd "/path/to/chisel" 
$ sbt 'testOnly simd.hw.*'
```

The output before finishing this homework would be:

```
[info] Total number of tests run: 17
[info] Suites: completed 4, aborted 0
[info] Tests: succeeded 7, failed 10, canceled 0, ignored 0, pending 0
[info] *** 10 TESTS FAILED ***
[error] Failed tests:
[error] 	simd.hw.TestHw2Addition
[error] 	simd.hw.TestHw2Multiplication
[error] 	simd.hw.TestHw2Quantization
[error] 	simd.hw.TestHw2Substraction
[error] (Test / testOnly) sbt.TestsFailedException: Tests unsuccessful
```

### Hint Hw13-1-1 SIMD Instructions in CFU Playground
Chisel testbench verifies the behavior of instructions in hardware design. To simualate in CFU playground, you should execute `make renode-headless` in the terminal and press "3" and "1".

:::success
- Test Signed Integer Instruction | Check Lab13-0-2 - Test in CFU Playground
    ```script
    > ~/CFU-Playground/proj/lab13 $ make renode-headless
    CFU Playground
    ==============
     ...
     3: Project menu
     ...     
    main> 3

    Running Project menu

    Project Menu
    ============
     ...
     1: test SIMD instruction - Integer Extension (Homework Edidtion)
     ...
    project> 1
    Running test SIMD instruction - Integer Extension (Homework Edidtion)
    =============== SIMD Vector-Vector ================
    [ TEST ] `sADDI8I8S_vv`     :               100/100
    [ TEST ] `sADDI16I16S_vv`   :               100/100
    [ TEST ] `sSUBI8I8S_vv`     :               100/100
    [ TEST ] `sSUBI16I16S_vv`   :               100/100
    [ TEST ] `sPMULI8I16S_vv_L` : only .L       100/100
    [ TEST ] `sPMULI8I16S_vv_H` : only .H       100/100
    [ TEST ] `sPMULI8I16S_vv`   : .L Before .H  100/100
    [ TEST ] `sPMULI8I16S_vv`   : .H Before .L  100/100
    [ TEST ] `sAMULI8I8S_vv_NQ` :               100/100
    [ TEST ] `sAMULI8I8S_vv_AQ` :                 0/100
    ---------------------------------------------------
    => SUMMARY | SIMD Vector-Vector :            | Fail
    =============== SIMD Vector-Scalar ================
    [ TEST ] `sADDI8I8S_vx`     :                 0/100
    [ TEST ] `sADDI16I16S_vx`   :                 0/100
    [ TEST ] `sSUBI8I8S_vx`     :                 0/100
    [ TEST ] `sSUBI16I16S_vx`   :                 0/100
    [ TEST ] `sPMULI8I16S_vx_L` : only .L         0/100
    [ TEST ] `sPMULI8I16S_vx_H` : only .H         1/100
    [ TEST ] `sPMULI8I16S_vx`   : .L Before .H    0/100
    [ TEST ] `sPMULI8I16S_vx`   : .H Before .L    1/100
    [ TEST ] `sAMULI8I8S_vx_NQ` :                 0/100
    [ TEST ] `sAMULI8I8S_vx_AQ` :                 0/100
    ---------------------------------------------------
    => SUMMARY | SIMD Vector-Scalar :            | Fail
    ---------------------------------------------------
    SIMD Integer Extension : (Homework Edition)  | Fail
    ===================================================
    ```
- If the terminal displays "Pass", it indicates that the corresponding SIMD Instruction with SW/HW support has executed successfully.
:::

Hw13-2 : CONV Acceleration
---
Conv2D is one of the most widely used techniques in Convolutional Neural Networks (CNN). In the area of machine learning, we often use this operator to extract features from 2D images or feature maps. Please refer to [Conv2d: Finally Understand What Happens in the Forward Pass](https://towardsdatascience.com/conv2d-to-finally-understand-what-happens-in-the-forward-pass-1bbaafb0b148) to understand the detailed operations happened during the forward path of Conv2D and the meanings of its parameters. In this homework, you are going to implement a Conv2D function in C/C++ and leverage the SIMD instructions designed in your Custom Function Unit to minimize the execution time. Necessary parameters are defined below: 

- Tensor size
    - Input feature maps
    - Kernel tensors
    - Output feature maps
- Stride
- Padding
- Dilation
- Groups

In this convolution function, both multiplication and addition operations should be precision preserved (i.e. use INT8 → INT16 multiplication instructions instead of the INT8 → INT8 version). After all multiplications finished, please quantize the INT16 output feature maps to INT8 with the following parameters. The output feature map of the convolution function should be an INT8 tensor.

- Scaling factor
- Bias

:::info
- [name=petersu3]

**ADD BY classmate** 
hint -> the naming for TA's code : 

opt -> output 
ipt -> input 

偷偷說: 其實每個testcase的stride都是1 (包括後面的alexnet)
:::
### Hint 1 : Convolution Implementation methodology.
The following shows the general implementation of the convolution.
1. Direct Convolution
2. Winograd convolution: [CVPR 2016 : Fast Algorithms for Convolutional Neural Networks](https://ieeexplore.ieee.org/document/7780804)
    - This algorithm achieves a reduction in the number of multiplications by introducing additional additions. (Addition operation has more efficiency and lower latency compared multiplication operations.)
3. Im2Col + GEMM
    -  The convolution operation can be viewed as a GEMM operator. This algorithm re-arranges the input data with the Im2Col technique to transform it into a matrix.
4. Implict GEMM

### Hint 2 : Software Implementation in CFU Playground
To implement the Convolution Operation in the CFU Playground, please make the following modifications to the provided code.

- `src/acal_lab/libs/op/simd/Conv.cc`
    ```cpp
    #include "acal_lab/includes/op/simd/Conv.h"

    void acal_lab::Conv::execPerOperationNaiveQuant()
    {
        /********************************************************
         * TODO:                                                *
         * For Homework 8.3, implement CONV with per Operation  *
         * Naive Quantization. Utilize `sAMULI8I8S(.vv/.vx).NQ` *
         * to generate int8 output.                             *
         *******************************************************/
    }
    ```

:::warning
1. If you choose method 3 (Im2Col + GEMM) or 2 (Winograd convolution), you should initialize the extraBuffer (C, H, W) in `acal_lab::simd::Conv::execPerOperationNaiveQuant()`.
2. The memory API `malloc()` is **not allowed** because CFU-Playground is a bare-metal machine. In this context, you are required to explicitly define the array size, and this specified array will be shared across each convolution operation in AlexNet Model. 
    - `src/acal_lab/libs/op/simd/Conv.cc`
    ```cpp
    #include "acal_lab/includes/op/simd/Conv.h"
    #define _CONV_IM2COL_INPUTS_SIZE_ 100000
    #define _CONV_IM2COL_KERNEL_SIZE_ 100000
    void acal_lab::Conv::execPerOperationNaiveQuant()
    {
        int8_t *im2col_inputs [_CONV_IM2COL_INPUTS_SIZE_];
        int8_t *im2col_kernel [_CONV_IM2COL_KERNEL_SIZE_];
        ...
    }
    ```
:::

To simualate in CFU playground, you should execute `make renode-headless` in the terminal and press "3" and "c". If the terminal displays "Pass", it indicates that the corresponding SIMD Instruction with SW/HW support has executed successfully.

:::success
- Test CONV | Check Lab13-0-2 - Test in CFU Playground
    ```script
    > ~/CFU-Playground/proj/lab13 $ make renode-headless
    CFU Playground
    ==============
     ...
     3: Project menu
     ...     
    main> 3

    Running Project menu

    Project Menu
    ============
     ...
     c: test CONV Operator    - with SIMD Extension (Homework Edidtion)
     ...
    project> c

    Running test CONV Operator    - with SIMD Extension (Homework Edidtion)
    ===================   CONV with SIMD   ======================
    [ TEST ] `CONV`  : per Operation  Naive  Quantization   0/100
    [ TEST ] `CONV`  : per   Layer    Naive  Quantization   0/100
    [ TEST ] `CONV`  : per Operation Advance Quantization   0/100
    [ TEST ] `CONV`  : per   Layer   Advance Quantization   0/100
    -------------------------------------------------------------
    `CONV` with SIMD (Homework Edition)                    | Fail
    ---
    ```
- If the terminal displays "Pass", it indicates that the corresponding SIMD Instruction with SW/HW support has executed successfully.
:::

Hw 13-3 : AlexNet Model Acceleration
---
In the paper "[ImageNet classification with deep convolutional neural networks](https://dl.acm.org/doi/abs/10.1145/3065386)" by Alex et al. in 2017, the authors proposed "Alexnet" architecture to improve the performance of image classification on the ImageNet dataset.

We aim to deploy an AlexNet model utilizing SIMD instructions in the CFU playground. The objective is to perform image classification inference on the CIFAR-10 dataset. Additionally, we need guidance on the quantization methodology to ensure the data-type consistency across operators.

Choosing **per-layer quantization** over per-operation quantization presents numerous advantages. Per-layer quantization exhibits increased stability during the fine-tuning process, as quantization parameters are uniformly applied across the entire layer. This uniform application simplifies the calibration process and contributes to enhancing the model's robustness. 

### Hint for AlexNet Model Deployment
It's essential to pay attention to the following aspects in your homework.
1. In Lab13-2, the GEMM operator with per-operation naive quantization is demonstrated. This implies the need to implement `acal_lab::simd::Gemm::execPerLayerAdvanceQuantization()` in `src/acal_lab/libs/op/simd/gemm.cc`.
2. For Hw13-2, you are required to implement the CONV operator with per-operation naive quantization. In Hw13-4, the task is to implement `acal_lab::simd::Conv::execPerLayerAdvanceQuantization()` in `src/acal_lab/libs/op/simd/conv.cc`.
3. The ReLU operation doesn't involve multiplication, there is no quantization issue. However, lab13 doesn't mention any compare operation with the help of SIMD instruction. You could create any specific instruction in hw13-3 for the comparison operator or even create domain-specific instructions for the RELU operation. In Hw13-3, the task is to implement `acal_lab::simd::RELU::exec()` in `src/acal_lab/libs/op/simd/ReLU.cc`.
4. The MaxPooling (MxPL) operation doesn't involve multiplication, either. The task is to implement `acal_lab::simd::MxPl::exec()` in `src/acal_lab/libs/op/simd/MxPl.cc`.


:::success
- Test AlexNet | Check Lab13-0-2 - Test in CFU Playground
    ```script
    > ~/CFU-Playground/proj/lab13 $ make renode-headless
    CFU Playground
    ==============
     ...
     3: Project menu
     ...     
    main> 3

    Running Project menu

    Project Menu
    ============
     ...
     g: test AlexNet (data0)  - with SIMD Extension (Homework Edition)
     ...
     x: eXit to previous menu
    project> g

    Running test AlexNet (data0)  - with SIMD Extension (Homework Edition)
    =====================   AlexNet with SIMD   =================
    data0 in AlexNet (Homework Edition)                    | Fail
    -------------------------------------------------------------

    Project Menu
    ============
     ...
     h: test AlexNet (daat1)  - with SIMD Extension (Homework Edition)
     ...
     x: eXit to previous menu
    project> h

    Running test AlexNet (daat1)  - with SIMD Extension (Homework Edition)
    =====================   AlexNet with SIMD   =================
    data1 in AlexNet (Homework Edition)                    | Fail
    -------------------------------------------------------------
    ```
- If the terminal displays "Pass", it indicates that the corresponding SIMD Instruction with SW/HW support has executed successfully.
:::

Hw 13-4 : Model and Operator Profile
---
### Performance Counter in CFU Playground
When building an AI accelerator, we usually profile target testbench and applications on our hardware or even the whole computing systems for the following reasons:

- To understand where and how much is room for improvement before starting.
- To identify which operations are the bottleneck for performance.
- To analyze how much the performance of our newly designed system has improved.

With CFU Playground, it is more easier to insert performance counters to record the cycle counts of target operations. For example, if we want to exam how many cycles does the testbench `test SIMD instruction - Integer Extension (Lab Edition)` take, we can modify the following code in `src/proj_menu.cc`

```cpp
void do_SIMD_TB_lab(void)
{
    printf("=============== SIMD Vector-Vector ================\n");
    bool vvTB = tb::tb_sADDS_vv() & tb::tb_sSUBS_vv() & tb::tb_sPMULI8I16S_vv() & tb::tb_sAMULI8I8S_vv_NQ();
    printf("---------------------------------------------------\n");
    printf("SIMD Integer Extension : (Lab Edition)       | %4s\n", vvTB ? "Pass" : "Fail");
    printf("===================================================\n");
}
```

to become:

```cpp
#include "perf.h"

void do_SIMD_TB_lab(void)
{
    printf("Entering loop at: %u\n", perf_get_mcycle());
    printf("=============== SIMD Vector-Vector ================\n");
    bool vvTB = tb::tb_sADDS_vv() & tb::tb_sSUBS_vv() & tb::tb_sPMULI8I16S_vv() & tb::tb_sAMULI8I8S_vv_NQ();
    printf("---------------------------------------------------\n");
    printf("SIMD Integer Extension : (Lab Edition)       | %4s\n", vvTB ? "Pass" : "Fail");
    printf("===================================================\n");
    printf("Exiting loop at: %u\n", perf_get_mcycle());
}
```

The terminal output would look like:

```
Running test SIMD instruction - Integer Extension (Lab Edition)
Entering loop at: 1225907521
=============== SIMD Vector-Vector ================
[ TEST ] `sADDI8I8S_vv`     :               100/100
[ TEST ] `sADDI16I16S_vv`   :               100/100
[ TEST ] `sSUBI8I8S_vv`     :               100/100
[ TEST ] `sSUBI16I16S_vv`   :               100/100
[ TEST ] `sPMULI8I16S_vv_L` : only .L       100/100
[ TEST ] `sPMULI8I16S_vv_H` : only .H       100/100
[ TEST ] `sPMULI8I16S_vv`   : .L Before .H  100/100
[ TEST ] `sPMULI8I16S_vv`   : .H Before .L  100/100
[ TEST ] `sAMULI8I8S_vv_NQ` :               100/100
---------------------------------------------------
SIMD Integer Extension : (Lab Edition)       | Pass
===================================================
Exiting loop at: 1226204854
---
```

From the printed message, we can find that this testbench requires about 297,733 cycles to finish. To simplify performance profiling, you can write a simple parser to collect the number of cycles or even create another variable in the software to accumulate the cycle counts you care about.

#### It's Your Turn

In Hw 13-3, after finish the software implementation for AlexNet model acceleration, you should also do at least two performance analyses with the performance counter infrastructure provided by CFU Playground. Here are some example target for comparison:

- The performance of a specific operator (e.g. Gemm or Conv2D) with scalar and SIMD instructions.
- The performance of the whole AlexNet with scalar and SIMD instructions.
- The performance before and after further improving your SIMD kernels.

Within your analyses, please briefly describe your opinion on the following aspects.

- What is the best performance (upper bound) should the analyzed changes make?
- How much did the changes actually improve the performance?
- What reasons led to these changes not achieving the desired outcome?
- What is the next step for further improvement?

#### How To Insert Performance Counters Before and After Specific Operators
With the `src/acal_lab/includes/op/Op.h` in the lab repository, there are two inline virtual functions `preOp()` and `postOp()` for you to insert any code related to performance profiling. 

```cpp
class Operator {
    // ...skip...
    virtual inline void preOp() {};
    virtual inline void postOp() {};
    void execute()
    {
        preOp();
        (this->*execFunction)();
        postOp();
    };
    // ...skip...
}
```

The prefix `virtual` and `inline` are used for the following purposes.

- `virtual` method: 
    - Derive classes can overwrite this method
    - The customized methods would be executed even with the pointer of the base class.
- `inline` function: 
    - The compiler is encouraged to merge this function (method) into the caller function to reduce overhead.
    - The implementation of these `inline` functions should be written in header files.

## Homework Submission Rule
- **Step 1**
    - 請在自己的 GitLab內建立 `lab13` repo，並將本次 Lab 撰寫的程式碼放入這個repo。另外記得開權限給助教還有老師。
- **Step 2**
    - 請參考[(校名_學號_姓名) ACAL 2024 Spring Lab 13 HW Submission Template](https://course.playlab.tw/md/P36D--fcQ-SEDgLnq4IFYA)，建立(複製一份)並自行撰寫 CodiMD 作業說明文件。請勿更動template裡的內容。
    - 關於 gitlab 開權限給助教群組的方式可以參照以下連結
        - [ACAL 2024 Curriculum GitLab 作業繳交方式說明 : Manage Permission](https://course.playlab.tw/md/CW_gy1XAR1GDPgo8KrkLgg#Manage-Permission)
- **Step 3**
    - When you are done, please submit your homework document link to the Playlab 作業中心, <font style="color:blue"> 清華大學與陽明交通大學的同學請注意選擇對的作業中心鏈結</font>
        - [清華大學Playlab 作業中心](https://nthu-homework.playlab.tw/course?id=2)
        - [陽明交通大學作業繳交中心](https://course.playlab.tw/homework/course?id=2)

Reference
===
- [RISC-V "P" Extension v0.9](https://github.com/riscv/riscv-p-spec/tree/master)
- [Boosting Machine Learning with tailored accelerators: Custom Function Units in Renode](https://antmicro.com/blog/2021/09/cfu-support-in-renode/)

Appendix 
===
Appx. 13-1 SIMD Extension Proposal: Naming Convention
---
|     | Type |   Vector-Vector    |   Vector-Scalar    | Appx.     |
|:---:|:----:|:------------------:|:------------------:|:--------- |
|  1  | ADD  |   `sADDI8I8S.vv`   |   `sADDI8I8S.vx`   | 13-3-1-1   |
|     |      |  `sADDI16I16S.vv`  |  `sADDI16I16S.vx`  |           |
|  1  | SUB  |   `sSUBI8I8S.vv`   |   `sSUBI8I8S.vx`   | 13-3-1-2   |
|     |      |  `sSUBI16I16S.vv`  |  `sSUBI16I16S.vx`  |           |
|  1  | PMUL | `sPMULI8I16S.vv.L` | `sPMULI8I16S.vx.L` | 13-3-1-3   |
|     |      | `sPMULI8I16S.vv.H` | `sPMULI8I16S.vx.H` |           |
|  1  | AMUL | `sAMULI8I8S.vv.NQ` | `sAMULI8I8S.vx.NQ` | 13-3-1-4-1 |
|     |      | `sAMULI8I8S.vv.AQ` | `sAMULI8I8S.vx.AQ` | 13-3-1-4-2 |
|  1  | QNT  | `sQNTI16I8S.vv.NQ` |                    | 13-3-2     |
|  1  |      | `sQNTI16I8S.vv.AQ` |                    | 13-3-2     |
|  2  | CFG  |    `sQNT.INFO`     |                    | 13-3-2     |

1. Arithmetic Instruction : 
    ```
    s{ADD, SUB, PMUL, AMUL, QNT}{I8,I16}{I8,I16}{S,U}.{vv,vx}{-, .L,.H}{-, .NQ, .AQ}
    ```
    - `ADD` / `SUB` / `PMUL` / `AMUL`
        1. `ADD`: Addition
        2. `SUB`: Substration
        3. `PMUL`: Precise(Lossless) Multiplication
        4. `AMUL`: Approximate(Lossy) Multiplication
        5. `QNT` : Quantization
    - `I8` / `I16` (1) :
        - Represents the input data type.
    - `I8` / `I16` (2) :
        - Represents the output data type.
    - `S` / `U` :
        - Indicates that the data is signed or unsigned.
    - `vv` / `vx` :
        - Indicates the forms of the two source operands. The former indicates both are vectors, while the latter indicates one is a vector and the other is a scalar.
    - `-` / `.L` / `.H` : (only `PMUL`)
        - If the output width is 8 bytes, it is used to distinguish between obtaining the lower word and the higher word
    - `-` / `.NQ` / `.AQ`: Quantization Type (only `AMUL` and `QNT`)
        - `NQ` : Naive Quantization
        - `AQ` : Advance Quantization
2. Quantization Configuation: `sQNT.INFO`

Appx. 13-2 SIMD Extension Proposal: System Registers
---
- Quantization
    - `R[scaling_factor]`: The scaling factor represented in minus power of two for quantization operations.
    - `R[zero_point]`: The zero point (a.k.a bias) for quantization operations.

Appx. 13-3 SIMD Extension Proposal: Instruction List
---
    
|               INST |   [31:25]   | [24:20] | [19:15] |  [14:12]   | [11:7] |   [6:0]    |
| ------------------:|:-----------:|:-------:|:-------:|:----------:|:------:|:----------:|
|            **ADD** | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
|     `sADDI8I8S.vv` | `7b0000000` |         |         |  `3b000`   |        |    SIMD    |
|   `sADDI16I16S.vv` | `7b0000000` |         |         |  `3b001`   |        |    SIMD    |
|     `sADDI8I8S.vx` | `7b1000000` |         |         |  `3b000`   |        |    SIMD    |
|   `sADDI16I16S.vx` | `7b1000000` |         |         |  `3b001`   |        |    SIMD    |
|            **SUB** | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
|     `sSUBI8I8S.vv` | `7b0000001` |         |         |  `3b000`   |        |    SIMD    |
|   `sSUBI16I16S.vv` | `7b0000001` |         |         |  `3b001`   |        |    SIMD    |
|     `sSUBI8I8S.vx` | `7b1000001` |         |         |  `3b000`   |        |    SIMD    |
|   `sSUBI16I16S.vx` | `7b1000001` |         |         |  `3b001`   |        |    SIMD    |
|           **PMUL** | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
| `sPMULI8I16S.vv.L` | `7b0000010` |         |         |  `3b100`   |        |    SIMD    |
| `sPMULI8I16S.vv.H` | `7b0000010` |         |         |  `3b101`   |        |    SIMD    |
| `sPMULI8I16S.vx.L` | `7b1000010` |         |         |  `3b100`   |        |    SIMD    |
| `sPMULI8I16S.vx.H` | `7b1000010` |         |         |  `3b101`   |        |    SIMD    |
|           **AMUL** | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
| `sAMULI8I8S.vv.NQ` | `7b0000010` |         |         |  `3b000`   |        |    SIMD    |
| `sAMULI8I8S.vv.AQ` | `7b0000010` |         |         |  `3b001`   |        |    SIMD    |
| `sAMULI8I8S.vx.NQ` | `7b1000010` |         |         |  `3b000`   |        |    SIMD    |
| `sAMULI8I8S.vx.AQ` | `7b1000010` |         |         |  `3b001`   |        |    SIMD    |
|        **QNT/CFG** | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
|        `sQNT.INFO` | `7b0000111` |         |         |  `3b000`   |        |    SIMD    |
| `sQNTI16I8S.vv.NQ` | `7b0000111` |         |         |  `3b001`   |        |    SIMD    |
| `sQNTI16I8S.vv.AQ` | `7b0000111` |         |         |  `3b010`   |        |    SIMD    |

### 13-3-1 Integer Extension

#### 13-3-1-1 Signed Integer Addition

##### Instruction Encoding
    
|             INST |   [31:25]   | [24:20] | [19:15] |  [14:12]   | [11:7] |   [6:0]    |
| ----------------:|:-----------:|:-------:|:-------:|:----------:|:------:|:----------:|
|                  | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
|   `sADDI8I8S.vv` | `7b0000000` |         |         |  `3b000`   |        |    SIMD    |
| `sADDI16I16S.vv` | `7b0000000` |         |         |  `3b001`   |        |    SIMD    |
|   `sADDI8I8S.vx` | `7b1000000` |         |         |  `3b000`   |        |    SIMD    |
| `sADDI16I16S.vx` | `7b1000000` |         |         |  `3b001`   |        |    SIMD    |

##### Behavior Description
1. `sADDI8I8S.vv rd(v), rs1(v), rs2(v)` : INT8 Vector-Vector Addition
    - Description: 
        This instruction adds the 8-bit integer vector from `rs1` with the 8-bit integer **vector** from `rs2`, and then writes the 8-bit integer vector results to `rd`.
    - Operation:
        ```cpp
        Rd.B[x] = Rs1.B[x] + Rs2.B[x];
        for RV32: x=3..0
        ```
2. `sADDI16I16S.vv rd(v), rs1(v), rs2(v)` : INT16 Vector-Vector Addition
    - Description: 
        This instruction adds the 16-bit integer vector from `rs1` with the 16-bit integer **vector** from `rs2`, and then writes the 16-bit integer vector results to `rd`.
    - Operation:
        ```cpp
        Rd.H[x] = Rs1.H[x] + Rs2.H[x];
        for RV32: x=1..0
        ```
3. `sADDI8I8S.vx rd(v), rs1(v), rs2(x)` : INT8 Vector-Scalar Addition
    - Description: 
        This instruction adds the 8-bit integer vector from `rs1` with the 8-bit integer **scalar** from `rs2`, and then writes the 8-bit integer vector results to `rd`.
    - Operation:
        ```cpp
        Rd.B[x] = Rs1.B[x] + Rs2.B[0];
        for RV32: x=3..0
        ```
4. `sADDI16I16S.vx rd(v), rs1(v), rs2(x)` : INT16 Vector-Scalar Addition
    - Description: 
        This instruction adds the 16-bit integer elements in `rs1` with the 16-bit integer **scalar** in `rs2`, and then writes the 16-bit integer element results to `rd`.
    - Operation:
        ```cpp
        Rd.H[x] = Rs1.H[x] + Rs2.H[0];
        for RV32: x=1..0
        ```

#### 13-3-1-2 Signed Integer Substration

##### Instruction Encoding
    
|             INST |   [31:25]   | [24:20] | [19:15] |  [14:12]   | [11:7] |   [6:0]    |
| ----------------:|:-----------:|:-------:|:-------:|:----------:|:------:|:----------:|
|                  | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
|   `sSUBI8I8S.vv` | `7b0000001` |         |         |  `3b000`   |        |    SIMD    |
| `sSUBI16I16S.vv` | `7b0000001` |         |         |  `3b001`   |        |    SIMD    |
|   `sSUBI8I8S.vx` | `7b1000001` |         |         |  `3b000`   |        |    SIMD    |
| `sSUBI16I16S.vx` | `7b1000001` |         |         |  `3b001`   |        |    SIMD    |

##### Behavior Description
1. `sSUBI8I8S.vv rd(v), rs1(v), rs2(v)` : INT8 Vector-Vector Substration
    - Description: 
        This instruction substracts the 8-bit integer vector from `rs1` with the 8-bit integer **vector** from `rs2`, and then writes the 8-bit integer vector results to `rd`.
    - Operation:
        ```cpp
        Rd.B[x] = Rs1.B[x] - Rs2.B[x];
        for RV32: x=3..0
        ```
2. `sSYBI16I16S.vv rd(v), rs1(v), rs2(v)` : INT16 Vector-Vector Substration
    - Description:
        This instruction substracts the 16-bit integer vector from `rs1` with the 16-bit integer **vector** from `rs2`, and then writes the 16-bit integer vector results to `rd`.
    - Operation:
        ```cpp
        Rd.H[x] = Rs1.H[x] - Rs2.H[x];
        for RV32: x=1..0
        ```
3. `sSUBI8I8S.vx rd(v), rs1(v), rs2(x)` : INT8 Vector-Scalar Substration
    - Description:
        This instruction substracts the 8-bit integer vector from `rs1` with the 8-bit integer **scalar** from `rs2`, and then writes the 8-bit integer vector results to `rd`.
    - Operation:
        ```cpp
        Rd.B[x] = Rs1.B[x] - Rs2.B[0];
        for RV32: x=3..0
        ```
4. `sSUBI16I16S.vx rd(v), rs1(v), rs2(x)` : INT16 Vector-Scalar Substration
    - Description: 
        This instruction substracts the 16-bit integer elements in `rs1` with the 16-bit integer **scalar** in `rs2`, and then writes the 16-bit integer vector results to `rd`.
    - Operation:
        ```cpp
        Rd.H[x] = Rs1.H[x] - Rs2.H[0];
        for RV32: x=1..0
        ```

#### 13-3-1-3  Precise(Lossless) Signed Integer Multiplication

##### Instruction Encoding
   
 |               INST |   [31:25]   | [24:20] | [19:15] |  [14:12]   | [11:7] |   [6:0]    |
 | ------------------:|:-----------:|:-------:|:-------:|:----------:|:------:|:----------:|
 |                    | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
 | `sPMULI8I16S.vv.L` | `7b0000010` |         |         |  `3b100`   |        |    SIMD    |
 | `sPMULI8I16S.vv.H` | `7b0000010` |         |         |  `3b101`   |        |    SIMD    |
 | `sPMULI8I16S.vx.L` | `7b1000010` |         |         |  `3b100`   |        |    SIMD    |
 | `sPMULI8I16S.vx.H` | `7b1000010` |         |         |  `3b101`   |        |    SIMD    |

##### Behavior Description
1. `sPMULI8I16S.vv.L rd(v), rs1(v), rs2(v)` : Precise(Lossless) INT8 Vector-Vector Multiplication
    - Description: 
        This instruction multiply the 8-bit integer vector from `rs1` with the 8-bit integer vector from  `rs2`, and then the 16-bit integer vector results are written into an even/odd pair of registers. Return the **lower** two of the 16-bits integer vector to `rd`.
    - Operation:
        ```cpp
        // R[SMUL] are an even/odd pair of registers
        R[SMUL.Lo].H[0] = Rs1.B[0] s* Rs2.B[0];
        R[SMUL.Lo].H[1] = Rs1.B[1] s* Rs2.B[1];
        R[SMUL.Hi].H[0] = Rs1.B[2] s* Rs2.B[2];
        R[SMUL.Hi].H[1] = Rs1.B[3] s* Rs2.B[3];
        Rd.H[0], Rd.H[1] = R[SMUL.Lo].H[0], R[SMUL.Lo].H[1]
        ```
2. `sPMULI8I16S.vv.H rd(v), rs1(v), rs2(v)` : Precise(Lossless) INT8 Vector-Vector Multiplication
    - Description: 
        This instruction multiply the 8-bit integer vector from `rs1` with the 8-bit integer vector from  `rs2`, and then the 16-bit integer vector results are written into an even/odd pair of registers. Return the **higher** two of the 16-bits integer vector to `rd`.
    - Operation:
        ```cpp
        // R[SMUL] are an even/odd pair of registers
        R[SMUL.Lo].H[0] = Rs1.B[0] s* Rs2.B[0];
        R[SMUL.Lo].H[1] = Rs1.B[1] s* Rs2.B[1];
        R[SMUL.Hi].H[0] = Rs1.B[2] s* Rs2.B[2];
        R[SMUL.Hi].H[1] = Rs1.B[3] s* Rs2.B[3];
        Rd.H[0], Rd.H[1] = R[SMUL.Hi].H[0], R[SMUL.Hi].H[1]
        ```
3. `sPMULI8I16S.vx.L rd(v), rs1(v), rs2(x)` : Precise(Lossless) INT8 Vector-Scalar Multiplication
    - Description: 
        This instruction multiply the 8-bit integer vector from `rs1` the 8-bit integer **scalar** from `rs2`, and then the 16-bit integer vector results are written into an even/odd pair of registers. Return the **lower** two of the 16-bits integer vector to `rd`.
    - Operation:
        ```cpp
        // R[SMUL] are an even/odd pair of registers
        R[SMUL.Lo].H[0] = Rs1.B[0] s* Rs2.B[0];
        R[SMUL.Lo].H[1] = Rs1.B[1] s* Rs2.B[0];
        R[SMUL.Hi].H[0] = Rs1.B[2] s* Rs2.B[0];
        R[SMUL.Hi].H[1] = Rs1.B[3] s* Rs2.B[0];
        Rd.H[0], Rd.H[1] = R[SMUL.Lo].H[0], R[SMUL.Lo].H[1]
        ```
4. `sPMULI8I16S.vx.H rd(v), rs1(v), rs2(x)` : Precise(Lossless) INT8 Vector-Scalar Multiplication
    - Description: 
        This instruction multiply the 8-bit integer vector from `rs1` the 8-bit integer **scalar** from `rs2`, and then the 16-bit integer vector results are written into an even/odd pair of registers. Return the **higher** two of the 16-bits integer vector to `rd`.
    - Operation:
        ```cpp
        // R[SMUL] are an even/odd pair of registers
        R[SMUL.Lo].H[0] = Rs1.B[0] s* Rs2.B[0];
        R[SMUL.Lo].H[1] = Rs1.B[1] s* Rs2.B[0];
        R[SMUL.Hi].H[0] = Rs1.B[2] s* Rs2.B[0];
        R[SMUL.Hi].H[1] = Rs1.B[3] s* Rs2.B[0];
        Rd.H[0], Rd.H[1] = R[SMUL.Hi].H[2], R[SMUL.Hi].H[3]
        ```       

#### 13-3-1-4 Approximate(Lossy) Signed Integer Multiplication

##### Instruction Encoding
    
|               INST |   [31:25]   | [24:20] | [19:15] |  [14:12]   | [11:7] |   [6:0]    |
| ------------------:|:-----------:|:-------:|:-------:|:----------:|:------:|:----------:|
|                    | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
| `sAMULI8I8S.vv.NQ` | `7b0000010` |         |         |  `3b000`   |        |    SIMD    |
| `sAMULI8I8S.vv.AQ` | `7b0000010` |         |         |  `3b001`   |        |    SIMD    |
| `sAMULI8I8S.vx.NQ` | `7b1000010` |         |         |  `3b000`   |        |    SIMD    |
| `sAMULI8I8S.vx.AQ` | `7b1000010` |         |         |  `3b001`   |        |    SIMD    |

##### 13-3-1-4-1 Naive Quantization
1. `sAMULI8IS.vv.NQ rd(v), rs1(v), rs2(v)` : Lossy INT8 Vector-Vector Multiplication w/ Naive Quantization
    - Description: 
        This instruction multiplies the 8-bit integer vector from `rs1` with the 8-bit integer **vector** from `rs2`, and then writes the vector of **higher** 8-bit integers results to `rd`.
    - Operation:
        ```cpp
        Rd.B[x] = (Rs1.B[x] s* Rs2.B[x])[15,8];
        for RV32: x=3..0
        ```
2. `sAMULI8I8S.vx.NQ rd(v), rs1(v), rs2(x)` : Lossy INT8 Vector-Scalar Multiplication
    - Description: 
        This instruction multiplies the 8-bit integer vector from `rs1` with the 8-bit integer **scalar** from `rs2`, and then writes the vector of **higher** 8-bit integers results to `rd`.
    - Operation:
        ```cpp
        Rd.B[x] = (Rs1.B[x] s* Rs2.B[0])[15,8];
        for RV32: x=3..0
        ```

##### 13-3-1-4-2 Advance Quantization
1. `sAMULI8I8S.vv.AQ rd(v), rs1(v), rs2(v)` :
    - Description:
        This instruction multiplies the 8-bit integer vector from `rs1` with the 8-bit integer **vector** from `rs2`, and then quantizes the produced 16-bit integers into 8-bit integers with the following equation
        $$
        Q = \frac{R}{S'} + Z
        $$
        where $S'=$ `2^(-1 * R[scaling_factor])` and $Z=$ `R[zero_point]` before writes the vector of 8-bit integers results to `rd`.
    - Operation:
        ```cpp
        Rd.B[x] = (Rs1.B[x] s* Rs2.B[x])
        Rd.B[x] = (Rd.B[x] >> R[scaling_factor]) + R[zero_point]
        for RV32: x=3..0
        ```
2. `sAMULI8I8S.vx.AQ rd(v), rs1(v), rs2(x)` :
    - Description:
        This instruction multiplies the 8-bit integer vector from `rs1` with the 8-bit integer **scalar** from `rs2`, and then quantizes the produced 16-bit integers into 8-bit integers with the following equation
        $$
        Q = \frac{R}{S'} + Z
        $$
        where $S'=$ `2^(-1 * R[scaling_factor])` and $Z=$ `R[zero_point]` before writes the vector of 8-bit integers results to `rd`.
    - Operation:
        ```cpp
        Rd.B[x] = (Rs1.B[x] s* Rs2.B[0])
        Rd.B[x] = (Rd.B[x] >> R[scaling_factor]) + R[zero_point]
        for RV32: x=3..0
        ```

### 13-3-2 Quantization

#### Instruction Encoding
    
|               INST |   [31:25]   | [24:20] | [19:15] |  [14:12]   | [11:7] |   [6:0]    |
| ------------------:|:-----------:|:-------:|:-------:|:----------:|:------:|:----------:|
|                    | **funct7**  | **rs2** | **rs1** | **funct3** | **rd** | **opcode** |
|        `sQNT.INFO` | `7b0000111` |         |         |  `3b000`   |        |    SIMD    |
| `sQNTI16I8S.vv.NQ` | `7b0000111` |         |         |  `3b001`   |        |    SIMD    |
| `sQNTI16I8S.vv.AQ` | `7b0000111` |         |         |  `3b010`   |        |    SIMD    |

#### Behavior Description
1. `sQNT.INFO  rs1, rs2` : Setup Quantization Parameters (scaling factor and zero point) in CFU unit.
    - Description: 
        Configure parameters for the quantization unit. Both `rs1` and `rs2` are signed integers. Further quantization operations can be done in the following manner.
        - Scaling factor $S = 2^{- rs1}$
        - zero point $Z = rs2$
    - Operation: 
        ```cpp
        R[scaling_factor] = rs1;
        R[zero_point]     = rs2;
        ```
2. `sQNTI16I8S.vv.NQ` : Quantize an INT16 vector to INT8 vector (Naive Quantization)
    - Description: 
        This instruction quantize the 16-bit integer vector from `rs1` and `rs2`, and then the 8-bit integer vector results to `rd`.
    - Operation:
        ```cpp
        Rd.B[0] = Rs1.H[0] >> 8;
        Rd.B[1] = Rs1.H[1] >> 8;
        Rd.B[2] = Rs2.H[0] >> 8;
        Rd.B[3] = Rs2.H[1] >> 8;
        ```
3. `sQNTI16I8S.vv.AQ` : Quantize an INT16 vector to INT8 vector with Quantization Parameters (Advance Quantization)
    - Description: 
        This instruction quantize the 16-bit integer vector from `rs1` and `rs2`, and then the 8-bit integer vector results to `rd`. The quantization operation is the same as the formula described in [Appx. 13-3-1-4-2 Advance Quantization](#13-3-1-4-2-Advance-Quantization).
    - Operation:
        ```cpp
        Rd.B[0] = Quantize(Rs1.H[0], R[scaling_factor], R[zero_point]);
        Rd.B[1] = Quantize(Rs1.H[1], R[scaling_factor], R[zero_point]);
        Rd.B[2] = Quantize(Rs2.H[0], R[scaling_factor], R[zero_point]);
        Rd.B[3] = Quantize(Rs2.H[1], R[scaling_factor], R[zero_point]);
        ```


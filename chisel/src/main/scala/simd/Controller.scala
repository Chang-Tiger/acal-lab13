package simd

import chisel3._
import chisel3.util._

class Controller extends Module {
  val io = IO(new Bundle {
    val cmd_payload = Flipped(Decoupled(new CfuInPayload()))
    val rsp_payload = Decoupled(new CfuOutPayload())

    val rsMatch     = Input(Bool())
    val addSubOpSel = Output(AddSubActivationOp())
    val mulOpSel    = Output(MulOp())
    val quaOpSel    = Output(QuaOp())
    val comOpSel    = Output(ComOp())
    val wenRs       = Output(Bool())
    val wenRd       = Output(Bool())
    val outputSel   = Output(OutputSel())

    val QUA_info    = Output(Bool())
  })

  val funct = io.cmd_payload.bits.funct7 ## io.cmd_payload.bits.funct3
  
  //funct
  when(io.cmd_payload.valid & io.rsp_payload.ready) {
    // AddSubActivationUnit controll
    io.addSubOpSel := MuxLookup(
      funct,
      AddSubActivationOp.NONE,
      Seq(
        "b0000000_000".U -> AddSubActivationOp.ADDI8I8S_VV,
        "b0000000_001".U -> AddSubActivationOp.ADDI16I16S_VV,
        "b0000001_000".U -> AddSubActivationOp.SUBI8I8S_VV,
        "b0000001_001".U -> AddSubActivationOp.SUBI16I16S_VV,

        //modified for hw
        "b1000000_000".U -> AddSubActivationOp.ADDI8I8S_VX,
        "b1000000_001".U -> AddSubActivationOp.ADDI16I16S_VX,
        "b1000001_000".U -> AddSubActivationOp.SUBI8I8S_VX,
        "b1000001_001".U -> AddSubActivationOp.SUBI16I16S_VX
      )
    )

    // AddSubActivationUnit MulUnit controll
    io.mulOpSel := MuxLookup(
      funct,
      MulOp.NONE,
      Seq(
        "b0000010_000".U -> MulOp.AMULI8I8S_VV,
        "b0000010_100".U -> MulOp.PMULI8I16S_VV_L,
        "b0000010_101".U -> Mux(
          io.rsMatch,
          MulOp.NONE,
          MulOp.PMULI8I16S_VV_H
        ),

        //modified for hw
        "b1000010_100".U -> MulOp.PMULI8I16S_VX_L,
        "b1000010_101".U -> Mux(
          io.rsMatch,
          MulOp.NONE,
          MulOp.PMULI8I16S_VX_H
        ),
        "b1000010_000".U -> MulOp.AMULI8I8S_VX_NQ,
        "b1000010_001".U -> MulOp.AMULI8I8S_VX_AQ,
        "b0000010_001".U -> MulOp.AMULI8I8S_VV_AQ,
      )
    )
    //modified ====
    io.quaOpSel := MuxLookup(
      funct,
      QuaOp.NONE,
      Seq(
        "b0000111_000".U -> QuaOp.QNT_INFO,
        "b0000111_001".U -> QuaOp.QNTI16I8S_VV_NQ,
        "b0000111_010".U -> QuaOp.QNTI16I8S_VV_AQ
      )
    )
    io.comOpSel := MuxLookup(
      funct,
      ComOp.NONE,
      Seq(
        "b0000000_110".U -> ComOp.RELUI8I8S_VX,
        "b0000001_110".U -> ComOp.MAXPLI8I8S_VX
      )
    )

    // Register controll
    io.wenRs := MuxLookup(
      funct,
      false.B,
      Seq(
        "b0000010_100".U -> true.B,
        "b1000010_100".U -> true.B
      )
    )
    io.wenRd := MuxLookup(
      funct,
      false.B,
      Seq(
        "b0000010_100".U -> true.B,
        "b1000010_100".U -> true.B
      )
    )

    io.QUA_info := MuxLookup(funct, false.B, Seq(
        "b0000111_000".U -> true.B
      ))
  }.otherwise {
    io.addSubOpSel := AddSubActivationOp.NONE
    io.mulOpSel    := MulOp.NONE
    io.quaOpSel    := QuaOp.NONE
    io.comOpSel    := ComOp.NONE
    io.wenRs       := false.B
    io.wenRd       := false.B
    io.QUA_info     :=false.B
  }

  // Output controll
  io.cmd_payload.ready   := true.B
  io.rsp_payload.valid   := io.cmd_payload.valid & io.rsp_payload.ready
  io.rsp_payload.bits.rd := DontCare

  when(io.addSubOpSel =/= AddSubActivationOp.NONE) {
    io.outputSel := OutputSel.ADDSUB
  }.elsewhen(io.mulOpSel =/= MulOp.NONE) {
    io.outputSel := OutputSel.MUL
  }.elsewhen(io.quaOpSel =/= QuaOp.NONE) {
    io.outputSel := OutputSel.QUA
  }.elsewhen(io.comOpSel =/= ComOp.NONE){
    io.outputSel := OutputSel.COM
  }.otherwise {
    io.outputSel := OutputSel.REG
  }
}

package simd

import chisel3._
import chisel3.util._
import chisel3.experimental.ChiselEnum

object AddSubActivationOp extends ChiselEnum {
  val NONE                  = Value
  val ADDI8S_VV, ADDI16S_VV = Value
  val SUBI8S_VV, SUBI16S_VV = Value
}

class AddSubActivationUnit extends Module {
  val io = IO(new Bundle {
    val opSel = Input(AddSubActivationOp())
    val rs1   = Input(UInt(32.W))
    val rs2   = Input(UInt(32.W))
    val rd    = Output(UInt(32.W))
  })

  val rs1ByteArray = Wire(Vec(4, UInt(8.W)))
  val rs2ByteArray = Wire(Vec(4, UInt(8.W)))
  val rdByteArray  = Wire(Vec(4, UInt(8.W)))
  val rdByteConcat = Wire(UInt(32.W))
  val rs1HalfArray = Wire(Vec(2, UInt(16.W)))
  val rs2HalfArray = Wire(Vec(2, UInt(16.W)))
  val rdHalfArray  = Wire(Vec(2, UInt(16.W)))
  val rdHalfConcat = Wire(UInt(32.W))

  // 8-bit wire assignment
  for (i <- 0 until 4) {
    rs1ByteArray(i) := io.rs1(8 * i + 7, 8 * i)
    rs2ByteArray(i) := io.rs2(8 * i + 7, 8 * i)

    rdByteArray(i) := MuxLookup(
      io.opSel.asUInt,
      DontCare,
      Seq(
        AddSubActivationOp.ADDI8S_VV.asUInt -> (rs1ByteArray(i).asSInt + rs2ByteArray(i).asSInt).asUInt,
        AddSubActivationOp.SUBI8S_VV.asUInt -> (rs1ByteArray(i).asSInt - rs2ByteArray(i).asSInt).asUInt
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
        AddSubActivationOp.ADDI16S_VV.asUInt -> (rs1HalfArray(i).asSInt + rs2HalfArray(i).asSInt).asUInt,
        AddSubActivationOp.SUBI16S_VV.asUInt -> (rs1HalfArray(i).asSInt - rs2HalfArray(i).asSInt).asUInt
      )
    )
  }

  rdByteConcat := Seq.range(3, -1, -1).map { i => rdByteArray(i) }.reduce(_ ## _)
  rdHalfConcat := Seq.range(1, -1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)

  when(io.opSel.isOneOf(
    AddSubActivationOp.ADDI8S_VV,
    AddSubActivationOp.SUBI8S_VV
  )) {
    io.rd := rdByteConcat
  }.elsewhen(io.opSel.isOneOf(
    AddSubActivationOp.ADDI16S_VV,
    AddSubActivationOp.SUBI16S_VV
  )) {
    io.rd := rdHalfConcat
  }.otherwise {
    io.rd := DontCare
  }
}

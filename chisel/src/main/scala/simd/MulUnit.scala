package simd

import chisel3._
import chisel3.util._
import chisel3.experimental.ChiselEnum

object MulOp extends ChiselEnum {
  val NONE                                        = Value
  val MULI8I8S_VV, MULI8I16S_VV_L, MULI8I16S_VV_H = Value
}

class MulUnit extends Module {
  val io = IO(new Bundle {
    val opSel = Input(MulOp())
    val rs1   = Input(UInt(32.W))
    val rs2   = Input(UInt(32.W))
    val rdMsb = Output(UInt(32.W))
    val rd    = Output(UInt(32.W))
  })

  val rs1ByteArray    = Wire(Vec(4, UInt(8.W)))
  val rs2ByteArray    = Wire(Vec(4, UInt(8.W)))
  val rdHalfArray     = Wire(Vec(4, UInt(16.W)))
  val rdMsbByteConcat = Wire(UInt(32.W))
  val rdLsbHalfConcat = Wire(UInt(32.W))
  val rdMsbHalfConcat = Wire(UInt(32.W))

  // 8-bit wire assignment
  for (i <- 0 until 4) {
    rs1ByteArray(i) := io.rs1(8 * i + 7, 8 * i)
    rs2ByteArray(i) := io.rs2(8 * i + 7, 8 * i)

    when(io.opSel.isOneOf(MulOp.MULI8I8S_VV, MulOp.MULI8I16S_VV_L, MulOp.MULI8I16S_VV_H)) {
      rdHalfArray(i) := (rs1ByteArray(i).asSInt * rs2ByteArray(i).asSInt).asUInt
    }.otherwise {
      rdHalfArray(i) := DontCare
    }
  }

  rdMsbByteConcat := Seq.range(3, -1, -1).map { i => rdHalfArray(i)(15, 8) }.reduce(_ ## _)
  rdLsbHalfConcat := Seq.range(1, -1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)
  rdMsbHalfConcat := Seq.range(3, 1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)

  // output assignment
  io.rdMsb := rdMsbHalfConcat
  io.rd    := MuxLookup(
    io.opSel.asUInt,
    DontCare,
    Seq(
      MulOp.MULI8I8S_VV.asUInt    -> rdMsbByteConcat,
      MulOp.MULI8I16S_VV_L.asUInt -> rdLsbHalfConcat,
      MulOp.MULI8I16S_VV_H.asUInt -> rdMsbHalfConcat
    )
  )
}

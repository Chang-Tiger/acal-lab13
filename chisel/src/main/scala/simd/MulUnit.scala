package simd

import chisel3._
import chisel3.util._
import chisel3.experimental.ChiselEnum

object MulOp extends ChiselEnum {
  val NONE                                           = Value
  val AMULI8I8S_VV, PMULI8I16S_VV_L, PMULI8I16S_VV_H = Value

  //modified for hw
  val PMULI8I16S_VX_L, PMULI8I16S_VX_H, AMULI8I8S_VX_NQ, AMULI8I8S_VX_AQ, AMULI8I8S_VV_AQ = Value
}

class MulUnit extends Module {
  val io = IO(new Bundle {
    val opSel = Input(MulOp())
    val rs1   = Input(UInt(32.W))
    val rs2   = Input(UInt(32.W))

    val scale = Input(UInt(8.W))
    val zpoint = Input(UInt(8.W))
    val s_pos     = Input(Bool())
    val z_pos     = Input(Bool())
    val q_info    = Input(Bool())

    val rdMsb = Output(UInt(32.W))
    val rd    = Output(UInt(32.W))
  })

  val rs1ByteArray    = Wire(Vec(4, UInt(8.W)))
  val rs2ByteArray    = Wire(Vec(4, UInt(8.W)))
  val rdHalfArray     = Wire(Vec(4, UInt(16.W)))
  val rdByteArrayQ     = Wire(Vec(4, UInt(8.W)))
  val rdMsbByteConcat = Wire(UInt(32.W))
  val rdLsbHalfConcat = Wire(UInt(32.W))
  val rdMsbHalfConcat = Wire(UInt(32.W))
  val rdByteConcatQ = Wire(UInt(32.W))
//
  val Scaling  = RegInit(0.U(8.W))
  val Zpoint  = RegInit(0.U(8.W))
  val Scaling_or  = RegInit(0.U(8.W))
  val Zpoint_or  = RegInit(0.U(8.W))


  // 8-bit wire assignment
  for (i <- 0 until 4) {
    rs1ByteArray(i) := io.rs1(8 * i + 7, 8 * i)
    rs2ByteArray(i) := io.rs2(8 * i + 7, 8 * i)

    when(io.opSel.isOneOf(MulOp.AMULI8I8S_VV, MulOp.PMULI8I16S_VV_L, MulOp.PMULI8I16S_VV_H, MulOp.AMULI8I8S_VV_AQ)) {
      rdHalfArray(i) := (rs1ByteArray(i).asSInt * rs2ByteArray(i).asSInt).asUInt
    }.elsewhen(io.opSel.isOneOf(MulOp.AMULI8I8S_VX_NQ, MulOp.PMULI8I16S_VX_L, MulOp.PMULI8I16S_VX_H, MulOp.AMULI8I8S_VX_AQ)) {
      rdHalfArray(i) := (rs1ByteArray(i).asSInt * rs2ByteArray(0).asSInt).asUInt
    }.otherwise {
      rdHalfArray(i) := DontCare
    }
  }
  for (i <- 0 until 4) {
    when(Scaling_or.asSInt > 0.S){
        when(Zpoint_or.asSInt >= 0.S){
          rdByteArrayQ(i)   := ((rdHalfArray(i) >> Scaling) + Zpoint).asUInt
        }.otherwise{
          rdByteArrayQ(i)   := ((rdHalfArray(i) >> Scaling) - Zpoint).asUInt
        }  
      }.elsewhen(Scaling_or.asSInt === 0.S){
        when(Zpoint_or.asSInt >= 0.S){
          rdByteArrayQ(i)   := ((rdHalfArray(i)) + Zpoint).asUInt
        }.otherwise{
          rdByteArrayQ(i)   := ((rdHalfArray(i)) - Zpoint).asUInt
        } 
      }.otherwise {
        when(Zpoint_or.asSInt >= 0.S){
          rdByteArrayQ(i)   := ((rdHalfArray(i) << Scaling) + Zpoint).asUInt
        }.otherwise{
          rdByteArrayQ(i)   := ((rdHalfArray(i) << Scaling) - Zpoint).asUInt
        }  
      }
  }

  rdMsbByteConcat := Seq.range(3, -1, -1).map { i => rdHalfArray(i)(15, 8) }.reduce(_ ## _)
  rdLsbHalfConcat := Seq.range(1, -1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)
  rdMsbHalfConcat := Seq.range(3, 1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)
  rdByteConcatQ := Seq.range(3, -1, -1).map { i => rdByteArrayQ(i) }.reduce(_ ## _)

//
  when(io.q_info) {
      Scaling_or := io.rs1
      Zpoint_or := io.rs2
      when(io.rs1(7,0).asSInt > 0.S){
        Scaling := io.rs1(7,0).asUInt
      }.elsewhen(io.rs1(7,0).asSInt === 0.S){
        Scaling := 0.U
      }.otherwise {
        Scaling := ((-1.S * io.rs1(7,0).asSInt).asUInt)
      }
      when(io.rs2(7,0).asSInt >= 0.S){
        Zpoint := io.rs2(7,0).asUInt
      }.otherwise {
        Zpoint := (-1.S * io.rs2(7,0).asSInt).asUInt
      }
    }

  // output assignment
  io.rdMsb := rdMsbHalfConcat
  io.rd    := MuxLookup(
    io.opSel.asUInt,
    DontCare,
    Seq(
      MulOp.AMULI8I8S_VV.asUInt    -> rdMsbByteConcat,
      MulOp.PMULI8I16S_VV_L.asUInt -> rdLsbHalfConcat,
      MulOp.PMULI8I16S_VV_H.asUInt -> rdMsbHalfConcat,

      //modified for hw
      MulOp.PMULI8I16S_VX_L.asUInt -> rdLsbHalfConcat,
      MulOp.PMULI8I16S_VX_H.asUInt -> rdMsbHalfConcat,
      MulOp.AMULI8I8S_VX_NQ.asUInt -> rdMsbByteConcat,

      MulOp.AMULI8I8S_VX_AQ.asUInt -> rdByteConcatQ,
      MulOp.AMULI8I8S_VV_AQ.asUInt -> rdByteConcatQ
    ),

  )
}

package simd

import chisel3._
import chisel3.util._
import chisel3.experimental.ChiselEnum

object QuaOp extends ChiselEnum {
  val NONE                              = Value
  val QNTI16I8S_VV_AQ, QNTI16I8S_VV_NQ,QNT_INFO  =Value
}

class QuantizationUnit extends Module {
  val io = IO(new Bundle {
    val opSel = Input(QuaOp())
    val rs1   = Input(UInt(32.W))
    val rs2   = Input(UInt(32.W))
    val rd    = Output(UInt(32.W))

    //modified
    val scale_out = Output(UInt(8.W))
    val zpoint_out = Output(UInt(8.W))
    val s_pos     = Output(Bool())
    val z_pos     = Output(Bool())
  })


  val rs1HalfArray = Wire(Vec(2, UInt(16.W)))
  val rs2HalfArray = Wire(Vec(2, UInt(16.W)))
  val rdByteArray  = Wire(Vec(4, UInt(8.W)))
  //val rdHalfArray  = Wire(Vec(4, UInt(16.W)))
  val rdByteConcat = Wire(UInt(32.W))
  val Scaling  = RegInit(0.U(8.W))
  val Zpoint  = RegInit(0.U(8.W))
  val Scaling_or  = RegInit(0.U(8.W))
  val Zpoint_or  = RegInit(0.U(8.W))

  // 8-bit wire assignment
  for (i <- 0 until 2) {
    rs1HalfArray(i) := io.rs1(16 * i + 15, 16 * i)
    rs2HalfArray(i) := io.rs2(16 * i + 15, 16 * i)

    when(io.opSel.isOneOf(QuaOp.QNTI16I8S_VV_NQ)) {
      rdByteArray(i)    := (rs1HalfArray(i) >> 8.U).asUInt
      rdByteArray(i+2)  := (rs2HalfArray(i) >> 8.U).asUInt
    }.elsewhen(io.opSel.isOneOf(QuaOp.QNTI16I8S_VV_AQ)) {
      when(Scaling_or.asSInt > 0.S){
        when(Zpoint_or.asSInt >= 0.S){
          rdByteArray(i)   := ((rs1HalfArray(i) >> Scaling) + Zpoint).asUInt
          rdByteArray(i+2) := ((rs2HalfArray(i) >> Scaling) + Zpoint).asUInt
        }.otherwise{
          rdByteArray(i)   := ((rs1HalfArray(i) >> Scaling) - Zpoint).asUInt
          rdByteArray(i+2) := ((rs2HalfArray(i) >> Scaling) - Zpoint).asUInt
        }  
      }.elsewhen(Scaling_or.asSInt === 0.S){
        when(Zpoint_or.asSInt >= 0.S){
          rdByteArray(i)   := ((rs1HalfArray(i)) + Zpoint).asUInt
          rdByteArray(i+2) := ((rs2HalfArray(i)) + Zpoint).asUInt
        }.otherwise{
          rdByteArray(i)   := ((rs1HalfArray(i)) - Zpoint).asUInt
          rdByteArray(i+2) := ((rs2HalfArray(i)) - Zpoint).asUInt
        } 
      }.otherwise {
        when(Zpoint_or.asSInt >= 0.S){
          rdByteArray(i)   := ((rs1HalfArray(i) << Scaling) + Zpoint).asUInt
          rdByteArray(i+2) := ((rs2HalfArray(i) << Scaling) + Zpoint).asUInt
        }.otherwise{
          rdByteArray(i)   := ((rs1HalfArray(i) << Scaling) - Zpoint).asUInt
          rdByteArray(i+2) := ((rs2HalfArray(i) << Scaling) - Zpoint).asUInt
        }  
      }
      
    }.otherwise {
      rdByteArray(i) := DontCare
      rdByteArray(i+2) := DontCare
    }
  }
  
  when(io.opSel.isOneOf(QuaOp.QNT_INFO)) {
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

  rdByteConcat := Seq.range(3, -1, -1).map { i => rdByteArray(i) }.reduce(_ ## _)


  // output assignment
  io.rd    := MuxLookup(
    io.opSel.asUInt,
    DontCare,
    Seq(
      QuaOp.QNTI16I8S_VV_NQ.asUInt -> rdByteConcat,
      QuaOp.QNTI16I8S_VV_AQ.asUInt -> rdByteConcat
    ),

  )

  io.scale_out  := Scaling
  io.zpoint_out := Zpoint
  io.s_pos := (Scaling_or.asSInt >= 0.S)
  io.z_pos := (Zpoint_or.asSInt >= 0.S)
}

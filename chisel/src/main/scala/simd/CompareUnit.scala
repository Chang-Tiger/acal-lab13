package simd

import chisel3._
import chisel3.util._
import chisel3.experimental.ChiselEnum

object ComOp extends ChiselEnum {
  val NONE         = Value
  val RELUI8I8S_VX, MAXPLI8I8S_VX = Value
}

class CompareUnit extends Module {
  val io = IO(new Bundle {
    val opSel = Input(ComOp())
    val rs1   = Input(UInt(32.W))
    val rs2   = Input(UInt(32.W))
    val rd    = Output(UInt(32.W))

    //modified
    
  })

  val rs1ByteArray    = Wire(Vec(4, UInt(8.W)))
  val rs2ByteArray    = Wire(Vec(4, UInt(8.W)))
  
  val rdByteArray  = Wire(Vec(4, UInt(8.W)))
  //val rdHalfArray  = Wire(Vec(4, UInt(16.W)))
  val rdByteConcat = Wire(UInt(32.W))
  //val max_num = Wire(UInt(32.W))
  val rs1Vec = io.rs1.asTypeOf(Vec(4, UInt(8.W)))



  for (i <- 0 until 4) {
    rs1ByteArray(i) := io.rs1(8 * i + 7, 8 * i)
    rs2ByteArray(i) := io.rs2(8 * i + 7, 8 * i)

    when(io.opSel.isOneOf(ComOp.RELUI8I8S_VX)) {
      rdByteArray(i) := Mux((rs1ByteArray(i).asSInt > 0.S), rs1ByteArray(i).asUInt, 0.U)
    }.elsewhen(io.opSel.isOneOf(ComOp.MAXPLI8I8S_VX)){
        when(i.U === 0.U){
            //rdByteArray(i) := io.rs1.reduce((a, b) => Mux(a.asSInt > b.asSInt, a, b))
            rdByteArray(i) := rs1Vec.reduce((a, b) => Mux(a.asSInt > b.asSInt, a, b))
        }.otherwise {
            rdByteArray(i) := 0.U
        }
    }.otherwise {
      rdByteArray(i) := DontCare
    }
  }
  

  rdByteConcat := Seq.range(3, -1, -1).map { i => rdByteArray(i) }.reduce(_ ## _)

  io.rd    := MuxLookup(
    io.opSel.asUInt,
    DontCare,
    Seq(
      ComOp.RELUI8I8S_VX.asUInt    -> rdByteConcat,
      ComOp.MAXPLI8I8S_VX.asUInt   -> rdByteConcat
    ),

  )
  
}

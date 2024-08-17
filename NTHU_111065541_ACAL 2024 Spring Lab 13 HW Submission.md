(NTHU_111065541_張騰午) ACAL 2024 Spring Lab 13 HW Submission
===


###### tags: `AIAS Spring 2024` `Submission Template`



[toc]

## Gitlab code link


- Gitlab link - https://course.playlab.tw/git/Tiger_Chang/lab13.git

# Homework 13
## Hw 13-1 : SIMD Extra Instruction Design
### Hardware Architecture
> 請附上你的硬體架構圖
> ![](https://course.playlab.tw/md/uploads/63d604ea-0de8-4186-8f9a-12141abb741f.jpg)
controller根據funct3與funct7決定opsel哪個unit，另外還有QUA_info給mul與quazitation unit通知他們進行qua的指令處理

### Chisel Implementation
> 請附上你修改的檔案路徑及主要的實作片段並做說明，下方的分項是助教的建議，同學可依自己設計的概念及具體架構增減項目。

- SIMDEngine: [Link](https://course.playlab.tw/git/Tiger_Chang/lab13/-/blob/main/chisel/src/main/scala/simd/SIMDEngine.scala?ref_type=heads) ← Fix me!!!
    ```scala=
        class SIMDEngine extends Module {
      val io = IO(new Bundle {
        val cmd_payload = Flipped(Decoupled(new CfuInPayload()))
        val rsp_payload = Decoupled(new CfuOutPayload())
      })

      val controller           = Module(new Controller())
      val addSubActivationUnit = Module(new AddSubActivationUnit())
      val mulUnit              = Module(new MulUnit())
      val register             = Module(new Register())
      //modified===
      val quantizationUnit     = Module(new QuantizationUnit())
      val compareUnit          = Module(new CompareUnit())

      //......

      //modified 新增unit並接線
      quantizationUnit.io.opSel     := controller.io.quaOpSel
      quantizationUnit.io.rs1       := io.cmd_payload.bits.rs1.asUInt
      quantizationUnit.io.rs2       := io.cmd_payload.bits.rs2.asUInt
      mulUnit.io.scale              := quantizationUnit.io.scale_out   
      mulUnit.io.zpoint             := quantizationUnit.io.zpoint_out  
      mulUnit.io.s_pos              := quantizationUnit.io.s_pos  
      mulUnit.io.z_pos              := quantizationUnit.io.z_pos 

      mulUnit.io.q_info             := controller.io.QUA_info

      compareUnit.io.opSel     := controller.io.comOpSel
      compareUnit.io.rs1       := io.cmd_payload.bits.rs1.asUInt
      compareUnit.io.rs2       := io.cmd_payload.bits.rs2.asUInt

//choose output
      io.rsp_payload.bits.rd := MuxLookup(
        controller.io.outputSel.asUInt,
        DontCare,
        Seq(
          OutputSel.ADDSUB.asUInt -> addSubActivationUnit.io.rd.asSInt,
          OutputSel.MUL.asUInt    -> mulUnit.io.rd.asSInt,
          OutputSel.REG.asUInt    -> register.io.rdMsbOut.asSInt,
          OutputSel.QUA.asUInt    -> quantizationUnit.io.rd.asSInt,
          OutputSel.COM.asUInt    -> compareUnit.io.rd.asSInt//modified
        )
      )
    }

    ```
- Controller: [Link](https://course.playlab.tw/git/Tiger_Chang/lab13/-/blob/main/chisel/src/main/scala/simd/Controller.scala?ref_type=heads) ← Fix me!!!
```scala=
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
    //to inform whether is quazitation
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
```
- Add_Sub Unit: [Link](https://course.playlab.tw/git/Tiger_Chang/lab13/-/blob/main/chisel/src/main/scala/simd/AddSubActivationUnit.scala?ref_type=heads)
    ```scala=
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
            AddSubActivationOp.ADDI8I8S_VV.asUInt -> (rs1ByteArray(i).asSInt + rs2ByteArray(i).asSInt).asUInt,
            AddSubActivationOp.SUBI8I8S_VV.asUInt -> (rs1ByteArray(i).asSInt - rs2ByteArray(i).asSInt).asUInt,

            //modified for hw
            AddSubActivationOp.ADDI8I8S_VX.asUInt -> (rs1ByteArray(i).asSInt + rs2ByteArray(0).asSInt).asUInt,
            AddSubActivationOp.SUBI8I8S_VX.asUInt -> (rs1ByteArray(i).asSInt - rs2ByteArray(0).asSInt).asUInt
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
            AddSubActivationOp.SUBI16I16S_VV.asUInt -> (rs1HalfArray(i).asSInt - rs2HalfArray(i).asSInt).asUInt,

            //modified for hw
            AddSubActivationOp.ADDI16I16S_VX.asUInt -> (rs1HalfArray(i).asSInt + rs2HalfArray(0).asSInt).asUInt,
            AddSubActivationOp.SUBI16I16S_VX.asUInt -> (rs1HalfArray(i).asSInt - rs2HalfArray(0).asSInt).asUInt
          )
        )
      }

      rdByteConcat := Seq.range(3, -1, -1).map { i => rdByteArray(i) }.reduce(_ ## _)
      rdHalfConcat := Seq.range(1, -1, -1).map { i => rdHalfArray(i) }.reduce(_ ## _)

      when(io.opSel.isOneOf(
        AddSubActivationOp.ADDI8I8S_VV,
        AddSubActivationOp.SUBI8I8S_VV,
        //modified for hw
        AddSubActivationOp.ADDI8I8S_VX,
        AddSubActivationOp.SUBI8I8S_VX
      )) {
        io.rd := rdByteConcat
      }.elsewhen(io.opSel.isOneOf(
        AddSubActivationOp.ADDI16I16S_VV,
        AddSubActivationOp.SUBI16I16S_VV,
        //modified for hw
        AddSubActivationOp.ADDI16I16S_VX,
        AddSubActivationOp.SUBI16I16S_VX
      )) {
        io.rd := rdHalfConcat
      }.otherwise {
        io.rd := DontCare
      }
    }

    ```
- Multiply Unit: [Link](https://course.playlab.tw/git/Tiger_Chang/lab13/-/blob/main/chisel/src/main/scala/simd/MulUnit.scala?ref_type=heads) ← Fix me!!!
    ```scala=
    class MulUnit extends Module {
      val io = IO(new Bundle {
        val opSel = Input(MulOp())
        val rs1   = Input(UInt(32.W))
        val rs2   = Input(UInt(32.W))

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

    //scaling factor and zero point
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
    ```
- Quantization Unit: [Link](https://course.playlab.tw/git/Tiger_Chang/lab13/-/blob/main/chisel/src/main/scala/simd/QuantizationUnit.scala?ref_type=heads) ← Fix me!!!
    ```scala=
        class QuantizationUnit extends Module {
      val io = IO(new Bundle {
        val opSel = Input(QuaOp())
        val rs1   = Input(UInt(32.W))
        val rs2   = Input(UInt(32.W))
        val rd    = Output(UInt(32.W))
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
        //scaling factor and zero point
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

    }

    ```
- Compare Unit: [Link](https://course.playlab.tw/git/Tiger_Chang/lab13/-/blob/main/chisel/src/main/scala/simd/CompareUnit.scala?ref_type=heads) ← Fix me!!!
    ```scala=
    class CompareUnit extends Module {
      val io = IO(new Bundle {
        val opSel = Input(ComOp())
        val rs1   = Input(UInt(32.W))
        val rs2   = Input(UInt(32.W))
        val rd    = Output(UInt(32.W))
      })

      val rs1ByteArray    = Wire(Vec(4, UInt(8.W)))
      val rs2ByteArray    = Wire(Vec(4, UInt(8.W)))
      val rdByteArray  = Wire(Vec(4, UInt(8.W)))
      val rdByteConcat = Wire(UInt(32.W))
      val rs1Vec = io.rs1.asTypeOf(Vec(4, UInt(8.W)))



      for (i <- 0 until 4) {
        rs1ByteArray(i) := io.rs1(8 * i + 7, 8 * i)
        rs2ByteArray(i) := io.rs2(8 * i + 7, 8 * i)
        //for relu and maxpool
        when(io.opSel.isOneOf(ComOp.RELUI8I8S_VX)) {
            rdByteArray(i) := Mux((rs1ByteArray(i).asSInt > 0.S), rs1ByteArray(i).asUInt, 0.U)
        }.elsewhen(io.opSel.isOneOf(ComOp.MAXPLI8I8S_VX)){
            when(i.U === 0.U){//find max value in rs1 and store in rd[0]
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
    ```

### Result
> 請附上 testbench 執行結果的擷圖

- Chisel Testbench
- ![](https://course.playlab.tw/md/uploads/23fc1665-e22a-427e-8d6a-68deb84748c7.png)

- Software Testbench
- ![](https://course.playlab.tw/md/uploads/0792bae2-be1b-44d6-b370-ee5a9153853d.png)


## Hw 13-2 : CONV Acceleration
### C++ code
請說明你是用哪個 Conv 演算法實作
> PerLayerAdvanceQuant


- `src/acal_lab/libs/op/simd/Conv.cc`
```cpp
//組合兩指令使PMUL可以一次做4個
inline void sPMULI8I16S_vv(int16_t c[4], int8_t a[4], int8_t b[4]) {
	sPMULI8I16S_vv_L(c, a, b);
	sPMULI8I16S_vv_H(c + 2, a, b);
}
void Conv::execPerLayerAdvanceQuant() {
	
	int8_t  temp_A[4], temp_B[4];
	int16_t temp_C[4], temp_D[4]; 
	int output_idx, res_idx, k_W_round;
	k_W_round = (info->kernel.W >> 2) << 2; 
	sQNT_INFO(qInfo->scaling_factor, qInfo->zero_point);

	for (int n = 0; n < info->kernel.N; n++) {
		for (int oh = 0; oh < output->H; oh++) {
			for (int ow = 0; ow < output->W; ow++) {
				output_idx = n * output->H * output->W + oh * output->W + ow;
				res_idx = output_idx % 4;
				temp_D[res_idx] = info->bias.data[n];
				for (int c = 0; c < info->kernel.C; c++) {
					for (int kh = 0; kh < info->kernel.H; kh++) {
						int input_idx = c * input->H * input->W + (oh + kh) * input->W + ow;
						int kernel_idx = ((n * info->kernel.C + c) * info->kernel.H + kh) * info->kernel.W;
						// unroll  for loop by 4
						for (int kw = 0; kw < k_W_round; kw += 4) {
							for (int i=0; i<4;++i){
								temp_A[i] =  input->data[input_idx + kw+i];
								temp_B[i] = info->kernel.data[kernel_idx + kw + i];
							}
							sPMULI8I16S_vv(temp_C, temp_A, temp_B);
							
							// Result
							for (int i=0; i<4;++i){
								temp_D[res_idx] += temp_C[i];
							}
						}
						for (int kw = k_W_round; kw < info->kernel.W; kw++) {//remain part
							temp_D[res_idx] += (int16_t)input->data[input_idx + kw] * (int16_t)info->kernel.data[kernel_idx + kw];
						}
					}
				}
				// PER LAYER QUANTIZATION
				//4個值時quantization並寫入output
				if (res_idx == 3) {
					sQNTI16I8S_vv_AQ(temp_A, temp_D, temp_D + 2);
					output->data[output_idx - 3] = temp_A[0];
					output->data[output_idx - 2] = temp_A[1];
					output->data[output_idx - 1] = temp_A[2];
					output->data[output_idx]     = temp_A[3];
				}
			}
		}
	}
	// PER LAYER QUANTIZATION for remain
	if (res_idx != 3) {
		for (int i = 0; i <= res_idx; i++) {
			output->data[output_idx - (res_idx - i)] = (int8_t)((temp_D[i] >> qInfo->scaling_factor) + qInfo->zero_point);
		}
	}
}
```
- `src/acal_lab/libs/op/scalar/Conv.cc`
```cpp
void Conv::execPerLayerAdvanceQuant() {
    int16_t temp_;
    int input_index, kernel_index;
	for (int n = 0; n < info->kernel.N; n++) {
		for (int oh = 0; oh < output->H; oh++) {
			for (int ow = 0; ow < output->W; ow++) {
				temp_ = info->bias.data[n];
				for (int c = 0; c < info->kernel.C; c++) {
					for (int kh = 0; kh < info->kernel.H; kh++) {

						input_index = c * input->H * input->W + (oh + kh) * input->W + ow;
						kernel_index = ((n * info->kernel.C + c) * info->kernel.H + kh) * info->kernel.W;
						for (int kw = 0; kw < info->kernel.W; kw++) {
							temp_ += (int16_t)input->data[input_index + kw] * (int16_t)info->kernel.data[kernel_index + kw];
						}
					}
				}
				output->data[n * output->H * output->W + oh * output->W + ow] = (int8_t)((temp_ >> qInfo->scaling_factor) + qInfo->zero_point);
			}
		}
	}
}
```

### Result
> 請附上 testbench 執行結果的擷圖
- ![](https://course.playlab.tw/md/uploads/6b395d5d-320a-400a-a21f-cdace95058f2.png)



## Hw 13-3 : AlexNet Model Acceleration
### Gemm
- `src/acal_lab/libs/op/simd/Gemm.cc`
```cpp=
void Gemm::execPerLayerAdvanceQuant() {
	int8_t  temp_A[4], temp_B[4];
	int16_t temp_C[4];
	int16_t tempINT16_Buffer[10000] = {0};//to store all result
	sQNT_INFO(qInfo->scaling_factor, qInfo->zero_point);
	//int remain = info->weight.W % 4
	int n_round, index_A, index_B, index_C;
	n_round = (info->weight.W >> 2) << 2; 
	for (int m = 0; m < input->H; m++) {
		index_A = m * input->W;   // M * K
		index_C = m * output->W;  // M * N
		for (int k = 0; k < input->W; k++) {
			index_B = k * info->weight.W;  // K * N


			for (int n = 0; n < n_round; n += 4) {
				for (int i = 0; i < 4; ++i) { temp_A[i] = input->data[index_A + k];}
				temp_B[0] = info->weight.data[index_B + n];
				temp_B[1] = info->weight.data[index_B + n + 1];
				temp_B[2] = info->weight.data[index_B + n + 2];
				temp_B[3] = info->weight.data[index_B + n + 3];
				//int output_index  = index_C + n;

				// SIMD MUL
				sPMULI8I16S_vv(temp_C, temp_A, temp_B);
				tempINT16_Buffer[index_C + n] += temp_C[0];
				tempINT16_Buffer[index_C + n + 1] += temp_C[1];
				tempINT16_Buffer[index_C + n + 2] += temp_C[2];
				tempINT16_Buffer[index_C + n + 3] += temp_C[3];
			}
			for (int n = n_round; n < info->weight.W; n++){//remain part
				tempINT16_Buffer[index_C + n] +=
				    (int16_t)input->data[index_A + k] * (int16_t)info->weight.data[index_B + n];
			}
		}
		for (int n = 0; n < info->weight.W; n++) { tempINT16_Buffer[index_C + n] += info->bias.data[index_C + n];}
	}
	// PER LAYER QUANTIZATION
	int tempH = 0, tempW = 0;
	int output_w_round = (output->W >> 2) << 2; 
	for (int h = 0; h < output->H; h++) {
		tempH = h * output->W;  // M * N
		for (int w = 0; w < output_w_round; w += 4) {
			tempW = tempH + w;
			temp_C[0] = tempINT16_Buffer[tempW];
			temp_C[1] = tempINT16_Buffer[tempW+1];
			temp_C[2] = tempINT16_Buffer[tempW+2];
			temp_C[3] = tempINT16_Buffer[tempW+3];
			sQNTI16I8S_vv_AQ(temp_A, temp_C, temp_C + 2);//layer quatization
			output->data[tempW] = temp_A[0];
			output->data[tempW + 1] = temp_A[1];
			output->data[tempW + 2] = temp_A[2];
			output->data[tempW + 3] = temp_A[3];
		}
		for (int w = output_w_round; w < output->W; w++){//remain part
			tempW = tempH + w;
			output->data[tempW] = (int8_t)((tempINT16_Buffer[tempW] >> qInfo->scaling_factor) + qInfo->zero_point);
		}
	}
}
```
![](https://course.playlab.tw/md/uploads/fef202a4-1681-482a-94c5-c9d397a1bbd3.png)
### Conv
> 如果 lab13-2 你選擇 PerLayerAdvanceQuant ，這部分就可以跳過了



### 2. RELU
The ReLU operation doesn't involve multiplication, there is no quantization issue. However, lab13 doesn't mention any compare operation with the help of SIMD instruction. You could create any specific instruction in hw13-3 for the comparison operator or even create domain-specific instructions for the RELU operation. In Hw13-3, the task is to implement `acal_lab::simd::RELU::exec()` in `src/acal_lab/libs/op/simd/ReLU.cc`.

- `src/acal_lab/libs/op/simd/ReLU.cc`
```cpp=
void ReLU::exec() {
    int c_idx = 0, h_idx = 0, w_idx = 0;
    int8_t temp_A[4], temp_C[4];
    int8_t temp_B[4] = {0};
	for (int c = 0; c < input->C; c++) {
		c_idx = c * input->H;
		for (int h = 0; h < input->H; h++) {
		    h_idx = (c_idx + h) * input->W;
                   int W_round = (input->W >> 2) << 2;
            //unroll for loop by 4
		    for (int w = 0; w < W_round; w += 4) {
			 w_idx = h_idx + w;
                        temp_A[0] = input->data[w_idx];
                        temp_A[1] = input->data[w_idx+1];
                        temp_A[2] = input->data[w_idx+2];
                        temp_A[3] = input->data[w_idx+3];
                        sRELUI8I8S_vx(temp_C, temp_A, temp_B);

                        output->data[w_idx] = temp_C[0];
                        output->data[w_idx+1] = temp_C[1];
                        output->data[w_idx+2] = temp_C[2];
                        output->data[w_idx+3] = temp_C[3];
		      }
                for (int w = W_round; w < input->W; w++){//remain part
                    w_idx               = h_idx + w;
                    output->data[w_idx] = input->data[w_idx] > 0 ? input->data[w_idx] : 0;
                }
	    }
	}
}
```
### 3. MaxPool
The MaxPooling (MxPL) operation doesn't involve multiplication, either. The task is to implement `acal_lab::simd::MxPl::exec()` in `src/acal_lab/libs/op/simd/MxPl.cc`.

- `src/acal_lab/libs/op/simd/MxPl.cc`
```cpp=
void MxPl::exec() {
    int k_round;
    int8_t  temp_A[4], temp_C[4];
    int8_t  temp_B[4] = {0};
    k_round = (info->kernelSize >> 2) << 2;
    for (int c = 0; c < output->C; c++) {
	for (int oh = 0; oh < output->H; oh++) {
		for (int ow = 0; ow < output->W; ow++) {
                    int    input_start_h = oh * info->stride - info->padding;
                    int    input_start_w = ow * info->stride - info->padding;
                    int8_t max_val       = INT8_MIN;
                    //算出kernel start與end範圍
                    int    input_end_h = input_start_h + info->kernelSize - 1;
                    int    input_end_w = input_start_w + info->kernelSize - 1;
                //只有在範圍全程不會被padding影響時才做平行化
                    if(input_start_h >= 0 && input_end_h < input->H && input_start_w >= 0 && input_end_w < input->W){
                        for (int kh = 0; kh < info->kernelSize; kh++) {
                            for (int kw = 0; kw < k_round; kw += 4) {    
                                int ih = input_start_h + kh;
                                int iw = input_start_w + kw;

                                temp_A[0] = input->data[c * input->H * input->W + ih * input->W + iw];
                                temp_A[1] = input->data[c * input->H * input->W + ih * input->W + (iw + 1)];
                                temp_A[2] = input->data[c * input->H * input->W + ih * input->W + (iw + 2)];
                                temp_A[3] = input->data[c * input->H * input->W + ih * input->W + (iw + 3)];
                                //store the max number of temp_A in temp_C[0]
                                sMAXPLI8I8S_vx(temp_C, temp_A, temp_B);
                                //compare and choose max_val 
                                max_val = (temp_C[0] > max_val) ? temp_C[0] : max_val;
                        }
                        for (int kw = k_round; kw < info->kernelSize; kw++) {//remain
                            int ih = input_start_h + kh;
                            int iw = input_start_w + kw;

                            if (ih >= 0 && ih < input->H && iw >= 0 && iw < input->W) {
                                int8_t val = input->data[c * input->H * input->W + ih * input->W + iw];
                                max_val    = (val > max_val) ? val : max_val;
                            }
                        }
                    }
                //not do unroll
                }else{
                    for (int kh = 0; kh < info->kernelSize; kh++) {
                        for (int kw = 0; kw < info->kernelSize; kw++) {
                            int ih = input_start_h + kh;
                            int iw = input_start_w + kw;

                            if (ih >= 0 && ih < input->H && iw >= 0 && iw < input->W) {
                                int8_t val = input->data[c * input->H * input->W + ih * input->W + iw];
                                max_val    = (val > max_val) ? val : max_val;
                            }
                        }
                    }
                }
				

	        output->data[c * output->H * output->W + oh * output->W + ow] = max_val;
	    	}
	}
    }
}
```
### Result
> 請附上 testbench 執行結果的擷圖
- Software Testbench
![](https://course.playlab.tw/md/uploads/064e4682-bbd2-4168-bc46-86d9f2571584.png)

- ![](https://course.playlab.tw/md/uploads/f9c57ad4-2129-4e24-8ca7-4bb414b14bc8.png)


> 我們以 MNIST 資料集作為 lenet 的資料及當作測試，然而不同 quantization 的關係有可能導致模型數值有些許差異，假使你的答案與預測結果不一致，我們將會從你所撰寫的 conv/gemm/relu/maxpool 程式碼與註解判斷你的演算法是否有錯誤


## Hw 13-4 : Model and Operator Profile
- What is the best performance (upper bound) should the analyzed changes make?
- How much did the changes actually improve the performance?
- What reasons led to these changes not achieving the desired outcome?
- What is the next step for further improvement?

> 請附上你的分析結果
分析ReLu還有Maxpool 使用perf_get_mcycle()計算cycles數量
![](https://course.playlab.tw/md/uploads/e906351f-e792-47a2-90cc-2d077f79f150.png)

![](https://course.playlab.tw/md/uploads/6d30f792-3653-4c8f-9d88-65ea27734aad.png)

1. 我做的平行化都是讓4個數字一起算因此best performance upper bound應該就是cycle數降為1/4，
2. 然而實際上這還得看實作平行化的方式，還有個處理單元間也需要通訊和同步。因此很難真的到達upper boound，以我的實作方式來講，這裡得承認為了在繳交期限內做完我選擇先做比較簡單但不一定是效果較好的方式，
3. 例如，因為MaxPool有padding等較複雜問題，MaxPool我做平行化的部分是在kernel的width上面，且得在不會受padding影響時才使用平行化，加上kernel的大小一般都較小，這樣也會讓平行化的範圍有限，導致平行化效果有限最終只下降了3%左右的cycles。而ReLu也是在input->W上做平行化，一次做4個，少了padding問題結果較好，cycles數下降超過40%。

4. 可以想一些更好的平行化方式，例如我以前別的課有用過OpenMP unroll之類套件，可以一次對多層迴圈unroll，可以嘗試想出如何設計實作。另外另一個直接的方式是維持一樣的平行化方式但可以一次多處理一些數值，我現在只把4個值放到一個reg中，但其實還有一個reg可以使用，這樣理論上可以一次處理8個值應該效果會更好。



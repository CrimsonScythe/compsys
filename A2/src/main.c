#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "support.h"
#include "wires.h"
#include "arithmetic.h"
#include "memory.h"
#include "registers.h"
#include "ip_reg.h"
#include "compute.h"

// major opcodes
#define RETURN         0x0
#define REG_ARITHMETIC 0x1
#define REG_MOVQ       0x2
#define REG_MOVQ_MEM   0x3
#define CFLOW          0x4
#define IMM_ARITHMETIC 0x5
#define IMM_MOVQ       0x6
#define IMM_MOVQ_MEM   0x7
#define LEAQ2          0x8
#define LEAQ3          0x9
#define LEAQ6          0xA
#define LEAQ7          0xB
#define IMM_CBRANCH    0xF        

//minor opcodes
//reg move to memory major op code: 0x3
#define REG_MOVQ_MT    0x9 
//reg move from memory major op code:0x3
#define REG_MOVQ_MF    0x1
//reg move from i(register) major op code: 0x7
#define REG_IMM_MULTF  0x5
//reg move to i(register) major op code: 0x7
#define REG_IMM_MULTT  0xD
// leaq 1 and 2 respectively
#define LEAQ3_MULT     0x2
#define LEAQ3_MULT_ADD 0x3

//LEAQ6 minor codes
#define LEAQ6_IMM      0x4
#define LEAQ6_IMM_ADD  0X5

#define LEAQ7_Z_ADD    0x7

#define JMP 0xF
#define CALL 0xE

int main(int argc, char* argv[]) {
    // Check command line parameters.
    if (argc < 2)
        error("missing name of programfile to simulate");

    if (argc < 3)
        error("Missing starting address (in hex notation)");

    /*** SETUP ***/
    // We set up global state through variables that are preserved between
    // cycles.
    
    // Program counter / Instruction Pointer
    ip_reg_p ip = ip_reg_create();

    // Register file:
    reg_p regs = regs_create();

    // Memory:
    // Shared memory for both instructions and data.
    mem_p mem = memory_create();
    memory_read_from_file(mem, argv[1]);

    int start;
    int scan_res = sscanf(argv[2],"%x", &start);
    if (scan_res != 1)
        error("Unable to interpret starting address");

    if (argc == 4) { // tracefile specified, hook memories to it
        memory_tracefile(mem, argv[3]);
        regs_tracefile(regs, argv[3]);
        ip_reg_tracefile(ip, argv[3]);
    }
    ip_write(ip, from_int(start), true);

    // a stop signal for stopping the simulation.
    bool stop = false;

    // We need the instruction number to show how far we get
    int instruction_number = 0;

    while (!stop) { // for each cycle:
        val pc = ip_read(ip);
        ++instruction_number;
        printf("%d %lx\n", instruction_number, pc.val);
        /*** FETCH ***/
        // We're fetching 10 bytes in the form of 10 vals with one byte each
        val inst_bytes[10];
        memory_read_into_buffer(mem, pc, inst_bytes, true);

        /*** DECODE ***/
        // read 4 bit values
        val major_op = pick_bits(4,  4, inst_bytes[0]);
        val minor_op = pick_bits(0,  4, inst_bytes[0]); // <--- essential for further decode, but not used yet

        val reg_d = pick_bits(4, 4, inst_bytes[1]);
        val reg_s = pick_bits(0, 4, inst_bytes[1]);

        // lsb for reg z is 4 size is 4
        val reg_z = pick_bits(4, 4, inst_bytes[2]);
        val shift_v = pick_bits(0, 4, inst_bytes[2]);

        // printf("major %lx\n", major_op);
        // printf("minor %x\n", minor_op);
        // printf("regd %x\n", reg_d);
        // printf("regs %x\n", reg_s);
        // stop=true;
        // decode instruction type
        // read major operation code
        bool is_return = is(RETURN, major_op);
        bool is_reg_movq = is(REG_MOVQ, major_op);
        bool is_imm_movq = is(IMM_MOVQ, major_op);
        bool is_imm_arith = is(IMM_ARITHMETIC, major_op);
        bool is_reg_movq_mem = is(REG_MOVQ_MEM, major_op);
        bool is_imm_movq_mem = is(IMM_MOVQ_MEM, major_op);
        bool is_leaq2        = is(LEAQ2, major_op);
        bool is_leaq3        = is(LEAQ3, major_op);
        bool is_leaq6        = is(LEAQ6, major_op);
        bool is_leaq7        = is(LEAQ7, major_op);
        bool is_reg_arith    = is(REG_ARITHMETIC, major_op);
        bool is_cb           = is(CFLOW, major_op);       
        bool is_cbi          = is(IMM_CBRANCH, major_op);
        // target add = minor op if conditional branch statement
        val comparison = use_if(is_cb || is_cbi, minor_op);

        //decode instruction type
        // read minor operation code
        bool is_imm_arith_sub = is_imm_arith && is(SUB, minor_op);
        bool is_imm_arith_add = is_imm_arith && is(ADD, minor_op);
        bool is_imm_arith_and = is_imm_arith && is(AND, minor_op);
        bool is_imm_arith_or  = is_imm_arith && is(OR, minor_op);

        // only check for memT and memF if major op code is mem
        bool is_reg_movq_memT = is_reg_movq_mem && is(REG_MOVQ_MT, minor_op);
        bool is_reg_movq_memF = is_reg_movq_mem && is(REG_MOVQ_MF, minor_op);
        // only check for memT and memF if major op code is imm_movq_mem
        bool is_imm_movq_memT = is_imm_movq_mem && is(REG_IMM_MULTT, minor_op);
        bool is_imm_movq_memF = is_imm_movq_mem && is(REG_IMM_MULTF, minor_op);

        bool is_leaq3_mult = is_leaq3 && is(LEAQ3_MULT, minor_op);
        bool is_leaq3_mult_add = is_leaq3 && is(LEAQ3_MULT_ADD, minor_op);

        bool is_leaq6_imm = is_leaq6 && is(LEAQ6_IMM, minor_op);
        bool is_leaq6_imm_add = is_leaq6 && is(LEAQ6_IMM_ADD, minor_op);
        // bool is_leaq7_z       = is_leaq7 && is(LEAQ7_Z,minor_op);
        bool is_leaq7_add     = is_leaq7 && is(LEAQ7_Z_ADD, minor_op);

        // minor op code for arithemtic operations
        bool is_reg_arith_add = is_reg_arith && is(ADD, minor_op);
        bool is_reg_arith_sub = is_reg_arith && is(SUB, minor_op);
        bool is_reg_arith_and = is_reg_arith && is(AND, minor_op);
        bool is_reg_arith_or = is_reg_arith && is(OR, minor_op);
        bool is_reg_arith_xor = is_reg_arith && is(XOR, minor_op);
        bool is_reg_arith_mul = is_reg_arith && is(MUL, minor_op);
        bool is_reg_arith_sar = is_reg_arith && is(SAR, minor_op);
        bool is_reg_arith_sal = is_reg_arith && is(SAL, minor_op);
        bool is_reg_arith_shr = is_reg_arith && is(SHR, minor_op);
        bool is_reg_arith_imul = is_reg_arith && is(IMUL, minor_op);
        bool is_cb_jmp = is_cb && is(JMP, minor_op);
        bool is_cb_call = is_cb && is(CALL, minor_op);
        // bool is_cb_e = is_cb && is(E, minor_op);

        // determine instruction size
        bool size2 = is_return || is_reg_movq || is_reg_movq_mem || is_leaq2 || is_leaq3 || is_reg_arith;
        bool size6 = is_imm_movq || is_imm_arith || is_imm_movq_mem || is_leaq6 || is_cb;   
        bool size3 = is_leaq3;
        bool size7 = is_leaq7;
        bool size10 = is_cbi;


        

        // printf("%d", is_leaq7_add);
          

        // setting up operand fetch and register read and write for the datapath:
	    bool use_imm = is_imm_movq || is_imm_arith;
        val reg_read_dz = reg_d;
        // - other read port is always reg_s
        // - write is always to reg_d
        bool reg_wr_enable = is_reg_movq || 
        is_imm_movq || is_imm_arith || is_reg_movq_memF || is_imm_movq_memF || 
        is_leaq2 || is_leaq3 || is_leaq6 || is_leaq7 || is_reg_arith || is_cb_call;

        // Datapath:
        //
        // read immediates based on instruction type
        val imm_offset_2 = or(or(put_bits(0, 8, inst_bytes[2]), put_bits(8,8, inst_bytes[3])),
                              or(put_bits(16, 8, inst_bytes[4]), put_bits(24,8, inst_bytes[5])));
        val imm_i = imm_offset_2; // <--- could be more
        // val sext_imm_i = sign_extend(31, imm_i);

        val imm_offset_3 = or(or(put_bits(0, 8, inst_bytes[3]), put_bits(8,8, inst_bytes[4])),
                              or(put_bits(16, 8, inst_bytes[5]), put_bits(24,8, inst_bytes[6])));
        val imm_i_3 = imm_offset_3; // <--- could be more
        // val sext_imm_i_3 = sign_extend(31, imm_i_3);


        val sext_imm_i = or(use_if(!is_leaq7, sign_extend(31, imm_i)),
         use_if(is_leaq7, sign_extend(31, imm_i_3)));

        val p_tar = or(or(put_bits(0, 8, inst_bytes[6]), put_bits(8,8, inst_bytes[7])),
                              or(put_bits(16, 8, inst_bytes[8]), put_bits(24,8, inst_bytes[9])));        

        // val target_address = 
        // or(
        //     use_if(is_cb, imm_i),
        //     use_if(is_cbi, reverse_bytes(4, sign_extend(31, imm_i_3)))); 

        val target_address = 
        or(
            use_if(is_cb, imm_i),
            use_if(is_cbi, p_tar)
            ); 

        printf("targetaddress %x\n", target_address);
        printf("immediate %x\n", imm_i_3);
        printf("byte3%lx\n", inst_bytes[3]);
        printf("byte4%lx\n", inst_bytes[4]);
        printf("byte5%lx\n", inst_bytes[5]);
        printf("byte6%lx\n", inst_bytes[6]);
        
        
        /*** EXECUTE ***/
        // read registers
        val reg_out_a = reg_read(regs, reg_read_dz); // <-- will generate warning, as we don't use it yet
        val reg_out_b = reg_read(regs, reg_s);
        val reg_out_z = reg_read(regs, reg_z);
	    val op_b = or(use_if(use_imm, sext_imm_i),use_if(!use_imm, reg_out_b));


        // perform calculations
        // ALU?? 

        val op_arith = 
            or(
                or(
                    or( use_if(is_reg_arith_add || is_imm_arith_add, alu_execute(from_int(ADD), reg_out_a, op_b)),
                        use_if(is_reg_arith_sub || is_imm_arith_sub, alu_execute(from_int(SUB), reg_out_a, op_b))),
                        
                    or(use_if(is_reg_arith_and || is_imm_arith_and, alu_execute(from_int(AND), reg_out_a, op_b)),
                        use_if(is_reg_arith_or || is_imm_arith_or, alu_execute(from_int(OR), reg_out_a, op_b)))
                ),
                or(
                    or(use_if(is_reg_arith_xor, alu_execute(from_int(XOR), reg_out_a, op_b)),
                    use_if(is_reg_arith_mul, alu_execute(from_int(MUL), reg_out_a, op_b))),

                    or(
                        or(
                            use_if(is_reg_arith_sar, alu_execute(from_int(SAR), reg_out_a, op_b)),

                        use_if(is_reg_arith_sal, alu_execute(from_int(SAL), reg_out_a, op_b))
                        ),
                        
                        or (
                            use_if(is_reg_arith_shr, alu_execute(from_int(SHR), reg_out_a, op_b)),
                                use_if(is_reg_arith_imul, alu_execute(from_int(IMUL), reg_out_a, op_b))
                        )
                        )
                )
            );

        
        // comparator
        // bool cb_jmp=0;
        // if (is_cb){
        bool cb_jmp = (is_cb && comparator(comparison, reg_out_a, reg_out_b))
        || (is_cbi && comparator(comparison, reg_out_a, sext_imm_i));

        printf("cbjump%d", cb_jmp);
        
        printf("compariosn%lx", comparison);
        // not really any calculations yet!
        // generate address for memory access

        //bool for checking whether immediate should be used in LEAQ calculation

        // comparator(, reg_out_a, reg_out_b)
        
        
        // shift amount only if leaq3
        bool is_shift = is_leaq3 || is_leaq7; 
        val shift_amount = use_if(is_shift, shift_v); 

        // if leaq3 or leaq7 then use register z for computation, write reg is still reg b
        val reg_wr_agenerate = or(use_if(is_leaq3 || is_leaq7, reg_out_z), reg_out_a);

        bool add_to_shifted = is_leaq3_mult_add || is_leaq7_add;
        
        
        //test if below works if is_leaq7_add is not added to add_to_shifted
        

        val agen_result = address_generate(reg_wr_agenerate, reg_out_b,
         sext_imm_i, shift_amount,
        is_shift,
        is_imm_movq_mem || is_reg_movq_mem || is_leaq2 || add_to_shifted || is_leaq6_imm_add,
        is_imm_movq_memF || is_imm_movq_memT || is_leaq6 || is_leaq7);   
        // ....

        val ins_size = 
        or(
            or(
                or(
                    use_if(size2, from_int(2)),
            use_if(size6, from_int(6))
                ),
                use_if(size10, from_int(10))
            
            ) ,
            or(
            use_if(size3, from_int(3)),
            use_if(size7, from_int(7))
            )
            );

        // address of succeeding instruction in memory
        // jump if the following minor codes are true
        bool jmp = is_cb_jmp || cb_jmp || is_cb_call;
        
        val pc_incremented =
        or(
            use_if(jmp, target_address),
            use_if(!jmp, add(pc, ins_size))
        ); 
        // add(pc, ins_size); 
        // or(
        //     use_if(cb_jmp, target_address),
        //     use_if(!cb_jmp, add(pc, ins_size))
        // );
        // add(pc, ins_size);

        // printf("%x\n", pc_incremented);
        // printf("lol%d", cb_jmp);
        // printf("loll%d", is_cb_jmp);
        // determine the next position of the program counter


        val pc_next = or(
            use_if(is_return, reg_out_b),
            use_if(!is_return, pc_incremented)
        );

        // val pc_next = or(
            
        //     or(
        //         use_if(is_cb_jmp, target_address),
        //         use_if(cb_jmp , target_address)),
        //     or(
        //         use_if(is_return, reg_out_b), 
        //         use_if(!is_return, pc_incremented)
        //     )
        //         );
        printf("\npcnextis %lx", pc_next);
        printf("\ntarget %lx", target_address);
        printf("\npcincr %lx", pc_incremented);
        printf("\nregoutb %lx \n", reg_out_b);

        /*** MEMORY ***/
        
        // || || etc. below
        bool is_load = is_reg_movq_memF || is_imm_movq_memF;
        // read from memory if needed
        // Not implemented yet!
        printf("yellow %lx", reg_out_a);
        printf("hello %lx", agen_result);
        val mem_out = memory_read(mem, agen_result, is_load);

        /*** WRITE ***/
        // choose result to write back to register
        //RESULT SELECT
        val datapath_result = or(
            or(
                use_if(is_imm_arith || is_reg_arith, op_arith), 
                use_if(is_reg_movq || is_imm_movq , op_b)
            ), 
            or(
                or(use_if(is_imm_movq_memF || is_reg_movq_memF, mem_out),
                use_if(is_leaq2 || is_leaq3 || is_leaq6 || is_leaq7, agen_result)),
                use_if(is_cb_call, add(pc,ins_size))
            ));
        printf("result to write %lx \n", datapath_result);

        // val datapath_result = op_b;
        
        // write to register if needed
        reg_write(regs, reg_d, datapath_result, reg_wr_enable);
        printf("read from reg %d\n", reg_read(regs, reg_read_dz));
        // write to memory if needed
        bool is_store = is_reg_movq_memT || is_imm_movq_memT;
        // Not implemented yet!
        memory_write(mem, agen_result, reg_out_a, is_store);
        // update program counter
        ip_write(ip, pc_next, true);
        // printf("is %ld", pc_next.val);
        // stop=true;
        // terminate when returning to zero
        // printf("%d", pc_next.val);

        if (pc_next.val == 0 && is_return) stop = true;
    }
    memory_destroy(mem);
    regs_destroy(regs);

    printf("Done\n");
}

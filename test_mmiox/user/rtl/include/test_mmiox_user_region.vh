/*****************/
/* Custom Region */
/*****************/

// wire clk_system;
// wire clk_core;
// wire clk_system_external;
// wire clk_system_debug;
// wire clk_local_access;
// wire clk_process_000;
// wire clk_noc;
// wire gclk_system;
// wire gclk_core;
// wire gclk_system_external;
// wire gclk_system_debug;
// wire gclk_local_access;
// wire gclk_process_000;
// wire gclk_noc;
// wire tick_1us;
// wire tick_62d5ms;
// wire tick_gpio;
// wire spi_common_sclk;
// wire spi_common_sdq0;
// wire global_rstnn;
// wire global_rstpp;
// wire [(6)-1:0] rstnn_seqeunce;
// wire [(6)-1:0] rstpp_seqeunce;
// wire rstnn_user;
// wire rstpp_user;
// wire i_test_mmiox_clk;
// wire i_test_mmiox_rstnn;
// wire [(64)-1:0] i_test_mmiox_rmx_core_config;
// wire [(32)-1:0] i_test_mmiox_rmx_core_status;
// wire i_test_mmiox_rmx_clear_request;
// wire i_test_mmiox_rmx_clear_finish;
// wire i_test_mmiox_rmx_log_fifo_wready;
// wire i_test_mmiox_rmx_log_fifo_wrequest;
// wire [(32)-1:0] i_test_mmiox_rmx_log_fifo_wdata;
// wire i_test_mmiox_rmx_inst_fifo_rready;
// wire [(62)-1:0] i_test_mmiox_rmx_inst_fifo_rdata;
// wire i_test_mmiox_rmx_inst_fifo_rrequest;
// wire i_test_mmiox_rmx_operation_finish;
// wire i_test_mmiox_rmx_input_fifo_rready;
// wire [(64)-1:0] i_test_mmiox_rmx_input_fifo_rdata;
// wire i_test_mmiox_rmx_input_fifo_rrequest;
// wire i_test_mmiox_rmx_output_fifo_wready;
// wire i_test_mmiox_rmx_output_fifo_wrequest;
// wire [(64)-1:0] i_test_mmiox_rmx_output_fifo_wdata;

/* DO NOT MODIFY THE ABOVE */
/* MUST MODIFY THE BELOW   */

reg inst_rrequest;
wire [32-1:0] waddr;
wire [08-1:0] wnum;
wire [08-1:0] opcode;
reg allows_transfer;

/*
USER_IP
#(
	.BW_INST(62),
	.BW_CONFIG(64),
	.BW_STATUS(32),
	.BW_INPUT(64),
	.BW_OUTPUT(64)
)
i_test_mmiox
(
	.clk(i_test_mmiox_clk),
	.rstnn(i_test_mmiox_rstnn),
	.rmx_core_config(i_test_mmiox_rmx_core_config),
	.rmx_core_status(i_test_mmiox_rmx_core_status),
	.rmx_clear_request(i_test_mmiox_rmx_clear_request),
	.rmx_clear_finish(i_test_mmiox_rmx_clear_finish),
	.rmx_log_fifo_wready(i_test_mmiox_rmx_log_fifo_wready),
	.rmx_log_fifo_wrequest(i_test_mmiox_rmx_log_fifo_wrequest),
	.rmx_log_fifo_wdata(i_test_mmiox_rmx_log_fifo_wdata),
	.rmx_inst_fifo_rready(i_test_mmiox_rmx_inst_fifo_rready),
	.rmx_inst_fifo_rdata(i_test_mmiox_rmx_inst_fifo_rdata),
	.rmx_inst_fifo_rrequest(i_test_mmiox_rmx_inst_fifo_rrequest),
	.rmx_operation_finish(i_test_mmiox_rmx_operation_finish),
	.rmx_input_fifo_rready(i_test_mmiox_rmx_input_fifo_rready),
	.rmx_input_fifo_rdata(i_test_mmiox_rmx_input_fifo_rdata),
	.rmx_input_fifo_rrequest(i_test_mmiox_rmx_input_fifo_rrequest),
	.rmx_output_fifo_wready(i_test_mmiox_rmx_output_fifo_wready),
	.rmx_output_fifo_wrequest(i_test_mmiox_rmx_output_fifo_wrequest),
	.rmx_output_fifo_wdata(i_test_mmiox_rmx_output_fifo_wdata)
);
*/
assign i_test_mmiox_clk = gclk_core;
//assign `NOT_CONNECT = i_test_mmiox_rstnn;
//assign `NOT_CONNECT = i_test_mmiox_rmx_core_config;
assign i_test_mmiox_rmx_core_status = 32'h 010a;
//assign `NOT_CONNECT = i_test_mmiox_rmx_log_fifo_wready;
assign i_test_mmiox_rmx_clear_finish = i_test_mmiox_rmx_clear_request;
assign i_test_mmiox_rmx_log_fifo_wrequest = inst_rrequest;
assign i_test_mmiox_rmx_log_fifo_wdata = 32'h dddd;
//assign `NOT_CONNECT = i_test_mmiox_rmx_inst_fifo_rready;
//assign `NOT_CONNECT = i_test_mmiox_rmx_inst_fifo_rdata;
assign i_test_mmiox_rmx_inst_fifo_rrequest = inst_rrequest;
assign i_test_mmiox_rmx_operation_finish = inst_rrequest;
//assign `NOT_CONNECT = i_test_mmiox_rmx_input_fifo_rready;
//assign `NOT_CONNECT = i_test_mmiox_rmx_input_fifo_rdata;
assign i_test_mmiox_rmx_input_fifo_rrequest = allows_transfer & i_test_mmiox_rmx_input_fifo_rready;
//assign `NOT_CONNECT = i_test_mmiox_rmx_output_fifo_wready;
assign i_test_mmiox_rmx_output_fifo_wrequest = allows_transfer & i_test_mmiox_rmx_input_fifo_rready;
assign i_test_mmiox_rmx_output_fifo_wdata = i_test_mmiox_rmx_input_fifo_rdata;

assign {opcode, wnum, waddr} = i_test_mmiox_rmx_inst_fifo_rdata;

// testbench
initial
begin
  inst_rrequest = 0;
  allows_transfer = 0;

  //
  wait(i_test_mmiox_rmx_inst_fifo_rready==1);
  wait(i_test_mmiox_clk==1);
  #1
  $display("0x%x %d %d", waddr, wnum, opcode);
  inst_rrequest = 1;
  wait(i_test_mmiox_clk==0);
  wait(i_test_mmiox_clk==1);
  #1
  inst_rrequest = 0;
  wait(i_test_mmiox_clk==0);
  wait(i_test_mmiox_clk==1);

  //
  wait(i_test_mmiox_rmx_inst_fifo_rready==1);
  wait(i_test_mmiox_clk==1);
  #1
  $display("0x%x %d %d", waddr, wnum, opcode);
  inst_rrequest = 1;
  wait(i_test_mmiox_clk==0);
  wait(i_test_mmiox_clk==1);
  #1
  inst_rrequest = 0;
  wait(i_test_mmiox_clk==0);
  wait(i_test_mmiox_clk==1);

  //
  wait(i_test_mmiox_rmx_inst_fifo_rready==1);
  wait(i_test_mmiox_clk==1);
  #1
  $display("0x%x %d %d", waddr, wnum, opcode);
  inst_rrequest = 1;
  wait(i_test_mmiox_clk==0);
  wait(i_test_mmiox_clk==1);
  #1
  inst_rrequest = 0;
  wait(i_test_mmiox_clk==0);
  wait(i_test_mmiox_clk==1);

  //
  #1
  allows_transfer = 1;
end

/*
initial
begin
  wait(inst_rrequest==1);
  wait(i_test_mmiox_clk==0);
  wait(i_test_mmiox_clk==1);
  wait(i_platform.i_rtl.i_test_mmiox_no_name_apb2mmiox_interrupt_list!=0);
  $display("Interrupt Occurs");
  wait(i_platform.i_rtl.i_test_mmiox_no_name_apb2mmiox_interrupt_list==0);
  $display("Interrupt Clears");
end
*/

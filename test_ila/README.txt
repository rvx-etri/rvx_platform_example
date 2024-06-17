[01] Xilinx ILA이 무엇인지 알아야 사용할 수 있음
[02] ${PLATFORM_DIR}/user/ila/common 또는 ${PLATFORM_DIR}/user/ila/${FPGA_NAME} 에 ila 생성을 위한 xml 작성
[03] clock과 signal의 value는 모두 ${PLATFORM_NAME}.v를 기준으로 작성
[04] FPGA에서만 사용하는 기능이라 hierarchical 이름 가능
[05] ${IMP_DIR}에서 make ila
[06] ${IMP_DIR}/set_design.tcl에서 IMP_MODE를 release에서 debug로 수정
[07] xml를 수정할 경우 make ila 해주어야함

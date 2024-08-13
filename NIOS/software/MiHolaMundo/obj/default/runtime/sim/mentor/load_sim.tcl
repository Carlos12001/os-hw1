# ------------------------------------------------------------------------------
# Top Level Simulation Script to source msim_setup.tcl
# ------------------------------------------------------------------------------
set QSYS_SIMDIR obj/default/runtime/sim
source msim_setup.tcl
# Copy generated memory initialization hex and dat file(s) to current directory
file copy -force C:/Users/carlos/Repos/os_hw1/NIOS/software/MiHolaMundo/mem_init/hdl_sim/cpu_memoria.dat ./ 
file copy -force C:/Users/carlos/Repos/os_hw1/NIOS/software/MiHolaMundo/mem_init/cpu_memoria.hex ./ 

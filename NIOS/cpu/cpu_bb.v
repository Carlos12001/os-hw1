
module cpu (
	clk_clk,
	leds_minutes_ls_external_connection_export,
	leds_minutes_ms_external_connection_export,
	leds_seconds_ls_external_connection_export,
	leds_seconds_ms_external_connection_export,
	pio_key_0_external_connection_export,
	pio_key_1_external_connection_export,
	pio_key_2_external_connection_export,
	pio_key_3_external_connection_export,
	pio_switches_external_connection_export,
	reset_reset_n);	

	input		clk_clk;
	output	[7:0]	leds_minutes_ls_external_connection_export;
	output	[7:0]	leds_minutes_ms_external_connection_export;
	output	[6:0]	leds_seconds_ls_external_connection_export;
	output	[6:0]	leds_seconds_ms_external_connection_export;
	input		pio_key_0_external_connection_export;
	input		pio_key_1_external_connection_export;
	input		pio_key_2_external_connection_export;
	input		pio_key_3_external_connection_export;
	input	[1:0]	pio_switches_external_connection_export;
	input		reset_reset_n;
endmodule

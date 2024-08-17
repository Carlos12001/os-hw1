
module cpu (
	clk_clk,
	leds_hours_ls_external_connection_export,
	leds_hours_ms_external_connection_export,
	leds_minutes_ls_external_connection_export,
	leds_minutes_ms_external_connection_export,
	pio_buzzer_external_connection_export,
	pio_key_0_external_connection_export,
	pio_key_1_external_connection_export,
	pio_switches_external_connection_export,
	reset_reset_n);	

	input		clk_clk;
	output	[6:0]	leds_hours_ls_external_connection_export;
	output	[6:0]	leds_hours_ms_external_connection_export;
	output	[6:0]	leds_minutes_ls_external_connection_export;
	output	[6:0]	leds_minutes_ms_external_connection_export;
	output		pio_buzzer_external_connection_export;
	input		pio_key_0_external_connection_export;
	input		pio_key_1_external_connection_export;
	input	[1:0]	pio_switches_external_connection_export;
	input		reset_reset_n;
endmodule

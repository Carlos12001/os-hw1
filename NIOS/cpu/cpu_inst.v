module cpu_inst(input 		  clk, reset,
									  key0, key1, key2, key3,
					 input  [1:0] switcher,
					 output [6:0] milisecondsDisplay1,
									  milisecondsDisplay2,
									  secondsDisplay1,
									  secondsDisplay2
);
	cpu u0 (
		.clk_clk                                    (clk),                                    //                                 clk.clk
		.reset_reset_n                              (reset),                              //                               reset.reset_n
		.leds_seconds_ls_external_connection_export (milisecondsDisplay1), // leds_seconds_ls_external_connection.export
		.leds_seconds_ms_external_connection_export (milisecondsDisplay2), // leds_seconds_ms_external_connection.export
		.leds_minutes_ls_external_connection_export (secondsDisplay1), // leds_minutes_ls_external_connection.export
		.leds_minutes_ms_external_connection_export (secondsDisplay2), // leds_minutes_ms_external_connection.export
		.pio_switches_external_connection_export    (switcher),     //    pio_switches_external_connection.export
		.pio_key_0_external_connection_export       (key0),       //       pio_key_0_external_connection.export
		.pio_key_1_external_connection_export       (key1),       //       pio_key_1_external_connection.export
		.pio_key_2_external_connection_export       (key2),       //       pio_key_2_external_connection.export
		.pio_key_3_external_connection_export       (key3)        //       pio_key_3_external_connection.export
	
	);
endmodule
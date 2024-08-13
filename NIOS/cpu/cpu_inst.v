module cpu_inst(input clk, reset, output [1:0] leds);
cpu u0 (
        .clk_clk       (clk),       //   clk.clk
        .reset_reset_n (reset), // reset.reset_n
        .leds_export   (leds)    //  leds.export
    );
endmodule

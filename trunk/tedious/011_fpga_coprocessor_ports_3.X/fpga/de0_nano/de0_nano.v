module de0_nano
(
    input         clock,
    input  [7: 0] port_e,
    input  [7: 5] port_d_in,
    output [3: 0] port_d_out,
    output [1:12] display,
    output [7: 0] leds
);


    top top (clock, port_e, port_d_in, port_d_out, display, leds);

endmodule

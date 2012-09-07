module top
(
    input             clock,
    input      [7: 0] in,
    output reg [1:12] out,
	 output reg [7: 0] led
);

wire [3:0] digit = 4'b1111;

wire a, b, c, d, e, f, g, dp;

assign { a, b, c, d, e, f, g, dp }
    = in;

always @(posedge clock)
begin
    { out [6], out [8], out [9], out [12] }
        = ~ digit;

    { out [11], out [ 7], out [4], out [2],
      out [ 1], out [10], out [5], out [3] }
        = { a, b, c, d, e, f, g, dp };

	 led = { a, b, c, d, e, f, g, dp };
end
	 
endmodule

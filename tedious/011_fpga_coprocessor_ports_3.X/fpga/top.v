module top
(
    input         clock,
    input  [7: 0] port_e,
    input  [4: 1] port_f,
    output reg [3: 0] port_d,
    output [1:12] display,
    output [7: 0] leds
);

wire reset_n = port_f [1];

/*
wire run     = port_f [2];

reg [7:0] data;
reg tag;
*/

/*
// Idea 1: Use run as a clock

always @(posedge run or negedge reset_n)
begin
    if (! reset_n)
    begin
        data <= 0;
        tag  <= 0;
    end
    else
    begin
        data <= port_e;
        tag  <= ! tag;
    end
end
*/

/*
// Idea 2: Variation - use run as a clock, add two registers

reg [7:0] data_1, data_2;
reg tag_1, tag_2;

always @(posedge run or negedge reset_n)
begin
    if (! reset_n)
    begin
        data_1 <= 0;
        tag_1  <= 0;
    end
    else
    begin
        data_1 <= port_e;
        tag_1  <= ! tag;
    end
end

always @(posedge clock or negedge reset_n)
begin
    if (! reset_n)
    begin
        data_2 <= 0;
        tag_2  <= 0;

        data   <= 0;
        tag    <= 0;
    end
    else
    begin
        data_2 <= data_1;
        tag_2  <= tag_1;

        data   <= data_2;
        tag    <= tag_2;
    end
end
*/

/*
// Idea 3: Use run as enable; use additional input as a tag

always @(posedge clock or negedge reset_n)
begin
    if (! reset_n)
    begin
        data <= 0;
        tag  <= 0;
    end
    else
    begin
        data <= port_e;
        tag  <= port_f [3];
    end
end

// Datapath

reg [7:0] add_a, add_b;
reg [7:0] mul_a, mul_b;

wire [7:0] add_result = add_a + add_b;
wire [7:0] mul_result = mul_a * mul_b;

// Main state machine - sequential

reg [7:0] r_add_result;
reg [7:0] r_mul_result;

reg [1:0] r_state;
reg       r_first_tag;
reg       r_prev_tag;

reg [1:0] state;
reg       first_tag;
reg       prev_tag;

// assign port_d = r_mul_result;
// assign port_d = r_state;
// assign port_d = port_f; // { r_first_tag, r_prev_tag };
*/

/*
always @(posedge clock or negedge reset_n)
    if (! reset_n)
        port_d <= 0;
    else
        port_d <= port_f [3];
*/

always @(posedge clock)
    port_d <= port_f [3];

/*		  
		  
always @(*)
begin
    state      = r_state;
    first_tag  = r_first_tag;
    prev_tag   = r_prev_tag;

    add_a      = 0;
    add_b      = 0;
    mul_a      = 0;
    mul_b      = 0;

    case (r_state)
    0:
        if (first_tag || tag != prev_tag)
        begin
            first_tag  = 0;
            prev_tag   = tag;

            mul_a      = data;
            mul_b      = data;

            state      = 1;
        end

    1:
        begin
            add_a      = r_mul_result;
            add_b      = 3;

            state      = 2;
        end

    2:
        begin
            mul_a      = r_add_result;
            mul_b      = r_add_result;

            state      = 0;
        end
    endcase
end

always @(posedge clock or negedge reset_n)
begin
    if (! reset_n)
    begin
        r_add_result  <= 0;
        r_mul_result  <= 0;

        r_state       <= 0;
        r_first_tag   <= 1;
        r_prev_tag    <= 0;
    end
    else
    begin
        r_add_result  <= add_result;
        r_mul_result  <= mul_result;

        r_state       <= state;
        r_first_tag   <= first_tag;
        r_prev_tag    <= prev_tag;
    end
end
*/

endmodule

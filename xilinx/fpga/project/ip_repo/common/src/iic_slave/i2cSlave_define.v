// ----------------------- i2cSlave_define.v --------------------

// stream states
`define STREAM_IDLE 2'b00
`define STREAM_READ 2'b01
`define STREAM_WRITE_ADDR 2'b10
`define STREAM_WRITE_DATA 2'b11

// start stop detection states
`define NULL_DET 2'b00
`define START_DET 2'b01
`define STOP_DET 2'b10

// i2c ack and nak
`define I2C_NAK 1'b1
`define I2C_ACK 1'b0

// ----------------------------------------------------------------
// ------------- modify constants below this line -----------------
// ----------------------------------------------------------------

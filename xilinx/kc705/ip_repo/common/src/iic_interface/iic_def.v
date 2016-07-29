// ----------------------- i2cSlave_define.v --------------------
// start stop detection states
`define NULL_DET 2'b00
`define START_DET 2'b01
`define STOP_DET 2'b10

// i2c ack and nak
`define I2C_NAK 1'b1
`define I2C_ACK 1'b0

//i2c master error status
`define I2C_NO_ERR 0
`define I2C_ERR_NO_ADDR_ACK 1
`define I2C_ERR_NO_DATA_ACK 2

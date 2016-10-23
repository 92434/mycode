#ifndef _INTERFACE_IO_H
#define _INTERFACE_IO_H


#if defined(DVBS2)

//164 : s0//FMC_LPC_LA22_P
//158 : s1//FMC_LPC_LA19_P
//160 : s2//FMC_LPC_LA19_N
#define SPI_MISO 166//FMC_LPC_LA20_N
#define SPI_MOSI 172//FMC_LPC_LA23_N
#define SPI_SCK 173//FMC_LPC_LA23_P
//#define SPI_CS 160//FMC_LPC_LA19_N
#define SPI_CS 169//FMC_LPC_LA32_P
//#define SPI_CS 165//FMC_LPC_LA22_P
#define IIC_SDA0 158 //FMC_LPC_LA30_P
#define IIC_SCL0 157 //FMC_LPC_LA30_N

#elif defined(KC705_CSA)

#define SPI_MISO (-1)//FMC_LPC_LA20_N
#define SPI_MOSI (-1)//FMC_LPC_LA23_N
#define SPI_SCK (-1)//FMC_LPC_LA23_P
#define SPI_CS (-1)
#define IIC_SDA0 (-1)
#define IIC_SCL0 (-1)

#elif defined(KC705_CSA_DEBUG)

#define SPI_MISO (-1)//FMC_LPC_LA20_N
#define SPI_MOSI (-1)//FMC_LPC_LA23_N
#define SPI_SCK (-1)//FMC_LPC_LA23_P
#define SPI_CS (-1)
#define IIC_SDA0 (-1)
#define IIC_SCL0 (-1)

#elif defined(KC705_FMC_GPIO)

#define SPI_MISO (-1)
#define SPI_MOSI (-1)
#define SPI_SCK (-1)
#define SPI_CS (-1)
#define IIC_SDA0 (-1)
#define IIC_SCL0 (-1)

#elif defined(KC705_I2S_RECEIVER_TEST)

#define SPI_MISO (246)
#define SPI_MOSI (247)
#define SPI_SCK (248)
#define SPI_CS (249)
#define IIC_SDA0 (-1)
#define IIC_SCL0 (-1)

#elif defined(KC705_IIC_SLAVE)

#define SPI_MISO (-1)
#define SPI_MOSI (-1)
#define SPI_SCK (-1)
#define SPI_CS (-1)
#define IIC_SDA0 (179)
#define IIC_SCL0 (178)

#elif defined(KC705_DVBS2)

#define SPI_MISO (249)
#define SPI_MOSI (248)
#define SPI_SCK (247)
#define SPI_CS (250)
#define IIC_SDA0 (231)
#define IIC_SCL0 (230)

#elif defined(KC705_TSP)
#define DEFINED_TSP 2

#define SPI_MISO (-1)
#define SPI_MOSI (-1)
#define SPI_SCK (-1)
#define SPI_CS (-1)
#if (DEFINED_TSP == 0)
#define IIC_SDA0 (254)
#define IIC_SCL0 (255)
#elif (DEFINED_TSP == 1)
#define IIC_SDA0 (254)
#define IIC_SCL0 (255)
#elif (DEFINED_TSP == 2)
#warning("DEFINED_TSP == 2")
#define IIC_SCL0 (236)
#define IIC_SDA0 (237)
#define IIC_SCL1 (241)
#define IIC_SDA1 (242)
#define IIC_SCL2 (246)
#define IIC_SDA2 (247)
#define IIC_SCL3 (251)
#define IIC_SDA3 (252)
#endif//if (DEFINED_TSP == 0)
#endif

#endif //ifndef _INTERFACE_IO_H

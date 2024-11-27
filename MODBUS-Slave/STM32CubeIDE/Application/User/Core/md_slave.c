#include "main.h"

extern uint8_t data[max_frame_size+1]; // for going through the data

//different functions

extern unsigned char response_array[256];
extern int flag;
extern int l;
extern int ttt;
extern int len_of_coils;
extern int len_of_regs;
extern uint16_t exception_code;
extern int sending_byte_count;

//----------------------------------------------------------------------------------------------------------------------------------------
/* Table of CRC values for high-order byte */
static const uint8_t table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const uint8_t table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

//----------------------------------------------------------------------------------------------------------------------------------------
uint16_t crc16(uint8_t*buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_lo ^ *buffer++; /* calculate the CRC  */
        crc_lo = crc_hi ^ table_crc_hi[i];
        crc_hi = table_crc_lo[i];
    }

    return (crc_hi <<8| crc_lo);
}

//----------------------------------------------------------------------------------------------------------------------------------------
void read_coils(uint16_t starting_Address,uint16_t num_of_coils,uint8_t coils[],uint8_t x[]){
	//the order of wrighting is LSB -> MSB
	uint16_t slave_crc_send;
	if (num_of_coils > 0x007D || num_of_coils < 0){
		flag = 1;// for setting the error needed
		exception_code = 0x03; // exception code according to the datasheet
	}else if(starting_Address + num_of_coils > len_of_coils){
		flag = 1;
    exception_code = 0x02;
	}else if(num_of_coils % 8 == 0 && flag == 0){
		 x[0] = Slave_Address;
		 x[1] = 0x01;
		 x[2] = num_of_coils/8;
		 for(int i = 3; i<num_of_coils/8 +3;i++){
			  uint8_t hi =0,lo =0;
			  int temp = 1;
			  for(int j=0; j<4 ;j++){
			     	lo += coils[starting_Address + j + (i - 3)*8] * temp;//lower nibble
					  hi += coils[starting_Address + 4 + j + (i-3)*8] * temp;//higher nibble
					  temp =temp *2 ;
				}
				x[i] = (hi << 4|lo);
	 }
		 slave_crc_send = crc16(x,num_of_coils/8+3);
	   x[num_of_coils/8 + 4] = slave_crc_send >> 8 & 0xFF;
	   x[num_of_coils/8 + 3] = slave_crc_send & 0xFF;
	   sending_byte_count = num_of_coils/8 + 5;
	}else if(num_of_coils % 8 != 0 && flag == 0){
		 uint16_t temp2 = num_of_coils;
		 x[0] = Slave_Address;
		 x[1] = 0x01;
		 x[2] = num_of_coils/8 +1;
		 for(int i = 3; i<num_of_coils/8 +4;i++){
		    uint8_t hi =0,lo =0;
			  int temp = 1;
			  if(temp2 >= 8){
			     for(int k=0; k<4; k++){
						lo += coils[starting_Address + k + (i-3)*8] * temp;
					  hi += coils[starting_Address + 4 + k + (i-3)*8] * temp;
					  temp =temp *2 ;
					 }

				}else if(temp2<8 && temp2>4){
				   for(int r=0 ; r<4;r++){
					   lo += coils[starting_Address + r + (i-3)*8] * temp;
						 if(temp2 - 4 - r > 0){
						    hi += coils[starting_Address + 4 + r + (i-3)*8] * temp;
						 }
						 temp =temp *2 ;
					 }
				}else if(temp2<=4){
				   for(int q=0; q<temp2; q++){
					    lo += coils[starting_Address + q + (i-3)*8] * temp;
						  temp =temp *2 ;
					 }
				}
				x[i] = (hi << 4|lo);
				temp2 -= 8;
		 }
		 slave_crc_send = crc16(x,num_of_coils/8 + 4);
	   x[num_of_coils/8 + 5] = slave_crc_send >> 8 & 0xFF;
	   x[num_of_coils/8 + 4] = slave_crc_send & 0xFF;
		 sending_byte_count = num_of_coils/8 + 6;

	 }else{
		  flag = 1;
		 exception_code = 0x04;

	}
	 if(flag == 1){
	   x[0] = Slave_Address;
		 x[1] = 0x81;
		 x[2] = exception_code;
		 slave_crc_send = crc16(x,3);
	   x[4] = slave_crc_send >> 8 & 0xFF;
	   x[3] = slave_crc_send & 0xFF;
		 sending_byte_count = 5;
		 flag = 0;
	 }
};

//----------------------------------------------------------------------------------------------------------------------------------------
void read_descrete_data(uint16_t starting_Address,uint16_t num_of_inputs,uint8_t data[],uint8_t x[]){
	uint16_t slave_crc_send;
	if (num_of_inputs > 0x007D || num_of_inputs < 0){
		flag = 1;// for setting the error needed
		exception_code = 0x03; // exception code according to the datasheet
	}else if(starting_Address + num_of_inputs > len_of_coils){
		flag = 1;
    exception_code = 0x02;
	}else if(num_of_inputs % 8 == 0 && flag == 0){
		 x[0] = Slave_Address;
		 x[1] = 0x02;
		 x[2] = num_of_inputs/8;
		 for(int i = 3; i<num_of_inputs/8 +3;i++){
			  uint8_t hi =0,lo =0;
			  int temp = 1;
			  for(int j=0; j<4 ;j++){
			     	lo += data[starting_Address + j + (i - 3)*8] * temp;
					  hi += data[starting_Address + 4 + j + (i-3)*8] * temp;
					  temp =temp *2 ;
				}
				x[i] = (hi << 4|lo);
	 }
		 slave_crc_send = crc16(x,num_of_inputs/8 + 3);
	   x[num_of_inputs/8 + 3] = slave_crc_send >> 8 & 0xFF;
	   x[num_of_inputs/8 + 4] = slave_crc_send & 0xFF;
	   sending_byte_count = num_of_inputs/8 + 5;
	}else if(num_of_inputs % 8 != 0 && flag == 0){
		 uint16_t temp2 = num_of_inputs;
		 x[0] = 0x01;
		 x[1] = num_of_inputs/8 +1;
		 for(int i = 3; i<num_of_inputs/8 +4;i++){
		    uint8_t hi =0,lo =0;
			  int temp = 1;
			  if(temp2 >= 8){
			     for(int k=0; k<4; k++){
						lo += data[starting_Address + k + (i-3)*8] * temp;
					  hi += data[starting_Address + 4 + k + (i-2)*8] * temp;
					  temp =temp *2 ;
					 }
				}else if(temp2<8 && temp2>4){
				   for(int r=0 ; r<4;r++){
					   lo += data[starting_Address + r + (i-3)*8] * temp;
						 if(temp2 - 4 - r > 0){
						    hi += data[starting_Address + 4 + r + (i-3)*8] * temp;
						 }
						 temp =temp *2 ;
					 }
				}else if(temp2<=4){
				   for(int q=0; q<temp2; q++){
					    lo += data[starting_Address + q + (i-3)*8] * temp;
						  temp =temp *2 ;
					 }
				}
				x[i] = (hi << 4|lo);
				temp2 -= 8;
		 }
		 slave_crc_send = crc16(x,num_of_inputs/8+4);
	   x[num_of_inputs/8 + 4] = slave_crc_send >> 8 & 0xFF;
	   x[num_of_inputs/8 + 5] = slave_crc_send & 0xFF;
	   sending_byte_count = num_of_inputs/8 + 6;
	 }else{
		  flag = 1;
		  exception_code = 0x04;

	}
	 	 if(flag == 1){
		 x[0] = Slave_Address;
	   x[1] = 0x82;
		 x[2] = exception_code;
		 slave_crc_send = crc16(x,3);
	   x[3] = slave_crc_send >> 8 & 0xFF;
	   x[4] = slave_crc_send & 0xFF;
		 sending_byte_count = 5;
		 flag = 0;
		}
}

//----------------------------------------------------------------------------------------------------------------------------------------
void read_holding_registers(uint16_t starting_Address,uint16_t num_of_regs,unsigned int regs[],uint8_t x[]){
  uint16_t slave_crc_send;
	if(num_of_regs > 0x007D || num_of_regs < 0){
		flag = 1;
		exception_code = 0x03;
	}else if(num_of_regs + starting_Address > len_of_regs){
	  flag = 1;
		exception_code = 0x02;
	}else if(num_of_regs + starting_Address < len_of_regs || num_of_regs > 0){
	  x[0] = Slave_Address;
	  x[1] = 0x03;
	  x[2] = num_of_regs * 2;
	  int round=0;
	  for(int i=3; i<num_of_regs*2 + 3; i++){
		  if(i%2 == 1){
			  x[i] = regs[round + starting_Address]>>8 & 0xFF;
		  }else if(i%2 == 0){
			  x[i] = regs[round + starting_Address] & 0xFF;
			  round++;
		  }
		}
	    slave_crc_send = crc16(x,num_of_regs * 2 + 3);
      x[num_of_regs*2 + 4] = 	slave_crc_send >> 8 & 0xFF;	// this was changede 3 with 4 to see modbus master change
      x[num_of_regs*2 + 3] = 	slave_crc_send & 0xFF;
      sending_byte_count = num_of_regs * 2 + 5;
	}else{
	  flag = 1;
		exception_code =0x04;
	}
	if(flag == 1){
		x[0] = Slave_Address;
		x[1] = 0x83;
		x[2] = exception_code;
		slave_crc_send = crc16(x,3);
	  x[4] = slave_crc_send >> 8 & 0xFF;
	  x[3] = slave_crc_send & 0xFF;
		sending_byte_count = 5;
		flag = 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
void read_input_registers(uint16_t starting_Address,uint16_t num_of_regs,unsigned int input_regs[],uint8_t x[]){
  uint16_t slave_crc_send;
	if(num_of_regs > 0x007D || num_of_regs < 0){
		flag = 1;
		exception_code = 0x04;
	}else if(num_of_regs + starting_Address > len_of_regs){
	  flag = 1;
		exception_code = 0x02;
	}else if(num_of_regs + starting_Address < len_of_regs || num_of_regs > 0){
	x[0] = Slave_Address;
	x[1] = 0x03;
	x[1] = num_of_regs * 2;
	int round=0;
	for(int i=3; i<num_of_regs*2 + 3; i++){
		if(i%2 == 1){
			x[i] = input_regs[round + starting_Address]>>8 & 0xFF;
		}else if(i%2 == 0){
			x[i] = input_regs[round + starting_Address] & 0xFF;
			round++;
		}
		}
	slave_crc_send = crc16(x,num_of_regs*2 + 3);
  x[num_of_regs*2 + 4] = 	slave_crc_send >> 8 & 0xFF;
  x[num_of_regs*2 + 3] = 	slave_crc_send & 0xFF;
  sending_byte_count = num_of_regs * 2 + 5;
	}else{
	  flag = 1;
		exception_code =0x04;
	}
	if(flag == 1){
		x[0] = Slave_Address;
		x[1] = 0x84;
		x[2] = exception_code;
		slave_crc_send = crc16(x,3);
	  x[4] = slave_crc_send >> 8 & 0xFF;
	  x[3] = slave_crc_send & 0xFF;
		sending_byte_count = 5;
		flag = 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
void write_single_coil(uint16_t Address,uint16_t cmd, uint8_t coils[],uint8_t x[]){
    uint16_t slave_crc_send;
    if(cmd != 0x0000 && cmd != 0xFF00){
			flag= 1;
		  exception_code = 0x03;
		}else if(Address > len_of_coils || Address<0){
		  flag = 1;
			exception_code = 0x02;
		}else if(cmd == 0x0000 || cmd == 0xFF00){
    	x[0] = Slave_Address;
			x[1] = 0x05;
    	x[2] = (Address >> 8) & 0xFF;
    	x[3] =  Address  & 0xFF;
    	if(cmd == 0xFF00){
    		coils[Address] = 1;
				ttt = 15;
        x[4] = 0xFF;
        x[5] = 0x00;
    	}else if(cmd == 0x0000){
    		coils[Address] = 0;
        x[4] = 0x00;
        x[5] = 0x00;
				ttt = 14;
    	}
			slave_crc_send = crc16(x,6);
			x[7] = slave_crc_send >> 8 & 0xFF;
			x[6] = slave_crc_send & 0xFF;
			sending_byte_count = 8;
		}else{
			  flag = 1;
    		exception_code = 0x04;
    	}
		if(flag == 1){
		x[0] = Slave_Address;
		x[1] = 0x85;
		x[2] = exception_code;
		slave_crc_send = crc16(x,3);
	  x[4] = slave_crc_send >> 8 & 0xFF;
	  x[3] = slave_crc_send & 0xFF;
		sending_byte_count = 5;
		flag = 0;
		}
}

//----------------------------------------------------------------------------------------------------------------------------------------
void write_single_register(uint16_t Address,uint16_t cmd, unsigned int regs[],uint8_t x[]){
    uint16_t slave_crc_send;
	  if(cmd > 0xFFFF || cmd < 0){
			flag = 1;
			exception_code = 0x03;
		}else if(Address < 0 || Address > len_of_regs){
			flag = 1;
			exception_code =0x02;
		}else if(flag == 0){
			regs[Address] = cmd;
    	x[0] = Slave_Address;
			x[1] = 0x06;
    	x[2] = (Address >> 8) & 0xFF;
    	x[3] =  Address  & 0xFF;
    	x[4] = (cmd >> 8) & 0xFF;
    	x[5] =  cmd & 0xFF;
			slave_crc_send = crc16(x,6);
			x[7] = slave_crc_send >> 8 & 0xFF;
			x[6] = slave_crc_send & 0xFF;
			sending_byte_count = 8;
		}else{
			flag = 1;
    	exception_code  = 0x04;
    	}
		if(flag == 1){
		x[0] = Slave_Address;
		x[1] = 0x86;
		x[2] = exception_code;
		slave_crc_send = crc16(x,3);
	  x[4] = slave_crc_send >> 8 & 0xFF;
	  x[3] = slave_crc_send & 0xFF;
	  sending_byte_count = 5;
		flag = 0;
		}
}

//----------------------------------------------------------------------------------------------------------------------------------------
void read_exception_status(uint8_t exp_coils[],uint8_t x[]){
	x[0] = Slave_Address;
	x[1] = 0x07;
	uint8_t hi =0,lo =0;
	int temp = 1;
	for(int j=0; j<4 ;j++){
		lo += exp_coils[j] * temp;
		hi += exp_coils[4 + j] * temp;
		temp =temp *2 ;
		}
	x[1] = (hi << 4|lo);
}

//----------------------------------------------------------------------------------------------------------------------------------------
void write_multiple_coils(uint16_t starting_Address, uint16_t num_of_coils,uint8_t bytes_count, uint16_t output_value[],uint8_t coils[],uint8_t x[]){
	  //the order of wrighting is LSB -> MSB
		uint16_t slave_crc_send;
    if(num_of_coils > 0x07B0 || num_of_coils < 0 || (bytes_count != num_of_coils/8 || bytes_count != num_of_coils/8 + 1)){
		  flag = 1;
			exception_code = 0x03;
		}else if(starting_Address > len_of_coils || starting_Address + num_of_coils > len_of_coils){
		  flag = 1;
			exception_code = 0x02;
		}else if(flag != 1){
    	x[0] = Slave_Address;
			x[1] = 0x0F;
			x[2] = (starting_Address >> 8) & 0xFF;
			x[3] =  starting_Address & 0xFF;
			x[4] = (num_of_coils >> 8) & 0xFF;
			x[5] =  num_of_coils & 0xFF;
		  slave_crc_send = crc16(x,6);
			x[7] = slave_crc_send >> 8 & 0xFF;
			x[6] = slave_crc_send & 0xFF;
			sending_byte_count = 8;
    	if(num_of_coils % 8 == 0){
    		uint8_t rep=0;
    	  for(int i=0; i<num_of_coils/8 ; i++){
    		  uint8_t temp_hex;
    			for(int j=0; j<2; j++){
    				if(j == 0){
    					temp_hex = output_value[i]&0x0F;
    				}else if(j==1){
    					temp_hex = (output_value[i] >> 4) & 0x0F;
    				}
    				switch (temp_hex) {
              case 0x00:
                coils[starting_Address + rep] = 0;
    				    coils[starting_Address + rep + 1] = 0;
    				    coils[starting_Address + rep + 2] = 0;
    				    coils[starting_Address + rep + 3] = 0;
                rep += 4;
    				    break;
              case 0x01:
                coils[starting_Address + rep] = 1;
    				    coils[starting_Address + rep + 1] = 0;
    				    coils[starting_Address + rep + 2] = 0;
    				    coils[starting_Address + rep + 3] = 0;
                rep += 4;
    				    break;
              case 0x02:
                coils[starting_Address + rep] = 0;
    				    coils[starting_Address + rep + 1] = 1;
    				    coils[starting_Address + rep + 2] = 0;
    				    coils[starting_Address + rep + 3] = 0;
                rep += 4;
    				    break;
              case 0x03:
                coils[starting_Address + rep] = 1;
    				    coils[starting_Address + rep + 1] = 1;
    				    coils[starting_Address + rep + 2] = 0;
    				    coils[starting_Address + rep + 3] = 0;
                rep += 4;
    				    break;
              case 0x04:
                coils[starting_Address + rep] = 0;
    				    coils[starting_Address + rep + 1] = 0;
    				    coils[starting_Address + rep + 2] = 1;
    				    coils[starting_Address + rep + 3] = 0;
                rep += 4;
    				    break;
              case 0x05:
                coils[starting_Address + rep] = 1;
    				    coils[starting_Address + rep + 1] = 0;
    				    coils[starting_Address + rep + 2] = 1;
    				    coils[starting_Address + rep + 3] = 0;
                rep += 4;
    				    break;
              case 0x06:
                coils[starting_Address + rep] = 0;
    				    coils[starting_Address + rep + 1] = 1;
    				    coils[starting_Address + rep + 2] = 1;
    				    coils[starting_Address + rep + 3] = 0;
                rep += 4;
    				    break;
              case 0x07:
                coils[starting_Address + rep] = 1;
    				    coils[starting_Address + rep + 1] = 1;
    				    coils[starting_Address + rep + 2] = 1;
    				    coils[starting_Address + rep + 3] = 0;
                rep += 4;
    				    break;
              case 0x08:
                coils[starting_Address + rep] = 0;
    				    coils[starting_Address + rep + 1] = 0;
    				    coils[starting_Address + rep + 2] = 0;
    				    coils[starting_Address + rep + 3] = 1;
                rep += 4;
    				    break;
              case 0x09:
                coils[starting_Address + rep] = 1;
    				    coils[starting_Address + rep + 1] = 0;
    				    coils[starting_Address + rep + 2] = 0;
    				    coils[starting_Address + rep + 3] = 1;
                rep += 4;
    				    break;
              case 0x0A:
                coils[starting_Address + rep] = 0;
    				    coils[starting_Address + rep + 1] = 1;
    				    coils[starting_Address + rep + 2] = 0;
    				    coils[starting_Address + rep + 3] = 1;
                rep += 4;
    				    break;
              case 0x0B:
                coils[starting_Address + rep] = 1;
    				    coils[starting_Address + rep + 1] = 1;
    				    coils[starting_Address + rep + 2] = 0;
    				    coils[starting_Address + rep + 3] = 1;
    				    rep += 4;
                break;
              case 0x0C:
                coils[starting_Address + rep] = 0;
    				    coils[starting_Address + rep + 1] = 0;
    				    coils[starting_Address + rep + 2] = 1;
    				    coils[starting_Address + rep + 3] = 1;
    				    rep += 4;
                break;
              case 0x0D:
                coils[starting_Address + rep] = 1;
    				    coils[starting_Address + rep + 1] = 0;
    				    coils[starting_Address + rep + 2] = 1;
    				    coils[starting_Address + rep + 3] = 1;
    				    rep += 4;
                break;
              case 0x0E:
                coils[starting_Address + rep] = 0;
    				    coils[starting_Address + rep + 1] = 1;
    				    coils[starting_Address + rep + 2] = 1;
    				    coils[starting_Address + rep + 3] = 1;
                rep += 4;
    				    break;
              case 0x0F:
                coils[starting_Address + rep] = 1;
    				    coils[starting_Address + rep + 1] = 1;
    				    coils[starting_Address + rep + 2] = 1;
    				    coils[starting_Address + rep + 3] = 1;
    				    rep += 4;
    					  break;
            }
    			}
    		}
    	}else{
    		ttt = 3;
    		int temp2 = num_of_coils;
    		int rep = 0;
    		uint8_t temp_hex;
    		for(int q=0; q<num_of_coils/8 +1;q++){
    			if(temp2>=8){
    	      for(int j=0; j<2; j++){
    				 if(j == 0){
    			     temp_hex = output_value[q]&0x0F;
    			   }else if(j==1){
    			     temp_hex = (output_value[q] >> 4) & 0x0F;
    				 }
    			   switch (temp_hex) {
               case 0x00:
                 coils[starting_Address + rep] = 0;
    		         coils[starting_Address + rep + 1] = 0;
    		         coils[starting_Address + rep + 2] = 0;
    				     coils[starting_Address + rep + 3] = 0;
                 rep += 4;
    				     break;
               case 0x01:
                  coils[starting_Address + rep] = 1;
    				      coils[starting_Address + rep + 1] = 0;
    				      coils[starting_Address + rep + 2] = 0;
    			        coils[starting_Address + rep + 3] = 0;
                  rep += 4;
    				      break;
                case 0x02:
                  coils[starting_Address + rep] = 0;
    		          coils[starting_Address + rep + 1] = 1;
    				      coils[starting_Address + rep + 2] = 0;
    				      coils[starting_Address + rep + 3] = 0;
                  rep += 4;
    				      break;
                case 0x03:
                  coils[starting_Address + rep] = 1;
    				      coils[starting_Address + rep + 1] = 1;
    				      coils[starting_Address + rep + 2] = 0;
    			      	coils[starting_Address + rep + 3] = 0;
                  rep += 4;
    				      break;
                case 0x04:
                  coils[starting_Address + rep] = 0;
    				      coils[starting_Address + rep + 1] = 0;
    				      coils[starting_Address + rep + 2] = 1;
    				      coils[starting_Address + rep + 3] = 0;
                  rep += 4;
    				      break;
                case 0x05:
                  coils[starting_Address + rep] = 1;
    				      coils[starting_Address + rep + 1] = 0;
    				      coils[starting_Address + rep + 2] = 1;
    				      coils[starting_Address + rep + 3] = 0;
                  rep += 4;
    				      break;
                case 0x06:
                  coils[starting_Address + rep] = 0;
    				      coils[starting_Address + rep + 1] = 1;
    				      coils[starting_Address + rep + 2] = 1;
    				      coils[starting_Address + rep + 3] = 0;
                  rep += 4;
    				      break;
                case 0x07:
                  coils[starting_Address + rep] = 1;
    				      coils[starting_Address + rep + 1] = 1;
    				      coils[starting_Address + rep + 2] = 1;
    				      coils[starting_Address + rep + 3] = 0;
                  rep += 4;
    				      break;
                case 0x08:
                  coils[starting_Address + rep] = 0;
    				      coils[starting_Address + rep + 1] = 0;
    				      coils[starting_Address + rep + 2] = 0;
    				      coils[starting_Address + rep + 3] = 1;
                  rep += 4;
    				      break;
                case 0x09:
                  coils[starting_Address + rep] = 1;
    				      coils[starting_Address + rep + 1] = 0;
    				      coils[starting_Address + rep + 2] = 0;
    				      coils[starting_Address + rep + 3] = 1;
                  rep += 4;
    				      break;
                case 0x0A:
                  coils[starting_Address + rep] = 0;
    				      coils[starting_Address + rep + 1] = 1;
    				      coils[starting_Address + rep + 2] = 0;
    				      coils[starting_Address + rep + 3] = 1;
                  rep += 4;
    				      break;
                case 0x0B:
                  coils[starting_Address + rep] = 1;
    				      coils[starting_Address + rep + 1] = 1;
    				      coils[starting_Address + rep + 2] = 0;
    				      coils[starting_Address + rep + 3] = 1;
    				      rep += 4;
                  break;
                case 0x0C:
                  coils[starting_Address + rep] = 0;
    				      coils[starting_Address + rep + 1] = 0;
    				      coils[starting_Address + rep + 2] = 1;
    				      coils[starting_Address + rep + 3] = 1;
    				      rep += 4;
                  break;
                case 0x0D:
                  coils[starting_Address + rep] = 1;
    				      coils[starting_Address + rep + 1] = 0;
    				      coils[starting_Address + rep + 2] = 1;
    				      coils[starting_Address + rep + 3] = 1;
    				      rep += 4;
                  break;
                case 0x0E:
                  coils[starting_Address + rep] = 0;
    				      coils[starting_Address + rep + 1] = 1;
    				      coils[starting_Address + rep + 2] = 1;
    				      coils[starting_Address + rep + 3] = 1;
                  rep += 4;
    				      break;
                case 0x0F:
                  coils[starting_Address + rep] = 1;
    				      coils[starting_Address + rep + 1] = 1;
    				      coils[starting_Address + rep + 2] = 1;
    				      coils[starting_Address + rep + 3] = 1;
    				      rep += 4;
    					    break;
               }
    				 }
    			 }else if(temp2 > 4 && temp2 <8){
    			    for(int j=0; j<2; j++){
    				    if(j == 0){
    			        temp_hex = output_value[q]&0x0F;
    							switch (temp_hex) {
                    case 0x00:
                      coils[starting_Address + rep] = 0;
    		              coils[starting_Address + rep + 1] = 0;
    		              coils[starting_Address + rep + 2] = 0;
    				          coils[starting_Address + rep + 3] = 0;
                      rep += 4;
    				          break;
                    case 0x01:
                      coils[starting_Address + rep] = 1;
    				          coils[starting_Address + rep + 1] = 0;
    				          coils[starting_Address + rep + 2] = 0;
    			            coils[starting_Address + rep + 3] = 0;
                      rep += 4;
    				          break;
                    case 0x02:
                      coils[starting_Address + rep] = 0;
    		              coils[starting_Address + rep + 1] = 1;
    				          coils[starting_Address + rep + 2] = 0;
    				          coils[starting_Address + rep + 3] = 0;
                      rep += 4;
    				          break;
                    case 0x03:
                      coils[starting_Address + rep] = 1;
    				          coils[starting_Address + rep + 1] = 1;
    				          coils[starting_Address + rep + 2] = 0;
    			      	    coils[starting_Address + rep + 3] = 0;
                      rep += 4;
    				          break;
                    case 0x04:
                      coils[starting_Address + rep] = 0;
    				          coils[starting_Address + rep + 1] = 0;
    				          coils[starting_Address + rep + 2] = 1;
    				          coils[starting_Address + rep + 3] = 0;
                      rep += 4;
    				          break;
                    case 0x05:
                      coils[starting_Address + rep] = 1;
    				          coils[starting_Address + rep + 1] = 0;
    				          coils[starting_Address + rep + 2] = 1;
    				          coils[starting_Address + rep + 3] = 0;
                      rep += 4;
    				          break;
                    case 0x06:
                      coils[starting_Address + rep] = 0;
    				          coils[starting_Address + rep + 1] = 1;
    				          coils[starting_Address + rep + 2] = 1;
    				          coils[starting_Address + rep + 3] = 0;
                      rep += 4;
    				          break;
                    case 0x07:
                      coils[starting_Address + rep] = 1;
    				          coils[starting_Address + rep + 1] = 1;
    				          coils[starting_Address + rep + 2] = 1;
    				          coils[starting_Address + rep + 3] = 0;
                      rep += 4;
    				          break;
                    case 0x08:
                      coils[starting_Address + rep] = 0;
    				          coils[starting_Address + rep + 1] = 0;
    				          coils[starting_Address + rep + 2] = 0;
    				          coils[starting_Address + rep + 3] = 1;
                      rep += 4;
    				          break;
                    case 0x09:
                      coils[starting_Address + rep] = 1;
    				          coils[starting_Address + rep + 1] = 0;
    				          coils[starting_Address + rep + 2] = 0;
    				          coils[starting_Address + rep + 3] = 1;
                      rep += 4;
    				          break;
                    case 0x0A:
                      coils[starting_Address + rep] = 0;
    				          coils[starting_Address + rep + 1] = 1;
    				          coils[starting_Address + rep + 2] = 0;
    				          coils[starting_Address + rep + 3] = 1;
                      rep += 4;
    				          break;
                    case 0x0B:
                      coils[starting_Address + rep] = 1;
    				          coils[starting_Address + rep + 1] = 1;
    				          coils[starting_Address + rep + 2] = 0;
    				          coils[starting_Address + rep + 3] = 1;
    				          rep += 4;
                      break;
                    case 0x0C:
                      coils[starting_Address + rep] = 0;
    				          coils[starting_Address + rep + 1] = 0;
    				          coils[starting_Address + rep + 2] = 1;
    				          coils[starting_Address + rep + 3] = 1;
    				          rep += 4;
                      break;
                    case 0x0D:
                      coils[starting_Address + rep] = 1;
    				          coils[starting_Address + rep + 1] = 0;
    				          coils[starting_Address + rep + 2] = 1;
    				          coils[starting_Address + rep + 3] = 1;
    				          rep += 4;
                      break;
                    case 0x0E:
                      coils[starting_Address + rep] = 0;
    				          coils[starting_Address + rep + 1] = 1;
    				          coils[starting_Address + rep + 2] = 1;
    				          coils[starting_Address + rep + 3] = 1;
                      rep += 4;
    				          break;
                    case 0x0F:
                      coils[starting_Address + rep] = 1;
    				          coils[starting_Address + rep + 1] = 1;
    				          coils[starting_Address + rep + 2] = 1;
    				          coils[starting_Address + rep + 3] = 1;
    				          rep += 4;
    					        break;
    							}
    			      }else if(j==1){
    			        temp_hex = (output_value[q] >> 4) & 0x0F;
    							switch (temp2 - 4){
    								case 1:
    									switch (temp_hex){
    										case 0x00:
    											coils[starting_Address +rep] = 0;
    										  break;
    										case 0x01:
    											coils[starting_Address +rep] = 1;
    										  break;
    									}
    									break;
    								case 2:
    									switch (temp_hex){
    									  case 0x00:
    										  coils[starting_Address +rep] = 0;
    										  coils[starting_Address +rep+1] = 0;
    										  break;
    										case 0x01:
    											coils[starting_Address +rep] = 1;
                          coils[starting_Address +rep+1] = 0;
                          break;
    										case 0x02:
    											coils[starting_Address +rep] = 0;
                          coils[starting_Address +rep+1] = 1;
                          break;
    										case 0x03:
    											coils[starting_Address +rep] = 1;
                          coils[starting_Address +rep+1] = 1;
                          break;
    									}
    									break;
    								case 3:
    									switch (temp_hex){
    									  case 0x00:
    										  coils[starting_Address +rep] = 0;
    										  coils[starting_Address +rep+1] = 0;
    										  coils[starting_Address +rep+2] = 0;
    										  break;
    										case 0x01:
    											coils[starting_Address +rep] = 1;
                          coils[starting_Address +rep+1] = 0;
    										  coils[starting_Address +rep+2] = 0;
                          break;
    										case 0x02:
    											coils[starting_Address +rep] = 0;
                          coils[starting_Address +rep+1] = 1;
    										  coils[starting_Address +rep+2] = 0;
                          break;
    										case 0x03:
    											coils[starting_Address +rep] = 1;
                          coils[starting_Address +rep+1] = 1;
    										  coils[starting_Address +rep+2] = 0;
                          break;
    									  case 0x04:
    										  coils[starting_Address +rep] = 0;
    										  coils[starting_Address +rep+1] = 0;
    										  coils[starting_Address +rep+2] = 1;
    										  break;
    										case 0x05:
    											coils[starting_Address +rep] = 1;
                          coils[starting_Address +rep+1] = 0;
    										  coils[starting_Address +rep+2] = 1;
                          break;
    										case 0x06:
    											coils[starting_Address +rep] = 0;
                          coils[starting_Address +rep+1] = 1;
    										  coils[starting_Address +rep+2] = 1;
                          break;
    										case 0x07:
    											coils[starting_Address +rep] = 1;
                          coils[starting_Address +rep+1] = 1;
    										  coils[starting_Address +rep+2] = 1;
                          break;
    									}
    									break;
    							}

    				    }
    					}
    			 }else if(temp2 <= 4 && temp2 >= 1){
    				  temp_hex = output_value[q]&0x0F;
    					switch (temp2){
    						case 1:
    							switch (temp_hex){
    								case 0x00:
    									coils[starting_Address +rep] = 0;
    								  break;
    								case 0x01:
    									coils[starting_Address +rep] = 1;
    								  break;
    							}
    							break;
    						case 2:
    							switch (temp_hex){
    							  case 0x00:
    								  coils[starting_Address +rep] = 0;
    									coils[starting_Address +rep+1] = 0;
    								  break;
    								case 0x01:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 0;
                      break;
    								case 0x02:
    									coils[starting_Address +rep] = 0;
                      coils[starting_Address +rep+1] = 1;
                      break;
    							  case 0x03:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 1;
                      break;
    									}
    							break;
    						case 3:
    							switch (temp_hex){
    								case 0x00:
    								  coils[starting_Address +rep] = 0;
    									coils[starting_Address +rep+1] = 0;
    								  coils[starting_Address +rep+2] = 0;
    									break;
    								case 0x01:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 0;
    								  coils[starting_Address +rep+2] = 0;
                       break;
    								case 0x02:
    									coils[starting_Address +rep] = 0;
                      coils[starting_Address +rep+1] = 1;
    									coils[starting_Address +rep+2] = 0;
                      break;
    								case 0x03:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 1;
    								  coils[starting_Address +rep+2] = 0;
                      break;
    								case 0x04:
    								  coils[starting_Address +rep] = 0;
    								  coils[starting_Address +rep+1] = 0;
    									coils[starting_Address +rep+2] = 1;
    									break;
    								case 0x05:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 0;
    									coils[starting_Address +rep+2] = 1;
                      break;
    								case 0x06:
    									coils[starting_Address +rep] = 0;
                      coils[starting_Address +rep+1] = 1;
    									coils[starting_Address +rep+2] = 1;
                      break;
    								case 0x07:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 1;
    								  coils[starting_Address +rep+2] = 1;
                      break;
    							}
    							break;
    						case 4:
    							switch (temp_hex){
    								case 0x00:
    								  coils[starting_Address +rep] = 0;
    									coils[starting_Address +rep+1] = 0;
    								  coils[starting_Address +rep+2] = 0;
    								  coils[starting_Address +rep+3] = 0;
    									break;
    								case 0x01:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 0;
    								  coils[starting_Address +rep+2] = 0;
    								  coils[starting_Address +rep+3] = 0;
                       break;
    								case 0x02:
    									coils[starting_Address +rep] = 0;
                      coils[starting_Address +rep+1] = 1;
    									coils[starting_Address +rep+2] = 0;
    								  coils[starting_Address +rep+3] = 0;
                      break;
    								case 0x03:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 1;
    								  coils[starting_Address +rep+2] = 0;
    								  coils[starting_Address +rep+3] = 0;
                      break;
    								case 0x04:
    								  coils[starting_Address +rep] = 0;
    								  coils[starting_Address +rep+1] = 0;
    									coils[starting_Address +rep+2] = 1;
    								  coils[starting_Address +rep+3] = 0;
    									break;
    								case 0x05:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 0;
    									coils[starting_Address +rep+2] = 1;
    									coils[starting_Address +rep+3] = 0;
                      break;
    								case 0x06:
    									coils[starting_Address +rep] = 0;
                      coils[starting_Address +rep+1] = 1;
    									coils[starting_Address +rep+2] = 1;
    								  coils[starting_Address +rep+3] = 0;
                      break;
    								case 0x07:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 1;
    								  coils[starting_Address +rep+2] = 1;
    								  coils[starting_Address +rep+3] = 0;
                      break;
    								case 0x08:
    								  coils[starting_Address +rep] = 0;
    									coils[starting_Address +rep+1] = 0;
    								  coils[starting_Address +rep+2] = 0;
    								  coils[starting_Address +rep+3] = 1;
    									break;
    								case 0x09:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 0;
    								  coils[starting_Address +rep+2] = 0;
    								  coils[starting_Address +rep+3] = 1;
                       break;
    								case 0x0A:
    									coils[starting_Address +rep] = 0;
                      coils[starting_Address +rep+1] = 1;
    									coils[starting_Address +rep+2] = 0;
    								  coils[starting_Address +rep+3] = 1;
                      break;
    								case 0x0B:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 1;
    								  coils[starting_Address +rep+2] = 0;
    								  coils[starting_Address +rep+3] = 1;
                      break;
    								case 0x0C:
    								  coils[starting_Address +rep] = 0;
    								  coils[starting_Address +rep+1] = 0;
    									coils[starting_Address +rep+2] = 1;
    								  coils[starting_Address +rep+3] = 1;
    									break;
    								case 0x0D:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 0;
    									coils[starting_Address +rep+2] = 1;
    								  coils[starting_Address +rep+3] = 1;
                      break;
    								case 0x0E:
    									coils[starting_Address +rep] = 0;
                      coils[starting_Address +rep+1] = 1;
    									coils[starting_Address +rep+2] = 1;
    								  coils[starting_Address +rep+3] = 1;
                      break;
    								case 0x0F:
    									coils[starting_Address +rep] = 1;
                      coils[starting_Address +rep+1] = 1;
    								  coils[starting_Address +rep+2] = 1;
    								  coils[starting_Address +rep+3] = 1;
                      break;
    							}
    						}
    			 }
    			 temp2 -=8;
    		}
    	}
		}else{
		  flag = 1;
			exception_code = 0x04;
		}
		if(flag == 1){

		x[0] = Slave_Address;
		x[1] = 0x8F;
		x[2] = exception_code;
		slave_crc_send = crc16(x,3);
	  x[4] = slave_crc_send >> 8 & 0xFF;
	  x[3] = slave_crc_send & 0xFF;
		sending_byte_count = 5;
		flag = 0;
		}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void write_multiple_registers(uint16_t starting_Address, uint16_t bytes,uint16_t num_of_regs, unsigned int input_regs[], unsigned int regs[],uint8_t x[]){
	 uint16_t slave_crc_send;
	 if(num_of_regs < 0 || num_of_regs > 0x007B || bytes != num_of_regs*2){
	     exception_code = 0x03;
		   flag = 1;
	 }else if(starting_Address < len_of_regs && starting_Address + num_of_regs < len_of_regs){
	     exception_code = 0x02;
		   flag = 1;
	 }else if(starting_Address + num_of_regs < len_of_regs && num_of_regs >= 0 && num_of_regs <= 0x007B){
    	x[0] = Slave_Address;
			x[1] = 0x10;
			x[2] = (starting_Address >> 8) & 0xFF;
			x[3] =  starting_Address & 0xFF;
			x[4] = (num_of_regs >> 8) & 0xFF;
			x[5] =  num_of_regs & 0xFF;
		  slave_crc_send = crc16(x,6);
			x[7] = slave_crc_send >> 8 & 0xFF;
			x[6] = slave_crc_send & 0xFF;
			sending_byte_count = 8;
   	for(int i=0; i<num_of_regs; i++){
		  regs[starting_Address + i] = input_regs[i];
	  }
	}else{
	    exception_code = 0x04;
		  flag = 1;
	}
	if(flag == 1){
		x[0] = Slave_Address;
		x[1] = 0x90;
		x[2] = exception_code;
		slave_crc_send = crc16(x,3);
	  x[4] = slave_crc_send >> 8 & 0xFF;
	  x[3] = slave_crc_send & 0xFF;
		sending_byte_count = 5;
		flag = 0;
	}
}

/* main-camellia-test.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 * camellia test-suit
 * 
 */
#include "main-test-common.h"

#include "camellia.h"
#include "nessie_bc_test.h"
#include "performance_test.h"
#include "cli.h"
#include "bcal_camellia128.h"
#include "bcal-performance.h"

char* algo_name = "Camellia";

const bcdesc_t* algolist[] = {
	(bcdesc_t*)&camellia128_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
void camellia128_init_dummy(void* key, uint16_t keysize_b, void* ctx){
	camellia128_init(key, ctx);
}

void testrun_nessie_camellia(void){
	nessie_bc_ctx.blocksize_B =  16;
	nessie_bc_ctx.keysize_b   = 128;
	nessie_bc_ctx.name        = algo_name;
	nessie_bc_ctx.ctx_size_B  = sizeof(camellia128_ctx_t);
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)camellia128_enc;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)camellia128_dec;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)camellia128_init_dummy;
	
	nessie_bc_run();
}

/*
 * P No.001 : 80 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 */

const uint8_t test_keys[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
	0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
	0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
	0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
	0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE, 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01,
	0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01, 0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE
};

void hexdump128(void* data){
	uint8_t i;
	for(i=0; i<16; ++i){
		cli_hexdump(data, 1);
		cli_putc(' ');
		data = (uint8_t*)data +1;
	}
}

void testrun_camellia128(void){
	uint8_t data[16];
	uint8_t data2[16];
	uint8_t key[16];
	char str[4];
	uint8_t i,j;
	str[3]= '\0';
	for(j=0; j<10; ++j){
		str[0] = ('0'+(j+1)/100);
		str[1] = ('0'+((j+1)/10)%10);
		str[2] = ('0'+(j+1)%10);
		memcpy(key, test_keys+j*16, 16);
		cli_putstr("\r\nK No.");
		cli_putstr(str);
		cli_putstr(" : ");
		hexdump128(key);
		camellia128_ctx_t ctx;
		camellia128_init(key, &ctx);
		for(i=0; i<128; ++i){
			memset(data, 0x00, 16);
			data[i/8] = 0x80>>i%8;
			memcpy(data2, data, 16);
			str[0] = ('0'+(i+1)/100);
			str[1] = ('0'+((i+1)/10)%10);
			str[2] = ('0'+(i+1)%10);
			cli_putstr("\r\nP No.");
			cli_putstr(str);
			cli_putstr(" : ");
			hexdump128(data);
			camellia128_enc(data, &ctx);
			cli_putstr("\r\nC No.");
			cli_putstr(str);
			cli_putstr(" : ");
			hexdump128(data);
			camellia128_dec(data, &ctx);
			if(memcmp(data, data2, 16)){
				cli_putstr("\r\n DECRYPTION ERROR !!!");
			}
		}
	}
}

void test_performance_camellia(void){
	bcal_performance_multiple(algolist);
}


/*****************************************************************************
 *  self tests																 *
 *****************************************************************************/
/*
128-bit key
key         01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
plaintext   01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
ciphertext  67 67 31 38 54 96 69 73 08 57 06 56 48 ea be 43
*/
void testrun_camellia(void){

  uint8_t data[16] = { 0x01, 0x23, 0x45, 0x67, 
                       0x89, 0xab, 0xcd, 0xef, 
                       0xfe, 0xdc, 0xba, 0x98, 
                       0x76, 0x54, 0x32, 0x10 };

  const uint8_t key[16] = { 0x01, 0x23, 0x45, 0x67,
                       0x89, 0xab, 0xcd, 0xef, 
                       0xfe, 0xdc, 0xba, 0x98, 
                       0x76, 0x54, 0x32, 0x10 };


  camellia128_ctx_t ctx;
  camellia128_init(key, &ctx);
  cli_putstr("\r\n key:        ");
  cli_hexdump(data, 16);
  cli_putstr("\r\n plaintext:  ");
  cli_hexdump(data, 16);
  camellia128_enc(data, &ctx);
  cli_putstr("\r\n ciphertext: ");
  cli_hexdump(data, 16);
  camellia128_dec(data, &ctx);
  cli_putstr("\r\n decrypted:  ");
  cli_hexdump(data, 16);

}


/*****************************************************************************
 * main                                                                      *
 *****************************************************************************/

const char nessie_str[]      = "nessie";
const char test_str[]        = "test";
const char test128_str[]     = "test128";
const char performance_str[] = "performance";
const char echo_str[]        = "echo";

cmdlist_entry_t cmdlist[] = {
	{ nessie_str,      NULL, testrun_nessie_camellia },
	{ test_str,        NULL, testrun_camellia},
	{ test128_str,     NULL, testrun_camellia128},
	{ performance_str, NULL, test_performance_camellia},
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ NULL,            NULL, NULL}
};

int main (void){
	main_setup();
	
	for(;;){
		welcome_msg(algo_name);
		cmd_interface(cmdlist);
	}
}

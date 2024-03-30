/*
 * Calculator.c
 *
 * Created: 3/29/2024 9:37:57 AM
 * Author : Ve-ga
 */ 

#include "keypad.h"
#include "lcd.h"
#include "std_macros.h"

int32_t calc(int8_t *);
uint8_t get_input_from_user();
int32_t get_result(int32_t , int8_t ,int32_t );

int main(void)
{
	LCD_init();
	KEYPAD_init();
	int8_t error_mes[] = "Mathematical error", input , arr[25], idx ,next_oper ;
	int32_t result = 0;

	while(1)
	{
		input , arr[25], idx = 0, next_oper = 0;
		while(1){
			input = get_input_from_user();
			if(next_oper == 0){
				LCD_write_command(1);
				next_oper = 1;
			}
			LCD_write_char(input);
			if(input == 'c'){
				LCD_write_command(1);
				continue;
			}
			arr[idx++] = input;
			if(input == '='){
				result = calc(arr);
				if(result == 'F'){
					LCD_write_command(1);
					LCD_write_string(error_mes);
				}
				else{
					LCD_write_number(result);
				}
				break;
			}
		}
		
	}
}
int32_t calc(int8_t *arr){
	int32_t num1 = 0, num2 = 0;
	int8_t op = '#', is_neg1 = 0, is_neg2 = 0, num1_done = 0;
	if(arr[0] == '-'){
		is_neg1 = 1;
		arr++;
	}
	while(*arr != '='){
		if(op != '#'){
			if(*arr == '-'){
				is_neg2 = 1;
				arr++;
				continue;
			}
		}
		if((*arr >= '0') && (*arr <= '9')){
			if(op == '#'){
				num1 = num1 * 10 + ((*arr) - '0');
				num1_done = 1;
			}
			else{
				num2 = num2 * 10 + ((*arr) - '0');
			}
		}
		else{
			if(op == '#' && num1_done == 1){
				op = *arr;
			}
			else{
				return 'F';
			}
		}
		arr++;
	}
	if((op == '/') && (num2 == 0)){
		return 'F';
	}
	if((num1 == 0 && is_neg1 == 1)|| (num2 == 0 && is_neg2 == 1)){
		return 'F';
	}
	if(is_neg1){
		num1 = -num1;
	}
	if(is_neg2){
		num2 = -num2;
	}
	return get_result(num1, op, num2);
	
}
int32_t get_result(int32_t num1, int8_t op,int32_t num2){
	switch(op){
		case '+': return num1 + num2;
		case '-': return num1 - num2;
		case '*': return num1 * num2;
		case '/': return num1 / num2;
	}
	return num1;
}
uint8_t get_input_from_user(){
	int8_t key;
	do{
		key = KEYPAD_read();
	}while(key == -1);
	
	//LCD_write_char(key);
	while(KEYPAD_read() != -1){}
	return key;
}
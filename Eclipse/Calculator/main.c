/*
 * main.c
 *
 *  Created on: Mar 26, 2025
 *      Author: LightStore
 *
 * Description:
 * This program implements a simple calculator using an LCD and a keypad.
 * It continuously reads user input from the keypad, performs calculations,
 * and displays results on the LCD.
 */

#include "lcd.h"      /* Include LCD driver header file */
#include "keypad.h"   /* Include Keypad driver header file */
#include "util/delay.h"  /* Include delay functions */
#include "std_types.h"   /* Include standard type definitions */

/* Global variable to store the currently pressed key */
uint8 key = 0;

/*
 * Function: Update_result
 * ------------------------
 * Updates the result based on the given operand and number.
 *
 * result: Pointer to the current result variable.
 * num: The number to be applied with the operand.
 * operand: The operation to perform (+, -, *, /).
 *
 * This function modifies the result in place.
 */
void Update_result(int * result, int num, uint8 operand)
{
	if (operand == '%') {
		(*result) /= num; /* Perform division */
	} else if (operand == '*') {
		(*result) *= num; /* Perform multiplication */
	} else if (operand == '+') {
		(*result) += num; /* Perform addition */
	} else if (operand == '-') {
		(*result) -= num; /* Perform subtraction */
	} else if(operand == 0) {
		(*result) = num;  /* Initialize result with the first number */
	}
}

int main(void)
{
	/* Variables to store operand, result, current number, and previous key pressed */
	uint8 operand = 0;
	int result = 0;
	int num = 0;
	uint8 prev = 0;

	LCD_init(); /* Initialize the LCD */

	while(1)
	{
		key = KEYPAD_getPressedKey(); /* Read input from the keypad */

		if(key == ENTER)
		{
			/* Reset everything if ENTER is pressed */
			LCD_clearScreen();
			result = 0;
			operand = 0;
			num = 0;
			prev = key;
		}
		else if(key > 9 && prev != '=') /* If the key is an operator and the previous key was not '=' */
		{
			if(prev <= 9) /* If the previous key was a number */
			{
				LCD_displayCharacter(key); /* Display the operator on the LCD */

				if(key == '=') /* If '=' is pressed, calculate and display the result */
				{
					Update_result(&result, num, operand);
					LCD_moveCursor(1, 0);
					LCD_intgerToString(result);
					result = 0;
					operand = 0;
				}
				else /* Otherwise, update the result and set the new operand */
				{
					Update_result(&result, num, operand);
					operand = key;
				}
			}
			num = 0; /* Reset number after operator is entered */
			prev = key; /* Store the current key as previous */
		}
		else if(key <= 9 && key >= 0) /* If the key is a number */
		{
			if(prev == '=') /* If '=' was previously pressed, reset the display */
			{
				LCD_clearScreen();
				operand = 0;
				result = 0;
			}

			num = (num * 10) + key; /* Construct multi-digit numbers */
			LCD_intgerToString(key); /* Display the number on the LCD */
			prev = key; /* Store the current key as previous */
		}

		_delay_ms(400); /* Debounce delay */
	}
}

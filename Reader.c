/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
 _________________________________
|                                 |
| ........ BOA LANGUAGE ......... |
|     __    __    __    __        |
|    /  \  /  \  /  \  /  \       |
| __/  __\/  __\/  __\/  __\__    |
| _/  /__/  /__/  /__/  /_____|   |
|  \_/ \   / \   / \   / \  \___  |
|       \_/   \_/   \_/   \___o_> |
|                                 |
| .. ALGONQUIN COLLEGE - 2022F .. |
|_________________________________|

*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152  Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */

ReaderPointer readerCreate(mba_intg size, mba_intg increment, mba_intg mode) {
	ReaderPointer readerPointer;
	if (!size)
	{
		size = READER_DEFAULT_SIZE;
		increment = READER_DEFAULT_INCREMENT;
	}
	if (!increment)
		mode = MODE_FIXED;
	if (mode != MODE_FIXED && mode != MODE_ADDIT && mode != MODE_MULTI)
		return NULL;

	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));
	if (!readerPointer)
		return NULL;
	readerPointer->content = (mba_char*)malloc(size);
	if (!readerPointer->content)
		return NULL;

	for (int i = 0; i < NCHAR; i++)
		readerPointer->histogram[i] = 0;
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;
	readerPointer->flags = READER_DEFAULT_FLAG;
	readerPointer->flags = readerPointer->flags | READER_EMP_FLAG; // 0000.0100
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ReaderPointer readerAddChar(ReaderPointer const readerPointer, mba_char ch) {
	mba_char* tempReader = NULL;
	mba_intg newSize = 0;

	if (!readerPointer || ch < 0)
		return NULL;
	readerPointer->flags = readerPointer->flags & READER_RESET_REL_FLAG;
	if (readerPointer->position.wrte * (mba_intg)sizeof(mba_char) < readerPointer->size) {
		readerPointer->flags = readerPointer->flags & READER_RESET_FUL_FLAG;
	}
	else {
		readerPointer->flags = readerPointer->flags | READER_FUL_FLAG;

		switch (readerPointer->mode) {
		case MODE_FIXED:
			return NULL;

		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;
			if (newSize < 0)
				return NULL;
			break;

		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			if (newSize < 0)
				return NULL;
			break;
		default:
			return NULL;
		}
		tempReader = realloc(readerPointer->content, newSize);
		if (!tempReader)
			return NULL;
		if (readerPointer->content != tempReader)
			readerPointer->flags = readerPointer->flags | READER_REL_FLAG;
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}
	readerPointer->content[readerPointer->position.wrte++] = ch;
	readerPointer->histogram[ch]++;
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_boln readerClear(ReaderPointer const readerPointer) {

	/* TO_DO: Adjust flags original */     	//Is orginal default or empty?
	if (!readerPointer)
		return MBA_FALSE;
	readerPointer->flags = readerPointer->flags & READER_DEFAULT_FLAG;
	return MBA_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_boln readerFree(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return MBA_FALSE;
	free(readerPointer);		// is this pointer freed?
	return MBA_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_boln readerIsFull(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return MBA_FALSE;
	if (readerPointer->flags && READER_FUL_FLAG) //chk full flag 
		return MBA_TRUE;
	return MBA_FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_boln readerIsEmpty(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return MBA_FALSE;

	if (readerPointer->flags && READER_EMP_FLAG)
		return MBA_TRUE;
	return MBA_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_boln readerSetMark(ReaderPointer const readerPointer, mba_intg mark) {
	if (!readerPointer || mark<0 || mark > readerPointer->position.wrte)
		return MBA_FALSE;
	readerPointer->position.mark = mark;
	return MBA_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_intg readerPrint(ReaderPointer const readerPointer) {
	mba_intg cont = 0;
	mba_char c;

	if (!readerPointer)
		return READER_ERROR;

	c = readerGetChar(readerPointer);
	if ((readerPointer->flags && READER_CHK_END_FLAG))
	{
		while (cont < readerPointer->position.wrte) {
			cont++;
			printf("%c", c);
			c = readerGetChar(readerPointer);
		}
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_intg readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	mba_intg size = 0;
	mba_char c;

	if (!readerPointer || !fileDescriptor)
		return READER_ERROR;
	c = (mba_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_boln readerRecover(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Recover positions */
	if (!readerPointer)
		return MBA_FALSE;
	//readerPointer->position.read = 0;
	//readerPointer->position.wrte = 0;
	return MBA_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_boln readerRetract(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return MBA_FALSE;
	readerPointer->position.read--;
	if (readerPointer->position.read < 0)
		return MBA_FALSE;

	return MBA_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_boln readerRestore(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return MBA_FALSE;
	readerPointer->position.read = readerPointer->position.mark;
	return MBA_TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_char readerGetChar(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	if (readerPointer->position.read == readerPointer->position.wrte) {
		readerPointer->flags = readerPointer->flags | READER_END_FLAG;
		return '\0';
	}
	readerPointer->flags = readerPointer->flags & READER_RESET_END_FLAG;

	return readerPointer->content[readerPointer->position.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_char* readerGetContent(ReaderPointer const readerPointer, mba_intg pos) {
	if (!readerPointer)
		return NULL;
	return readerPointer->content;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_intg readerGetPosRead(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

mba_intg readerGetPosWrte(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_intg readerGetPosMark(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_intg readerGetSize(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_intg readerGetInc(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_intg readerGetMode(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->mode;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_byte readerGetFlags(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;
	return readerPointer->flags;
}


/*
***********************************************************
* Function name: readerShowStat
* Purpose: Returns the number of chars used.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mba_intg readerShowStat(ReaderPointer const readerPointer) {
	int count = 0;
	if (!readerPointer)
		return READER_ERROR;
	for (int i = 0; i < NCHAR; i++)
	{
		if (readerPointer->histogram[i] != 0) {
			count++;
		}
	}
	return count;
}
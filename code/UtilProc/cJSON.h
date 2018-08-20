/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef cJSON__h
#define cJSON__h


/* The cJSON structure: */
typedef struct {
	char *pmName;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
	char *pmValue;
	int len;
	unsigned char type;
	double number;
} stJSON;


#ifdef __cplusplus
extern "C"
{
#endif



extern int parseJsonCurLevel(char *pInput, int iLen, stJSON *pstJson, int *pCount);
extern stJSON *getJsonObjByName(stJSON *pstJson, int count, char *pName);
extern int getJsonString(stJSON *pstJson, int count, char *pOut, int *pLen);

#ifdef __cplusplus
}
#endif

#endif

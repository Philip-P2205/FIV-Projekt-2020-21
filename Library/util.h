/*
 * util.h
 *
 *  Created on: 31.03.2021
 *      Author: Philip Prohaska
 */

#ifndef UTIL_H_
#define UTIL_H_

#define BIT_DISABLE(t, b) (t) &= ~(b)
#define BIT_ENABLE(t, b) (t) |= (b)
#define BIT_TOGGLE(t, b) (t) ^= (b)

#define BIT_ENABLED(t, b) (((t) & (b)) !=0 )
#define BIT_DISABLED(t, b) (((t) & (b)) == 0)

#define TIE_TO_STATE(t, b, s) { \
    if (s) BIT_ENABLE(t, b);     \
    else BIT_DISABLE(t, b);      \
}

#define _UINT(bits) uint##bits##_t
#define UINT(bits) _UINT(bits)

#define __GET_DIR(port) port ## DIR
#define __GET_OUT(port) port ## OUT
#define __GET_IN (port) port ## IN
#define __GET_REN(port) port ## REN
#define __GET_IES(port) port ## IES
#define __GET_IFG(port) port ## IFG
#define __GET_IE (port) port ## IE

#define GET_DIR(port) __GET_DIR(port)
#define GET_OUT(port) __GET_OUT(port)
#define GET_IN (port) __GET_IN (port)
#define GET_REN(port) __GET_REN(port)
#define GET_IES(port) __GET_IES(port)
#define GET_IFG(port) __GET_IFG(port)
#define GET_IE (port) __GET_IE (port)

#define DIR_OUTPUT(t) BIT_ENABLE(t##_DIR, t)
#define DIR_INPUT(t) BIT_DISABLE(t##_DIR, t)
#define OUT_HIGH(t) BIT_ENABLE(t##_OUT, t)
#define OUT_LOW(t) BIT_DISABLE(t##_OUT, t)

#endif /* UTIL_H_ */

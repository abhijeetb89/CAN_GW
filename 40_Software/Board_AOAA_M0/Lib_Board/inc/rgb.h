/*****************************************************************************
 *
 *   Copyright(C) 2011, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * Embedded Artists AB assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. Embedded Artists AB
 * reserves the right to make changes in the software without
 * notification. Embedded Artists AB also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *****************************************************************************/
#ifndef __RGB_H
#define __RGB_H


#define RGB_RED   0x01
#define RGB_BLUE  0x02
#define RGB_GREEN 0x04


void rgb_init (void);
void rgb_setLeds (uint8_t ledOnMask, uint8_t ledOffMask);


#endif /* end __RGB_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/

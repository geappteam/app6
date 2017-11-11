/*
 * SPI_Config.h
 *
 *  Created on: 11 nov. 2017
 *      Author: para2709
 */

#ifndef INCLUDES_SPI_CONFIG_H_
#define INCLUDES_SPI_CONFIG_H_

#include <csl.h>
#include <csl_mcbsp.h>


#ifdef SPI_DRIVER_CONFIG_IMPORT
    #define EXTERN
#else
    #define EXTERN extern
#endif

EXTERN McBSP_Config MCBSP0_SPI_Cfg;

#endif /* INCLUDES_SPI_CONFIG_H_ */

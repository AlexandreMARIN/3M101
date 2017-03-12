#ifndef ROAD_TRAFFIC_H
#define ROAD_TRAFFIC_H

/** \file road_traffic.h
 * \brief That file defines the type 'road' and operators on it.
 * \details It enables us to handle roads.
 * \author Alexandre MARIN
 *
 */


#include <stdint.h>

#define UINT uint8_t
#define BITSIZE (sizeof(UINT)*8)

typedef UINT road;

road * road_generation(int places, unsigned long int *n);

void road_show(road r);

void road_function(road r, road *res);

/** \def UINT
 * \details It is an unsigned integer like \c uint32_t .
 */

/** \def BITSIZE
 * \details It is the number of bits of \c UINT .
 */

/** \typedef road
 * \details A road is here an integer. Each bit is matched to a location on the road : the bit is set to 0 if and only if no car is located at this place.
 *
 */

/** \fn road * road_generation(int places, unsigned long int* n)
 * \brief This function creates a table which contains all the roads of length \c BITSIZE and \c places cars.
 * \param places The number of cars in the roads.
 * \param n number of possibilities
 * \return A handle towards a table of roads.
 */

/** \fn void road_show(road r)
 * \brief That function displays the road \c r on a terminal, with 1 and 0.
 * \param r A road
 */

/** \fn void road_function(road r, road *res)
 * \brief This is the function studied here.
 * \param[in] r The input.
 * \param[out] res The output.
 */




#endif // ROAD_TRAFFIC_H

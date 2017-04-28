#ifndef ROAD_TRAFFIC_HPP
#define ROAD_TRAFFIC_HPP


/** \file road_traffic.hpp
 * \brief That file defines the type 'road' and operators on it.
 * \details It enables us to handle roads.
 * \author Alexandre MARIN
 *
 */


#include <cstdint>


class road {
    static int length;
    uint32_t i;//only the first 'length'-th bits from right are considered

public :

    /**
    * There are here some methods and functions, some of them enable us to use symbols like & for the class road.
    */
    static int len();
    static void set_length(int l);
    road operator= (const road &op);
    road operator= (const int &op);
    road operator<< (const int &op) const;
    road operator>> (const int &op) const;
    operator uint32_t() const;
    friend road operator~ (const road &op);
    friend road operator& (const road &op1, const road &op2);
    friend road operator| (const road &op1, const road &op2);
    friend road operator|= (road &op1, const unsigned int &op2);
    friend bool operator== (const road &op1, const road &op2);
    /** \c key() returns the location of the car in the table \c locations . */
    int key(int c, int cars);
};



road * road_generation(int places, uint64_t *n);

void road_show(road r);

void road_function(road r, road *res);


/** \typedef road
 * \details A road is here an integer whose length equals \c length . Each bit is matched to a location on the road : the bit is set to 0 if and only if no car is located at this place.
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



#endif // ROAD_TRAFFIC_HPP

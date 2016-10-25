/*
 * map.c 
 * A simple map object fo Pd (Pure Data) similar to Arduino map function : <https://www.arduino.cc/en/Reference/Map>
 * (cc) 2016, HappyCodeFarm, Guillaume Stagnaro. 
 * http://www.happycodefarm.net
 *
 *
 * Provided under the terms of the WTFPL 2.0 license (Do What the Fuck You Want to Public License)
 *
 *
 *          DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                  Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.

 *          DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 * TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 * 0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "m_pd.h"

// Structure definition of a single Pd 'mpu6050' object.
typedef struct map {
  	t_object x_ob;
	t_outlet *maped_outlet;
	
	float in_min;
	float in_max;
	
	float out_min;
	float out_max;
	
	float value;
	
} t_map;

// map outlets/inlets
void map_output_value(t_map *map) {

	float result = (map->value - map->in_min) * (map->out_max - map->out_min) / (map->in_max - map->in_min) + map->out_min;
	
	outlet_float(map->maped_outlet,result);
}

void map_set_in_min(t_map *map, t_floatarg f) {
    map->in_min = (float)f;
    map_output_value(map);
}
void map_set_in_max(t_map *map, t_floatarg f) {
	map->in_max = (float)f;
	map_output_value(map);
}
void map_set_out_min(t_map *map, t_floatarg f) {
	map->out_min = (float)f;
	map_output_value(map);
}
void map_set_out_max(t_map *map, t_floatarg f) {
	map->out_max = (float)f;
	map_output_value(map);
}

void map_set_value(t_map *map, t_floatarg f) {
	float result = ((float)f - map->in_min) * (map->out_max - map->out_min) / (map->in_max - map->in_min) + map->out_min;
	map->value = result;
	outlet_float(map->maped_outlet,result);
}


t_class *map_class;

// Create an instance of a Pd 'map' object.
void *map_new(t_floatarg in_min, t_floatarg in_max, t_floatarg out_min, t_floatarg out_max) {
    // post("map: new. %f %f %f %f", 
//     	in_min, 
//     	in_max, 
//     	out_min, 
//     	out_max);
	
    t_map *map = (t_map *)pd_new(map_class);

	map->value = 0.0;
	map->in_min = (float)in_min;
	map->in_max = (float)in_max;
	map->out_min = (float)out_min;
	map->out_max = (float)out_max;
	
	// Create object outlet
	map->maped_outlet = outlet_new(&map->x_ob,NULL);
	
	// Create object inlets
	inlet_new(&map->x_ob,&map->x_ob.ob_pd, &s_float, gensym("in_min"));
	inlet_new(&map->x_ob,&map->x_ob.ob_pd, &s_float, gensym("in_max"));
	inlet_new(&map->x_ob,&map->x_ob.ob_pd, &s_float, gensym("out_min"));
	inlet_new(&map->x_ob,&map->x_ob.ob_pd, &s_float, gensym("out_max"));
	
	return (void *)map;
}

// Realease an instance of a Pd 'map' object.
void map_free(t_map *map) {
	post("map: free.");
	if (map) {
		outlet_free(map->maped_outlet);
	
		map->maped_outlet = NULL;

	}
}

// Initialization of the Pd 'map' external.
void map_setup(void) {
    post("map: setup.");

    map_class = class_new(gensym("map"),		// t_symbol *name
				(t_newmethod)map_new,	// new method
				(t_method)map_free,		// free method
                sizeof(t_map),		// size
				0,				// int flag
				A_DEFFLOAT, A_DEFFLOAT,A_DEFFLOAT, A_DEFFLOAT,0);
							

    
    class_addfloat(map_class, map_set_value);
    class_addbang(map_class, map_output_value);
    
    class_addmethod(map_class, (t_method)map_set_in_min, gensym("in_min"),  A_FLOAT, A_NULL );   
    class_addmethod(map_class, (t_method)map_set_in_max, gensym("in_max"),  A_FLOAT, A_NULL ); 
    
    class_addmethod(map_class, (t_method)map_set_out_min, gensym("out_min"),  A_FLOAT, A_NULL ); 
    class_addmethod(map_class, (t_method)map_set_out_max, gensym("out_max"),  A_FLOAT, A_NULL ); 
}

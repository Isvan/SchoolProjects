#include <stdio.h>
#include "sphere.h"
#include <stdlib.h>
#include <math.h>



/**********************************************************************
 * This function intersects a ray with a given sphere 'sph'. You should
 * use the parametric representation of a line and do the intersection.
 * The function should return the parameter value for the intersection, 
 * which will be compared with others to determine which intersection
 * is closest. The value -1.0 is returned if there is no intersection
 *
 * If there is an intersection, the point of intersection should be
 * stored in the "hit" variable
 *
 *
 * http://www.ahinson.com/algorithms_general/Sections/Geometry/IntersectionOfParametricLineAndSphere.pdf
 *
 **********************************************************************/
float intersect_sphere(Point o, Vector u, Spheres *sph, Point *hit) {

	Vector ray = u;

	normalize(&ray);

	Point center = sph->center;
	float radius = sph->radius;

	float a = powf(vec_len(ray), 2);
	Vector diff = get_vec(center, o);

	float b = 2 * vec_dot(ray, diff);

	float c = powf(vec_len(diff), 2) - powf(radius, 2);

//	a = ray.x*ray.x + ray.y*ray.y + ray.z*ray.z;
//	b = 2 * (ray.x*(o.x - center.x) + ray.y*(o.y - center.y) + ray.z*(o.z - center.z));
//	c = powf(o.x - center.x,2) + powf(o.y - center.y,2) + powf(o.z - center.z,2) - powf(radius,2);

	float innerOp = powf(b, 2) - 4 * a * c;

	if (innerOp >= 0) {

		float d1 = -1.0 * b + sqrt(innerOp);
		float d2 = -1.0 * b - sqrt(innerOp);
		d1 = d1 / (2.0 * a);
		d2 = d2 / (2.0 * a);
		float d = d2;

//		if (d1 < d2 && d1 > 0.0) {
//			d = d1;
//		} else {
//			d = d2;
//		}

		Vector scaledVec = vec_scale(ray, d);

		hit->x = o.x + scaledVec.x;
		hit->y = o.y + scaledVec.y;
		hit->z = o.z + scaledVec.z;

		return d;
	} else {
		return -1.0;
	}

}

float hitFloor(Point o, Vector u, Point *hit, Spheres * plane) {

	float planeWidth = 5.0;
	float planeHeight = 5.0;
	Point planeCenter;
	planeCenter.x = 2.5;
	planeCenter.y = -5.5;
	planeCenter.z = -3.2;

	Vector planeVector;
	planeVector.x = 0;
	planeVector.y = -1.00;
	planeVector.z = 0;

	normalize(&u);
	normalize(&planeVector);

	float dotUNorm = vec_dot(planeVector, u);

	if (dotUNorm < 0.0000000001) {
		//Vectors say we cant hit each other
		//printf("Dot of the Normal and the ray : %f \n",dotUNorm);
		return -1.0;
	}

	Point planeEq;
	planeEq.x = planeCenter.x * planeVector.x;
	planeEq.y = planeCenter.y * planeVector.y;
	planeEq.z = planeCenter.z * planeVector.z;
	float planeD = 0.0f;

	Vector W = get_vec(o, planeCenter);
	float SI = -1.0 * vec_dot(planeVector, W) / dotUNorm;

	Point hitPlace = get_point(planeCenter, vec_plus(vec_scale(u, SI), W));

	hit->x = hitPlace.x;
	hit->y = hitPlace.y;
	hit->z = hitPlace.z;

	float width = vec_len(get_vec(hitPlace, planeCenter));


	float xDistance = (planeCenter.x - hitPlace.x);
	float zDistance = (planeCenter.z - hitPlace.z);


	//printf("Hit location of plane %f %f %f \n",hitPlace.x,hitPlace.y,hitPlace.z);

	if (xDistance * xDistance > 10) {
		return -1.0;
	}


	if (zDistance * zDistance > 10) {
		return -1.0;
	}

	int zDisMod = (zDistance * 1.2) - 5;
	int xDisMod = (xDistance * 1.2) - 5;

	bool zDis = zDisMod % 2 == 0;
	bool xDis = xDisMod % 2 == 0;

	if (zDis ^ xDis) {
		plane->mat_diffuse[0] = 1;
		plane->mat_diffuse[1] = 1;
		plane->mat_diffuse[2] = 1;

	} else {
		plane->mat_diffuse[0] = 0;
		plane->mat_diffuse[1] = 0;
		plane->mat_diffuse[2] = 0;

	}



	float dist = vec_len(get_vec(hitPlace, o));

	return dist;
}

/*********************************************************************
 * This function returns a pointer to the sphere object that the
 * ray intersects first; NULL if no intersection. You should decide
 * which arguments to use for the function. For exmaple, note that you
 * should return the point of intersection to the calling function.
 **********************************************************************/
Spheres *intersect_scene(Point origin, Vector ray, Spheres *scence, Point * hit,
		int originPoint) {

	Spheres *tempSph = scence;
	//Some big number
	float tempDistance = 10000000.0;
	Point tempHit;

	Spheres *hitSphere = NULL;

	float dist = 0;
	while (tempSph != NULL) {

		if (tempSph->index == originPoint) {
			tempSph = tempSph->next;
			continue;
		}

		if (tempSph->type == 0) {
			dist = intersect_sphere(origin, ray, tempSph, &tempHit);

			if (dist < tempDistance && dist >= 0) {

				tempDistance = dist;
				*hit = tempHit;
				hitSphere = tempSph;
			}
		} else {
			dist = hitFloor(origin, ray, &tempHit, tempSph);

			if (dist < tempDistance && dist != -1.0) {
				*hit = tempHit;
				//printf("Dist to hit the floor %f \n", dist);
				//Use the first sphere and steal its mats for now
				hitSphere = tempSph;

			}
		}

		tempSph = tempSph->next;
	}

	return hitSphere;
}

/*****************************************************
 * This function adds a sphere into the sphere list
 *
 * You need not change this.
 *****************************************************/
Spheres *add_sphere(Spheres *slist, Point ctr, float rad, float amb[],
		float dif[], float spe[], float shine, float refl, int sindex,
		int type) {
	Spheres *new_sphere;

	new_sphere = (Spheres *) malloc(sizeof(Spheres));
	new_sphere->index = sindex;
	new_sphere->center = ctr;
	new_sphere->radius = rad;
	new_sphere->type = type;
	(new_sphere->mat_ambient)[0] = amb[0];
	(new_sphere->mat_ambient)[1] = amb[1];
	(new_sphere->mat_ambient)[2] = amb[2];
	(new_sphere->mat_diffuse)[0] = dif[0];
	(new_sphere->mat_diffuse)[1] = dif[1];
	(new_sphere->mat_diffuse)[2] = dif[2];
	(new_sphere->mat_specular)[0] = spe[0];
	(new_sphere->mat_specular)[1] = spe[1];
	(new_sphere->mat_specular)[2] = spe[2];
	new_sphere->mat_shineness = shine;
	new_sphere->reflectance = refl;
	new_sphere->next = NULL;

	if (slist == NULL) { // first object
		slist = new_sphere;
	} else { // insert at the beginning
		new_sphere->next = slist;
		slist = new_sphere;
	}

	return slist;
}

/******************************************
 * computes a sphere normal - done for you
 ******************************************/
Vector sphere_normal(Point q, Spheres *sph) {
	Vector rc;

	if (sph->type == 1) {
		Vector planeVector;
		planeVector.x = 0;
		planeVector.y = 1.00;
		planeVector.z = 0;
		normalize(&planeVector);
		return planeVector;
	}

	rc = get_vec(sph->center, q);
	normalize(&rc);
	return rc;
}

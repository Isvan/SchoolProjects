#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "global.h"
#include "sphere.h"

//
// Global variables
//
extern int win_width;
extern int win_height;

extern GLfloat frame[WIN_HEIGHT][WIN_WIDTH][3];

extern float image_width;
extern float image_height;

extern Point eye_pos;
extern float image_plane;
extern RGB_float background_clr;
extern RGB_float null_clr;

extern Spheres *scene;

// light 1 position and color
extern Point light1;
extern float light1_ambient[3];
extern float light1_diffuse[3];
extern float light1_specular[3];

// global ambient term
extern float global_ambient[3];

// light decay parameters
extern float decay_a;
extern float decay_b;
extern float decay_c;

extern int shadow_on;
extern int step_max;
extern int reflec_on;
extern int refrag_on;
extern int diff_ext_on;
extern int superSamp_on;

/////////////////////////////////////////////////////////////////////

/*********************************************************************
 * Phong illumination - you need to implement this!
 *********************************************************************/
RGB_float phong(Point q, Vector v, Vector surf_norm, Spheres *sph) {

	RGB_float color;

	Vector rayToLight = get_vec(q, light1);

	float distanceToLight = vec_len(rayToLight);

	float lightDecay = 1
			/ (decay_a + decay_b * distanceToLight
					+ decay_c * distanceToLight * distanceToLight);

	normalize(&rayToLight);
	normalize(&v);
	float diffDot = vec_dot(surf_norm, rayToLight);

	Vector refVec = vec_minus(
			vec_scale(surf_norm, 2 * vec_dot(surf_norm, rayToLight)),
			rayToLight);

	float specDot = vec_dot(refVec, v);

	if (diffDot < 0.1) {
		specDot = 0;

		if (diffDot < 0) {
			diffDot = 0;
		}
	}

	float diffuseValueR = light1_diffuse[0] * sph->mat_diffuse[0] * diffDot;
	float specularValueR = light1_specular[0] * sph->mat_specular[0]
			* powf(specDot, sph->mat_shineness);

	color.r = light1_ambient[0] * sph->mat_ambient[0]
			+ lightDecay * (diffuseValueR + specularValueR);
	color.r += global_ambient[0];

	//color.r = specularValueR;

	float diffuseValueG = light1_diffuse[1] * sph->mat_diffuse[1] * diffDot;
	float specularValueG = light1_specular[1] * sph->mat_specular[1]
			* powf(specDot, sph->mat_shineness);

	color.g = light1_ambient[1] * sph->mat_ambient[1]
			+ lightDecay * (diffuseValueG + specularValueG);
	color.g += global_ambient[1];

	//color.g = specularValueG;

	float diffuseValueB = light1_diffuse[2] * sph->mat_diffuse[2] * diffDot;
	float specularValueB = light1_specular[2] * sph->mat_specular[2]
			* powf(specDot, sph->mat_shineness);

	color.b = light1_ambient[2] * sph->mat_ambient[2]
			+ lightDecay * (diffuseValueB + specularValueB);
	color.b += global_ambient[2];

	//color.b = specularValueB;

//	color.r = surf_norm.x;
//	color.g = surf_norm.y;
//	color.b = surf_norm.z;
//

//	printf("R : %f \n",color.r);
//	printf("G : %f \n",color.g);
//	printf("B : %f \n",color.b);

	return color;
}

/************************************************************************
 * This is the recursive ray tracer - you need to implement this!
 * You should decide what arguments to use.
 ************************************************************************/
RGB_float recursive_ray_trace(int steps, Point pixel_pos, Vector ray,
		Spheres *list, int originPoint, Point * hitPoint) {

	RGB_float color = background_clr;
//	color.r = 1.0;
//	color.g = 1.0;
//	color.b = 1.0;

	Spheres *start = list;
	Point hitLoc;
	Spheres *sph = intersect_scene(pixel_pos, ray, list, &hitLoc, originPoint);

	hitPoint = &hitLoc;
	list = start;

	if (sph != NULL) {

		Vector norm = sphere_normal(hitLoc, sph);

		if (shadow_on == 1) {
			Point lightCp = light1;
			Vector toLight = get_vec(hitLoc, light1);
			Point newHit;
			Point startinPoint = hitLoc;
			Spheres *hit = intersect_scene(startinPoint, toLight, list, &newHit,
					sph->index);

			if (hit == NULL) {

				color = phong(hitLoc, ray, norm, sph);

			} else {

				//Make sure we didnt hit ourselfs

				color.r = global_ambient[0];
				color.g = global_ambient[1];
				color.b = global_ambient[2];

			}

		} else {
			color = phong(hitLoc, ray, norm, sph);
		}

		if (steps < step_max) {
			if (reflec_on) {

				Vector refVec = vec_minus(
						vec_scale(norm, 2 * vec_dot(norm, ray)), ray);

				refVec = vec_scale(refVec, -1.0);
				RGB_float reflect = recursive_ray_trace(steps + 1, hitLoc,
						refVec, list, 0, NULL);

				//printf("Reflected light %f %f %f \n",reflect.r,reflect.g,reflect.b);
				//printf("Original Light %f %f %f \n",color.r,color.g,color.b);

				color = clr_add(color, clr_scale(reflect, sph->reflectance));
				//color = clr_add(color,reflect);
				//printf("New Light %f %f %f \n",color.r,color.g,color.b);

			}

			if (diff_ext_on == 1) {
				int hits = 0;
				RGB_float diffCol;
				for (int i = 0; i < DIFF_RAYS; i++) {

					bool good = false;

					Vector diffRefRay;

					while (good != true) {

						float randX = cosf(rand());
						float randY = cosf(rand());
						float randZ = cosf(rand());

						diffRefRay.x = randX;
						diffRefRay.y = randY;
						diffRefRay.z = randZ;

						if (vec_dot(diffRefRay, norm) < 0) {
							good = false;
						} else {
							good = true;
						}
					}

					Point hitPlace;
					//printf("Before the ref step # %d \n", steps);
					RGB_float diffColTemp = recursive_ray_trace(steps + 1,
							hitLoc, diffRefRay, list, sph->index, &hitPlace);
					//printf("After the ref \n");

//					Spheres *hit = intersect_scene(hitLoc, diffRefRay, list,
//							&hitPlace, sph->index);

					float distance = vec_len(get_vec(hitPlace, hitLoc));

					float lightDecay = 1
							/ (decay_a + decay_b * distance
									+ decay_c * distance * distance);
					//	lightDecay = 1.0/(distance);

//					printf("Light Decay on Diff Bouce : %f \n",lightDecay);

					if (diffColTemp.r == background_clr.r
							&& diffColTemp.g == background_clr.g
							&& diffColTemp.b == background_clr.b) {

					} else {
						diffColTemp = clr_scale(diffColTemp, lightDecay);
						diffCol = clr_add(diffCol, diffColTemp);

					}

					//printf("Ligh Decay Value : %f \n",lightDecay);

				}

//				printf("Color values before : %f %f %f \n", color.r, color.g,
//						color.b);
				diffCol = clr_scale(diffCol, 1.0 / DIFF_RAYS);
				color = clr_add(color, diffCol);
				//color = clr_scale(color, 1.0 / DIFF_RAYS);

//				printf("Color values after : %f %f %f \n", color.r, color.g,
//						color.b);
			}
		}
	}

	return color;
}

/*********************************************************************
 * This function traverses all the pixels and cast rays. It calls the
 * recursive ray tracer and assign return color to frame
 *
 * You should not need to change it except for the call to the recursive
 * ray tracer. Feel free to change other parts of the function however,
 * if you must.
 *********************************************************************/
void ray_trace() {
	int i, j;
	int totalPixals = win_width * win_height;
	int curPixal = 0;
	float x_grid_size = image_width / float(win_width);
	float y_grid_size = image_height / float(win_height);
	float x_start = -0.5 * image_width;
	float y_start = -0.5 * image_height;
	RGB_float ret_color;
	Point cur_pixel_pos;
	Vector ray;

	// ray is cast through center of pixel
	cur_pixel_pos.x = x_start + 0.5 * x_grid_size;
	cur_pixel_pos.y = y_start + 0.5 * y_grid_size;
	cur_pixel_pos.z = image_plane;

	for (i = 0; i < win_height; i++) {
		for (j = 0; j < win_width; j++) {
			ray = get_vec(eye_pos, cur_pixel_pos);

			//Cur pixal gives us the center of the pixals
			//We want the four corners so lets just offset

			if (superSamp_on == 1) {

				float offSet = 0.25;

				Point UL;
				UL.y = cur_pixel_pos.y + offSet * y_grid_size;
				UL.x = cur_pixel_pos.x - offSet * x_grid_size;
				UL.z = cur_pixel_pos.z;

				Point UR;
				UR.y = cur_pixel_pos.y + offSet * y_grid_size;
				UR.x = cur_pixel_pos.x + offSet * x_grid_size;
				UR.z = cur_pixel_pos.z;

				Point LL;
				LL.y = cur_pixel_pos.y - offSet * y_grid_size;
				LL.x = cur_pixel_pos.x - offSet * x_grid_size;
				LL.z = cur_pixel_pos.z;

				Point LR;
				LR.y = cur_pixel_pos.y - offSet * y_grid_size;
				LR.x = cur_pixel_pos.x + offSet * x_grid_size;
				LR.z = cur_pixel_pos.z;

				ret_color = recursive_ray_trace(0, eye_pos, ray, scene, 0,
				NULL);

				//	printf("Color 1 %f %f %f \n",ret_color.r,ret_color.g,ret_color.b);

				RGB_float ULCol = recursive_ray_trace(0, eye_pos,
						get_vec(eye_pos, UL), scene, 0, NULL);

				RGB_float URCol = recursive_ray_trace(0, eye_pos,
						get_vec(eye_pos, UR), scene, 0, NULL);

				RGB_float LLCol = recursive_ray_trace(0, eye_pos,
						get_vec(eye_pos, LL), scene, 0, NULL);

				RGB_float LRCol = recursive_ray_trace(0, eye_pos,
						get_vec(eye_pos, LR), scene, 0, NULL);

				ret_color.r += ULCol.r + URCol.r + LLCol.r + LRCol.r;
				ret_color.g += ULCol.g + URCol.g + LLCol.g + LRCol.g;
				ret_color.b += ULCol.b + URCol.b + LLCol.b + LRCol.b;

//				ret_color = clr_add(ret_color,ULCol);

				//	printf("Color End %f %f %f \n",ret_color.r,ret_color.g,ret_color.b);

			} else {

				ret_color = recursive_ray_trace(0, eye_pos, ray, scene, 0,
				NULL);

			}

			// Parallel rays can be cast instead using below

			// ret_color = recursive_ray_trace(cur_pixel_pos, ray, 1);

			frame[i][j][0] = GLfloat(ret_color.r);
			frame[i][j][1] = GLfloat(ret_color.g);
			frame[i][j][2] = GLfloat(ret_color.b);

			cur_pixel_pos.x += x_grid_size;
			curPixal += 1;

		}

		cur_pixel_pos.y += y_grid_size;
		cur_pixel_pos.x = x_start;

		float done = (100.0 * curPixal / totalPixals);
		if ((int)floorf(done) % 10 == 0) {
			printf("Render is %f done \n", done);
		}

	}

	Spheres *tempS = scene;

	while (tempS != NULL) {

		printf("Sphere Location : # %d, X %f Y %f Z %f , r : %f \n",
				tempS->index, tempS->center.x, tempS->center.y, tempS->center.z,
				tempS->radius);
		tempS = tempS->next;
	}

}

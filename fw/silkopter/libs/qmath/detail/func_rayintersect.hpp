namespace math
{



template<class T>
int quadraticEquation(T a, T b, T c, T& x1, T& x2)
{
	T q = b*b - 4*a*c; 
	if( q >= 0.0f )
	{
		T sq = sqrt(q);
		T d = 1.0f / (2.0f*a);
		x1 = ( -b + sq ) * d;
		x2 = ( -b - sq ) * d;

		return ( abs(x1-x2) < (T)1e-06 ) ? 1 : 2; // real roots
	}
	else
	{
		return 0;//complex roots
	}
}

template<class T>
bool ray_intersect_plane(vec3<T> const& origin, vec3<T> const& dir, plane<T> const& plane, T& t)
{
	// Solution: t = -(p dot N + d) / (N dot dir)
	T denum = dot(plane.normal, dir);

	// parallel to plane
	// if (denum!=0.0f) // cos 0 = 90
	if (abs(denum) < (T)1e-6) // cos 0 = 90
	{
		return false;
	}

	T num = plane.get_distance_to(origin);
	t = -num/denum;
	return true;
}

template<class T>
int ray_intersect_aabb(vec3<T> const& rayOrigin, vec3<T> const& rayDir, vec3<T> const& boxCenter, vec3<T> const& boxHalfsize, T& tmin, T& tmax)
{
	//http://www.cs.utah.edu/~awilliam/box/
	T tymin, tymax, tzmin, tzmax;
	vec3<T> bounds[2] = {boxCenter-boxHalfsize, boxCenter+boxHalfsize};
	int sign[3];
	vec3<T> inv_direction;
	inv_direction.x = 1.0f/rayDir.x;
	inv_direction.y = 1.0f/rayDir.y;
	inv_direction.z = 1.0f/rayDir.z;
	sign[0] = (inv_direction.x < 0);
	sign[1] = (inv_direction.y < 0);
	sign[2] = (inv_direction.z < 0);

	tmin = (bounds[sign[0]].x - rayOrigin.x) * inv_direction.x;
	tmax = (bounds[1-sign[0]].x - rayOrigin.x) * inv_direction.x;

	tymin = (bounds[sign[1]].y - rayOrigin.y) * inv_direction.y;
	tymax = (bounds[1-sign[1]].y - rayOrigin.y) * inv_direction.y;
	if ( (tmin > tymax) || (tymin > tmax) )
	{
		return 0;
	}
	if (tymin > tmin)
	{
		tmin = tymin;
	}
	if (tymax < tmax)
	{
		tmax = tymax;
	}
	tzmin = (bounds[sign[2]].z - rayOrigin.z) * inv_direction.z;
	tzmax = (bounds[1-sign[2]].z - rayOrigin.z) * inv_direction.z;
	if ( (tmin > tzmax) || (tzmin > tmax) )
	{
		return 0;
	}
	if (tzmin > tmin)
	{
		tmin = tzmin;
	}
	if (tzmax < tmax)
	{
		tmax = tzmax;
	}
	return 2;
}

template<class T>
int ray_intersect_sphere(vec3<T> const& rayOrigin, vec3<T> const& rayDirection, vec3<T> const& sphereCenter, T radius, T& t0, T& t1)
{
	// christer ericson code
	vec3<T> m = rayOrigin - sphereCenter;
	T b = dot(m,rayDirection);
	T c = dot(m,m) - radius*radius;

	// exit if ray origin outside sphere ( c > 0) and r pointing away from sphere (b > 0)
	// if (c > 0.0f && b > 0.0f) return 0;

	T discr = b*b - c;
	if (discr < 0.0f)
	{
		return 0;
	}

	T sqdiscr = sqrt(discr);
	t0 = -b - sqdiscr;
	t1 = -b + sqdiscr;
	return 2;
}

template<class T>
int ray_intersect_hemisphere(vec3<T> const& rayOrigin, vec3<T> const& rayDirection, vec3<T> const& sphereCenter, vec3<T> const& sphereNormal, T radius, T& t0, T& t1)
{
	// christer ericson code
	vec3<T> m = rayOrigin - sphereCenter;
	T b = dot(m,rayDirection);
	T c = dot(m,m) - radius*radius;

	T discr = b*b - c;
	if (discr < 0.0f)
	{
		return 0;
	}

	T sqdiscr = sqrt(discr);
	int ret = 0;
	t0 = -b - sqdiscr;
	t1 = -b + sqdiscr;
	if (dot(m+rayDirection*t0,sphereNormal)>0.0f)
	{
		ret++;
	}
	if (dot(m+rayDirection*t1,sphereNormal)>0.0f)
	{
		if (ret==0) t0 = t1;
		ret++;
	}
	return ret;
}

template<class T>
bool ray_intersect_disk(
	vec3<T> const& rayOrigin, vec3<T> const& rayDir, 
	vec3<T> const& circleCenter, vec3<T> const& circleNormal, T circleRadius,
	T& t)
{
	plane<T> plane(circleNormal,circleCenter);
	T tt;
	if (!ray_intersect_plane(rayOrigin,rayDir,plane,tt))
	{
		return false;
	}

	vec3<T> dist = circleCenter - (rayOrigin + rayDir * tt);

	if (length_sq(dist) < circleRadius*circleRadius)
	{
		t = tt;
		return true;
	}

	return false;
}

template<class T>
int ray_intersect_cylinder(
						 vec3<T> const& p, vec3<T> const& d,
						 T length, T radius,
						 T* t, bool intersectDisks)
{
	T hh = length/2;
	T r2 = radius*radius;
	// check if ray is parallel to cylinder axis
	if (abs(d.z) >= (T)0.999f)
	{
		if (!intersectDisks)
		{
			return 0;
		}
		// The line is parallel to the cylinder axis.  Determine if the line
		// intersects the cylinder end disks.

		T c = r2 - p.x*p.x - p.y*p.y;
		if (c < 0.0f)
		{
			// line outside the cylinder, no intersection
			return 0;
		}
		// line intersects the cylinder end disks
		if ( d.z > 0.0f )
		{
			t[0] = -p.z - hh;
			t[1] = -p.z + hh;
		}
		else
		{
			t[0] = p.z - hh;
			t[1] = p.z + hh;
		}
		return 2;
	}
	// check cylinder walls
	// x^2 + y^2 = r^2
	// (Px+t*Dx)^2 + (Py+t*Dy)^2 = r^2
	// (Px^2 + t^2Dx^2 + 2*Px*t*Dx) + ... = r^2
	// t2*(Dx^2 + Dy^2) + t*(2*Px*Dx + 2*Py*Dy) + Px^2 + Py^2 - r^2 = 0;

	T a = d.x*d.x + d.y*d.y;
	T b = 2.0f*(p.x*d.x + p.y*d.y);
	T c = (p.x*p.x + p.y*p.y) - r2;
	T t0,t1;
	int ret = quadraticEquation(a,b,c,t0,t1);
	if (ret==0)
	{
		return 0;
	}
	// check z bounds
	T z0 = p.z + d.z*t0;
	T z1 = p.z + d.z*t1;
	ret = 0;
	if (abs(z0)<hh)
	{
		t[ret++] = t0;
	}
	else if (intersectDisks)
	{
		if (ray_intersect_disk(p, d, vec3<T>(0,0,sgn(z0)*hh), vec3<T>(0,0,1), radius, t0))
		{
			t[ret++] = t0;
		}
	}
	if (abs(z1)<hh)
	{
		t[ret++] = t1;
	}
	else if (intersectDisks)
	{
		if (ray_intersect_disk<T>(p,d,vec3<T>(0,0,sgn(z1)*hh), vec3<T>(0,0,1), radius, t1))
		{
			t[ret++] = t1;
		}
	}
	if (ret==2)
	{
		if (t[0]>t[1]) std::swap(t[0],t[1]);
	}
	return ret;
}

template<class T>
int ray_intersect_capsule(vec3<T> const& rayOrigin, vec3<T> const& rayDir, T height, T radius, T& t0, T& t1)
{
	// test caps first
	vec3<T> capCenter0(0,0,height/2);
	vec3<T> capCenter1(0,0,-height/2);
	T ts[4];
	int ret0 = ray_intersect_hemisphere(rayOrigin,rayDir,capCenter0,vec3<T>(0,0, 1),radius,ts[0],ts[1]);
	int ret1 = ray_intersect_hemisphere(rayOrigin,rayDir,capCenter1,vec3<T>(0,0,-1),radius,ts[2],ts[3]);
	if (ret0>0 && ret1>0)
	{
		//ray intersects both caps
		t0 = abs(ts[0]) > abs(ts[1]) ? ts[0] : ts[1];
		t1 = abs(ts[2]) > abs(ts[3]) ? ts[2] : ts[3];
		return 2;
	}
	// test walls
	T twall[2];
	int ret2 = ray_intersect_cylinder(rayOrigin,rayDir,height,radius,twall,false);
	if (ret2==0)// ray does not collide with the walls
	{
		// check cap collisions
		if (ret0>0)
		{
			t0 = ts[0];
			t1 = ts[1];
			return ret0;
		}
		else if (ret1>0)
		{
			t0 = ts[2];
			t1 = ts[3];
			return ret1;
		}
		return 0;
	}
	else if (ret2>1)
	{
		// ray crosses the cylinder wall
		t0 = twall[0];
		t1 = twall[1];
		return 2;
	}

	// if there is one collision point with the wall, it could have one collision point with the caps
	//PASSERT(ret2==1);
	if (ret0+ret1==0)
	{
		t0 = twall[0];
		return 1;
	}
	t0 = (ret0>0) ? ts[0] : ts[2];
	t1 = twall[0];
	if (t0>t1)
	{
		std::swap(t0,t1);
	}

	return 2;
}


}
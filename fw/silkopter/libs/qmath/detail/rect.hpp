namespace math
{


///////////////////////////////////////////////////////////////////////////////
// constructors
///////////////////////////////////////////////////////////////////////////////

template <typename T>
rect<T>::rect() : upperLeftCorner (0, 0) , lowerRightCorner(0, 0)
{
}

template <typename T>
rect<T>::rect(T x, T y, T x2, T y2) : upperLeftCorner(x, y), lowerRightCorner(x2, y2)
{
}

template <typename T>
rect<T>::rect(const vec2<T>& upperLeft, const vec2<T>& lowerRight) : upperLeftCorner(upperLeft), lowerRightCorner(lowerRight)
{
}

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

template <typename T>
void rect<T>::set(T x, T y, T x2, T y2)
{
	upperLeftCorner = vec2<T>(x, y);
	lowerRightCorner = vec2<T>(x2, y2);
}

template <typename T>
void rect<T>::set(const vec2<T>& upperLeft, const vec2<T>& lowerRight)
{
	upperLeftCorner = upperLeft;
	lowerRightCorner = lowerRight;
}

template <typename T>
void rect<T>::scale(T scale)
{
	upperLeftCorner  *= scale;
	lowerRightCorner *= scale;
}

template <typename T>
T rect<T>::get_area() const
{
return get_width() * get_height();
}

template <typename T>
T rect<T>::get_width() const
{
	return lowerRightCorner.x - upperLeftCorner.x;
}

template <typename T>
T rect<T>::get_height() const
{
	return lowerRightCorner.y - upperLeftCorner.y;
}

template <typename T>
void rect<T>::repair()
{
	if (lowerRightCorner.x < upperLeftCorner.x)
	{
		T t = lowerRightCorner.x;
		lowerRightCorner.x = upperLeftCorner.x;
		upperLeftCorner.x = t;
	}

	if (lowerRightCorner.y < upperLeftCorner.y)
	{
		T t = lowerRightCorner.y;
		lowerRightCorner.y = upperLeftCorner.y;
		upperLeftCorner.y = t;
	}
}

template <typename T>
bool rect<T>::is_valid() const
{
	return ((lowerRightCorner.x >= upperLeftCorner.x)  && (lowerRightCorner.y >= upperLeftCorner.y));
}

template <typename T>
vec2<T> rect<T>::get_center() const
{
	return vec2<T>((upperLeftCorner.x + lowerRightCorner.x) / 2,
		(upperLeftCorner.y + lowerRightCorner.y) / 2);
}

template <typename T>
vec2<T> rect<T>::get_size() const
{
	return vec2<T>(get_width(), get_height());
}


///////////////////////////////////////////////////////////////////////////////
// containment methods
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool rect<T>::is_point_inside(const vec2<T>& pos) const
{
	return (
		upperLeftCorner.x  <= pos.x && upperLeftCorner.y  <= pos.y &&
		lowerRightCorner.x >= pos.x && lowerRightCorner.y >= pos.y
		);
}

template <typename T>
bool rect<T>::is_rect_collided(const rect<T>& other) const
{
	return (
		lowerRightCorner.y > other.upperLeftCorner.y && upperLeftCorner.y  < other.lowerRightCorner.y  &&
		lowerRightCorner.x > other.upperLeftCorner.x && upperLeftCorner.x  < other.lowerRightCorner.x
		);
}

template <typename T>
void rect<T>::clip_against(const rect<T>& other)
{
	if (other.lowerRightCorner.x < lowerRightCorner.x)
		lowerRightCorner.x = other.lowerRightCorner.x;
	if (other.lowerRightCorner.y < lowerRightCorner.y)
		lowerRightCorner.y = other.lowerRightCorner.y;

	if (other.upperLeftCorner.x > upperLeftCorner.x)
		upperLeftCorner.x = other.upperLeftCorner.x;
	if (other.upperLeftCorner.y > upperLeftCorner.y)
		upperLeftCorner.y = other.upperLeftCorner.y;

	// correct possible invalid rect resulting from clipping
	if (upperLeftCorner.y > lowerRightCorner.y)
		upperLeftCorner.y = lowerRightCorner.y;
	if (upperLeftCorner.x > lowerRightCorner.x)
		upperLeftCorner.x = lowerRightCorner.x;
}

template <typename T>
bool rect<T>::constrain_to(const rect<T>& other)
{
	if (other.get_width() < get_width()  ||  other.get_height() < get_height())
		return false;

	T diff = other.lowerRightCorner.x - lowerRightCorner.x;
	if (diff < 0)
	{
		lowerRightCorner.x += diff;
		upperLeftCorner.x  += diff;
	}

	diff = other.lowerRightCorner.y - lowerRightCorner.y;
	if (diff < 0)
	{
		lowerRightCorner.y += diff;
		upperLeftCorner.y  += diff;
	}

	diff = upperLeftCorner.x - other.upperLeftCorner.x;
	if (diff < 0)
	{
		upperLeftCorner.x  -= diff;
		lowerRightCorner.x -= diff;
	}

	diff = upperLeftCorner.y - other.upperLeftCorner.y;
	if (diff < 0)
	{
		upperLeftCorner.y  -= diff;
		lowerRightCorner.y -= diff;
	}

	return true;
}

template <typename T>
vec2<T> rect<T>::get_closest_point(const vec2<T>& pt) const
{
	vec2<T> result(clamp(pt, upperLeftCorner, lowerRightCorner));
	return result;
}

template <typename T>
void rect<T>::grow(const T& size)
{
	grow(vec2<T>(size));
}

template <typename T>
void rect<T>::grow(const vec2<T>& size)
{
	upperLeftCorner -= size;
	lowerRightCorner += size;
}

template <typename T>
void rect<T>::clamp_to_min(const T& size)
{
	clamp_to_min(vec2<T>(size));
}

template <typename T>
void rect<T>::clamp_to_min(const vec2<T>& minSize)
{
	T curWidth = get_width();
	if (curWidth < minSize.x)
	{
		T widthToGrow = (minSize.x - curWidth) * (T)(0.5);
		upperLeftCorner.x -= widthToGrow;
		lowerRightCorner.x += widthToGrow;
	}

	T curHeight = get_height();
	if (curHeight < minSize.y)
	{
		T heightToGrow = (minSize.y - curHeight) * (T)(0.5);
		upperLeftCorner.y -= heightToGrow;
		lowerRightCorner.y += heightToGrow;
	}
}

template <typename T>
void rect<T>::clamp_to_max(const T& size)
{
	ClampToMax(vec2<T>(size));
}

template <typename T>
void rect<T>::clamp_to_max(const vec2<T>& maxSize)
{
	assert(maxSize.x >= 0.0f && maxSize.y >= 0.0f);

	T curWidth = get_width();
	if (curWidth > maxSize.x)
	{
		T widthToShrink = (curWidth - maxSize.x) * (T)(0.5);
		upperLeftCorner.x += widthToShrink;
		lowerRightCorner.x -= widthToShrink;
	}

	T curHeight = get_height();
	if (curHeight > maxSize.y)
	{
		T heightToShrink = (curHeight - maxSize.y) * (T)(0.5);
		upperLeftCorner.y += heightToShrink;
		lowerRightCorner.y -= heightToShrink;
	}
}

template <typename T>
void rect<T>::add_internal_point(const vec2<T>& p)
{
	add_internal_point(p.x, p.y);
}

template <typename T>
void rect<T>::add_internal_point(T x, T y)
{
	if (x>lowerRightCorner.x)
		lowerRightCorner.x = x;
	if (y>lowerRightCorner.y)
		lowerRightCorner.y = y;

	if (x<upperLeftCorner.x)
		upperLeftCorner.x = x;
	if (y<upperLeftCorner.y)
		upperLeftCorner.y = y;
}

///////////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
rect<T> rect<T>::operator+(const vec2<T>& pos) const
{
	rect<T> ret(*this);
	return ret+=pos;
}

template <typename T>
rect<T>& rect<T>::operator+=(const vec2<T>& pos)
{
	upperLeftCorner  += pos;
	lowerRightCorner += pos;
	return *this;
}

template <typename T>
rect<T> rect<T>::operator-(const vec2<T>& pos) const
{
	rect<T> ret(*this);
	return ret-=pos;
}

template <typename T>
rect<T> rect<T>::operator*(const T& scale) const
{
	rect<T> ret(upperLeftCorner*scale, lowerRightCorner*scale);
	return ret;
}

template <typename T>
rect<T> rect<T>::operator/(const T& scale) const
{
	rect<T> ret(upperLeftCorner/scale, lowerRightCorner/scale);
	return ret;
}

template <typename T>
rect<T>& rect<T>::operator-=(const vec2<T>& pos)
{
	upperLeftCorner  -= pos;
	lowerRightCorner -= pos;
	return *this;
}

template <typename T>
bool rect<T>::operator==(const rect<T>& other) const
{
	return (upperLeftCorner  == other.upperLeftCorner  &&
		lowerRightCorner == other.lowerRightCorner);
}

template <typename T>
bool rect<T>::operator!=(const rect<T>& other) const
{
	return (upperLeftCorner  != other.upperLeftCorner  ||
		lowerRightCorner != other.lowerRightCorner);
}

template <typename T>
rect<T>& rect<T>::operator+=(const rect<T>& other)
{
	add_internal_point(other.upperLeftCorner);
	add_internal_point(other.lowerRightCorner);
	return *this;
}

template <typename T>
bool rect<T>::operator<(const rect<T>& other) const
{
	return get_area() < other.getArea();
}

}
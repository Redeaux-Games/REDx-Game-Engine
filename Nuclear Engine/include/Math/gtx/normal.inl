/// @ref gtx_normal
/// @file glm/gtx/normal.inl

namespace Math
{
	template<typename T, qualifier Q> 
	GLM_FUNC_QUALIFIER vec<3, T, Q> triangleNormal
	(
		vec<3, T, Q> const& p1, 
		vec<3, T, Q> const& p2, 
		vec<3, T, Q> const& p3
	)
	{
		return Normalize(Cross(p1 - p2, p1 - p3));
	}
}//namespace glm
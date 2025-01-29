// This file is part of the supplementary material for the paper
// "GPU-Centered Font Rendering Directly from Glyph Outlines",
// by Eric Lengyel. It is licensed under the Creative Commons
// BY-NC-ND 4.0 license available here:
//
// https://creativecommons.org/licenses/by-nc-nd/4.0/
//
// Commercial use of this software requires a separate license
// from the author. Copyright and additional IP protections apply.


in vresult
{
	vec4 vcolor;				// Vertex color.
	vec2 texcoord;				// Em-space glyph coordinates for pixel.
	flat uvec2 glyphParam;		// Constant data for current glyph.
	flat vec3 bandParam;		// Scale and offset for band indexes.
};

uniform sampler2DRect curveTex;		// Control point texture.
uniform usampler2DRect bandTex;		// Band data texture.

out vec4 fcolor;

const float kQuadraticEpsilon = 0.0001;

void main()
{
	float coverage = 0.0;

	// The effective pixel dimensions of the em square are computed
	// independently for x and y directions with texcoord derivatives.

	vec2 pixelsPerEm = vec2(1.0 / fwidth(texcoord.x),
	                        1.0 / fwidth(texcoord.y));

	// The low 12 bits of each component of glyphParam give the coordinates
	// at which the glyph data begins in the index texture. Bits 12-15 give
	// one less than the number of bands in the x and y directions.

	uvec2 glyphLoc = glyphParam & 0x0FFFU;
	uvec2 bandMax = glyphParam >> 12U;

	// Determine what bands the current pixel lies in by applying a scale
	// and offset to the texture coordinates. The scale given by bandParam.z
	// is the same for both directions, but there are different offsets given
	// by bandParam.xy. Band indexes are clamped to [0, bandMax.xy].

	uvec2 bandIndex = uvec2(clamp(ivec2(texcoord * bandParam.z
	                    + bandParam.xy), ivec2(0U, 0U), ivec2(bandMax)));

	// Fetch data for the horizontal band from the index texture. The number
	// of curves intersecting the band is in the x component, and the offset
	// to the list of locations for those curves is in the y component.

	uvec2 hbandData = texelFetch(bandTex,
	        ivec2(glyphLoc.x + bandIndex.y, glyphLoc.y)).xy;
	uvec2 hbandLoc = uvec2(glyphLoc.x + hbandData.y, glyphLoc.y);

	// If the offset caused the x coordinate to exceed 4096 (texture width),
	// then wrap to the next line.

	hbandLoc.y += hbandLoc.x >> 12U;
	hbandLoc.x &= 0x0FFFU;

	// Loop over all curves in the horizontal band.

	for (uint curve = 0U; curve < hbandData.x; curve++)
	{
		// Fetch the location of the current curve from the index texture.

		ivec2 curveLoc = ivec2(texelFetch(bandTex,
		                   ivec2(hbandLoc.x + curve, hbandLoc.y)).xy);

		// Fetch the three 2D control points for the current curve from the
		// curve texture. The first texel contains both p1 and p2 in the
		// (x,y) and (z,w) components, respectively, and the the second texel
		// contains p3 in the (x,y) components. The quadratic Bézier curve
		// C(t) is given by
		//
		//     C(t) = (1 - t)^2 p1 + 2t(1 - t) p2 + t^2 p3

		vec4 p12 = texelFetch(curveTex, curveLoc) - vec4(texcoord, texcoord);
		vec2 p3 = texelFetch(curveTex, ivec2(curveLoc.x + 1, curveLoc.y)).xy
		            - texcoord;

		// If the largest x coordinate among all three control points falls
		// left of the current pixel, then there are no more curves in the
		// horizontal band that can influence the result, so exit the loop.
		// (The curves are sorted in descending order by max x coordinate.)

		if (max(max(p12.x, p12.z), p3.x) * pixelsPerEm.x < -0.5) break;

		// Generate the root contribution code based on the signs of the
		// y coordinates of the three control points.

		uint code = (0x2E74U >> (((p12.y > 0.0) ? 2U : 0U) +
		        ((p12.w > 0.0) ? 4U : 0U) + ((p3.y > 0.0) ? 8U : 0U))) & 3U;

		if (code != 0U)
		{
			// At least one root makes a contribution, so solve for the
			// values of t where the curve crosses y = 0. The quadratic
			// polynomial in t is given by
			//
			//     a t^2 - 2b t + c,
			//
			// where a = p1.y - 2 p2.y + p3.y, b = p1.y - p2.y, and c = p1.y.
			// The discriminant b^2 - ac is clamped to zero, and imaginary
			// roots are treated as a double root at the global minimum
			// where t = b / a.

			float ax = p12.x - p12.z * 2.0 + p3.x;
			float ay = p12.y - p12.w * 2.0 + p3.y;
			float bx = p12.x - p12.z;
			float by = p12.y - p12.w;
			float ra = 1.0 / ay;

			float d = sqrt(max(by * by - ay * p12.y, 0.0));
			float t1 = (by - d) * ra;
			float t2 = (by + d) * ra;

			// If the polynomial is nearly linear, then solve -2b t + c = 0.

			if (abs(ay) < kQuadraticEpsilon) t1 = t2 = p12.y * 0.5 / by;

			// Calculate the x coordinates where C(t) = 0, and transform
			// them so that the current pixel corresponds to the range
			// [0,1]. Clamp the results and use them for root contributions.

			float x1 = (ax * t1 - bx * 2.0) * t1 + p12.x;
			float x2 = (ax * t2 - bx * 2.0) * t2 + p12.x;
			x1 = clamp(x1 * pixelsPerEm.x + 0.5, 0.0, 1.0);
			x2 = clamp(x2 * pixelsPerEm.x + 0.5, 0.0, 1.0);

			// Bits in code tell which roots make a contribution.

			if ((code & 1U) != 0U) coverage += x1;
			if (code > 1U) coverage -= x2;
		}
	}

	// Fetch data for the vertical band from the index texture. This follows
	// the data for all horizontal bands, so we have to add bandMax.y + 1.

	uvec2 vbandData = texelFetch(bandTex,
	        ivec2(glyphLoc.x + bandMax.y + 1U + bandIndex.x, glyphLoc.y)).xy;
	uvec2 vbandLoc = uvec2(glyphLoc.x + vbandData.y, glyphLoc.y);

	// Wrap to the next line if necessary.

	vbandLoc.y += vbandLoc.x >> 12U;
	vbandLoc.x &= 0x0FFFU;

	// Loop over all curves in the vertical band.

	for (uint curve = 0U; curve < vbandData.x; curve++)
	{
		ivec2 curveLoc = ivec2(texelFetch(bandTex,
		                   ivec2(vbandLoc.x + curve, vbandLoc.y)).xy);

		vec4 p12 = texelFetch(curveTex, curveLoc) - vec4(texcoord, texcoord);
		vec2 p3 = texelFetch(curveTex, ivec2(curveLoc.x + 1, curveLoc.y)).xy
		            - texcoord;

		// If the largest y coordinate among all three control points falls
		// below the current pixel, then exit the loop.

		if (max(max(p12.y, p12.w), p3.y) * pixelsPerEm.y < -0.5) break;

		// Generate the root contribution code based on the signs of the
		// x coordinates of the three control points.

		uint code = (0x2E74U >> (((p12.x > 0.0) ? 2U : 0U) +
		        ((p12.z > 0.0) ? 4U : 0U) + ((p3.x > 0.0) ? 8U : 0U))) & 3U;

		if (code != 0U)
		{
			// At least one root makes a contribution, so solve for the
			// values of t where the rotated curve crosses y = 0.

			float ax = p12.y + p12.w * 2.0 + p3.y;
			float ay = p12.x - p12.z * 2.0 + p3.x;
			float bx = p12.y - p12.w;
			float by = p12.x - p12.z;
			float ra = 1.0 / ay;

			float d = sqrt(max(by * by - ay * p12.x, 0.0));
			float t1 = (by - d) * ra;
			float t2 = (by + d) * ra;

			if (abs(ay) < kQuadraticEpsilon) t1 = t2 = p12.x * 0.5 / by;

			float x1 = (ax * t1 - bx * 2.0) * t1 + p12.y;
			float x2 = (ax * t2 - bx * 2.0) * t2 + p12.y;
			x1 = clamp(x1 * pixelsPerEm.y + 0.5, 0.0, 1.0);
			x2 = clamp(x2 * pixelsPerEm.y + 0.5, 0.0, 1.0);

			if ((code & 1U) != 0U) coverage += x1;
			if (code > 1U) coverage -= x2;
		}
	}

	// Take the average of the horizontal and vertical results. The absolute
	// value ensures that either winding convention works. The square root
	// approximates gamma correction.

	coverage = sqrt(clamp(abs(coverage) * 0.5, 0.0, 1.0));
	float alpha = coverage * vcolor.w;
	fcolor = vec4(vcolor.xyz * alpha, alpha);
}

#include "../headers.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <corecrt_math_defines.h>
namespace FCT { 
	const float Command_End = -1;
	const float Command_MoveTo = 1;
	const float Command_LineTo = 2;
	const float Command_BezierCurveTo = 3;
	const float Command_SetColor = 4;
	const float CommandArcTo = 5.0;
	FCT::VertexRenderScreen::VertexRenderScreen(VertexRenderPipeline* pl) : Object(pl)
	{
		m_pl = pl;
		m_vf = pl->getVertexFactory();
		m_ctx = pl->getContext();
		m_arr = new VertexArray(m_vf, 6);
		size(Vec3(1, 1, 1));
		viewport(0, 0, 1, 1);
	}


	void FCT::VertexRenderScreen::size(Vec3 centerToCorner)
	{
		Vec3 topRight = centerToCorner;
		Vec3 topLeft = Vec3(-centerToCorner.x, centerToCorner.y, centerToCorner.z);
		Vec3 bottomRight = Vec3(centerToCorner.x, -centerToCorner.y, centerToCorner.z);
		Vec3 bottomLeft = Vec3(-centerToCorner.x, -centerToCorner.y, centerToCorner.z);

		m_arr->setPosition(0, topLeft);
		m_arr->setPosition(1, topRight);
		m_arr->setPosition(2, bottomLeft);
		m_arr->setPosition(3, bottomRight);
		m_arr->setPosition(4, topRight);
		m_arr->setPosition(5, bottomLeft);
	}

	void VertexRenderScreen::viewport(float x, float y, float w, float h)
	{
		Vec2 topLeft(x, y);
		Vec2 topRight(x + w, y);
		Vec2 bottomLeft(x, y + h);
		Vec2 bottomRight(x + w, y + h);
		m_arr->setAttribute(0,"vectorCoord", topLeft);
		m_arr->setAttribute(1,"vectorCoord", topRight);
		m_arr->setAttribute(2,"vectorCoord", bottomLeft);
		m_arr->setAttribute(3,"vectorCoord", bottomRight);
		m_arr->setAttribute(4,"vectorCoord", topRight);
		m_arr->setAttribute(5,"vectorCoord", bottomLeft);
	}

	void VertexRenderScreen::create()
	{
		m_il = m_ctx->createInputLayout(m_vf);
		m_vb = m_ctx->createVertexBuffer(m_arr);
		m_vb->create(m_ctx);
		m_il->create(m_ctx, m_vb);
		m_dc = m_ctx->createDrawCall(FCT::PrimitiveType::Triangles, 0, 6);
		addResource(m_il);
		addResource(m_vb);
		addResource(m_dc);
	}

	FCT::VertexRenderPipeline::VertexRenderPipeline(Context* ctx) : Pipeline(ctx, this->createFactory())
	{
		m_vrvs = ctx->createVertexShader(m_defaultFactory);
		if (!m_vrvs->compileFromSource(R"(
VertexOutput main(VertexInput input)
{
	VertexOutput output;
	output.position = input.position;
	output.vectorCoord = input.vectorCoord;
	return output;
})")) {
			std::cout << "vs±¨´í:" << std::endl;
			std::cout << m_vrvs->getCompileError() << std::endl;
		}
		m_vrps = ctx->createPixelShader(m_vrvs->getOutput());
		if (!m_vrps->compileFromSource(R"(
layout(binding = 4) uniform sampler2D commandQueue;

const float CommandEnd = -1.0;
const float CommandMoveTo = 1.0;
const float CommandLineTo = 2.0;
const float CommandBezierCurveTo = 3.0;
const float CommandSetColor = 4.0;
const float CommandArcTo = 5.0;

vec2 LineTest(vec2 v0, vec2 v1, vec2 pixelsPerUnit) {
    vec2 result = vec2(0.0);
    
    if (max(v0.x, v1.x) * pixelsPerUnit.x >= -0.5) {
        int signX = (v0.y > 0.0 ? 1 : 0) - (v1.y > 0.0 ? 1 : 0);
        if (signX != 0) {
            float xt = (v1.y * v0.x - v0.y * v1.x) / (v1.y - v0.y);
            result.x = float(signX) * clamp(xt * pixelsPerUnit.x + 0.5, 0.0, 1.0);
        }
    }
    
    if (max(v0.y, v1.y) * pixelsPerUnit.y >= -0.5) {
        int signY = (v1.x > 0.0 ? 1 : 0) - (v0.x > 0.0 ? 1 : 0);
        if (signY != 0) {
            float yt = (v1.x * v0.y - v0.x * v1.y) / (v1.x - v0.x);
            result.y = float(signY) * clamp(yt * pixelsPerUnit.y + 0.5, 0.0, 1.0);
        }
    }
    
    return result;
}
vec2 ArcTest(vec2 pos, vec2 bp,vec2 ep, vec2 c, float b, float e, float r, float q, vec2 pixelsPerUnit) {
    vec2 ret = vec2(0.0, 0.0);
    float minX = min(bp.x, ep.x);
    float minY = min(bp.y, ep.y);
    float maxX = max(bp.x, ep.x);
    float maxY = max(bp.y, ep.y);
    bool isInx = (pos.x >= minX && pos.x <= maxX);
    bool isIny = (pos.y >= minY && pos.y <= maxY);
    bool isInCircle = length(pos - c) <= r;
    float bLessE = float(b < e);
    
    if (q == 1.0 || q == 3.0) {
        ret.x = float(isIny && (isInCircle || pos.x < minX));
        ret.x *= -bLessE;
    } else {
        ret.x = float(isIny && pos.x < maxX && !isInCircle);
        ret.x *= bLessE;
    }
    
    if (q == 1.0 || q == 2.0) {
        ret.y = float(isInx && (isInCircle || pos.y < minY));
        ret.y *= bLessE;
    } else {
        ret.y = float(isInx && pos.y < maxY && !isInCircle);
        ret.y *= -bLessE;
    }
    
    return ret;
}

PixelOutput main(PixelInput input) {
    PixelOutput output;
    output.position = input.position;
    output.vectorCoord = input.vectorCoord;
    output.color = vec4(1.0, 1.0, 1.0, 1.0);
    
    vec2 unitsPerPixel = fwidth(input.vectorCoord);
    vec2 pixelsPerUnit = 1.0 / unitsPerPixel;
    
    vec2 crossings = vec2(0.0);
    
    int i = 0;
    vec2 lastPos;
    vec4 fillColor = vec4(1.0, 0.647, 0.0, 1.0);
    
    while(true) {
        float command = texelFetch(commandQueue, ivec2(i, 0), 0).x;
        i++;

        if (command == CommandEnd) break;

        if (command == CommandMoveTo) {
            lastPos.x = texelFetch(commandQueue, ivec2(i, 0), 0).x;
            lastPos.y = texelFetch(commandQueue, ivec2(i+1, 0), 0).x;
            i += 2;
        } 
        else if (command == CommandLineTo) {
            vec2 to;
            to.x = texelFetch(commandQueue, ivec2(i, 0), 0).x;
            to.y = texelFetch(commandQueue, ivec2(i+1, 0), 0).x;
            i += 2;

            vec2 v0 = lastPos - input.vectorCoord;
            vec2 v1 = to - input.vectorCoord;
            crossings += LineTest(v0, v1, pixelsPerUnit);
            lastPos = to;
        } 
        else if (command == CommandSetColor) {
            fillColor.r = texelFetch(commandQueue, ivec2(i, 0), 0).x;
            fillColor.g = texelFetch(commandQueue, ivec2(i+1, 0), 0).x;
            fillColor.b = texelFetch(commandQueue, ivec2(i+2, 0), 0).x;
            fillColor.a = texelFetch(commandQueue, ivec2(i+3, 0), 0).x;
            i += 4;
        }
		else if (command == CommandArcTo) {
		    vec2 bp = lastPos;
			vec2 c;
			c.x = texelFetch(commandQueue, ivec2(i, 0), 0).x;
			c.y = texelFetch(commandQueue, ivec2(i+1, 0), 0).x;
			float r = length(c - bp);
			float b = texelFetch(commandQueue, ivec2(i+2, 0), 0).x;
			float e = texelFetch(commandQueue, ivec2(i+3, 0), 0).x;
			float q = texelFetch(commandQueue, ivec2(i+4, 0), 0).x;
    		vec2 ep = c + vec2(r * cos(e), r * sin(e));
			crossings += ArcTest(input.vectorCoord, bp, ep, c, b, e, r, q, pixelsPerUnit);
			lastPos = ep;
			i+=5;
		}
    }
    
    float weightX = 1.0 - abs(crossings.x * 2.0 - 1.0);
    float weightY = 1.0 - abs(crossings.y * 2.0 - 1.0);
    float coverage = max(abs(crossings.x * weightX + crossings.y * weightY) / max(weightX + weightY, 0.0001220703125), min(abs(crossings.x), abs(crossings.y)));
    
    output.color = fillColor;
    output.color.a *= coverage;
    return output;
}
)")) {
			std::cout << "ps±¨´í:" << std::endl;
			std::cout << m_vrps->getCompileError() << std::endl;
		}
		m_vrMaterial = ctx->createMaterial(m_vrvs, m_vrps);
		m_vrMaterial->compile();
		setDefaultMaterial(m_vrMaterial);
	}

	void VertexRenderPipeline::begin(VertexRenderScreen* screen)
	{
		m_screeen = screen;
		Pipeline::begin();
		m_commandQueue.clear();
	}

	void VertexRenderPipeline::setColor(Vec4 color)
	{
		m_commandQueue.push_back(Command_SetColor);
		m_commandQueue.push_back(color.x);
		m_commandQueue.push_back(color.y);
		m_commandQueue.push_back(color.z);
		m_commandQueue.push_back(color.w);
	}

	void VertexRenderPipeline::moveTo(Vec2 to)
	{
		m_commandQueue.push_back(Command_MoveTo);
		m_commandQueue.push_back(to.x);
		m_commandQueue.push_back(to.y);
	}

	void VertexRenderPipeline::lineTo(Vec2 to)
	{
		m_commandQueue.push_back(Command_LineTo);
		m_commandQueue.push_back(to.x);
		m_commandQueue.push_back(to.y);
	}

	void VertexRenderPipeline::fillrect(Vec2 pos, Vec2 size)
	{
		moveTo(pos);
		lineTo(Vec2(pos.x, pos.y + size.y));
		lineTo(Vec2(pos.x + size.x, pos.y + size.y));
		lineTo(Vec2(pos.x + size.x, pos.y));
		lineTo(pos);
	}


	void VertexRenderPipeline::arcTo(Vec2 center, float beginAngle, float endAngle)
	{
		beginAngle = fmod(beginAngle, 2 * M_PI);
		endAngle = fmod(endAngle, 2 * M_PI);
		if (beginAngle < 0) beginAngle += 2 * M_PI;
		if (endAngle < 0) endAngle += 2 * M_PI;

		if (abs(endAngle - beginAngle) < 1e-6 || abs(endAngle - beginAngle) >= 2 * M_PI - 1e-6) {
			for (int q = 1; q <= 4; ++q) {
				float qBegin = (q - 1) * M_PI_2;
				float qEnd = q * M_PI_2;
				unwrapperArcTo(center, qBegin, qEnd, q);
			}
			return;
		}

		bool isClockwise = beginAngle > endAngle;

		const float quadrantAngles[5] = { 0, M_PI_2, M_PI, 3 * M_PI_2, 2 * M_PI };

		if (isClockwise) {
			endAngle += 2 * M_PI;
		}

		float currentAngle = beginAngle;
		int currentQuadrant = (int)(currentAngle / M_PI_2) % 4 + 1;

		while ((isClockwise && currentAngle < endAngle) || (!isClockwise && currentAngle > endAngle)) {
			float nextQuadrantAngle = quadrantAngles[currentQuadrant % 4];
			if (isClockwise && nextQuadrantAngle <= currentAngle) nextQuadrantAngle += 2 * M_PI;

			float segmentEnd = isClockwise ?
				std::min(nextQuadrantAngle, endAngle) :
				std::max(nextQuadrantAngle, endAngle);

			unwrapperArcTo(center, currentAngle, segmentEnd, currentQuadrant);

			currentAngle = segmentEnd;
			currentQuadrant = isClockwise ? (currentQuadrant % 4) + 1 : ((currentQuadrant - 2 + 4) % 4) + 1;
		}
	}
	void VertexRenderPipeline::rectangle(Vec2 pos, Vec2 size, float width)
	{
		moveTo(Vec2(pos.x - width / 2, pos.y - width / 2));
		lineTo(Vec2(pos.x + size.x + width / 2, pos.y - width / 2));
		lineTo(Vec2(pos.x + size.x + width / 2, pos.y + size.y + width / 2));
		lineTo(Vec2(pos.x - width / 2, pos.y + size.y + width / 2));
		lineTo(Vec2(pos.x - width / 2, pos.y - width / 2));

		moveTo(Vec2(pos.x + width / 2, pos.y + width / 2));
		lineTo(Vec2(pos.x + width / 2, pos.y + size.y - width / 2));
		lineTo(Vec2(pos.x + size.x - width / 2, pos.y + size.y - width / 2));
		lineTo(Vec2(pos.x + size.x - width / 2, pos.y + width / 2));
		lineTo(Vec2(pos.x + width / 2, pos.y + width / 2));
	}

	void FCT::VertexRenderPipeline::end()
	{
		m_commandQueue.push_back(Command_End);
		Texture* texture = m_context->createTexture();
		texture->setSlot(4);
		texture->create(m_commandQueue.size(), 1, FCT::Texture::Format::R32F);
		texture->setData(m_commandQueue.data(), m_commandQueue.size());
		texture->bind();
		m_screeen->draw();
		Pipeline::end();
		m_screeen = nullptr;
		texture->release();
	}

	void VertexRenderPipeline::unwrapperArcTo(Vec2 center, float beginAngle, float endAngle, float q)
	{
		m_commandQueue.push_back(CommandArcTo);

		m_commandQueue.push_back(center.x);
		m_commandQueue.push_back(center.y);

		m_commandQueue.push_back(beginAngle);
		m_commandQueue.push_back(endAngle);

		m_commandQueue.push_back(q);
	}

	FCT::VertexFactory* FCT::VertexRenderPipeline::createFactory()
	{
		m_vrf = new VertexFactory();
		m_vrf->addAttribute(FCT::PipelineAttributeType::Position3f, "position");
		m_vrf->addAttribute(FCT::PipelineAttributeType::Custom, "vectorCoord", DataType::Vec2);
		return m_vrf;
	}


}
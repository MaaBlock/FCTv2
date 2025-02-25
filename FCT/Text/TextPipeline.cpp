#include "../headers.h"
#include <iostream>
namespace FCT
{
    void TextPipeline::color(Vec4 color)
    {
        m_color = color;
    }
    TextPipeline::TextPipeline(Context *ctx) : Pipeline(ctx, createFactory())
    {
        m_transform = ctx->createTexture();
        m_transform->setSlot(0);
        m_transform->create(4, 1, FCT::Texture::Format::RGBA32F);
        m_transform->setData(&m_cachedTransform, sizeof(m_cachedTransform));
        m_ctx = ctx;
        m_vs = ctx->createVertexShader(m_vf);
        m_color = Vec4(1, 1, 1, 1);
        m_vs->addCustomOutput(FCT::PipelineAttributeType::Position3f, "position");
        if (!m_vs->compileFromSource(R"(
layout(std140,binding = 5) uniform TextScreenInformation{
	vec3 g_screenPosition;
	vec3 g_screenXHalfSize;
	vec3 g_screenYHalfSize;
    vec2 g_screenLTCoord;
    vec2 g_screenRBCoord;
};
VertexOutput main(VertexInput vs_input) {
    VertexOutput vs_output;

    vec2 texCoordRange = g_screenRBCoord - g_screenLTCoord;
    float s = (vs_input.vectorCoord.x - g_screenLTCoord.x) / texCoordRange.x;
    float t = (vs_input.vectorCoord.y - g_screenLTCoord.y) / texCoordRange.y;

    vec3 xOffset = (2.0 * s - 1.0) * g_screenXHalfSize;
    vec3 yOffset = (1.0 - 2.0 * t) * g_screenYHalfSize;

    vs_output.position = g_screenPosition + xOffset + yOffset;
    vs_output.vectorCoord = vs_input.vectorCoord;
	vs_output.commandOffset = vs_input.commandOffset;
	vs_output.commandSize = vs_input.commandSize;

    return vs_output;
}
)"))
        {
            std::cout << "vs报错:" << std::endl;
            std::cout << m_vs->getCompileError() << std::endl;
        }
        m_ps = ctx->createPixelShader(m_vs->getOutput());
        if (!m_ps->compileFromSource(R"(
layout(binding = 4) uniform sampler2D commandQueue;

const float CommandEnd = -1.0;
const float CommandMoveTo = 1.0;
const float CommandLineTo = 2.0;
const float CommandBezierCurveTo = 3.0;
const float CommandSetColor = 4.0;
const float CommandArcTo = 5.0;
const float CommandSetTransform = 6.0;
const float CommandBeginPath = 7.0;
const float CommandEndPath = 8.0;

int GetCurveRootFlags(float v0, float v1, float v2) {
    int shift = ((v0 > 0.0) ? 2 : 0) + ((v1 > 0.0) ? 4 : 0) + ((v2 > 0.0) ? 8 : 0);
    return (0x2E74 >> shift) & 0x03;
}

vec2 QuadCurveSolveXAxis(vec2 v0, vec2 v1, vec2 v2) {
    vec2 a = v0 - 2.0 * v1 + v2;
    vec2 b = v0 - v1;
    float c = v0.y;
    float ra = 1.0 / a.y;
    float rb = 0.5 / b.y;
    float delta = sqrt(max(b.y * b.y - a.y * c, 0.0));
    vec2 t = vec2((b.y - delta) * ra, (b.y + delta) * ra);
    if (abs(a.y) < 0.0001220703125) t = vec2(c * rb, c * rb);
    return (a.x * t - b.x * 2.0) * t + v0.x;
}

vec2 QuadCurveSolveYAxis(vec2 v0, vec2 v1, vec2 v2) {
    vec2 a = v0 - 2.0 * v1 + v2;
    vec2 b = v0 - v1;
    float c = v0.x;
    float ra = 1.0 / a.x;
    float rb = 0.5 / b.x;
    float delta = sqrt(max(b.x * b.x - a.x * c, 0.0));
    vec2 t = vec2((b.x - delta) * ra, (b.x + delta) * ra);
    if (abs(a.x) < 0.0001220703125) t = vec2(c * rb, c * rb);
    return (a.y * t - b.y * 2.0) * t + v0.y;
}

float CurveTestXAxis(vec2 v0, vec2 v1, vec2 v2, vec2 pixelsPerUnit) {
    if (max(max(v0.x, v1.x), v2.x) * pixelsPerUnit.x < -0.5) return 0.0;
    int flags = GetCurveRootFlags(v0.y, v1.y, v2.y);
    if (flags == 0) return 0.0;
    vec2 x1x2 = QuadCurveSolveXAxis(v0, v1, v2) * pixelsPerUnit.x;
    float ret = 0.0;
    if ((flags & 1) != 0) {
        ret += clamp(x1x2.x + 0.5, 0.0, 1.0);
    }
    if ((flags & 2) != 0) {
        ret -= clamp(x1x2.y + 0.5, 0.0, 1.0);
    }
    return ret;
}

float CurveTestYAxis(vec2 v0, vec2 v1, vec2 v2, vec2 pixelsPerUnit) {
    if (max(max(v0.y, v1.y), v2.y) * pixelsPerUnit.y < -0.5) return 0.0;
    int flags = GetCurveRootFlags(v0.x, v1.x, v2.x);
    if (flags == 0) return 0.0;
    vec2 y1y2 = QuadCurveSolveYAxis(v0, v1, v2) * pixelsPerUnit.y;
    float ret = 0.0;
    if ((flags & 0x01) != 0) {
        ret -= clamp(y1y2.x + 0.5, 0.0, 1.0);
    }
    if ((flags & 0x02) != 0) {
        ret += clamp(y1y2.y + 0.5, 0.0, 1.0);
    }
    return ret;
}

vec2 CurveTest(vec2 v0, vec2 v1, vec2 v2, vec2 pixelsPerUnit) {
    return vec2(
        CurveTestXAxis(v0, v1, v2, pixelsPerUnit),
        CurveTestYAxis(v0, v1, v2, pixelsPerUnit)
    );
}

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
    float bLessE = b < e ? 1.0 : -1.0;
    
    if (q == 1.0 || q == 4.0) {
        ret.x = float(isIny && (isInCircle || pos.x < minX));
        ret.x *= -bLessE;
    } else {
        ret.x = float(isIny && pos.x < maxX && !isInCircle);
        ret.x *= bLessE;
    }
    
    if (q == 1.0 || q == 2.0) {
        ret.y = float(isInx && (isInCircle || pos.y < minY));
        ret.y *= -bLessE;
    } else {
        ret.y = float(isInx && pos.y < maxY && !isInCircle);
        ret.y *= bLessE;
    }
    
    return ret;
}

float NoZeroSign(float x) {
    return x >= 0.0 ? 1.0 : -1.0;
}

mat3 transform = mat3(1.0);
float transformSign = 1.0; 
vec2 applyTransform(vec2 point) {
    vec3 transformedPoint = transform * vec3(point, 1.0);
    return transformedPoint.xy / transformedPoint.z;
}
float fetchCommand(int index) {
    int x = index % 4096;
    int y = index / 4096;
    return texelFetch(commandQueue, ivec2(x, y), 0).x;
}

PixelOutput main(PixelInput ps_input) {
    PixelOutput ps_output;
    ps_output.position = ps_input.position;
    ps_output.vectorCoord = ps_input.vectorCoord;
    ps_output.color = vec4(1.0, 1.0, 1.0, 1.0);
    
    vec2 unitsPerPixel = fwidth(ps_input.vectorCoord);
    vec2 pixelsPerUnit = 1.0 / unitsPerPixel;
    
    vec2 crossings = vec2(0.0);
    
    int i = int(ps_input.commandOffset); 
    vec2 lastPos;
	vec4 fillColor = vec4(1.0, 0.647, 0.0, 1.0);
	vec4 currentColor = vec4(1.0, 0.647, 0.0, 1.0);
	bool isPathStarted = false;
	vec2 pathCrossings = vec2(0.0);

	float pathOperation = 0.0; // 0: 默认 1:并 2:交 3:补
    int edge = i + int(ps_input.commandSize);
    while(i < edge) {
        float command = fetchCommand(i);
        i++;

        if (command == CommandEnd) break;
		if (command == CommandBeginPath) {
        	isPathStarted = true;
        	pathCrossings = vec2(0.0);
			pathOperation = fetchCommand(i);
			i += 1;
    	}
    	else if (command == CommandEndPath) {
        	isPathStarted = false;
        	if (length(pathCrossings) > 0.0) {
            	fillColor = currentColor;
        	}
			if (pathOperation == 0.0) {
            	crossings += pathCrossings;
        	} else if (pathOperation == 1.0) { 
				crossings = vec2(
                	NoZeroSign(crossings.x) * NoZeroSign(pathCrossings.x) * max(abs(crossings.x), abs(pathCrossings.x)),
                	NoZeroSign(crossings.y) * NoZeroSign(pathCrossings.y) * max(abs(crossings.y), abs(pathCrossings.y))
            	);
        	} else if (pathOperation == 2.0) { 
				crossings = vec2(
                	sign(crossings.x) * sign(pathCrossings.x) * min(abs(crossings.x), abs(pathCrossings.x)),
                	sign(crossings.y) * sign(pathCrossings.y) * min(abs(crossings.y), abs(pathCrossings.y))
            	);
        	} else if (pathOperation == 3.0) {
			    crossings += pathCrossings;
				crossings = vec2(
					NoZeroSign(crossings.x) * (1.0 - abs(crossings.x)),
					NoZeroSign(crossings.y) * (1.0 - abs(crossings.y))
            	);
			}
			pathCrossings = vec2(0);
			pathOperation = 0.0;
    	}
        if (command == CommandMoveTo) {
            lastPos.x = fetchCommand(i);
            lastPos.y = fetchCommand(i+1);
			lastPos = applyTransform(lastPos);
            i += 2;
        } 
        else if (command == CommandLineTo) {
            vec2 to;
            to.x = fetchCommand(i);
            to.y = fetchCommand(i+1);
			to = applyTransform(to);
            i += 2;

            vec2 v0 = lastPos - ps_input.vectorCoord;
            vec2 v1 = to - ps_input.vectorCoord;
			//if (isPathStarted) {
				pathCrossings += transformSign * LineTest(v0, v1, pixelsPerUnit);
			//} else {
			//	crossings += LineTest(v0, v1, pixelsPerUnit);
			//}
          ;
            lastPos = to;
        } 
        else if (command == CommandSetColor) { 
			currentColor.r = fetchCommand(i);
        	currentColor.g = fetchCommand(i+1);
        	currentColor.b = fetchCommand(i+2);
        	currentColor.a = fetchCommand(i+3);
            i += 4;
        }
		else if (command == CommandArcTo) {
		    vec2 bp = lastPos;
			vec2 c;
			c.x = fetchCommand(i);
			c.y = fetchCommand(i+1);
			c = applyTransform(c);
			float r = length(c - bp);
			float b = fetchCommand(i+2);
			float e = fetchCommand(i+3);
			float q = fetchCommand(i+4);
    		vec2 ep = c + vec2(r * cos(e), r * sin(e));
			//if (isPathStarted) {
				pathCrossings += transformSign * ArcTest(ps_input.vectorCoord, bp, ep, c, b, e, r, q, pixelsPerUnit);
			//} else {
			//	crossings += ArcTest(ps_input.vectorCoord, bp, ep, c, b, e, r, q, pixelsPerUnit);
			//}
			lastPos = ep;
			i+=5;
		}
		else if (command == CommandBezierCurveTo) {
            vec2 control = vec2(fetchCommand(i),
                                fetchCommand(i+1));
            vec2 end = vec2(fetchCommand(i+2),
                            fetchCommand(i+3));
			control = applyTransform(control);
			end = applyTransform(end);
            i += 4;

            vec2 v0 = lastPos - ps_input.vectorCoord;
            vec2 v1 = control - ps_input.vectorCoord;
            vec2 v2 = end - ps_input.vectorCoord;
			//if (isPathStarted) {
				pathCrossings += transformSign * CurveTest(v0, v1, v2, pixelsPerUnit);
			//} else {
            //	crossings += CurveTest(v0, v1, v2, pixelsPerUnit);
			//}
            lastPos = end;
        }
		else if (command == CommandSetTransform) {
		    for (int j = 0; j < 3; j++) {
		        for (int k = 0; k < 3; k++) {
		            transform[j][k] = fetchCommand(i + j * 4 + k);
		        }
		    }
		    i += 16;
            transformSign = NoZeroSign(determinant(transform));
		}
    }
    /*
    ps_output.color = vec4((crossings + vec2(2,2)) / 4,0,1.0);
	return ps_output;
	*/
    float weightX = 1.0 - abs(crossings.x * 2.0 - 1.0);
    float weightY = 1.0 - abs(crossings.y * 2.0 - 1.0);
    float coverage = max(abs(crossings.x * weightX + crossings.y * weightY) / max(weightX + weightY, 0.0001220703125), min(abs(crossings.x), abs(crossings.y)));
    
    ps_output.color = fillColor;
    ps_output.color.a *= coverage;
	return ps_output;
}
)"))
        {
            std::cout << "ps报错:" << std::endl;
            std::cout << m_ps->getCompileError() << std::endl;
            std::cout << "ps source:" << std::endl;
            std::cout << m_ps->getSource() << std::endl;
        }
        m_material = ctx->createMaterial(m_vs, m_ps);
        m_material->compile();
        setDefaultMaterial(m_material);
        m_cb = ctx->createConstBuffer();
        m_cb->create(sizeof(TextScreenInformation), 5);
        screen(Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(0, 1, 0));
        viewport(Vec2(0, 0), Vec2(800, 600));
        updataScreenInformation();
        m_arr = new VertexArray(m_defaultFactory, 0);
        m_dc = ctx->createDrawCall(PrimitiveType::Triangles, 0, 0);
    }
    TextPipeline::~TextPipeline()
    {
        m_vf->release();
    }
    void TextPipeline::screen(Vec3 pos, Vec3 xHalfVec, Vec3 yHalfVec)
    {
        m_si.g_screenPosition = pos;
        m_si.g_screenXHalfSize = xHalfVec;
        m_si.g_screenYHalfSize = yHalfVec;
    }
    void TextPipeline::viewport(Vec2 lt, Vec2 rb)
    {
        m_si.g_screenLTCoord = lt;
        m_si.g_screenRBCoord = rb;
    }
    void TextPipeline::beginDraw()
    {
    }
    void TextPipeline::drawText(const char32_t *str, float x, float y, float w, float h)
    {
        TextLayout layout;
        for (auto font : m_fonts)
        {
            layout.addFont(font);
        }
        layout.layoutRect(Vec2(x, y), Vec2(x + w, y + h));
        layout.layoutText(str);
        auto rets = layout.layout();
        {
            auto vectorCoordOffset = m_arr->getAttributeOffset("vectorCoord");
            auto commandOffsetOffset = m_arr->getAttributeOffset("commandOffset");
            auto commandSizeOffset = m_arr->getAttributeOffset("commandSize");
            // AutoTimer timer("uploadText");
            size_t currentSize;
            for (auto ret : rets)
            {
                const GlyphInfo *glyph = ret.font->getGlyphInfo(ret.id);
                auto id = ret.id;
                float offset = m_commandQueue.size();
                float size = 0;
                auto font = ret.font;
                auto data = font->getGlyphInfo(id);
                m_commandQueue.push_back(Command_SetColor);

                currentSize = m_commandQueue.size();
                m_commandQueue.resize(currentSize + 4);
                std::memcpy(m_commandQueue.data() + currentSize, &m_color, 4 * sizeof(float));
                m_commandQueue.push_back(Command_SetTransform);
                currentSize = m_commandQueue.size();
                m_commandQueue.resize(m_commandQueue.size() + 16);
                std::memcpy(m_commandQueue.data() + currentSize, ret.transform.m, 16 * sizeof(float));
                currentSize = m_commandQueue.size();
                size_t additionalSize = data->outlineCommands.size();
                m_commandQueue.resize(currentSize + additionalSize);

                std::memcpy(m_commandQueue.data() + currentSize,
                            data->outlineCommands.data(),
                            additionalSize * sizeof(float));

                size = m_commandQueue.size() - offset;

                float begin = m_arr->getVertexCount();
                m_arr->addVertex(6);
                m_arr->setAttribute(begin, vectorCoordOffset, Vec2(ret.bbox.min.x, ret.bbox.min.y));
                m_arr->setAttribute(begin, commandOffsetOffset, offset);
                m_arr->setAttribute(begin, commandSizeOffset, size);

                m_arr->setAttribute(begin + 1, vectorCoordOffset, Vec2(ret.bbox.min.x, ret.bbox.min.y + ret.bbox.size.y));
                m_arr->setAttribute(begin + 1, commandOffsetOffset, offset);
                m_arr->setAttribute(begin + 1, commandSizeOffset, size);

                m_arr->setAttribute(begin + 2, vectorCoordOffset, Vec2(ret.bbox.min.x + ret.bbox.size.x, ret.bbox.min.y));
                m_arr->setAttribute(begin + 2, commandOffsetOffset, offset);
                m_arr->setAttribute(begin + 2, commandSizeOffset, size);

                m_arr->setAttribute(begin + 3, vectorCoordOffset, Vec2(ret.bbox.min.x, ret.bbox.min.y + ret.bbox.size.y));
                m_arr->setAttribute(begin + 3, commandOffsetOffset, offset);
                m_arr->setAttribute(begin + 3, commandSizeOffset, size);

                m_arr->setAttribute(begin + 4, vectorCoordOffset, Vec2(ret.bbox.min.x + ret.bbox.size.x, ret.bbox.min.y + ret.bbox.size.y));
                m_arr->setAttribute(begin + 4, commandOffsetOffset, offset);
                m_arr->setAttribute(begin + 4, commandSizeOffset, size);

                m_arr->setAttribute(begin + 5, vectorCoordOffset, Vec2(ret.bbox.min.x + ret.bbox.size.x, ret.bbox.min.y));
                m_arr->setAttribute(begin + 5, commandOffsetOffset, offset);
                m_arr->setAttribute(begin + 5, commandSizeOffset, size);
            }
        }
    }
    void TextPipeline::addFont(Font *font)
    {
        m_fonts.push_back(font);
    }
    void TextPipeline::flush()
    {
        m_vb = m_ctx->createVertexBuffer(m_arr);
        m_vb->create(m_ctx);
        m_il = m_ctx->createInputLayout(m_vf);
        m_il->create(m_ctx, m_vb);
        m_dc->setCount(m_arr->getVertexCount());
        GL_Check("TextPipeline::endDraw m_il->bind();");
        Texture *texture = m_context->createTexture();
        GL_Check("TextPipeline::endDraw createTexture->bind();");
        texture->setSlot(4);
        texture->create(4096, m_commandQueue.size() / 4096 + 1, FCT::Texture::Format::R32F);
        GL_Check("TextPipeline::endDraw texture->create();");
        texture->setData(m_commandQueue.data(), m_commandQueue.size());
        begin();
        texture->bind();
        GL_Check("TextPipeline::endDraw texture->bind();");
        m_cb->bind();
        GL_Check("TextPipeline::endDraw m_cb->bind();");
        m_transform->bind();
        GL_Check("TextPipeline::endDraw m_transform->bind();");
        m_material->bind();
        GL_Check("TextPipeline::endDraw m_material->bind();");
        m_vb->bind();
        GL_Check("TextPipeline::endDraw m_vb->bind();");
        m_il->bind();
        GL_Check("TextPipeline::endDraw m_il->bind();");
        m_dc->bind();
        GL_Check("TextPipeline::endDraw m_dc->bind();");
        m_vb->release();
        m_il->release();
        m_arr->clear();
        texture->release();
        m_commandQueue.clear();
    }
    void TextPipeline::toggleOriginVertical(bool toggle)
    {
    }
    void TextPipeline::updataScreenInformation()
    {
        m_cb->setData(&m_si, sizeof(m_si));
    }
    VertexFactory *TextPipeline::createFactory()
    {
        m_vf = new VertexFactory();
        // m_vf->addAttribute(FCT::PipelineAttributeType::Position3f, "position");
        m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "vectorCoord", DataType::Vec2);
        m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "commandOffset", DataType::Float, true);
        m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "commandSize", DataType::Float, true);
        return m_vf;
    }
}
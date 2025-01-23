#pragma once
#include "../MutilThreadBase/RefCount.h"
#include "./IPipelineResource.h"
#include "./VertexFactory.h"

namespace FCT {

class VertexBuffer : public RefCount, public IPipelineResource {
public:
    PipelineResourceType getType() const override;

protected:
};

} // namespace FCT


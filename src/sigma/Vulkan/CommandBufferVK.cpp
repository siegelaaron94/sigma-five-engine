#include <sigma/Vulkan/CommandBufferVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/FramebufferVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>
#include <sigma/Vulkan/PipelineVK.hpp>
#include <sigma/Vulkan/VertexBufferVK.hpp>
#include <sigma/Vulkan/IndexBufferVK.hpp>

#include <limits>

CommandBufferVK::CommandBufferVK(std::shared_ptr<DeviceVK> inDevice, VkCommandPool inCommandPool)
    : mDevice(inDevice)
    , mCommandPool(inCommandPool)
{
}

CommandBufferVK::~CommandBufferVK()
{
    if (mDevice && mCommandPool && mBuffer) {
        vkFreeCommandBuffers(mDevice->handle(), mCommandPool, 1, &mBuffer);
    }
}

bool CommandBufferVK::initialize()
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(mDevice->handle(), &allocInfo, &mBuffer) != VK_SUCCESS) {
        return false;
    }

    return true;
}

void CommandBufferVK::begin()
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    // TODO check for error
    vkBeginCommandBuffer(mBuffer, &beginInfo);
}

void CommandBufferVK::beginRenderPass(const RenderPassBeginParams& inParams)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<RenderPassVK>(inParams.renderPass), "Must use vulkan render pass with vulkan command buffer!");
    SIGMA_ASSERT(std::dynamic_pointer_cast<FramebufferVK>(inParams.frameBuffer), "Must use vulkan framebuffer with vulkan command buffer!");

    // TODO: correct clear colors!
    VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = std::static_pointer_cast<RenderPassVK>(inParams.renderPass)->handle();
    renderPassInfo.framebuffer = std::static_pointer_cast<FramebufferVK>(inParams.frameBuffer)->handle();
    renderPassInfo.renderArea.offset = { inParams.renderArea.origin.x, inParams.renderArea.origin.y };
    renderPassInfo.renderArea.extent = { inParams.renderArea.size.x, inParams.renderArea.size.y };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(mBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBufferVK::bindPipeline(std::shared_ptr<Pipeline> inPipeline)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<PipelineVK>(inPipeline), "Must use vulkan pipeline with vulkan command buffer!");
	mCurrentPipeline = std::static_pointer_cast<PipelineVK>(inPipeline);
	vkCmdBindPipeline(mBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mCurrentPipeline->handle());
}

void CommandBufferVK::bindVertexBuffer(std::shared_ptr<VertexBuffer> inBuffer)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<VertexBufferVK>(inBuffer), "Must use vulkan vertex buffer with vulkan command buffer!");
	mCurrentVertexBuffer = std::static_pointer_cast<VertexBufferVK>(inBuffer);
	VkDeviceSize offset = 0;
	VkBuffer vertexBuffer = mCurrentVertexBuffer->handle();
	vkCmdBindVertexBuffers(mBuffer, 0, 1, &vertexBuffer, &offset);
}

void CommandBufferVK::bindIndexBuffer(std::shared_ptr<IndexBuffer> inBuffer)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<IndexBufferVK>(inBuffer), "Must use vulkan index buffer with vulkan command buffer!");
	mCurrentIndexBuffer = std::static_pointer_cast<IndexBufferVK>(inBuffer);

	vkCmdBindIndexBuffer(mBuffer, mCurrentIndexBuffer->handle(), 0, mCurrentIndexBuffer->dataType() == DataType::UInt ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16);
}

void CommandBufferVK::draw(uint32_t inVertexCount, uint32_t inInstanceCount, uint32_t inFirstVertex, uint32_t inFirstInstance)
{
	vkCmdDraw(mBuffer, inVertexCount, inInstanceCount, inFirstVertex, inFirstInstance);
}

void CommandBufferVK::drawIndexed(uint32_t inIndexCount, uint32_t inInstanceCount, uint32_t inFirstIndex, int32_t inVertexOffset, uint32_t inFirstInstance)
{
	vkCmdDrawIndexed(mBuffer, inIndexCount, inInstanceCount, inFirstIndex, inVertexOffset, inFirstInstance);
}

void CommandBufferVK::endRenderPass()
{
    vkCmdEndRenderPass(mBuffer);
}

void CommandBufferVK::end()
{
    // TODO check for error
    vkEndCommandBuffer(mBuffer);
}

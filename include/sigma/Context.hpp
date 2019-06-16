#pragma once

#include <memory>
#include <vector>

class Window;

enum class DataType;

class Shader;
enum class ShaderType;

class Program;

class VertexBuffer;
struct VertexMemberDescription;

class IndexBuffer;
enum class PrimitiveType;

class Context 
{
public:
    virtual ~Context() = default;

    virtual void initialize() = 0;

	virtual void makeCurrent(Window *inWindow) = 0;

    virtual std::shared_ptr<Shader> createShader(ShaderType inType, const std::string &inCode) = 0;

    virtual std::shared_ptr<Program> createProgram(const std::vector<std::shared_ptr<Shader>>& inShaders) = 0;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const std::initializer_list<VertexMemberDescription> &inLayout) = 0;

    virtual std::shared_ptr<IndexBuffer> createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType) = 0;

    virtual void draw(std::shared_ptr<Program> inProgram, std::shared_ptr<VertexBuffer> inVertexBuffer, std::shared_ptr<IndexBuffer> inIndexBuffer) = 0;
};

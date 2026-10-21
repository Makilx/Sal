#pragma once

#include <SalInterface/Renderer/IMesh.hpp>

namespace sal::backend {
    class OpenGLMesh final : public interface::IMesh {
    public:
        OpenGLMesh() : interface::IMesh(RendererAPI::OpenGL) {}
        ~OpenGLMesh() override { Destroy(); }

        bool Create() override;
        bool IsCreated() const override { return _created; }
        void Destroy() override;

        void SetVertices(const Vector<interface::Vertex> &vertices) override;
        void SetIndices(const Vector<unsigned int> &indices) override;

        SizeT GetIndicesCount() const override { return _indicesCount; }

    public:
        unsigned int GetHandle() const { return _vao; }
        void SetInstanceTransforms(const Vector<Matrix4> &transforms);
        unsigned int GetInstanceBuffer() const { return _instanceVBO; }

    private:
        bool _created = false;
        SizeT _verticesCount = 0;
        SizeT _indicesCount = 0;

        unsigned int _vao = 0;
        unsigned int _vbo = 0;
        unsigned int _ibo = 0;
        unsigned int _instanceVBO = 0;
    };
}

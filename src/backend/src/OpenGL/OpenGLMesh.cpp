#include <SalInterface/Renderer/IRenderer.hpp>

#include <SalBackend/Rendering/OpenGL/OpenGLMesh.hpp>

#include <glad/glad.h>

namespace sal::backend {
    bool OpenGLMesh::Create() {
        if (_created) {
            Destroy();
        }

        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ibo);
        glGenBuffers(1, &_instanceVBO);
        glGenBuffers(1, &_instanceStackVBO);

        if (_vao == 0 || _vbo == 0 || _ibo == 0 ||
            _instanceVBO == 0 || _instanceStackVBO == 0) {
            Destroy();
            return false;
        }

        glBindVertexArray(_vao);

        // Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, sizeof(interface::Vertex),
            reinterpret_cast<void *>(offsetof(interface::Vertex, Position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE, sizeof(interface::Vertex),
            reinterpret_cast<void *>(offsetof(interface::Vertex, Normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE, sizeof(interface::Vertex),
            reinterpret_cast<void *>(offsetof(interface::Vertex, TextureUV)));

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

        // Instance buffer
        glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);

        constexpr GLsizei instanceStride =
            static_cast<GLsizei>(sizeof(interface::RenderInstance));

        // Model matrix
        for (GLuint i = 0; i < 4; ++i) {
            GLuint location = 3 + i;

            glEnableVertexAttribArray(location);
            glVertexAttribPointer(
                location, 4, GL_FLOAT, GL_FALSE, instanceStride,
                reinterpret_cast<void *>(
                    offsetof(interface::RenderInstance, Model) +
                    sizeof(float) * 4 * i));

            glVertexAttribDivisor(location, 1);
        }

        // Material tint
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(
            7, 4, GL_FLOAT, GL_FALSE, instanceStride,
            reinterpret_cast<void *>(
                offsetof(interface::RenderInstance, Material) +
                offsetof(Material, Tint)));

        glVertexAttribDivisor(7, 1);

        // Metallic
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(
            8, 1, GL_FLOAT, GL_FALSE, instanceStride,
            reinterpret_cast<void *>(
                offsetof(interface::RenderInstance, Material) +
                offsetof(Material, Metallic)));

        glVertexAttribDivisor(8, 1);

        // Roughness
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(
            9, 1, GL_FLOAT, GL_FALSE, instanceStride,
            reinterpret_cast<void *>(
                offsetof(interface::RenderInstance, Material) +
                offsetof(Material, Roughness)));

        glVertexAttribDivisor(9, 1);

        // Instance texture-stack data
        glBindBuffer(GL_ARRAY_BUFFER, _instanceStackVBO);

        glEnableVertexAttribArray(10);
        glVertexAttribIPointer(
            10, 1, GL_UNSIGNED_INT,
            sizeof(interface::InstanceTextureStackData),
            reinterpret_cast<void *>(
                offsetof(interface::InstanceTextureStackData, Albedo)));

        glVertexAttribDivisor(10, 1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        _created = true;
        _indicesCount = 0;
        _verticesCount = 0;

        return true;
    }

    void OpenGLMesh::Destroy() {
        if (_vao != 0) {
            glDeleteVertexArrays(1, &_vao);
        }

        if (_vbo != 0) {
            glDeleteBuffers(1, &_vbo);
        }

        if (_ibo != 0) {
            glDeleteBuffers(1, &_ibo);
        }

        if (_instanceVBO != 0) {
            glDeleteBuffers(1, &_instanceVBO);
        }

        if (_instanceStackVBO != 0) {
            glDeleteBuffers(1, &_instanceStackVBO);
        }

        _vao = 0;
        _vbo = 0;
        _ibo = 0;
        _instanceVBO = 0;
        _instanceStackVBO = 0;

        _created = false;
        _verticesCount = 0;
        _indicesCount = 0;
    }

    void OpenGLMesh::SetVertices(const Vector<interface::Vertex> &vertices) {
        if (!_created) {
            return;
        }

        if (vertices.size() >
            static_cast<SizeT>(std::numeric_limits<GLsizeiptr>::max() /
                               sizeof(interface::Vertex))) {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size() *
                                             sizeof(interface::Vertex)),
                     vertices.empty() ? nullptr : vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        _verticesCount = vertices.size();
    }

    void OpenGLMesh::SetIndices(const Vector<unsigned int> &indices) {
        if (!_created) {
            return;
        }

        if (indices.size() >
            static_cast<SizeT>(std::numeric_limits<GLsizeiptr>::max() /
                               sizeof(unsigned int))) {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(_vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
            indices.empty() ? nullptr : indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);

        _indicesCount = indices.size();
    }

    void OpenGLMesh::SetInstanceTransforms(
        const Vector<interface::RenderInstance> &instances) {

        if (!_created) {
            return;
        }

        if (instances.size() >
            static_cast<SizeT>(std::numeric_limits<GLsizeiptr>::max() /
                               sizeof(interface::RenderInstance))) {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                instances.size() * sizeof(interface::RenderInstance)),
            instances.empty() ? nullptr : instances.data(),
            GL_STREAM_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLMesh::SetInstanceStackData(
        const Vector<interface::InstanceTextureStackData> &stackData) {

        if (!_created) {
            return;
        }

        if (stackData.size() >
            static_cast<SizeT>(std::numeric_limits<GLsizeiptr>::max() /
                               sizeof(interface::InstanceTextureStackData))) {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _instanceStackVBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                stackData.size() * sizeof(interface::InstanceTextureStackData)),
            stackData.empty() ? nullptr : stackData.data(),
            GL_STREAM_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

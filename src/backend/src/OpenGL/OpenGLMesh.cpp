#include <SalBackend/Rendering/OpenGL/OpenGLMesh.hpp>
#include <glad/glad.h>

namespace sal::backend {
    bool OpenGLMesh::Create() {
        if (_created) {
            Destroy();
        }

        glCreateVertexArrays(1, &_vao);
        glCreateBuffers(1, &_vbo);
        glCreateBuffers(1, &_ibo);

        if (_vao == 0 || _vbo == 0 || _ibo == 0) {
            Destroy();
            return false;
        }

        glBindVertexArray(_vao);

        // Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, sizeof(interface::Vertex),
            reinterpret_cast<void *>(offsetof(interface::Vertex, Position)));

        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE, sizeof(interface::Vertex),
            reinterpret_cast<void *>(offsetof(interface::Vertex, Normal)));

        // Texture UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE, sizeof(interface::Vertex),
            reinterpret_cast<void *>(offsetof(interface::Vertex, TextureUV)));

        // Associate the index buffer with this VAO.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

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

        _vao = 0;
        _vbo = 0;
        _ibo = 0;

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

    void OpenGLMesh::SetInstanceTransforms(const Vector<Matrix4> &transforms) {
        if (!_created) {
            return;
        }

        if (transforms.size() >
            static_cast<SizeT>(std::numeric_limits<GLsizeiptr>::max() /
                               sizeof(Matrix4))) {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(transforms.size() * sizeof(Matrix4)),
            transforms.empty() ? nullptr : transforms.data(), GL_STREAM_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

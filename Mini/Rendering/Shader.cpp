//
//  Shader.cpp
//  Shaders
//
//  Created by Jeppe Nielsen on 03/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Shader.hpp"

namespace Mini {
template<>
void Shader<Mini::Vertex>::RenderObject(Mini::VertexRenderer<Mini::Vertex>& renderer, const typename Mini::VertexMesh<Mini::Vertex>::Vertices& vertices, const Mini::IVertexMesh::Triangles& triangles, const Mini::Matrix4x4& world) {
    
    size_t verticesSize = vertices.size();
    size_t trianglesSize = triangles.size();
    
    renderer.Begin(verticesSize, trianglesSize);

    size_t index = renderer.vertexIndex;
    for (size_t v=0; v<verticesSize; ++v) {
        const Vertex& source = vertices[v];
        Vertex& dest = renderer.vertices[renderer.vertexIndex++];
        world.TransformPositionAffine(source.Position, dest.Position);
        world.TransformVectorAffine(source.Normal, dest.Normal);
        dest.Color = source.Color;
        dest.TextureCoords.x = source.TextureCoords.x;
        dest.TextureCoords.y = source.TextureCoords.y;
    }
    for (size_t v = 0; v<trianglesSize; v++) {
        renderer.triangles[renderer.triangleIndex++] = (GLshort)(index + triangles[v]);
    }
}
}

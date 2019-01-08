//
//  Mesh.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "VertexMesh.hpp"
#include "BoundingBox.hpp"
#include "DirtyProperty.hpp"

namespace Mini {

class RenderSystem;

struct Mesh;
struct MeshIntersector {
    public:
    virtual bool IntersectsRay(Mesh& mesh, const Ray& ray,
                            float* pickDistance, float* barycentricU, float* barycentricV,
                            size_t* triangleIndex, Vector3* normal) = 0;
};

struct Mesh {
public:
    
    Mesh() : vertexMesh(0), vertexType(0), customIntersector(0) { }
    ~Mesh() { delete vertexMesh; }
    
    void operator=(const Mesh& other) {
        if (other.vertexMesh) {
            vertexMesh = other.vertexMesh->Clone();
            vertexType = other.vertexType;
            RevertDefaultCalcBoundingBox();
            LocalBoundingBox.MakeDirty();
        } else {
             delete vertexMesh;
            vertexMesh = 0;
            customIntersector = 0;
        }
    }
    
    template<class Vertex>
    class VertexMesh<Vertex>& GetMesh() {
        if (!vertexMesh) {
            vertexMesh = new VertexMesh<Vertex>();
            vertexType = 0;//Vertex::ID; // TODO: fix this from scripts
            RevertDefaultCalcBoundingBox();
        } else if (vertexType != 0) {//Vertex::ID) {// TODO: fix this from scripts
            return VertexMesh<Vertex>::empty;
        }
        VertexMesh<Vertex>& mesh = *((VertexMesh<Vertex>*)vertexMesh);
        LocalBoundingBox.MakeDirty();
        return mesh;
    }

    template<class Vertex>
    const VertexMesh<Vertex>& ConstMesh() const {
        return (const VertexMesh<Vertex>&)*((VertexMesh<Vertex>*)vertexMesh);
    }
    
    size_t VerticesSize() {
        return vertexMesh ? vertexMesh->Size() : 0;
    }
    
    const Vector3& Position(size_t index) {
        return vertexMesh ? vertexMesh->GetPosition(index) : Vector3::zero;
    }
    
    int VertexType() { return vertexType; }
    
    IVertexMesh::Triangles& Triangles() { return vertexMesh->triangles; }
    DirtyProperty<BoundingBox> LocalBoundingBox;

    bool IntersectsRay(const Ray& ray, float* pickDistance, float* barycentricU, float* barycentricV, size_t* triangleIndex, Vector3* normal) {
        if (customIntersector) {
            return customIntersector->IntersectsRay(*this, ray, pickDistance, barycentricU, barycentricV, triangleIndex, normal);
        }
        if (!vertexMesh) return false;
        return vertexMesh->IntersectsRay(ray, pickDistance, barycentricU, barycentricV, triangleIndex, normal);
    }

    MeshIntersector* customIntersector;
    
    void RevertDefaultCalcBoundingBox() {
        LocalBoundingBox.Method = [this] (BoundingBox& box) {
            if (vertexMesh) {
                vertexMesh->CalcBoundingBox(box);
            }
        };
    }
    
    void Clear() {
        if (!vertexMesh) return;
        vertexMesh->Clear();
    }
    
    //template<class T>
    VertexMesh<class Vertex>::Vertices& Vertices() {
        return GetMesh<Vertex>().vertices;
    }
    
    /*
    TypeInfo GetType() {
        TypeInfo info("Mesh");
        if (!vertexType) {
            GetMesh<Vertex>(); //TODO: needs to support other vertex types
        }
        info.AddField("VertexMesh", vertexMesh);
        return info;
    }
    *///TODO : re-enable this
    
    void LoadAsset(const std::string& path);
    
private:
    IVertexMesh* vertexMesh;
    int vertexType;
    
    friend class RenderSystem;
};

}

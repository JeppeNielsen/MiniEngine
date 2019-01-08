#pragma once
#include "BoundingBox.hpp"
#include "BoundingFrustum.hpp"
#include "Ray.hpp"
#include <vector>

namespace Mini {
    template<typename T>
	class Octree {
	public:
		struct Node {
			BoundingBox box;
			Octree* node;
			size_t childIndex;
            int order;
			T data;
		};

		using Nodes = std::vector<Node*>;

        void Get(const BoundingFrustum& frustum, std::vector<T>& list) {
            if (!nodes) return;
            
            BoundingFrustum::Intersection test = frustum.Intersect(box);
            
            if (test == BoundingFrustum::OUTSIDE) {
                return;
            } else if (test == BoundingFrustum::INSIDE) {
                GetRecursive(list);
                return;
            } else {
                for(size_t i = 0, size=nodes->size(); i<size; i++) {
                    Node* node = nodes->at(i);
                    
                    if (frustum.Intersect(node->box)!=BoundingFrustum::OUTSIDE) list.push_back(node->data);
                    //list.push_back(node);
                }
                if (!children) return;
                for (int i=0; i<8; i++) {
                    children[i].Get(frustum, list);
                }
            }
        }
        
        void GetRecursive(std::vector<T>& list) {
            if (!nodes) return;
            for	(unsigned i=0;i<nodes->size(); i++) {
                list.push_back((T)nodes->at(i)->data);
            }
            if (!children) return;
            for(int i=0; i<8; i++) children[i].GetRecursive(list);
        }
        
        void Get(const Ray& ray, std::vector<T>& list) {
            if (!nodes) return;
            
            if (!ray.Intersect(box)) {
                return;
            }
            else {
                for(size_t i = 0, size=nodes->size(); i<size; i++) {
                    Node* node = nodes->at(i);
                    
                    if (ray.Intersect(node->box)) list.push_back((T)node->data);
                }
                if (!children) return;
                for (int i=0; i<8; i++) {
                    children[i].template Get(ray, list);
                }
            }
        }
        
        const static unsigned MaxObjectsInNode = 32;

        Octree() : children(nullptr), nodes(nullptr), parent(nullptr) { }

        Octree(const BoundingBox& box) {
            SetBoundingBox(box);
        }

        ~Octree() {
            if (children) delete[] children;
            if (nodes) delete nodes;
        }

        void SetBoundingBox(const BoundingBox& box) {
            this->box = box;
        }

        bool Insert(Octree::Node& node) {
            bool test;
            if (parent) {
                test = box.Contains(node.box);
            } else {
                test = box.Intersects(node.box);
            }

            if (test) {
                if (!nodes) nodes = new Nodes();

                if (nodes->size()>=MaxObjectsInNode) {
                    Split();
                    bool insertedIntoChildren = false;
                    for (int i=0; i<8; i++) {
                        insertedIntoChildren = children[i].Insert(node);
                        if (insertedIntoChildren) break;
                    }
                    if (!insertedIntoChildren) {
                        node.childIndex = nodes->size();
                        node.node = this;
                        nodes->push_back(&node);
                    }
                } else {
                    node.childIndex = nodes->size();
                    node.node = this;
                    nodes->push_back(&node);
                }
                return true;
            }

            return false;
        }

        void Remove(Node& node) {
            Nodes& nodes = *node.node->nodes;
            
            if (nodes.size()<=1) {
                nodes.clear();
            } else {
                Node* last = nodes.back();
                nodes[node.childIndex] = last;
                last->childIndex = node.childIndex;
                nodes.pop_back();
            }
            
            node.node = 0;
        }

        void Move(Node& node) {
            
            if (node.node->box.Contains(node.box)) {
                return;
            } else {
                Remove(node);
                Insert(node);
            }
        }

        void Split() {
            if (children) return;
            children = new Octree[8];

            Vector3 size = box.extends * 0.5f;
            Vector3 offset = size * 0.5f;

            children[0].box = BoundingBox(box.center + Vector3(-offset.x, -offset.y, -offset.z), size);
            children[1].box = BoundingBox(box.center + Vector3(-offset.x, offset.y, -offset.z), size);
            children[2].box = BoundingBox(box.center + Vector3(offset.x, offset.y, -offset.z), size);
            children[3].box = BoundingBox(box.center + Vector3(offset.x, -offset.y, -offset.z), size);
            children[4].box = BoundingBox(box.center + Vector3(-offset.x, -offset.y, offset.z), size);
            children[5].box = BoundingBox(box.center + Vector3(-offset.x, offset.y, offset.z), size);
            children[6].box = BoundingBox(box.center + Vector3(offset.x, offset.y, offset.z), size);
            children[7].box = BoundingBox(box.center + Vector3(offset.x, -offset.y, offset.z), size);

            for (int i=0; i<8; i++) {
                children[i].parent = this;
            }
        }
    private:
        BoundingBox box;
		Octree* parent;
		Octree* children;
		Nodes* nodes;
	};
}

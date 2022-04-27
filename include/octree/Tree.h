/**
 * @file Root.h
 * @author Ryan Purse
 * @date 27/04/2022
 */


#pragma once

#include "Pch.h"

#include "Node.h"
#include "OctreeHelpers.h"

namespace octree
{
    /**
     * A 'root' node of an octree. Helps and manages the octree.
     * @author Ryan Purse
     * @date 27/04/2022
     */
    template<typename T>
    class Tree  // Does not inherit from Node, instead contains a 'root' node.
    {
    public:
        explicit Tree(const AABB &bounds, const uint32_t splitThreshold = 10, const uint32_t maxDepth = 50);
        
        void insert(const T &data, const AABB &bounds);
        
        std::vector<T> getIntersecting(const AABB &bounds);
        
    protected:
        Node<T> mRoot;
        const uint32_t mMaxDepth { 50 };
        std::vector<Package<T>> mUnboundItems;
    };
    
    template<typename T>
    Tree<T>::Tree(const AABB &bounds, const uint32_t splitThreshold, const uint32_t maxDepth) :
        mRoot(bounds, splitThreshold), mMaxDepth(maxDepth)
    {}
    
    template<typename T>
    void Tree<T>::insert(const T &data, const AABB &bounds)
    {
        Package<T> package { data, bounds };
        if (mRoot.insert(package, mMaxDepth))
            mUnboundItems.push_back(package);
    }
    
    template<typename T>
    std::vector<T> Tree<T>::getIntersecting(const AABB &bounds)
    {
        std::vector<T> hitItems;
        for (const auto &item : mUnboundItems)
        {
            if (intersects(item.bounds, bounds))
                hitItems.push_back(item.data);
        }
        
        mRoot.getIntersecting(bounds, hitItems);
        return hitItems;
    }
}


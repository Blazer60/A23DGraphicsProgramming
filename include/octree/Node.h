/**
 * @file Node.h
 * @author Ryan Purse
 * @date 26/04/2022
 */


#pragma once

#include "Pch.h"

#include "OctreeHelpers.h"
#include "PhysicsHelpers.h"
#include <array>

namespace octree
{
    /**
     * @author Ryan Purse
     * @date 26/04/2022
     */
    template<typename T>
    class Node
    {
        static constexpr uint32_t sRegionCount { 8 };
    public:
        explicit Node(const AABB &bounds, const uint32_t splitThreshold=10);
        
        bool insert(Package<T> item, int depth=0);
        
        void getIntersecting(const AABB &bounds, std::vector<T> &hitItems) const;

    protected:
        void subdivide(int depth);
        
        const AABB                              mBounds;
        const uint32_t                          mSplitThreshold { 10 };
        std::unique_ptr<std::array<Node, sRegionCount>>    mSubRegions     { nullptr };
        std::vector<Package<T>>                 mItems;
    };
    
    
    template<typename T>
    Node<T>::Node(const AABB &bounds, const uint32_t splitThreshold) :
        mBounds(bounds), mSplitThreshold(splitThreshold)
    {}
    
    template<typename T>
    bool Node<T>::insert(Package<T> item, int depth)
    {
        if (!contains(mBounds, item.bounds))
            return false;
        if (mSubRegions)
        {
            bool success = false;
            for (auto &region : mSubRegions)
            {
                success |= region.insert(item, depth - 1);
            }
            if (!success)
            {
                mItems.push_back(item);
            }
        }
        else
        {
            mItems.push_back(item);
            subdivide(depth);
        }
        return true;
    }
    
    template<typename T>
    void Node<T>::subdivide(int depth)
    {
        if (depth <= 0)
            return;
        if (mItems.size() < mSplitThreshold)
            return;
        
        const glm::vec3 quarterSize = 0.5f * mBounds.halfSize;
        const std::array<glm::vec3, sRegionCount> positions {
            mBounds.position + glm::vec3(+quarterSize.x, +quarterSize.y, +quarterSize.z),
            mBounds.position + glm::vec3(+quarterSize.x, +quarterSize.y, -quarterSize.z),
            mBounds.position + glm::vec3(-quarterSize.x, +quarterSize.y, -quarterSize.z),
            mBounds.position + glm::vec3(-quarterSize.x, +quarterSize.y, +quarterSize.z),
            mBounds.position + glm::vec3(+quarterSize.x, -quarterSize.y, +quarterSize.z),
            mBounds.position + glm::vec3(+quarterSize.x, -quarterSize.y, -quarterSize.z),
            mBounds.position + glm::vec3(-quarterSize.x, -quarterSize.y, -quarterSize.z),
            mBounds.position + glm::vec3(-quarterSize.x, -quarterSize.y, +quarterSize.z)
        };
        
        mSubRegions = std::make_unique<std::array<Node<T>, sRegionCount>>();
        for (int i = 0; i < sRegionCount; ++i)
            mSubRegions[i] = Node<T>(AABB { positions[i], quarterSize }, mSplitThreshold);
        
        std::vector<Package<T>> tmp = mItems;
        mItems.clear();
        for (auto &item : tmp)
            insert(item);
    }
    
    template<typename T>
    void Node<T>::getIntersecting(const AABB &bounds, std::vector<T> &hitItems) const
    {
        if (!intersects(mBounds, bounds))
            return;
    
        for (auto &item : mItems)
        {
            if (intersects(item.bounds, bounds))
                hitItems.emplace_back(item.data);
        }
        
        if (mSubRegions)
        {
            for (const auto &region : *mSubRegions)
                region.getIntersecting(bounds, hitItems);
        }
    }
}



/* 
 * File:   PageFrameAllocator.h
 * Author: Tristan Gay, Peter Gish, Chenchen Mao
 *
 * Created on January 27, 2018, 12:39 AM
 */
#ifndef PAGEFRAMEALLOCATOR_H
#define PAGEFRAMEALLOCATOR_H

//#include <cstdlib>
#include <stdint.h> //uint8_t, uint32_t
#include <vector> //vector

/* PageFrameAllocator - manages allocation/deallocation of page frames. */
class PageFrameAllocator {
public:
    /**PageFrameAllocator     prepares memory vector for some # of page frames
     * @param numPageFrames   the number of page frames to allocate
     */
    PageFrameAllocator(uint32_t numPageFrames);

    /**Allocate           determines if allocating more page frames is necessary
     * @param count       # of page frames to allocate from the free list
     * @param page_frames vector on which to push allocated page frames
     * @return            if # free page frames < count arg, return false
     *                    if page frames are successfully allocated, return true
     */
    bool Allocate(uint32_t count, std::vector<uint32_t> &page_frames);
    
    /**Deallocate         pops the last count page frame #s from page_frames
     * @param count       # of page frames to return to the free list
     * @param page_frames the vector from which we access our page frames
     * @return            if count <= page_frames.size(), return true
     *                    return false without freeing any page frames
     */
    bool Deallocate(uint32_t count, std::vector<uint32_t> &page_frames);
    
    /* Returns the current number of free page frames. */
    uint32_t get_page_frames_free() const { return page_frames_free; };
    
    /* Returns the total number of frame pages. */
    uint32_t get_page_frames_total() const { return page_frames_total; };
    
    /* Returns page frame # of first page frame in our free list 
     * (returns 0xFFFFFFF if empty) */
    uint32_t get_free_list_head() const { return free_list_head; };

    /* Disallowed move/copy constructors */
    PageFrameAllocator(const PageFrameAllocator &orig) = delete;
    PageFrameAllocator(PageFrameAllocator &&orig) = delete;
    PageFrameAllocator operator=(const PageFrameAllocator &orig) = delete;
    PageFrameAllocator operator=(PageFrameAllocator &&orig) = delete;

    /* Unused destructor */
    virtual ~PageFrameAllocator();
private:
    std::vector<uint8_t> memory; //Byte array containing page frames to be managed
    uint32_t page_frames_total; //Counts total # of page frames in memory (* 0x1000)
    uint32_t page_frames_free; //Current # of free page frames
    uint32_t free_list_head; //Page frame # of the first page frame in free list (0xFFFFFFFF if empty)
    const uint32_t PAGE_FRAME_SIZE = 0x1000; //Page frame size (4096 in decimal)
    
    /* -- Linked List Implementation --
     * Contains the free page frames in the form of a linked list. 
     * The "links" are page frame numbers (which are stored in 
     * the first four bytes of a block). The links indicate the
     * next free page frame in the list. The last page frame on 
     * the free list has link 0xFFFFFFFF. The head "pointer" is just
     * the number of the first page frame in the list (0xFFFFFFFF if empty).
     * Page frame numbered 'n' consists of the block of 0x1000 bytes
     * (uint8_t) starting at address n*0x1000 in the memory vector. */
};

#endif /* PAGEFRAMEALLOCATOR_H */

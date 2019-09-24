/* 
 * File:   PageFrameAllocator.cpp
 * Authors: Tristan Gay, Peter Gish, Chenchen Mao
 * 
 * Created on January 27, 2018, 12:39 AM
 */

#include <cstring> //memcpy
#include "PageFrameAllocator.h"

PageFrameAllocator::PageFrameAllocator(uint32_t numPageFrames){
/* Should resize memory memory vector to numPageFrames * 0x1000 
 * Then build free list consistent of all page frames in memory.
 * Initialize other class member data variables as needed. */
    memory.resize(numPageFrames * PAGE_FRAME_SIZE, 0);
    int pos = 0;
    for(uint32_t i = 1; i <= numPageFrames; ++i){
        memcpy(&memory[((i-1)*PAGE_FRAME_SIZE)], &i, sizeof(uint32_t));
    }
    //Initialize class member variables
    page_frames_total = numPageFrames; 
    page_frames_free = numPageFrames;
    free_list_head = 0x0;
}

bool PageFrameAllocator::Allocate(uint32_t count, std::vector<uint32_t> &page_frames){
/* Push the numbers of all the allocated page frames onto the back of the 
 * vector page_frames (specified as the second argument). If the number of free 
 * page frames is less than the count argument, then no page frames should be 
 * allocated, and method should return false. 
 * If page frames are successfully allocated, return true.*/
    if(page_frames_free < count){ 
        return false;
    }
    for(uint32_t i = free_list_head/0x1000; i < count; i++){
        page_frames.push_back(free_list_head);
        uint32_t index = i * 0x1000;
        uint32_t v32;
        memcpy(&v32, &memory[index], sizeof(uint32_t));
        free_list_head = v32;
        page_frames_free--;
    }

    if(free_list_head/0x1000 == page_frames_total){
        free_list_head=0xFFFFFFFF;
    }

    return true;
}

bool PageFrameAllocator::Deallocate(uint32_t count, std::vector<uint32_t> &page_frames){
/* Return the last count page frame numbers from the vector page_frames to the 
 * free list. Page frame numbers should be popped from the back of the page_frames
 * vector as they are returned to the free list. Returns true if
 * count <= page_frames.size() otherwise returns false without freeing any page frames
 */
    if(count > page_frames.size()){
        return false;
    }
    if(free_list_head == 0xFFFFFFFF){
        free_list_head = page_frames_total;
    }
    for(uint32_t i = 0; i < count; i++){
        page_frames.pop_back();
        uint32_t index = (page_frames_total - i) * 0x1000;
        uint32_t v32;
        memcpy(&v32, &memory[index], sizeof(uint32_t));
        free_list_head = v32;
    }
    page_frames_free += count;
}

PageFrameAllocator::~PageFrameAllocator() {}


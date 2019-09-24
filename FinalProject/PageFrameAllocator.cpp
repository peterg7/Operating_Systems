/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageFrameAllocator.cpp
 * Author: Tristan Gay
 * 
 * Created on January 27, 2018, 12:39 AM
 */

#include "PageFrameAllocator.h"

PageFrameAllocator::PageFrameAllocator(uint32_t numPageFrames){
/* Should resize memory memory vector to numPageFrames * 0x1000 
 * Then build free list consistent of all page frames in memory.
 * Initialize other class member data variables as needed. */
}

bool PageFrameAllocator::Allocate(uint32_t count, std::vector<uint32_t> &page_frames){
/* Push the numbers of all the allocated page frames onto the back of the 
 * vector page_frames (specified as the second argument). If the number of free 
 * page frames is less than the count argument, then no page frames should be 
 * allocated, and method should return false. 
 * If page frames are successfully allocated, return true.*/
}
bool PageFrameAllocator::Deallocate(uint32_t count, std::vector<uint32_t> &page_frames){}
PageFrameAllocator::~PageFrameAllocator() {}


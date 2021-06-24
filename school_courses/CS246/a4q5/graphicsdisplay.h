//
//  graphicsdisplay.hpp
//  a4q5
//
//  Created by Ethan on 2019-07-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "observer.h"
#include "subject.h"
#include "state.h"
#include "cell.h"
#include "grid.h"
#include "info.h"
#include "window.h"


#ifndef _GRAPHICSDIAPLAY_H_
#define _GRAPHICSDIAPLAY_H_

class GraphicsDisplay: public Observer<Info, StateType> {
    size_t size;
    size_t cellSize;
    Xwindow window;
public:
    GraphicsDisplay(size_t size);
    
    void notify(Subject<Info, State> &whoFrom) override;
};

#endif /* graphicsdisplay_hpp */

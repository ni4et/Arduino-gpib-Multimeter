
// Draw a window in a panel with an angled opening.
// The passed size is the back/inside opening.
// The front of the hole will be grown according to the
// trig calculation and the thickness.
// A suggested thickness is 3.5-4mm

// Place this in a difference() block with other holes in the 
// panel.
// Translate as needed.  Include a small -z to clean up the hole
// in the rendering.
module Bezel(size)
{
    
    bx=size.x;  // Size of the cutout
    by=size.y;
    bz=size.z; // Thk at bezel
    bang=60;  // Angle at the bezel
    be=bz/tan(bang);

    
    
    bp=[[-bx/2-be,-by/2-be,0],[-bx/2-be,by/2+be,0],
        [bx/2+be,by/2+be,0],[bx/2+be,-by/2-be,0],
        [-bx/2,-by/2,bz],[-bx/2,by/2,bz],
        [bx/2,by/2,bz],[bx/2,-by/2,bz]];
    
    bf=[[3,2,1,0],[4,5,6,7],[0,4,7,3],[5,1,2,6],[0,1,5,4],[3,7,6,2]];   
    
    
    polyhedron(bp,bf,3);

}

module BezelTest()
{
    // the opening should be slightly thicker and lower
    // than the surrounding panel for the display.
    panel=[100,50,4];
    opening=[80,40,4.01];
    
    difference()
    {
        translate([0,0, panel.z/2])
            cube(panel,center=true);
        translate([0,0,-.005])
            Bezel(opening);
    }
}

BezelTest();